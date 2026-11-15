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
#include "MernelPlatform/Profiler.hpp"

#include "Modules/ModuleGlobal.hpp"

#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include "MernelPlatform/AppLocations.hpp"

#include <random>
#include <format>

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
    if (m_config.m_globalSeed == 0 || forceUpdate) {
        m_config.m_globalSeed = rd();
        m_config.notifyChangeInner();
    }
}

bool ConfigHandler::generate(const GenerationEnvironment& env, std::string& crc)
{
    using namespace Mernel;
    auto generateTotal = std::make_unique<ProfilerScope>("ConfigHandler::generate");
    updateSeeds(m_appConfig.m_refreshSeed);

    const bool fileIOVerbose    = env.extraTrace.contains("file_io");
    const bool serializeVerbose = env.extraTrace.contains("serialize");

    FolderStorage outDebugStorage(m_appData / "debug");

    DataContext previousData;
    previousData.useBaseTable = env.needDataSubfolder;

    Logger(Logger::Notice) << "started generate in: " << path2string(env.outputPath);

    {
        FolderStorage outStoragePrev(env.outputPath, env.outputMode);
        outStoragePrev.setModName(env.modName);
        IStorage::StoredData previousStoredData = outStoragePrev.readData();

        if (previousStoredData.valid) {
            ProfilerScope scope("previousData.parseData");
            previousData.parseData(previousStoredData, false);
        }
    }

    DataContext output;
    DataContext input;
    output.useBaseTable = env.needDataSubfolder;
    output.verbose      = serializeVerbose;

    Logger() << "Loading data from main storage...";
    IStorage::StoredData inData;
    {
        ProfilerScope scope("mainStorage.load");
        inData = m_mainStorageCache->load({ env.inputMode, env.inputPath }, fileIOVerbose);
    }
    if (!inData.valid) {
        Logger(Logger::Err) << "Failed to read data files from D2 folder.";
        return false;
    }

    {
        ProfilerScope scope("output.parseData");
        Logger() << "Parsing files into object data...";
        if (!output.parseData(inData)) {
            Logger(Logger::Err) << "Failed parse D2 data files.";
            return false;
        }
        input.copyDataFrom(output);
        output.verbose = false; // so other can copy me with no traces
    }
    if (env.extraTrace.contains("dump_in")) {
        outDebugStorage.setFileSuffix(".in");
        if (outDebugStorage.create())
            outDebugStorage.writeMainData(output.writeDebug());
    }

    {
        output.verbose = serializeVerbose;

        ProfilerScope scope("all.generate");
        ConfigHasher  hasher;
        m_config.calchash(hasher);
        crc = std::format("{:08X}", hasher.hashv);

        ModuleGlobal all(crc, output, m_config, env);
        try {
            Logger() << "generate() start";
            all.generate();
        }
        catch (const std::exception& ex) {
            Logger(Logger::Err) << ex.what();
            return false;
        }
    }

    if (env.extraTrace.contains("dump_out")) {
        outDebugStorage.setFileSuffix(".out");
        if (outDebugStorage.create())
            outDebugStorage.writeMainData(output.writeDebug());
    }

    if (env.extraTrace.contains("dump_diff")) {
        outDebugStorage.setFileSuffix(".diff");
        if (outDebugStorage.create()) {
            DataContext diff;
            output.calculateDiff(diff, input);
            outDebugStorage.writeMainData(diff.writeDebug());
        }
    }

    IStorage::StoredData outData;
    {
        output.removeIdenticalRecords(input);

        output.verbose = serializeVerbose;
        Logger() << "Serializing object data into files...";
        ProfilerScope scope("output.serializeData");
        if (!output.serializeData(outData))
            return false;
    }

    if (env.needDataSubfolder) {
        // if we target pre-RotW, still generate copy of monstats tables in excel/.
        // Need to use new column layout, that's why we read them again from inData.
        IStorage::StoredData data;
        DataContext          baseOutput;
        baseOutput.useBaseTable = false; // switch to tablesMain read/write
        baseOutput.parseData(inData);
        baseOutput.monstats.rows   = output.monstats.rows;
        baseOutput.monstatsEx.rows = output.monstatsEx.rows;

        baseOutput.verbose = false;

        ProfilerScope scope("outputCopy.serializeData");
        if (!baseOutput.serializeData(data))
            return false;

        outData.tablesMain.clear();
        outData.tablesMain[TableId::monstats]  = data.tablesMain[TableId::monstats];
        outData.tablesMain[TableId::monstats2] = data.tablesMain[TableId::monstats2];
    }

    FolderStorage outStorage(env.outputPath, env.outputMode);
    outStorage.setModName(env.modName);
    outStorage.setVerbose(fileIOVerbose);
    if (!outStorage.create())
        return false;

    if (env.extraTrace.contains("dump_bmp")) {
        ProfilerScope scope("outDebugStorageBmp.writeBitmaps");
        FolderStorage outDebugStorageBmp(m_appData / "debugBitmaps");
        if (outDebugStorageBmp.create())
            outDebugStorageBmp.writeBitmaps(outData);
    }

    {
        ProfilerScope scope("outStorage.write");
        if (!outStorage.writeMainData(outData) || !outStorage.writeMeta(outData))
            return false;
    }

    if (env.extraTrace.contains("profiler")) {
        generateTotal.reset();
        Logger() << "Execution profile:\n"
                 << ProfilerScope::printToStr();
    }

    return true;
}

ConfigHandler::~ConfigHandler() = default;

}
