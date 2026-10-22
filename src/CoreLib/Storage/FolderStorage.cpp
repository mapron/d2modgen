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
static const std::string s_tableBase      = s_tableSubfolder + "base\\";

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

std::string IStorage::makeTableRelativePath(TableId id, bool baseSub)
{
    const std::string& base = baseSub ? s_tableBase : s_tableSubfolder;
    return base + std::string(getTableIdString(id)) + ".txt";
}

FolderStorage::FolderStorage(const std_path&    storageRoot,
                             StorageType        storage,
                             const std::string& modname,
                             bool               needBaseSubfolder,
                             std::string        suffix)
    : m_storageType(storage)
    , m_modName(modname)
    , m_root(storage == StorageType::D2ResurrectedModFolder ? storageRoot / "mods" / modname / (modname + ".mpq") : (storageRoot / "fake").parent_path())
    , m_needBaseSubfolder(needBaseSubfolder)
    , m_suffix(suffix)
{
}

IStorage::StoredData FolderStorage::readData() const noexcept
{
    StoredData result{ true };

    if (m_storageType == StorageType::None)
        return result;

    const auto rootLength = path2string(m_root).size() + 1;

    auto readFile = [this](std::string& data, const std::string& relPath) -> bool {
        const auto absPath = m_root / relPath;
        if (!Mernel::readFileIntoBufferNoexcept(absPath, data))
            return false;

        Logger() << "read  " << path2string(absPath);
        return true;
    };

    for (TableId id : getTableIds()) {
        std::string       buffer;
        const std::string relPath = m_storageType == StorageType::CsvFolder ? std::string(getTableIdString(id)) + ".txt" : makeTableRelativePath(id, m_needBaseSubfolder);
        if (!readFile(buffer, relPath))
            continue;

        result.tables[id] = std::move(buffer);
    }
    if (m_storageType != StorageType::CsvFolder) {
        for (JsonId id : getJsonIds()) {
            std::string buffer;
            if (!readFile(buffer, std::string(getJsonRelPath(id))))
                continue;

            result.jsonFiles[id] = std::move(buffer);
        }
        for (BlobId id : getBlobIds()) {
            std::string buffer;
            if (!readFile(buffer, std::string(getBlobRelPath(id))))
                continue;

            result.blobs[id] = std::move(buffer);
        }
    }

    result.valid = !result.tables.empty();

    return result;
}

bool FolderStorage::prepareForWrite() const noexcept
{
    if (m_storageType == StorageType::None)
        return true;

    const auto data = m_root / "data";
    if (m_storageType != StorageType::CsvFolder) {
        if (std_fs::exists(data)) {
            removeRecursively(data);
            Logger() << "Remove dir: " << path2string(data);
        }
    }

    if (!createDirectories(m_root)) {
        Logger() << "Failed to create: " << m_root;
        return false;
    }
    if (m_storageType == StorageType::D2ResurrectedModFolder) {
        auto jsonPath = m_root / "modinfo.json";
        if (std_fs::exists(jsonPath))
            std_fs::remove(jsonPath);
        Mernel::PropertyTree modinfo;
        modinfo.convertToMap();
        modinfo["name"]     = Mernel::PropertyTreeScalar(m_modName);
        modinfo["savepath"] = Mernel::PropertyTreeScalar(m_modName + "/");
        std::string buffer;
        writeJsonToBufferNoexcept(buffer, modinfo);
        if (!Mernel::writeFileFromBufferNoexcept(jsonPath, buffer)) {
            Logger() << "Failed to write: " << jsonPath;
            return false;
        } else {
            Logger() << "Write: " << path2string(jsonPath);
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        const auto excelRoot = m_root / s_tableSubfolder;
        if (!createDirectories(excelRoot))
            return false;
    }

    return true;
}

bool FolderStorage::writeData(const StoredData& data) const noexcept
{
    if (m_storageType == StorageType::None)
        return true;
    auto writeData = [](const std::string& data, const std_path& absPath) {
        if (!createDirectoriesForFile(absPath))
            return false;
        auto res = Mernel::writeFileFromBufferNoexcept(absPath, data);
        if (res) {
            Logger() << "Write: " << path2string(absPath);
        } else {
            Logger(Logger::Warning) << "failed to write to:" << absPath;
        }
        return res;
    };

    for (const auto& [id, tableData] : data.tables) {
        const std::string relPath = m_storageType == StorageType::CsvFolder ? std::string(getTableIdString(id)) + m_suffix + ".txt" : makeTableRelativePath(id, m_needBaseSubfolder);
        const auto        absPath = m_root / relPath;
        if (!writeData(tableData, absPath)) {
            return false;
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        if (data.dataVersion) {
            const auto absPath = m_root / IStorage::s_dataVersionPath;
            if (!writeData(std::to_string(data.dataVersion), absPath)) {
                return false;
            }
        }
        for (const auto& [id, memoryData] : data.jsonFiles) {
            const auto absPath = m_root / std::string(getJsonRelPath(id));
            if (!writeData(memoryData, absPath)) {
                return false;
            }
        }
        for (const auto& [id, memoryData] : data.blobs) {
            const auto absPath = m_root / std::string(getBlobRelPath(id));
            if (!writeData(memoryData, absPath)) {
                return false;
            }
        }
    }

    return true;
}

}
