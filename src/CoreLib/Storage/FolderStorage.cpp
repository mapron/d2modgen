/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FolderStorage.hpp"

#include "FileIOUtils.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include <cassert>

namespace D2ModGen {

namespace {
static const std::string s_tableSubfolder     = "data/global/excel/";
static const std::string s_tableSubfolderBack = "data\\global\\excel\\";

void removeRecursively(const std_path& folder)
{
    std::deque<std_path> forDelete;
    for (auto it : std_fs::recursive_directory_iterator(folder)) {
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

std::string IStorage::makeTableRelativePath(const std::string& id, bool backslash)
{
    const std::string& base = backslash ? s_tableSubfolderBack : s_tableSubfolder;
    return base + id + ".txt";
}

FolderStorage::FolderStorage(const std_path& storageRoot, StorageType storage, const std::string& modname)
    : m_storageType(storage)
    , m_modName(modname)
    , m_root(storage == StorageType::D2ResurrectedModFolder ? storageRoot / "mods" / modname / (modname + ".mpq") : (storageRoot / "fake").parent_path())
{
}

IStorage::StoredData FolderStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    StoredData result{ true };
    const auto rootLength = path2string(m_root).size() + 1;
    for (auto it : std_fs::recursive_directory_iterator(m_root)) {
        if (!it.is_regular_file())
            continue;
        const std_path&   path           = it.path();
        const std::string currentAbsPath = path2string(path);
        const std::string currentRelPath = currentAbsPath.substr(rootLength);
        if (path.filename() == "modinfo.json")
            continue;

        if (path.extension() == ".txt" && (path.parent_path().filename() == "excel" || path.parent_path() == m_root)) { // not a best guess, but however...
            auto        id = toLower(path2string(path.stem()));
            std::string buffer;
            if (!readFileIntoBuffer(path, buffer))
                return {};

            result.tables.push_back(StoredFileTable{ std::move(buffer), id });
        } else {
            if (filenames.contains(currentRelPath) || path.extension() == ".json") {
                std::string buffer;
                if (!readFileIntoBuffer(path, buffer))
                    return {};

                result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), string2path(currentRelPath) });
            } else {
                result.refFiles.push_back(StoredFileRef{ path, string2path(currentRelPath) });
            }
        }
    }

    return result;
}

bool FolderStorage::prepareForWrite() const noexcept
{
    if (std_fs::exists(m_root / "data"))
        removeRecursively(m_root / "data");

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
        if (!writeFileFromBuffer(jsonPath, buffer)) {
            Logger() << "Failed to write: " << jsonPath;
            return false;
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
    auto writeData = [](const std::string& data, const std_path& absPath) {
        if (!createDirectoriesForFile(absPath))
            return false;

        return writeFileFromBuffer(absPath, data);
    };
    auto copyFile = [](const std_path& absSrc, const std_path& absDest) {
        if (!createDirectoriesForFile(absDest))
            return false;

        std::error_code ec;
        std_fs::copy(absSrc, absDest, std_fs::copy_options::overwrite_existing, ec);
        return !ec;
    };
    for (const auto& tableData : data.tables) {
        const std::string relPath = m_storageType == StorageType::CsvFolder ? tableData.id + ".txt" : makeTableRelativePath(tableData.id, false);
        const auto        absPath = m_root / relPath;
        if (!writeData(tableData.data, absPath)) {
            Logger(Logger::Warning) << "failed to write to:" << absPath;
            return false;
        }
    }
    for (const auto& memoryData : data.inMemoryFiles) {
        const auto absPath = m_root / memoryData.relFilepath;
        if (!writeData(memoryData.data, absPath)) {
            Logger(Logger::Warning) << "failed to write to:" << absPath;
            return false;
        }
    }
    for (const auto& refData : data.refFiles) {
        const auto absPathDest = m_root / refData.relFilepath;
        if (!copyFile(refData.absSrcFilepath, absPathDest)) {
            Logger(Logger::Warning) << "failed to copy file:" << refData.absSrcFilepath << " -> " << absPathDest;
            return false;
        }
    }

    return true;
}

}
