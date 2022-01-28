/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "FileIOUtils.hpp"
#include "RAIIUtils.hpp"
#include "StorageConstants.hpp"

#include <StormLib.h>

#include <QJsonDocument>

bool ExtractTablesLegacy(const QString& d2rpath, GenOutput& output)
{
    const std::string utf8path = d2rpath.toStdString() + "patch_d2.mpq";
    HANDLE            mpq;
    if (!SFileOpenArchive(utf8path.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq))
        return false;

    MODGEN_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });
    for (const QString& id : g_tableNames) {
        const std::string fullId = QString("data\\global\\excel\\%1.txt").arg(id).toStdString();
        if (!SFileHasFile(mpq, fullId.c_str()))
            continue;

        HANDLE fileHandle;
        if (!SFileOpenFileEx(mpq, fullId.c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle)) {
            qWarning() << "failed to open:" << fullId.c_str();
            return false;
        }
        MODGEN_SCOPE_EXIT([fileHandle] { SFileCloseFile(fileHandle); });

        auto       dataSize = SFileGetFileSize(fileHandle, nullptr);
        QByteArray buffer;
        buffer.resize(dataSize);
        DWORD wread;
        if (!SFileReadFile(fileHandle, buffer.data(), dataSize, &wread, nullptr)) {
            qWarning() << "failed to read:" << fullId.c_str();
            return false;
        }

        QString data = QString::fromUtf8(buffer);
        Table   table;
        table.id = id;
        if (!readCSV(data, table)) {
            qWarning() << "failed to parse:" << fullId.c_str();
            return false;
        }

        output.tableSet.tables[id] = std::move(table);
    }
    if (output.tableSet.tables.empty())
        return false;

    return true;
}
