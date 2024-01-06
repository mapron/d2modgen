/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "TableUtils.hpp"
#include "FileIOUtils.hpp"
#include "Logger.hpp"
#include "MernelPlatform/FileFormatCSV.hpp"
#include "MernelPlatform/FileFormatJson.hpp"

namespace D2ModGen {

DataContext::DataContext() = default;

DataContext::~DataContext() = default;

bool DataContext::readData(const IStorage::StoredData& data)
{
    for (const auto& fileData : data.tables) {
        if (fileData.id.empty()) {
            Logger(Logger::Warning) << "Empty csv id found!";
            return false;
        }
        TableId id;
        if (!findTableId(fileData.id, id)) {
            Logger(Logger::Warning) << "Unknown table id:" << fileData.id;
            return false;
        }

        Table table;
        table.id = id;
        //Logger() << "read txt:" << fileData.id;
        if (!readCSVFromBuffer(fileData.data, table)) {
            Logger(Logger::Warning) << "failed to parse csv:" << fileData.id;
            return false;
        }
        tableSet.tables[id] = std::move(table);
        tableSet.relativeNames.insert(string2path(IStorage::makeTableRelativePath(fileData.id, false)));
    }
    for (const auto& fileData : data.inMemoryFiles) {
        if (fileData.data.empty()) {
            Logger(Logger::Warning) << "Json data is empty:" << fileData.relFilepath;
            return false;
        }

        Mernel::PropertyTree doc;
        //Logger() << "read json:" << fileData.relFilepath;
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
    for (const auto& p : tableSet.tables) {
        const Table& table = p.second;
        if (!table.modified && !table.forceOutput)
            continue;

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

bool DataContext::mergeWith(const DataContext& source, ConflictPolicy policy)
{
    for (const auto& p : source.tableSet.tables) {
        const Table& sourceTable = p.second;
        Table&       destTable   = tableSet.tables[sourceTable.id];
        if (destTable.modified) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }

        const TableView sourceView(sourceTable);
        TableView       destView(destTable);
        destTable.modified = true;

        // already modified, need to resolve conflicts:
        switch (policy) {
            case ConflictPolicy::Skip:
            case ConflictPolicy::RaiseError:
            case ConflictPolicy::Replace: // clear previous data
            {
                destView.clear();
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Append: // place all data at the end
            {
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Update: // overwrite records with same key
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, false, true);
                break;
            }
            case ConflictPolicy::AppendNew: // place new data at the end
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, false);
                break;
            }
            case ConflictPolicy::Merge: // try Update, then AppendNew
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, true);
                break;
            }
        };
    }

    for (const auto& p : source.jsonFiles) {
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        jsonFiles[p.first] = p.second;
    }

    for (const auto& p : source.copyFiles) {
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        if (tableSet.relativeNames.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        copyFiles[p.first] = p.second;
    }

    return true;
}

}
