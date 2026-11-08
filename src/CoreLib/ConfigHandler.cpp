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

#include "Modules/ModuleGlobal.hpp"

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

ConfigHandler::ConfigHandler()
    : m_mainStorageCache(std::make_unique<StorageCache>())
    , m_appData(Mernel::AppLocations("D2R mod generator").getAppdataDir())
    , m_defaultPath(m_appData / "config.json")
{
    m_config.m_main.suppressPresets();
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
    auto prevMain = m_config.m_main;
    m_config.readFromProperty(data);
    updateSeed(false);
    if (!resetMain)
        m_config.m_main = prevMain;

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
    auto& main = m_config.m_main;
    if (main.m_seed == 0 || main.m_refreshSeed && onGenerate) {
        std::random_device rd;
        main.m_seed = rd();
        m_config.m_main.notifyChangeInner();
    }
}

bool ConfigHandler::generate(const GenerationEnvironment& env)
{
    auto startAll = Mernel::ChronoPoint(true);
    updateSeed(true);

    FolderStorage outDebugStorage(m_appData / "debug");
    FolderStorage outStorage(env.outputPath, env.outputMode);
    outStorage.setModName(env.modName);
    outStorage.setUseBaseSubolder(env.needDataSubfolder);
    outStorage.setSilentTrace(false);

    DataContext previousData;

    Logger(Logger::Notice) << "started in: " << path2string(env.outputPath) << ", seed:" << env.seed;

    IStorage::StoredData previousStoredData = outStorage.readData();

    if (previousStoredData.valid) {
        previousData.parseData(previousStoredData, false);
    }

    DataContext output;
    output.silent = false;

    DataContext initialOutput;

    Logger() << "Loading data from main storage...";
    const IStorage::StoredData inData = m_mainStorageCache->load({ env.inputMode, env.inputPath, env.needDataSubfolder });
    if (!inData.valid) {
        Logger(Logger::Err) << "Failed to read data files from D2 folder.";
        return false;
    }
    Logger(Logger::Info) << "Load files from game storage time: " << startAll.GetElapsedTime().ToProfilingTime();

    Logger() << "Parsing json and txt...";
    if (!output.parseData(inData)) {
        Logger(Logger::Err) << "Failed parse D2 data files.";
        return false;
    }
    if (env.extraTrace.contains("dump_diff")) {
        initialOutput = output.makeSilent();
    }
    if (env.extraTrace.contains("dump_in")) {
        outDebugStorage.setFileSuffix(".in");
        if (outDebugStorage.create())
            outDebugStorage.writeMainData(output.makeSilent().writeDebug());
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

        ModuleGlobal all(output, rngWrapper, m_config, env);
        try {
            all.generate();
        }
        catch (const std::exception& ex) {
            Logger(Logger::Err) << ex.what();
            return false;
        }

        Logger(Logger::Info) << "generate logic finished in: " << start.GetElapsedTime().ToProfilingTime();
    }

    if (env.extraTrace.contains("dump_out")) {
        outDebugStorage.setFileSuffix(".out");
        if (outDebugStorage.create())
            outDebugStorage.writeMainData(output.makeSilent().writeDebug());
    }

    if (env.extraTrace.contains("dump_diff")) {
        outDebugStorage.setFileSuffix(".diff");
        if (outDebugStorage.create())
            outDebugStorage.writeMainData(output.calculateDiff(initialOutput).makeSilent().writeDebug());
    }

    IStorage::StoredData outData;
    if (!output.serializeData(outData))
        return false;

    if (!outStorage.create())
        return false;

    if (env.extraTrace.contains("dump_bmp")) {
        FolderStorage outDebugStorageBmp(m_appData / "debugBitmaps");
        if (outDebugStorageBmp.create())
            outDebugStorageBmp.writeBitmaps(outData);
    }

    if (!outStorage.writeMainData(outData) || !outStorage.writeMeta(outData))
        return false;

    Logger(Logger::Info) << "Total time: " << startAll.GetElapsedTime().ToProfilingTime();
    return true;
}

ConfigHandler::~ConfigHandler() = default;

}
