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

#include "MernelPlatform/ScopeExit.hpp"

#include "Modules/ModuleChallenge.hpp"
#include "Modules/ModuleCharacter.hpp"
#include "Modules/ModuleCube.hpp"
#include "Modules/ModuleDropFiltering.hpp"
#include "Modules/ModuleGambling.hpp"
#include "Modules/ModuleItemDrops.hpp"
#include "Modules/ModuleItemRandomizer.hpp"
#include "Modules/ModuleMain.hpp"
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

#define TRACK_IO(filename, title) \
    bool result = false; \
    MERNEL_SCOPE_EXIT([&result, &filename]() { \
        if (result) \
            Mernel::Logger() << title << " OK: " << path2string(filename); \
        else \
            Mernel::Logger(Mernel::Logger::LogLevel::Err) << title << " FAIL: " << path2string(filename); \
    });

namespace D2ModGen {

namespace {

template<class T>
ConfigHandler::ModuleData create()
{
    return ConfigHandler::ModuleData{ std::make_shared<T>(), std::string(T::moduleKey), std::string(T::configKey) };
}

}

ConfigHandler::ConfigHandler()
    : m_modules({
          create<ModuleMain>(),

          // light modules
          create<ModuleChallenge>(),
          create<ModuleCharacter>(),
          create<ModuleGambling>(),
          create<ModuleItemDrops>(),
          create<ModuleMonDensity>(),
          create<ModuleMonStats>(),
          create<ModulePerfectRoll>(),
          create<ModuleQol>(),
          create<ModuleRequirements>(),
          create<ModuleRuneDrops>(),

          // modules that can add more rows, thus in the end
          create<ModuleCube>(),
          create<ModuleDropFiltering>(),
          create<ModuleMonRandomizer>(),
          create<ModuleItemRandomizer>(),
          create<ModuleSkillRandomizer>(),

      })
    , m_mainStorageCache(std::make_unique<StorageCache>())
    , m_appData(Mernel::AppLocations("D2R mod generator").getAppdataDir())
    , m_defaultPath(m_appData / "config.json")
{
}

bool ConfigHandler::loadAppConfig()
{
    const Mernel::std_path filename = m_appData / "app.json";
    TRACK_IO(filename, "Load");
    std::string buffer;
    m_appConfig = {};
    m_appConfig.convertToMap();
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, m_appConfig)) {
        return false;
    }
    result = true;
    return result;
}

bool ConfigHandler::saveAppConfig() const
{
    const Mernel::std_path filename = m_appData / "app.json";
    TRACK_IO(filename, "Save");
    if (!createDirectoriesForFile((filename)))
        return false;
    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, m_appConfig);
    result = Mernel::writeFileFromBufferNoexcept((filename), buffer);
    return result;
}

bool ConfigHandler::loadConfig(const Mernel::std_path& filename, bool resetMain)
{
    TRACK_IO(filename, "Load");
    std::string          buffer;
    Mernel::PropertyTree doc;
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, doc)) {
        return false;
    }
    result = loadConfig(doc, resetMain);
    return result;
}

bool ConfigHandler::saveConfig(const Mernel::std_path& filename) const
{
    TRACK_IO(filename, "Save");
    Mernel::PropertyTree data;
    saveConfig(data);
    if (!createDirectoriesForFile((filename)))
        return false;
    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, data);
    result = Mernel::writeFileFromBufferNoexcept((filename), buffer);
    return result;
}

bool ConfigHandler::loadConfig(const Mernel::PropertyTree& data, bool resetMain)
{
    for (auto& p : m_modules) {
        if (!resetMain && p.m_key == ModuleMain::configKey)
            continue;

        p.m_enabled       = data.value(p.m_key + "_enabled", Mernel::PropertyTreeScalar(false)).toBool();
        p.m_currentConfig = {};
        if (data.contains(p.m_key))
            p.m_currentConfig = data[p.m_key];

        p.m_currentConfig.convertToMap();
    }
    updateSeed(false);

    return true;
}

bool ConfigHandler::saveConfig(Mernel::PropertyTree& data) const
{
    for (auto& p : m_modules) {
        Mernel::PropertyTree cfg = p.m_currentConfig;
        cfg.convertToMap();

        auto defValues = Mernel::PropertyTree{ p.m_module->defaultValues() };
        std::erase_if(cfg.getMap(), [&defValues](const auto& pair) {
            return !defValues.contains(pair.first);
        });
        Mernel::PropertyTree::removeEqualValues(cfg, defValues);

        if (p.m_enabled)
            data[p.m_key + "_enabled"] = Mernel::PropertyTreeScalar{ true };

        if (!cfg.isNull() && !cfg.getMap().empty())
            data[p.m_key] = std::move(cfg);
    }
    return true;
}

void ConfigHandler::updateSeed(bool onGenerate)
{
    auto&    main = m_modules[0].m_currentConfig;
    uint32_t seed = static_cast<uint32_t>(main.value("seed", Mernel::PropertyTreeScalar(0)).toInt());

    bool refreshSeed = main.value("refreshSeed", Mernel::PropertyTreeScalar(false)).toBool();
    if (seed == 0 || refreshSeed && onGenerate) {
        std::random_device rd;
        seed         = rd();
        main["seed"] = Mernel::PropertyTreeScalar(seed);
    }
}

ConfigHandler::GenerateResult ConfigHandler::generate()
{
    updateSeed(true);

    const GenerationEnvironment env = getEnv();
    if (env.d2Path.empty()) {
        return { "D2 path is empty" };
    }
    if (!Mernel::isExistingDirectory(env.d2Path)) {
        return { "D2 path is not a valid dir:" + path2string(env.d2Path) };
    }

    const StorageType storage           = (env.isLegacy) ? StorageType::D2LegacyInternal : StorageType::D2ResurrectedInternal;
    const StorageType storageOut        = (env.isLegacy) ? StorageType::D2LegacyFolder : StorageType::D2ResurrectedModFolder;
    const bool        needBaseSubfolder = !env.isLegacy && !env.d2rUseROTW;

    FolderStorage outStorage(env.outPath, storageOut, env.modName, needBaseSubfolder);

    Logger(Logger::Notice) << "started generation in " << env.outPath << ", seed:" << env.seed;
    if (!outStorage.prepareForWrite()) {
        return { "Failed to write data in destination folder; try to launch as admin." };
    }

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
        const IStorage::StoredData data = m_mainStorageCache->load(storage, env.d2Path, pregenContext.m_extraJson, needBaseSubfolder);
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
    Logger() << "prepare ended; Starting generate phase.";
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
    const auto&           c = m_modules[0].m_currentConfig;
    GenerationEnvironment env;
    env.modName         = c.value("modname", Mernel::PropertyTreeScalar("rando")).toString();
    env.isLegacy        = c.value("isLegacy", Mernel::PropertyTreeScalar(false)).toBool();
    env.d2rUseROTW      = c.value("d2rUseROTW", Mernel::PropertyTreeScalar(true)).toBool();
    env.d2Path          = string2path(c.value(env.isLegacy ? "d2legacyPath" : "d2rPath", Mernel::PropertyTreeScalar("")).toString());
    env.exportAllTables = c.value("exportAllTables", Mernel::PropertyTreeScalar(false)).toBool();
    env.seed            = static_cast<uint32_t>(c.value("seed", Mernel::PropertyTreeScalar(0)).toInt());
    env.outPath         = string2path(c.value("outPath", Mernel::PropertyTreeScalar("")).toString());
    if (env.outPath.empty())
        env.outPath = env.d2Path;

    return env;
}

ConfigHandler::~ConfigHandler() = default;

}
