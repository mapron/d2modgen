/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "RAIIUtils.hpp"
#include "TableId.hpp"
#include "Logger.hpp"
#include "FileIOUtils.hpp"

#include <StormLib.h>

namespace D2ModGen {

IStorage::StoredData StormStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    const std::string utf8data  = m_storageRoot + "d2data.mpq";
    const std::string utf8data2 = m_storageRoot + "pd2data.mpq";
    const std::string utf8patch = m_storageRoot + "patch_d2.mpq";

    const std::wstring basedata  = string2path(utf8data).wstring();
    const std::wstring basedata2 = string2path(utf8data2).wstring();
    const std::wstring patchdata = string2path(utf8patch).wstring();

    bool   hasData   = true;
    bool   needPatch = true;
    HANDLE mpq;
    if (!SFileOpenArchive(basedata.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
        if (Mernel::std_fs::exists(Mernel::string2path(utf8data2))) {
            if (!SFileOpenArchive(basedata2.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
                hasData = false;
            } else {
                needPatch = false;
            }
        } else {
            hasData = false;
        }
    }
    if (!hasData) {
        return {};
    }

    MODGEN_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });

    if (needPatch && !SFileOpenPatchArchive(mpq, patchdata.c_str(), nullptr, STREAM_FLAG_READ_ONLY)) {
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
