/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "RAIIUtils.hpp"
#include "TableId.hpp"
#include "Logger.hpp"
#include "FileIOUtils.hpp"

#include <CascLib.h>

namespace D2ModGen {

IStorage::StoredData CascStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    const std::string  utf8path = m_storageRoot;
    const std::wstring wdata    = string2path(utf8path).wstring();
    HANDLE             storage;
    if (!CascOpenStorage(wdata.c_str(), 0, &storage)) {
        Logger(Logger::Warning) << "failed to open storage:" << utf8path;
        return {};
    }
    MODGEN_SCOPE_EXIT([storage] { CascCloseStorage(storage); });

    auto readCascFile = [storage](std::string& data, const std::string& filename) -> bool {
        const std::string fullId = "data:" + filename;
        HANDLE            fileHandle;

        if (!CascOpenFile(storage, fullId.c_str(), CASC_LOCALE_ALL, 0, &fileHandle)) {
            Logger(Logger::Warning) << "failed to open:" << fullId;
            return false;
        }
        MODGEN_SCOPE_EXIT([fileHandle] { CascCloseFile(fileHandle); });

        auto dataSize = CascGetFileSize(fileHandle, nullptr);

        data.resize(dataSize);
        DWORD wread;
        if (!CascReadFile(fileHandle, data.data(), dataSize, &wread)) {
            Logger(Logger::Warning) << "failed to read:" << fullId;
            return false;
        }
        return true;
    };

    IStorage::StoredData result{ true };

    for (const std::string& id : getTableNames()) {
        std::string buffer;
        if (!readCascFile(buffer, IStorage::makeTableRelativePath(id, true, m_needBaseSubfolder)))
            continue;

        result.tables.push_back(StoredFileTable{ std::move(buffer), id });
    }
    for (const std::string& relativePath : filenames) {
        std::string buffer;
        if (!readCascFile(buffer, relativePath))
            continue;

        result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), relativePath });
    }
    return result;
}

}
