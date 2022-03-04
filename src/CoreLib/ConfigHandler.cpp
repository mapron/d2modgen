/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigHandler.hpp"

#include "ModuleFactory.hpp"
#include "DataContext.hpp"
#include "FileIOUtils.hpp"

#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

#include "Logger.hpp"
#include <QDir>
#include <QFileInfo>

#include <random>

namespace D2ModGen {

ConfigHandler::ConfigHandler()
    : m_mainStorageCache(std::make_unique<StorageCache>())
{
    auto modules = createAllModules();
    for (const auto& p : modules) {
        m_modules[p.first] = { p.second, {} };
    }
    m_modules[std::string(IModule::Key::testConfig)].m_enabled = true;
}

bool ConfigHandler::loadConfig(const std::string& filename)
{
    Logger() << "Load:" << filename;
    std::string  buffer;
    PropertyTree doc;
    if (!readFileIntoBuffer(filename, buffer) || !readJsonFromBuffer(buffer, doc)) {
        loadConfig(PropertyTree{});
        return false;
    }
    return loadConfig(doc);
}

bool ConfigHandler::saveConfig(const std::string& filename) const
{
    PropertyTree data;
    saveConfig(data);
    QDir().mkpath(QFileInfo(QString::fromStdString(filename)).absolutePath());
    std::string buffer;
    writeJsonToBuffer(buffer, data);
    return writeFileFromBuffer(filename, buffer);
}

bool ConfigHandler::loadConfig(const PropertyTree& data)
{
    for (auto& p : m_modules) {
        p.second.m_enabled = data.value(p.first + "_enabled", false).toBool();

        p.second.m_currentConfig = {};
        if (data.contains(p.first))
            p.second.m_currentConfig = data[p.first];
    }
    m_modules[std::string(IModule::Key::testConfig)].m_enabled = true;
    m_currentMainConfig                                        = {};
    if (data.contains(std::string(IModule::Key::main)))
        m_currentMainConfig = data[std::string(IModule::Key::main)];
    return true;
}

bool ConfigHandler::saveConfig(PropertyTree& data) const
{
    for (auto& p : m_modules) {
        data[p.first]              = p.second.m_currentConfig;
        data[p.first + "_enabled"] = PropertyTreeScalar{ p.second.m_enabled };
    }
    data[std::string(IModule::Key::main)] = m_currentMainConfig;
    return true;
}

bool ConfigHandler::isConfigEnabled(const std::string& key) const
{
    return m_modules.at(key).m_enabled;
}

void ConfigHandler::setConfigEnabled(const std::string& key, bool value)
{
    m_modules.at(key).m_enabled = value;
}

IModule::Ptr ConfigHandler::getModule(std::string_view key) const
{
    return m_modules.at(std::string(key)).m_module;
}

ConfigHandler::GenerateResult ConfigHandler::generate()
{
    const GenerationEnvironment env = getEnv();
    if (env.d2rPath.empty()) {
        Logger(Logger::Warning) << "D2R path is empty";
        return {};
    }

    const StorageType storage    = (env.isLegacy) ? StorageType::D2LegacyInternal : StorageType::D2ResurrectedInternal;
    const StorageType storageOut = (env.isLegacy) ? StorageType::D2LegacyFolder : StorageType::D2ResurrectedModFolder;

    FolderStorage outStorage(env.outPath, storageOut, env.modName);
    if (!outStorage.prepareForWrite()) {
        return { "Failed to write data in destination folder; try to launch as admin." };
    }

    auto mergeContext = [this, &env](DataContext& targetContext, const IModule::ExtraDependencies::Source& source) -> bool {
        const auto    logInfo = source.srcRoot + " / " + source.modname;
        const bool    isMod   = source.type == StorageType::D2ResurrectedModFolder;
        const auto    root    = isMod ? env.outPath : source.srcRoot;
        FolderStorage inStorage(root, source.type, isMod ? source.modname : "");
        auto          storedData = inStorage.readData({});
        if (!storedData.valid) {
            Logger(Logger::Warning) << "Failed to read data files from D2 folder:" << logInfo.c_str();
            return false;
        }
        DataContext dataContext;
        if (!dataContext.readData(storedData)) {
            Logger(Logger::Warning) << "Failed to parse files into input:" << logInfo.c_str();
            return false;
        }
        if (!targetContext.mergeWith(dataContext, source.policy)) {
            Logger(Logger::Warning) << "Merge failed:" << logInfo.c_str();
            return false;
        }
        return true;
    };

    DataContext output;

    IModule::PreGenerationContext pregenContext;
    {
        for (auto& p : m_modules) {
            if (!p.second.m_enabled)
                continue;

            IModule::InputContext input;
            input.m_env           = env;
            input.m_settings      = p.second.m_currentConfig;
            input.m_defaultValues = p.second.m_module->defaultValues();
            p.second.m_module->gatherInfo(pregenContext, input);
        }
    }
    {
        const IStorage::StoredData data = m_mainStorageCache->load(storage, env.d2rPath, pregenContext.m_extraJson);
        if (!data.valid) {
            return { "Failed to read data files from D2 folder." };
        }
        if (!output.readData(data)) {
            return { "Failed parse D2 data files." };
        }
        if (env.exportAllTables)
            for (auto& p : output.tableSet.tables)
                p.second.forceOutput = true;
    }
    Logger() << "Loading pre-gen data.";
    {
        for (const auto& source : pregenContext.m_preGen.m_sources)
            if (!mergeContext(output, source)) {
                return { std::string("Failed to merge with source: ") + source.srcRoot + " / " + source.modname };
            }
    }
    Logger() << "prepare ended; Starting generate phase. seed=" << env.seed;
    {
        using Distribution32 = std::uniform_int_distribution<int32_t>;
        std::mt19937_64 engine;
        engine.seed(env.seed); // really we need A LOT of bits to safely seed mt engine. But for our purpose 32 bits more then enough.
        for (auto& p : m_modules) {
            if (!p.second.m_enabled)
                continue;

            Logger() << "start module:" << p.first;
            IModule::InputContext input;
            input.m_env               = env;
            input.m_settings          = p.second.m_currentConfig;
            input.m_defaultValues     = p.second.m_module->defaultValues();
            std::function<int(int)> r = [&engine](int bound) { 
                if (bound <= 1) 
                    return 0;
                return Distribution32(0, bound - 1)(engine); 
            };
            p.second.m_module->generate(output, r, input);
        }
    }
    Logger() << "Loading post-gen data.";
    {
        for (const auto& source : pregenContext.m_postGen.m_sources)
            if (!mergeContext(output, source)) {
                return { std::string("Failed to merge with source: ") + source.srcRoot + " / " + source.modname };
            }
    }
    Logger() << "prepare output data.";

    IStorage::StoredData outData;
    if (!output.writeData(outData)) {
        Logger() << "Failed to prepare data buffers"; // highly unlikely.
        return {};
    }

    Logger() << "writing output to disk.";
    if (!outStorage.writeData(outData)) {
        return { "Failed write output data to disk" };
    }

    Logger() << "generation ends.";
    return { "", true };
}

GenerationEnvironment ConfigHandler::getEnv() const
{
    const auto&           c = m_currentMainConfig;
    GenerationEnvironment env;
    env.modName         = c.value("modname", "").toString();
    env.isLegacy        = c.value("isLegacy", false).toBool();
    env.d2rPath         = ensureTrailingSlash(c.value(env.isLegacy ? "d2legacyPath" : "d2rPath", "").toString());
    env.exportAllTables = c.value("exportAllTables", false).toBool();
    env.seed            = static_cast<uint32_t>(c.value("seed", 0).toInt());
    env.outPath         = ensureTrailingSlash(c.value("outPath", "").toString());
    if (env.outPath.empty())
        env.outPath = env.d2rPath;

    return env;
}

ConfigHandler::~ConfigHandler() = default;

}
