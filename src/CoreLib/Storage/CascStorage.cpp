/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "MernelPlatform/ScopeExit.hpp"
#include "TableId.hpp"
#include "Logger.hpp"

#ifdef _WIN32
#include <CascLib.h>

namespace D2ModGen {

IStorage::StoredData CascStorage::readData() const noexcept
{
    const Mernel::std_path utf8path = m_storageRoot;
    const std::wstring     wdata    = utf8path.wstring();
    HANDLE                 storage;

    CASC_OPEN_STORAGE_ARGS openArgs = { sizeof(CASC_OPEN_STORAGE_ARGS) };

    openArgs.dwFlags = CASC_FEATURE_ALLOW_DOWNLOAD;
    if (!CascOpenStorageEx(wdata.c_str(), &openArgs, false, &storage)) {
        Logger(Logger::Warning) << "failed to open storage:" << path2string(utf8path) << ", err=" << GetCascError();
        return {};
    } else {
        Logger() << "Opened CASC:" << path2string(utf8path);
    }
    MERNEL_SCOPE_EXIT([storage] { CascCloseStorage(storage); });

    auto readCascFile = [storage](std::string& data, const std::string& filename) -> bool {
        const std::string fullId = "data:" + filename;
        HANDLE            fileHandle;

        if (!CascOpenFile(storage, fullId.c_str(), CASC_LOCALE_ALL, 0, &fileHandle)) {
            Logger(Logger::Warning) << "failed to open:" << fullId << ", err=" << GetCascError();
            return false;
        }
        MERNEL_SCOPE_EXIT([fileHandle] { CascCloseFile(fileHandle); });

        auto dataSize = CascGetFileSize(fileHandle, nullptr);

        data.resize(dataSize);
        DWORD wread;
        if (!CascReadFile(fileHandle, data.data(), dataSize, &wread)) {
            Logger(Logger::Warning) << "failed to read:" << fullId << ", err=" << GetCascError();
            return false;
        }
        return true;
    };

    IStorage::StoredData result{ true };

    for (TableId id : getTableIds()) {
        std::string buffer;
        if (!readCascFile(buffer, IStorage::makeTableRelativePath(id, m_needBaseSubfolder)))
            continue;

        result.tables[id] = std::move(buffer);
    }
    for (JsonId id : getJsonIds()) {
        std::string buffer;
        if (!readCascFile(buffer, std::string(getJsonRelPath(id))))
            continue;

        result.jsonFiles[id] = std::move(buffer);
    }

    std::string buffer;
    if (readCascFile(buffer, std::string(IStorage::s_dataVersionPath))) {
        result.dataVersion = std::atoll(buffer.c_str());
    }
    result.valid = !result.tables.empty();

    return result;
}

}
#else
namespace D2ModGen {

IStorage::StoredData CascStorage::readData() const noexcept
{
    return {};
}
}
#endif
