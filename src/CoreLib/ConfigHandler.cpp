/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ConfigHandler.hpp"

#include "DataContext.hpp"
#include "FileIOUtils.hpp"
#include "Logger.hpp"

#include "MernelPlatform/ScopeExit.hpp"
#include "MernelPlatform/ChronoPoint.hpp"

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

struct ModuleRecord {
    std::string              m_id;
    std::unique_ptr<IModule> m_gen;
};

template<class T>
ModuleRecord create(const std::string& id)
{
    return ModuleRecord{ id, std::make_unique<T>() };
}

struct ModuleAll : public IModule {
    void generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env) override
    {
        std::vector<ModuleRecord> modules;
        // light modules
        modules.push_back(create<ModuleChallenge>("Challenge"));
        modules.push_back(create<ModuleCharacter>("Character"));
        modules.push_back(create<ModuleGambling>("Gambling"));
        modules.push_back(create<ModuleItemDrops>("ItemDrops"));
        modules.push_back(create<ModuleMonDensity>("MonDensity"));
        modules.push_back(create<ModuleMonStats>("MonStats"));
        modules.push_back(create<ModulePerfectRoll>("PerfectRoll"));
        modules.push_back(create<ModuleQol>("Qol"));
        modules.push_back(create<ModuleRequirements>("Requirements"));
        modules.push_back(create<ModuleRuneDrops>("RuneDrops"));

        // modules that can add more rows, thus in the end
        modules.push_back(create<ModuleCube>("Cube"));
        modules.push_back(create<ModuleDropFiltering>("DropFiltering"));
        modules.push_back(create<ModuleMonRandomizer>("MonRandomizer"));
        modules.push_back(create<ModuleItemRandomizer>("ItemRandomizer"));
        modules.push_back(create<ModuleSkillRandomizer>("SkillRandomizer"));

        for (auto& module : modules) {
            Logger() << "start module: " << module.m_id;
            try {
                module.m_gen->generate(output, rng, config, env);
            }
            catch (const std::exception& ex) {
                m_err = std::string("module '" + module.m_id + "': " + std::string(ex.what()));

                return;
            }
        }
    }

    std::string m_err;
};

}

ConfigHandler::ConfigHandler()
    : m_mainStorageCache(std::make_unique<StorageCache>())
    , m_appData(Mernel::AppLocations("D2R mod generator").getAppdataDir())
    , m_defaultPath(m_appData / "config.json")
{
    loadConfig(Mernel::PropertyTree{}, true);
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
    auto prevMain = m_config.m_moduleMain;
    m_config.readFromProperty(data);
    updateSeed(false);
    if (!resetMain)
        m_config.m_moduleMain = prevMain;

    return true;
}

bool ConfigHandler::saveConfig(Mernel::PropertyTree& data) const
{
    data.convertToMap();
    m_config.writeToProperty(data);
    return true;
}

void ConfigHandler::updateSeed(bool onGenerate)
{
    auto& main = m_config.m_moduleMain;
    if (main.m_seed == 0 || main.m_refreshSeed && onGenerate) {
        std::random_device rd;
        main.m_seed = rd();
        m_config.m_moduleMain.notifyChangeInner();
    }
}

