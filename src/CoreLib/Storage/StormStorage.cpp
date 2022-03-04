/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "RAIIUtils.hpp"
#include "TableId.hpp"
#include "Logger.hpp"

#include <StormLib.h>

namespace D2ModGen {

IStorage::StoredData StormStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    const std::string utf8data  = m_storageRoot + "d2data.mpq";
    const std::string utf8patch = m_storageRoot + "patch_d2.mpq";
    bool              hasData   = true;
    HANDLE            mpq;
    if (!SFileOpenArchive(utf8data.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
        hasData = false;
        if (!SFileOpenArchive(utf8patch.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
            return {};
        } else {
            Logger() << "patch_d2.mpq found, but d2data.mpq is missing";
        }
    }

    MODGEN_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });

    if (hasData && !SFileOpenPatchArchive(mpq, utf8patch.c_str(), nullptr, STREAM_FLAG_READ_ONLY)) {
        Logger() << "d2data.mpq found, but patch_d2.mpq is missing";
        return {};
    }

    auto readStormFile = [mpq](std::string& data, const std::string& filename) -> bool {
        const std::string fullId = filename;
        if (!SFileHasFile(mpq, fullId.c_str())) {
            Logger() << "no such file:" << filename;
            return false;
        }

        HANDLE fileHandle;
        if (!SFileOpenFileEx(mpq, fullId.c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle)) {
            Logger(Logger::Warning) << "failed to open:" << fullId;
            return false;
        }
        MODGEN_SCOPE_EXIT([fileHandle] { SFileCloseFile(fileHandle); });

        auto dataSize = SFileGetFileSize(fileHandle, nullptr);
        data.resize(dataSize);
        DWORD wread;
        if (!SFileReadFile(fileHandle, data.data(), dataSize, &wread, nullptr)) {
            Logger(Logger::Warning) << "failed to read:" << fullId.c_str();
            return false;
        }

        return true;
    };

    StoredData result{ true };

    for (const std::string& id : getTableNames()) {
        std::string buffer;
        if (!readStormFile(buffer, IStorage::makeTableRelativePath(id, true)))
            continue;

        result.tables.push_back(StoredFileTable{ std::move(buffer), id });
    }
    for (const std::string& relativePath : filenames) {
        std::string buffer;
        if (!readStormFile(buffer, relativePath))
            continue;

        result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), relativePath });
    }
    return result;
}

}
