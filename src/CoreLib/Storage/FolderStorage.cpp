/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "FolderStorage.hpp"

#include "FileIOUtils.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include <cassert>

namespace D2ModGen {

namespace {
static const std::string s_tableSubfolder = "data\\global\\excel\\";

void removeRecursively(const std_path& folder)
{
    std::deque<std_path> forDelete;
    for (auto& it : std_fs::recursive_directory_iterator(folder)) {
        if (it.is_regular_file()) {
            const std_path& path = it.path();

            std::error_code ec;
            std_fs::remove(path, ec);
        } else if (it.is_directory()) {
            forDelete.push_back(it.path());
        }
    }
    std::reverse(forDelete.begin(), forDelete.end());
    for (const std_path& path : forDelete) {
        std::error_code ec;
        std_fs::remove(path, ec);
    }
}

}

FolderStorage::FolderStorage(const std_path& storageRoot,
                             StorageType     storage)
    : m_storageType(storage)
    , m_root(storageRoot)
{
}

std_path FolderStorage::getRoot() const
{
    std_path root = m_root;
    if (m_storageType == StorageType::D2ResurrectedModFolder)
        root = m_root / "mods" / m_modname / (m_modname + ".mpq");
    return root;
}

bool FolderStorage::writeFile(const std::string& data, const std_path& relPath) const
{
    const auto absPath = getRoot() / toFsPath(relPath);
    if (!createDirectoriesForFile(absPath))
        return false;

    if (std_fs::exists(absPath))
        std_fs::remove(absPath);
    if (data.empty())
        return true;

    auto res = Mernel::writeFileFromBufferNoexcept(absPath, data);
    if (!m_silent) {
        if (res) {
            Logger() << "Write: " << path2string(relPath) << " [size: " << data.size() << "]";
        } else {
            Logger(Logger::Err) << "Failed to write file:" << relPath;
        }
    }
    return res;
}

bool FolderStorage::readFile(std::string& data, const std_path& relPath, bool warnNonExistent) const
{
    const auto absPath = getRoot() / toFsPath(relPath);
    if (!warnNonExistent && !std_fs::exists(absPath))
        return false;

    auto res = Mernel::readFileIntoBufferNoexcept(absPath, data);
    if (!m_silent) {
        if (res) {
            Logger() << "Read: " << path2string(relPath) << " [size: " << data.size() << "]";
        } else {
            Logger(Logger::Err) << "Failed to read file:" << relPath;
        }
    }
    return res;
}

IStorage::StoredData FolderStorage::readData() const noexcept
{
    StoredData result{ true };

    if (m_storageType == StorageType::None)
        return result;

    if (!m_silent) {
        Logger(Logger::Notice) << "Start reading from root:" << getRoot();
    }

    const auto root = getRoot();

    for (TableId id : getTableIdsRead()) {
        std::string       buffer;
        const std::string relPath = m_storageType == StorageType::CsvFolder ? std::string(getTableIdString(id)) + ".txt" : makeTableRelativePath(id, false);
        if (!readFile(buffer, relPath, true))
            continue;

        result.tablesMain[id] = std::move(buffer);
    }
    if (m_storageType != StorageType::CsvFolder) {
        for (TableId id : getTableIdsRead()) {
            std::string       buffer;
            const std::string relPath = makeTableRelativePath(id, true);
            if (!readFile(buffer, relPath))
                continue;

            result.tablesBase[id] = std::move(buffer);
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        for (JsonId id : getJsonIdsRead()) {
            std::string buffer;
            if (!readFile(buffer, std::string(getJsonRelPath(id))))
                continue;

            result.jsonFiles[id] = std::move(buffer);
        }
        for (BlobId id : getBlobIdsLegacy()) {
            std::string buffer;
            if (!readFile(buffer, std::string(getBlobRelPath(id))))
                continue;

            result.blobs[id] = std::move(buffer);
        }
        for (BlobId id : getBlobIdsResurrected()) {
            std::string buffer;
            if (!readFile(buffer, std::string(getBlobRelPath(id))))
                continue;

            result.blobs[id] = std::move(buffer);
        }
    }

    result.valid = !(result.tablesMain.empty() && result.tablesBase.empty());

    return result;
}

bool FolderStorage::create() const noexcept
{
    if (m_storageType == StorageType::None)
        return true;

    if (!m_silent) {
        Logger(Logger::Notice) << "Start creating: " << getRoot();
    }

    const auto root = getRoot();
    const auto data = root / "data";
    if (m_storageType != StorageType::CsvFolder) {
        if (std_fs::exists(data)) {
            const auto global = data / "global";
            if (!std_fs::exists(global)) {
                Logger(Logger::Err) << "There is /data/ subfolder, but no /data/global/. Did you select output mode as Full Folders for D2R?";
                return false;
            }

            removeRecursively(data);
        }
    }

    if (!createDirectories(root)) {
        Logger(Logger::Err) << "Failed to create: " << path2string(root);
        return false;
    }

    if (m_storageType != StorageType::CsvFolder) {
        const auto excelRoot = root / s_tableSubfolder;
        if (!createDirectories(excelRoot)) {
            Logger(Logger::Err) << "Failed to create: " + path2string(excelRoot);
            return false;
        }
    }

    return true;
}

bool FolderStorage::writeMeta(const StoredData& data) const noexcept
{
    if (m_storageType != StorageType::D2ResurrectedModFolder)
        return true;

    if (!m_silent) {
        Logger(Logger::Info) << "Write meta to: " << getRoot();
    }

    const auto           root     = getRoot();
    auto                 jsonPath = root / "modinfo.json";
    Mernel::PropertyTree modinfo;
    modinfo.convertToMap();
    modinfo["name"]     = Mernel::PropertyTreeScalar(m_modname);
    modinfo["savepath"] = Mernel::PropertyTreeScalar(m_modname + "/");
    std::string buffer;
    writeJsonToBufferNoexcept(buffer, modinfo, true);
    if (!writeFile(buffer, jsonPath))
        return false;

    const auto absPath = root / IStorage::s_dataVersionPath;
    if (data.dataVersion && !writeFile(std::to_string(data.dataVersion), absPath))
        return false;

    return true;
}

bool FolderStorage::writeMainData(const StoredData& data) const noexcept
{
    if (m_storageType == StorageType::None)
        return true;

    const auto root = getRoot();
    if (!m_silent) {
        Logger(Logger::Notice) << "Write data to: " << getRoot();
    }
    for (const auto& [id, tableData] : data.tablesMain) {
        const std::string relPath = m_storageType == StorageType::CsvFolder ? std::string(getTableIdString(id)) + m_suffix + ".txt" : makeTableRelativePath(id, false);
        const auto        absPath = root / relPath;
        if (!writeFile(tableData, absPath)) {
            return false;
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        for (const auto& [id, tableData] : data.tablesBase) {
            const std::string relPath = makeTableRelativePath(id, true);
            const auto        absPath = root / relPath;
            if (!writeFile(tableData, absPath)) {
                return false;
            }
        }
        for (const auto& [id, memoryData] : data.jsonFiles) {
            const auto absPath = root / std::string(getJsonRelPath(id));
            if (!writeFile(memoryData, absPath)) {
                return false;
            }
        }
        for (const auto& [id, memoryData] : data.blobs) {
            const auto absPath = root / std::string(getBlobRelPath(id));
            if (!writeFile(memoryData, absPath)) {
                return false;
            }
        }
    }

    return true;
}

bool FolderStorage::writeBitmaps(const StoredData& data) const noexcept
{
    if (m_storageType == StorageType::None)
        return true;
    if (data.bitmaps.empty())
        return true;

    if (!m_silent) {
        Logger(Logger::Info) << "Write bitmaps to: " << getRoot();
    }
    const auto root = getRoot();
    for (auto& [key, buffer] : data.bitmaps) {
        auto path = root / (key + m_suffix + ".bmp");
        if (!writeFile(buffer, path)) {
            return false;
        }
    }
    return false;
}

}
