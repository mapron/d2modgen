/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "MernelPlatform/ScopeExit.hpp"

#include "Logger.hpp"

#include <CascLib.h>

namespace D2ModGen {

IStorage::StoredData CascStorage::readData() const noexcept
{
    const Mernel::std_path utf8path = m_storageRoot;
    const auto             wdata    = utf8path.native();
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

    for (TableId id : getTableIdsRead()) {
        std::string buffer;
        if (!readCascFile(buffer, IStorage::makeTableRelativePath(id, false)))
            continue;

        result.tablesMain[id] = std::move(buffer);
    }
    for (TableId id : getTableIdsRead()) {
        std::string buffer;
        if (!readCascFile(buffer, IStorage::makeTableRelativePath(id, true)))
            continue;

        result.tablesBase[id] = std::move(buffer);
    }
    for (JsonId id : getJsonIdsRead()) {
        std::string buffer;
        if (!readCascFile(buffer, std::string(getJsonRelPath(id))))
            continue;

        result.jsonFiles[id] = std::move(buffer);
    }
    for (BlobId id : getBlobIdsLegacyIcons()) {
        std::string buffer;
        if (!readCascFile(buffer, std::string(getBlobRelPath(id))))
            continue;

        result.blobs[id] = std::move(buffer);
    }
    for (BlobId id : getBlobIdsResurrected()) {
        std::string buffer;
        if (!readCascFile(buffer, std::string(getBlobRelPath(id))))
            continue;

        result.blobs[id] = std::move(buffer);
    }

    std::string buffer;
    if (readCascFile(buffer, std::string(IStorage::s_dataVersionPath))) {
        result.dataVersion = std::atoll(buffer.c_str());
    }
    result.valid = !result.tablesMain.empty();

    return result;
}

}
