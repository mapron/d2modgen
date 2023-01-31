/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigHandler.hpp"

#include "ModuleFactory.hpp"
#include "DataContext.hpp"
#include "FileIOUtils.hpp"
#include "Logger.hpp"
#include "PluginModule.hpp"

#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include <random>

namespace D2ModGen {

ConfigHandler::ConfigHandler(const std::string& pluginsRoot)
    : m_mainStorageCache(std::make_unique<StorageCache>())
{
    for (const auto& it : std_fs::directory_iterator(string2path(pluginsRoot))) {
        if (!it.is_regular_file())
            continue;

        const std_path& jsonDeclFilepath = it.path();
        std::string     id               = path2string(jsonDeclFilepath.stem());
        if (jsonDeclFilepath.extension() != ".json" || id.empty())
            continue;

        std::string          buffer;
        Mernel::PropertyTree info;
        if (!Mernel::readFileIntoBufferNoexcept(jsonDeclFilepath, buffer) || !readJsonFromBufferNoexcept(buffer, info)) {
            Logger(Logger::Err) << "Failed to read json file for plugin:" << jsonDeclFilepath;
            continue;
        }
        if (info.value("id", Mernel::PropertyTreeScalar("")).toString().empty())
            info["id"] = Mernel::PropertyTreeScalar(id);
        id           = info.value("id", Mernel::PropertyTreeScalar("")).toString();
        info["root"] = Mernel::PropertyTreeScalar(path2string(jsonDeclFilepath.parent_path() / id));

        try {
            auto pluginModule = createModule(info, id);
            if (m_modules.contains(pluginModule->settingKey())) {
                Logger(Logger::Err) << "Duplicate plugin id:" << pluginModule->settingKey();
                continue;
            }

            m_modules[pluginModule->settingKey()] = { pluginModule, {}, true, pluginModule->pluginInfo().value("loadOrder", Mernel::PropertyTreeScalar(1000)).toInt() };

            if (pluginModule->settingKey() != IModule::Key::testConfig)
                m_pluginIds.push_back(pluginModule->settingKey());
        }
        catch (std::exception& e) {
            Logger(Logger::Err) << "Error on loading (" << jsonDeclFilepath << ") = " << e.what();
        }
    }
    m_modules[std::string(IModule::Key::testConfig)].m_enabled = true;
}

bool ConfigHandler::loadConfig(const std::string& filename)
{
    Logger() << "Load:" << filename;
    std::string          buffer;
    Mernel::PropertyTree doc;
    if (!Mernel::readFileIntoBufferNoexcept(string2path(filename), buffer) || !readJsonFromBufferNoexcept(buffer, doc)) {
        loadConfig(Mernel::PropertyTree{});
        return false;
    }
    return loadConfig(doc);
}

bool ConfigHandler::saveConfig(const std::string& filename) const
{
    Mernel::PropertyTree data;
    saveConfig(data);
    if (!createDirectoriesForFile(string2path(filename)))
        return false;
    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, data);
    return Mernel::writeFileFromBufferNoexcept(string2path(filename), buffer);
}

bool ConfigHandler::loadConfig(const Mernel::PropertyTree& data)
{
    for (auto& p : m_modules) {
        p.second.m_enabled = data.value(p.first + "_enabled", Mernel::PropertyTreeScalar(false)).toBool();

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

bool ConfigHandler::saveConfig(Mernel::PropertyTree& data) const
{
    for (auto& p : m_modules) {
        data[p.first]              = p.second.m_currentConfig;
        data[p.first + "_enabled"] = Mernel::PropertyTreeScalar{ p.second.m_enabled };
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

    FolderStorage outStorage(string2path(env.outPath), storageOut, env.modName);

    Logger() << "started generation in " << env.outPath;
    if (!outStorage.prepareForWrite()) {
        return { "Failed to write data in destination folder; try to launch as admin." };
    }

    auto mergeContext = [this, &env](DataContext& targetContext, const IModule::ExtraDependencies::Source& source) -> bool {
        const auto    logInfo = source.srcRoot + " / " + source.modname;
        const bool    isMod   = source.type == StorageType::D2ResurrectedModFolder;
        const auto    root    = isMod ? env.outPath : source.srcRoot;
        FolderStorage inStorage(string2path(root), source.type, isMod ? source.modname : "");
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
            input.m_env      = env;
            input.m_settings = p.second.m_currentConfig;
            Mernel::PropertyTree::mergePatch(input.m_mergedSettings, p.second.m_module->defaultValues());
            Mernel::PropertyTree::mergePatch(input.m_mergedSettings, input.m_settings);

            p.second.m_module->gatherInfo(pregenContext, input);
        }
    }
    {
        Logger() << "Loading data from main storage...";
        const IStorage::StoredData data = m_mainStorageCache->load(storage, env.d2rPath, pregenContext.m_extraJson);
        if (!data.valid) {
            return { "Failed to read data files from D2 folder." };
        }
        Logger() << "Parsing json and txt...";
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
        std::vector<ModuleData*> orderedModules;
        for (auto& p : m_modules) {
            if (!p.second.m_enabled)
                continue;
            orderedModules.push_back(&p.second);
        }
        std::sort(orderedModules.begin(), orderedModules.end(), [](ModuleData* lh, ModuleData* rh) {
            return lh->m_order < rh->m_order;
        });
        for (ModuleData* module : orderedModules) {
            Logger() << "start module:" << module->m_module->settingKey();
            IModule::InputContext input;
            input.m_env      = env;
            input.m_settings = module->m_currentConfig;
            Mernel::PropertyTree::mergePatch(input.m_mergedSettings, module->m_module->defaultValues());
            Mernel::PropertyTree::mergePatch(input.m_mergedSettings, input.m_settings);

            std::function<int(int)> r = [&engine](int bound) {
                if (bound <= 1)
                    return 0;
                return Distribution32(0, bound - 1)(engine);
            };
            try {
                module->m_module->generate(output, r, input);
            }
            catch (const std::exception& ex) {
                return { std::string("Generate failed in module '" + module->m_module->settingKey() + "': " + std::string(ex.what())) };
            }
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
    env.modName         = c.value("modname", Mernel::PropertyTreeScalar("")).toString();
    env.isLegacy        = c.value("isLegacy", Mernel::PropertyTreeScalar(false)).toBool();
    env.d2rPath         = ensureTrailingSlash(c.value(env.isLegacy ? "d2legacyPath" : "d2rPath", Mernel::PropertyTreeScalar("")).toString());
    env.exportAllTables = c.value("exportAllTables", Mernel::PropertyTreeScalar(false)).toBool();
    env.seed            = static_cast<uint32_t>(c.value("seed", Mernel::PropertyTreeScalar(0)).toInt());
    env.outPath         = ensureTrailingSlash(c.value("outPath", Mernel::PropertyTreeScalar("")).toString());
    if (env.outPath.empty())
        env.outPath = env.d2rPath;

    return env;
}

ConfigHandler::~ConfigHandler() = default;

}
