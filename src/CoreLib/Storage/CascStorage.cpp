/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "MernelPlatform/ScopeExit.hpp"
#include "MernelPlatform/Profiler.hpp"

#include "Logger.hpp"

#include <CascLib.h>

namespace D2ModGen {

namespace {

/// unused. Maybe re-think it later.
struct CacheStorage {
public:
    std_path m_root;

    bool writeFile(const std::string& data, const std_path& relPath) const
    {
        const auto absPath = m_root / toFsPath(relPath);
        if (!createDirectoriesForFile(absPath))
            return false;

        if (std_fs::exists(absPath))
            std_fs::remove(absPath);
        if (data.empty())
            return true;

        auto res = Mernel::writeFileFromBufferNoexcept(absPath, data);
        return res;
    }

    bool readFile(std::string& data, uint32_t expectedSize, const std_path& relPath) const
    {
        const auto absPath = m_root / toFsPath(relPath);
        if (!std_fs::exists(absPath))
            return false;

        if (std_fs::file_size(absPath) != expectedSize)
            return false;

        if (!Mernel::readFileIntoBufferNoexcept(absPath, data))
            return false;

        return true;
    }
};

}

IStorage::StoredData CascStorage::readData() const noexcept
{
    const Mernel::std_path utf8path = m_storageRoot;
    const auto             wdata    = utf8path.native();
    HANDLE                 storage;

    CASC_OPEN_STORAGE_ARGS openArgs = { sizeof(CASC_OPEN_STORAGE_ARGS) };

    openArgs.dwFlags = CASC_FEATURE_ALLOW_DOWNLOAD;
    {
        Mernel::ProfilerScope scope("CascOpenStorageEx");
        if (!CascOpenStorageEx(wdata.c_str(), &openArgs, false, &storage)) {
            Logger(Logger::Warning) << "failed to open storage:" << path2string(utf8path) << ", err=" << GetCascError();
            return {};
        } else {
            Logger(m_verbose) << "Opened CASC: " << path2string(utf8path);
        }
    }
    MERNEL_SCOPE_EXIT([storage] { CascCloseStorage(storage); });

    auto readCascFile = [storage, this](std::string& data, const std::string& filename, bool useCache = false) -> bool {
        const std::string fullId = "data:" + filename;
        HANDLE            fileHandle;

        if (!CascOpenFile(storage, fullId.c_str(), CASC_LOCALE_ALL, 0, &fileHandle)) {
            Logger(Logger::Warning) << "failed to open:" << fullId << ", err=" << GetCascError();
            return false;
        }
        MERNEL_SCOPE_EXIT([fileHandle] { CascCloseFile(fileHandle); });

        auto dataSize = CascGetFileSize(fileHandle, nullptr);

        data.resize(dataSize);
        DWORD                 wread;
        Mernel::ProfilerScope scope("CascReadFile");
        if (!CascReadFile(fileHandle, data.data(), dataSize, &wread)) {
            Logger(Logger::Warning) << "failed to read:" << fullId << ", err=" << GetCascError();
            return false;
        }

        Logger(m_verbose) << "Read: " << filename << " [size: " << data.size() << "]";
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
    for (BlobId id : getBlobIdsResurrected()) {
        std::string buffer;
        if (!readCascFile(buffer, std::string(getBlobRelPath(id)), true))
            continue;

        result.blobs[id] = std::move(buffer);
    }

    std::string buffer;
    if (readCascFile(buffer, std::string(IStorage::s_dataVersionPath))) {
        result.dataVersion = std::atoll(buffer.c_str());
    }
    result.valid = !result.tablesMain.empty();
    Logger(Logger::Info) << "Read CASC result = " << result.getStatistics();

    return result;
}

}
