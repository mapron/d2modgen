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

#include "Modules/ModuleChallenge.hpp"
#include "Modules/ModuleCharacter.hpp"
#include "Modules/ModuleCube.hpp"
#include "Modules/ModuleDropFiltering.hpp"
#include "Modules/ModuleGambling.hpp"
#include "Modules/ModuleItemDrops.hpp"
#include "Modules/ModuleItemRandomizer.hpp"
#include "Modules/ModuleMonDensity.hpp"
#include "Modules/ModuleMonRandomizer.hpp"
#include "Modules/ModuleMonStats.hpp"
#include "Modules/ModulePerfectRoll.hpp"
#include "Modules/ModuleQol.hpp"
#include "Modules/ModuleRequirements.hpp"
#include "Modules/ModuleRuneDrops.hpp"
#include "Modules/ModuleSkillRandomizer.hpp"

#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include <random>

#include "MernelPlatform/AppLocations.hpp"

namespace D2ModGen {

ConfigHandler::ConfigHandler()
    : m_mainStorageCache(std::make_unique<StorageCache>())
    , m_appData(Mernel::AppLocations("D2R mod generator").getAppdataDir())
    , m_defaultPath(m_appData / "config.json")
{
    std::vector<IModule::Ptr> modules{
        // light modules
        std::make_shared<ModuleChallenge>(),
        std::make_shared<ModuleCharacter>(),
        std::make_shared<ModuleGambling>(),
        std::make_shared<ModuleItemDrops>(),
        std::make_shared<ModuleMonDensity>(),
        std::make_shared<ModuleMonStats>(),
        std::make_shared<ModulePerfectRoll>(),
        std::make_shared<ModuleQol>(),
        std::make_shared<ModuleRequirements>(),
        std::make_shared<ModuleRuneDrops>(),

        // modules that can add more rows, thus in the end
        std::make_shared<ModuleCube>(),
        std::make_shared<ModuleDropFiltering>(),
        std::make_shared<ModuleMonRandomizer>(),
        std::make_shared<ModuleItemRandomizer>(),
        std::make_shared<ModuleSkillRandomizer>(),
    };
    for (auto&& module : modules) {
        std::string id = module->settingKey();
        m_modules.push_back(ModuleData{ .m_module = std::move(module), .m_key = id, .m_key16 = std::u16string(id.cbegin(), id.cend()) });
    }
    for (auto&& module : m_modules) {
        m_moduleIndex8[module.m_key]    = &module;
        m_moduleIndex16[module.m_key16] = &module;
    }
}

bool ConfigHandler::loadAppConfig()
{
    const Mernel::std_path filename = m_appData / "app.json";
    Logger() << "Load:" << path2string(filename);
    std::string buffer;
    m_appConfig = {};
    m_appConfig.convertToMap();
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, m_appConfig)) {
        return false;
    }
    return true;
}

bool ConfigHandler::saveAppConfig() const
{
    const Mernel::std_path filename = m_appData / "app.json";
    Logger() << "Save:" << path2string(filename);
    if (!createDirectoriesForFile((filename)))
        return false;
    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, m_appConfig);
    return Mernel::writeFileFromBufferNoexcept((filename), buffer);
}

bool ConfigHandler::loadConfig(const Mernel::std_path& filename, bool resetMain)
{
    Logger() << "Load:" << path2string(filename);
    std::string          buffer;
    Mernel::PropertyTree doc;
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, doc)) {
        loadConfig(Mernel::PropertyTree{}, resetMain);
        return false;
    }
    return loadConfig(doc, resetMain);
}

bool ConfigHandler::saveConfig(const Mernel::std_path& filename) const
{
    Logger() << "Save:" << path2string(filename);
    Mernel::PropertyTree data;
    saveConfig(data);
    if (!createDirectoriesForFile((filename)))
        return false;
    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, data);
    return Mernel::writeFileFromBufferNoexcept((filename), buffer);
}

