/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "RAIIUtils.hpp"
#include "StorageConstants.hpp"

#include <StormLib.h>

#include <QDebug>

namespace D2ModGen {

IStorage::StoredData StormStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    const std::string utf8data  = m_storageRoot.toStdString() + "d2data.mpq";
    const std::string utf8patch = m_storageRoot.toStdString() + "patch_d2.mpq";
    bool              hasData   = true;
    HANDLE            mpq;
    if (!SFileOpenArchive(utf8data.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
        hasData = false;
        if (!SFileOpenArchive(utf8patch.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
            return {};
        } else {
            qDebug() << "patch_d2.mpq found, but d2data.mpq is missing";
        }
    }

    MODGEN_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });

    if (hasData && !SFileOpenPatchArchive(mpq, utf8patch.c_str(), nullptr, STREAM_FLAG_READ_ONLY)) {
        qDebug() << "d2data.mpq found, but patch_d2.mpq is missing";
        return {};
    }

    auto readStormFile = [mpq](QByteArray& data, const QString& filename) -> bool {
        const std::string fullId = filename.toStdString();
        if (!SFileHasFile(mpq, fullId.c_str())) {
            qDebug() << "no such file:" << filename;
            return false;
        }

        HANDLE fileHandle;
        if (!SFileOpenFileEx(mpq, fullId.c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle)) {
            qWarning() << "failed to open:" << fullId.c_str();
            return false;
        }
        MODGEN_SCOPE_EXIT([fileHandle] { SFileCloseFile(fileHandle); });

        auto dataSize = SFileGetFileSize(fileHandle, nullptr);
        data.resize(dataSize);
        DWORD wread;
        if (!SFileReadFile(fileHandle, data.data(), dataSize, &wread, nullptr)) {
            qWarning() << "failed to read:" << fullId.c_str();
            return false;
        }

        return true;
    };

    StoredData result{ true };

    for (const QString& id : g_tableNames) {
        QByteArray buffer;
        if (!readStormFile(buffer, IStorage::makeTableRelativePath(id, true)))
            continue;

        result.tables.push_back(StoredFileTable{ std::move(buffer), id });
    }
    for (const QString& relativePath : filenames) {
        QByteArray buffer;
        if (!readStormFile(buffer, relativePath))
            continue;

        result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), relativePath });
    }
    return result;
}

}
