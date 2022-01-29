/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StorageCache.hpp"

#include "FileIOUtils.hpp"

#include "CascStorage.hpp"
#include "StormStorage.hpp"

#include <QDebug>
#include <QJsonDocument>

namespace D2ModGen {

DataContextPtr StorageCache::LoadImpl(const Context& context)
{
    auto extractTables = [](DataContext&         output,
                            const IInputStorage& storage,
                            const QString&       root,
                            const QStringList&   tableIds,
                            const JsonFileSet&   jsonFiles) -> bool {
        IInputStorage::RequestFileList filenames;
        for (const QString& id : tableIds)
            filenames << IInputStorage::RequestFile{ IStorage::makeTableRelativePath(id, true), id };
        for (const QString& path : jsonFiles)
            filenames << IInputStorage::RequestFile{ path };

        auto result = storage.readData(root, filenames);
        if (!result.success)
            return false;

        for (const auto& fileData : result.files) {
            if (!fileData.id.isEmpty()) {
                QString data = QString::fromUtf8(fileData.data);
                Table   table;
                table.id = fileData.id;
                if (!readCSV(data, table)) {
                    qWarning() << "failed to parse csv:" << fileData.relFilepath;
                    return false;
                }
                output.tableSet.tables[fileData.id] = std::move(table);
            } else if (jsonFiles.contains(fileData.relFilepath)) {
                QJsonParseError err;
                auto            loadDoc(QJsonDocument::fromJson(fileData.data, &err));
                if (loadDoc.isNull()) {
                    qWarning() << "failed to parse json:" << fileData.relFilepath << ", " << err.errorString();
                    return false;
                }
                output.jsonFiles[fileData.relFilepath] = loadDoc;
            } else {
                qWarning() << "Unexpected file got from Read(): " << fileData.relFilepath;
            }
        }

        return true;
    };
    IInputStorage::Ptr storage;
    if (context.m_storage == StorageType::D2ResurrectedInternal)
        storage = std::make_shared<CascStorage>();
    else if (context.m_storage == StorageType::D2LegacyInternal)
        storage = std::make_shared<StormStorage>();
    else {
        assert(!"TODO");
        return nullptr;
    }

    DataContextPtr result(new DataContext());
    if (!extractTables(*result, *storage, context.m_root, context.m_tableIds, context.m_jsonFiles))
        return nullptr;

    return result;
}

}
