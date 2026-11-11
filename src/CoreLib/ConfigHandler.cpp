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

#include "MernelPlatform/AppLocations.hpp"

#include <random>

namespace D2ModGen {

ConfigHandler::ConfigHandler()
    : m_mainStorageCache(std::make_unique<StorageCache>())
    , m_appData(Mernel::AppLocations("D2R mod generator").getAppdataDir())
    , m_defaultPath(m_appData / "config.json")
{
}

bool ConfigHandler::loadAppConfig()
{
    const Mernel::std_path filename = m_appData / "app.json";
    Logger() << "Read: " << path2string(filename);
    m_appConfig.resetToZero();

    std::string          buffer;
    Mernel::PropertyTree data;
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, data)) {
        return false;
    }

    m_appConfig.readFromProperty(data);
    return true;
}

bool ConfigHandler::saveAppConfig() const
{
    const Mernel::std_path filename = m_appData / "app.json";
    Logger() << "Write: " << path2string(filename);

    if (!createDirectoriesForFile((filename))) {
        Logger(Logger::Err) << "Failed to create directories for: " << path2string(filename);
        return false;
    }

    Mernel::PropertyTree data;
    m_appConfig.writeToProperty(data);

    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, data, true);
    if (!Mernel::writeFileFromBufferNoexcept(filename, buffer)) {
        Logger(Logger::Err) << "Failed to write: " << path2string(filename);
        return false;
    }
    return true;
}

bool ConfigHandler::loadConfig(const Mernel::std_path& filename)
{
    Logger() << "Read: " << path2string(filename);

    std::string          buffer;
    Mernel::PropertyTree data;
    if (!Mernel::readFileIntoBufferNoexcept((filename), buffer) || !readJsonFromBufferNoexcept(buffer, data)) {
        return false;
    }

    return loadConfig(data);
}

bool ConfigHandler::saveConfig(const Mernel::std_path& filename) const
{
    Logger() << "Write: " << path2string(filename);
    Mernel::PropertyTree data;
    data.convertToMap();
    saveConfig(data);
    if (!createDirectoriesForFile((filename))) {
        Logger(Logger::Err) << "Failed to create directories for: " << path2string(filename);
        return false;
    }

    std::string buffer;
    Mernel::writeJsonToBufferNoexcept(buffer, data, true);
    if (!Mernel::writeFileFromBufferNoexcept((filename), buffer)) {
        Logger(Logger::Err) << "Failed to write: " << path2string(filename);
        return false;
    }
    return true;
}

bool ConfigHandler::loadConfig(const Mernel::PropertyTree& data)
{
    m_config.readFromProperty(data);
    updateSeeds(false);

    m_config.notifyChangeInner();

    return true;
}

bool ConfigHandler::saveConfig(Mernel::PropertyTree& data) const
{
    data.convertToMap();
    m_config.writeToProperty(data);
    return true;
}

void ConfigHandler::updateSeeds(bool forceUpdate)
{
    std::random_device rd;
    if (m_config.m_itemRandomizer.m_seed == 0 || forceUpdate) {
        m_config.m_itemRandomizer.m_seed = rd();
        m_config.m_itemRandomizer.notifyChangeInner();
    }
    if (m_config.m_monsterRandomizer.m_seed == 0 || forceUpdate) {
        m_config.m_monsterRandomizer.m_seed = rd();
        m_config.m_monsterRandomizer.notifyChangeInner();
    }
    if (m_config.m_skillRandomizer.m_seed == 0 || forceUpdate) {
        m_config.m_skillRandomizer.m_seed = rd();
        m_config.m_skillRandomizer.notifyChangeInner();
    }
}

bool ConfigHandler::generate(const GenerationEnvironment& env)
{
    auto startAll = Mernel::ChronoPoint(true);
    updateSeeds(m_appConfig.m_refreshSeed);

    FolderStorage outDebugStorage(m_appData / "debug");
    FolderStorage outStorage(env.outputPath, env.outputMode);
    outStorage.setModName(env.modName);
    outStorage.setUseBaseSubolder(env.needDataSubfolder);
    outStorage.setSilentTrace(false);

    DataContext previousData;

    Logger(Logger::Notice) << "started in: " << path2string(env.outputPath);

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
        auto start = Mernel::ChronoPoint(true);

        ModuleGlobal all(output, m_config, env);
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