bool ConfigHandler::loadConfig(const Mernel::PropertyTree& data, bool resetMain)
{
    for (auto& p : m_modules) {
        p.m_enabled       = data.value(p.m_key + "_enabled", Mernel::PropertyTreeScalar(false)).toBool();
        p.m_currentConfig = {};
        if (data.contains(p.m_key))
            p.m_currentConfig = data[p.m_key];
    }
    if (resetMain) {
        m_currentMainConfig = {};
        if (data.contains(std::string(IModule::Key::main)))
            m_currentMainConfig = data[std::string(IModule::Key::main)];
    }
    return true;
}

bool ConfigHandler::saveConfig(Mernel::PropertyTree& data) const
{
    for (auto& p : m_modules) {
        data[p.m_key] = p.m_currentConfig;

        auto defValues = Mernel::PropertyTree{ p.m_module->defaultValues() };
        Mernel::PropertyTree::removeEqualValues(data[p.m_key], defValues);

        data[p.m_key + "_enabled"] = Mernel::PropertyTreeScalar{ p.m_enabled };
    }
    data[std::string(IModule::Key::main)] = m_currentMainConfig;
    return true;
}

ConfigHandler::GenerateResult ConfigHandler::generate()
{
    const GenerationEnvironment env = getEnv();
    if (env.d2rPath.empty()) {
        Logger(Logger::Warning) << "D2R path is empty";
        return { "D2R path is empty" };
    }

    const StorageType storage           = (env.isLegacy) ? StorageType::D2LegacyInternal : StorageType::D2ResurrectedInternal;
    const StorageType storageOut        = (env.isLegacy) ? StorageType::D2LegacyFolder : StorageType::D2ResurrectedModFolder;
    const bool        needBaseSubfolder = !env.isLegacy && !env.d2rUseROTW;

    FolderStorage outStorage(string2path(env.outPath), storageOut, env.modName, needBaseSubfolder);

    Logger() << "started generation in " << env.outPath;
    if (!outStorage.prepareForWrite()) {
        return { "Failed to write data in destination folder; try to launch as admin." };
    }

    auto mergeContext = [this, &env, needBaseSubfolder](DataContext& targetContext, const IModule::ExtraDependencies::Source& source) -> bool {
        const auto    logInfo = source.srcRoot + " / " + source.modname;
        const bool    isMod   = source.type == StorageType::D2ResurrectedModFolder;
        const auto    root    = isMod ? env.outPath : source.srcRoot;
        FolderStorage inStorage(string2path(root), source.type, isMod ? source.modname : "", needBaseSubfolder);
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
            if (!p.m_enabled)
                continue;

            IModule::InputContext input;
            input.m_env            = env;
            input.m_settings       = p.m_currentConfig;
            input.m_mergedSettings = Mernel::PropertyTree{ p.m_module->defaultValues() };
            if (!input.m_settings.isNull())
                Mernel::PropertyTree::mergePatch(input.m_mergedSettings, input.m_settings);

            p.m_module->gatherInfo(pregenContext, input);
        }
    }
    {
        Logger() << "Loading data from main storage...";
        const IStorage::StoredData data = m_mainStorageCache->load(storage, env.d2rPath, pregenContext.m_extraJson, needBaseSubfolder);
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
        for (ModuleData& module : m_modules) {
            if (!module.m_enabled)
                continue;

            Logger() << "start module:" << module.m_key;
            IModule::InputContext input;
            input.m_env            = env;
            input.m_settings       = module.m_currentConfig;
            input.m_mergedSettings = Mernel::PropertyTree{ module.m_module->defaultValues() };
            if (!input.m_settings.isNull())
                Mernel::PropertyTree::mergePatch(input.m_mergedSettings, input.m_settings);

            std::function<int(int)> r = [&engine](int bound) {
                if (bound <= 1)
                    return 0;
                return Distribution32(0, bound - 1)(engine);
            };
            try {
                module.m_module->generate(output, r, input);
            }
            catch (const std::exception& ex) {
                return { std::string("module '" + module.m_key + "': " + std::string(ex.what())) };
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
    env.d2rUseROTW      = c.value("d2rUseROTW", Mernel::PropertyTreeScalar(true)).toBool();
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