ConfigHandler::GenerateResult ConfigHandler::generate()
{
    auto startAll = Mernel::ChronoPoint(true);
    updateSeed(true);

    std::string                 err;
    const GenerationEnvironment env = getEnv(err);
    if (!err.empty()) {
        return { err };
    }

    FolderStorage outStorage(env.outputPath, env.outputMode, env.modName, env.needDataSubfolder);

    Logger(Logger::Notice) << "started generation in " << path2string(env.outputPath) << ", seed:" << env.seed;
    if (!outStorage.prepareForWrite()) {
        return { "Failed to write data in destination folder; try to launch as admin." };
    }

    DataContext output;

    Logger() << "Loading data from main storage...";
    const IStorage::StoredData inData = m_mainStorageCache->load({ env.inputMode, env.inputPath, env.needDataSubfolder });
    if (!inData.valid) {
        return { "Failed to read data files from D2 folder." };
    }
    Logger(Logger::Notice) << "Load files from game storage time: " << startAll.GetElapsedTime().ToProfilingTime();

    Logger() << "Parsing json and txt...";
    if (!output.parseData(inData)) {
        return { "Failed parse D2 data files." };
    }
    {
        IStorage::StoredData inDebug;
        output.writeDebug(inDebug);
        FolderStorage outDebugStorage(m_appData / "debug", StorageType::CsvFolder, env.modName, false, ".in");
        outDebugStorage.prepareForWrite();
        outDebugStorage.writeData(inDebug);
    }

    Logger() << "prepare ended; Starting generate phase.";

    {
        auto start           = Mernel::ChronoPoint(true);
        using Distribution32 = std::uniform_int_distribution<int32_t>;
        std::mt19937_64 engine;
        engine.seed(env.seed); // really we need A LOT of bits to safely seed mt engine. But for our purpose 32 bits more than enough.
        std::function<int(int)> rngWrapper = [&engine](int bound) {
            if (bound <= 1)
                return 0;
            return Distribution32(0, bound - 1)(engine);
        };

        ModuleAll all;
        all.generate(output, rngWrapper, m_config, env);
        if (!all.m_err.empty())
            return { all.m_err };

        Logger(Logger::Notice) << "generate logic finished in: " << start.GetElapsedTime().ToProfilingTime();
    }
    Logger() << "prepare output data.";

    {
        IStorage::StoredData outDebug;
        output.writeDebug(outDebug);
        FolderStorage outDebugStorage(m_appData / "debug", StorageType::CsvFolder, env.modName, false, ".out");
        outDebugStorage.prepareForWrite();
        outDebugStorage.writeData(outDebug);
    }

    IStorage::StoredData outData;
    if (!output.serializeData(outData)) {
        Logger() << "Failed to prepare data buffers"; // highly unlikely.
        return {};
    }

    Logger() << "writing output to disk.";
    if (!outStorage.writeData(outData)) {
        return { "Failed write output data to disk" };
    }

    Logger(Logger::Notice) << "Total time: " << startAll.GetElapsedTime().ToProfilingTime();
    return { "", true };
}

GenerationEnvironment ConfigHandler::getEnv(std::string& err) const
{
    auto&                 main = m_config.m_moduleMain;
    GenerationEnvironment env;

    auto versionMajor = GenerationEnvironment::VersionMajor(main.m_versionMajor);
    auto versionD2R   = GenerationEnvironment::VersionD2R(main.m_versionD2R);
    auto inputMode    = GenerationEnvironment::InputMode(main.m_inputMode);
    auto outputMode   = GenerationEnvironment::OutputMode(main.m_outputMode);

    std::vector<std::string> localeIds{ "enUS", "deDE", "esES", "esMX", "frFR", "itIT", "jaJP", "koKR", "plPL", "ptBR", "ruRU", "zhCN", "zhTW" };
    int                      localeIdIndex = main.m_d2rLocaleId;

    env.d2rLocaleId = localeIds.at(localeIdIndex);
    env.modName     = main.m_modname;
    env.inputPath   = main.m_inputPath;
    env.outputPath  = main.m_outputPath;
    env.refreshSeed = main.m_refreshSeed;
    env.seed        = static_cast<uint32_t>(main.m_seed);
    if (env.outputPath.empty()) {
        if (inputMode != GenerationEnvironment::InputMode::Game) {
            err = "Cannot detect output path.";
            return env;
        }
        env.outputPath = env.inputPath;
    }

    if (env.inputPath.empty()) {
        err = "Input path is empty!";
        return env;
    }

    if (!Mernel::isExistingDirectory(env.inputPath)) {
        err = "Input path is not a valid dir:" + path2string(env.inputPath);
        return env;
    }

    env.isLegacy          = versionMajor == GenerationEnvironment::VersionMajor::Legacy;
    env.needDataSubfolder = versionD2R == GenerationEnvironment::VersionD2R::LoD;
    env.isRotW            = !env.isLegacy && versionD2R == GenerationEnvironment::VersionD2R::RotW;
    if (inputMode == GenerationEnvironment::InputMode::Game) {
        if (env.isLegacy) {
            env.inputMode = StorageType::D2LegacyInternal;
        }
    } else if (inputMode == GenerationEnvironment::InputMode::FullFolders) {
        if (env.isLegacy) {
            env.inputMode = StorageType::D2LegacyFolder;
        } else {
            env.inputMode = StorageType::D2ResurrectedModFolder;
        }
    } else {
        env.inputMode = StorageType::CsvFolder;
    }

    if (outputMode == GenerationEnvironment::OutputMode::D2RMod) {
        if (env.isLegacy) {
            err = "You must select another output mode!";
            return env;
        }
    } else if (outputMode == GenerationEnvironment::OutputMode::FullFolders) {
        env.outputMode = StorageType::D2LegacyFolder;
    } else if (outputMode == GenerationEnvironment::OutputMode::Txt) {
        env.outputMode = StorageType::CsvFolder;
    } else {
        env.outputMode = StorageType::None;
    }

    return env;
}

ConfigHandler::~ConfigHandler() = default;

}
