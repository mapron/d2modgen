/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "RAIIUtils.hpp"

#include <StormLib.h>

#include <QDebug>

namespace D2ModGen {

IStorage::Result StormStorage::ReadData(const QString& storageRoot, const RequestFileList& filenames) const noexcept
{
    const std::string utf8path = storageRoot.toStdString() + "patch_d2.mpq";
    HANDLE            mpq;
    if (!SFileOpenArchive(utf8path.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq))
        return {};

    MODGEN_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });

    auto readStormFile = [mpq](QByteArray& data, const QString& filename) -> bool {
        const std::string fullId = filename.toStdString();
        if (!SFileHasFile(mpq, fullId.c_str()))
            return false;

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

        return true;
    };

    IStorage::Result result{ true };

    for (const auto& filename : filenames) {
        QByteArray buffer;
        if (!readStormFile(buffer, filename.relFilepath))
            return {};
        result.files << StoredFile{ buffer, filename.relFilepath, filename.id };
    }
    return result;
}

}
