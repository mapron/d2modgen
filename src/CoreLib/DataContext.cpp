/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "FileIOUtils.hpp"
#include "Logger.hpp"
#include "MernelPlatform/FileFormatCSV.hpp"
#include "MernelPlatform/FileFormatJson.hpp"

namespace D2ModGen {

DataContext::DataContext() = default;

DataContext::~DataContext() = default;

bool DataContext::readData(const IStorage::StoredData& data)
{
    dataVersion = data.dataVersion;
    for (const auto& fileData : data.tables) {
        if (fileData.id.empty()) {
            Logger(Logger::Warning) << "Empty csv id found!";
            return false;
        }
        TableId id;
        if (!findTableId(fileData.id, id)) {
            Logger() << "Skip table:" << fileData.id;
            continue;
        }

        Table table;
        table.id = id;
        Logger() << "read txt table:" << fileData.id;
        if (!readCSVFromBuffer(fileData.data, table)) {
            Logger(Logger::Warning) << "failed to parse csv:" << fileData.id;
            return false;
        }
        table.valid         = true;
        tableSet.tables[id] = std::move(table);
        tableSet.relativeNames.insert(string2path(IStorage::makeTableRelativePath(fileData.id, false, false)));
    }
    for (const auto& fileData : data.inMemoryFiles) {
        if (fileData.data.empty()) {
            Logger(Logger::Warning) << "Json data is empty:" << fileData.relFilepath;
            return false;
        }

        Mernel::PropertyTree doc;
        Logger() << "read json:" << fileData.relFilepath;
        if (!readJsonFromBufferNoexcept(fileData.data, doc)) {
            Logger(Logger::Warning) << "failed to parse json:" << fileData.relFilepath;
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "duplicate json file found:" << fileData.relFilepath;
            return false;
        }

        jsonFiles[fileData.relFilepath] = doc;
    }
    for (const auto& fileData : data.refFiles) {
        std::error_code ec;
        if (!std_fs::exists(fileData.absSrcFilepath, ec)) {
            Logger(Logger::Warning) << "Non-existent file:" << fileData.absSrcFilepath;
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "File for plain copy already exists in json list:" << fileData.relFilepath;
            return false;
        }
        if (tableSet.relativeNames.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "File for plain copy already exists in CSV list:" << fileData.relFilepath;
            return false;
        }
        if (copyFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "duplicate copy file found:" << fileData.relFilepath;
            return false;
        }
        copyFiles[fileData.relFilepath] = fileData;
    }

    return true;
}

bool DataContext::writeData(IStorage::StoredData& data) const
{
    data.dataVersion = dataVersion;
    for (const auto& p : tableSet.tables) {
        const Table& table = p.second;
        if (!table.modified)
            continue;

        if (!table.valid) {
            Logger(Logger::Warning) << "Skipped invalid table that was modified: " << getTableIdString(p.first);
            continue;
        }

        std::string tableStr;
        if (!writeCSVToBuffer(tableStr, table))
            return false;
        data.tables.push_back(IStorage::StoredFileTable{ std::move(tableStr), getTableIdString(table.id) });
    }
    for (const auto& p : jsonFiles) {
        std::string buffer;
        writeJsonToBufferNoexcept(buffer, p.second);
        data.inMemoryFiles.push_back(IStorage::StoredFileMemory{ buffer, p.first });
    }
    for (const auto& p : copyFiles) {
        data.refFiles.push_back(p.second);
    }
    return true;
}

}
