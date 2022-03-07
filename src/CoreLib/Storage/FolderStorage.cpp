/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FolderStorage.hpp"

#include "FileIOUtils.hpp"
#include "Logger.hpp"

#include "Formats/FileFormatJson.hpp"

#include <cassert>

namespace D2ModGen {

namespace {
static const std::string s_tableSubfolder     = "data/global/excel/";
static const std::string s_tableSubfolderBack = "data\\global\\excel\\";

void removeRecursively(const std::string& folder)
{
    std::deque<std_path> forDelete;
    for (auto it : std_fs::recursive_directory_iterator(string2path(folder))) {
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

FolderStorage::FolderStorage(const std::string& storageRoot, StorageType storage, const std::string& modname)
    : m_storageType(storage)
    , m_modName(modname)
    , m_root(storage == StorageType::D2ResurrectedModFolder ? storageRoot + ("mods/" + modname + "/" + modname + ".mpq/") : storageRoot)
{
    assert(m_root.ends_with('/') || m_root.ends_with('\\'));
}

IStorage::StoredData FolderStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    StoredData result{ true };
    const auto root = string2path(m_root);
    for (auto it : std_fs::recursive_directory_iterator(root)) {
        if (!it.is_regular_file())
            continue;
        const std_path& path           = it.path();
        const auto      currentAbsPath = path2string(path);
        const auto      currentRelPath = currentAbsPath.substr(m_root.size());

        if (path.extension() == ".txt" && (path.parent_path().filename() == "excel" || path.parent_path() == root)) { // not a best guess, but however...
            auto        id = toLower(path2string(path.stem()));
            std::string buffer;
            if (!readFileIntoBuffer(currentAbsPath, buffer))
                return {};

            result.tables.push_back(StoredFileTable{ std::move(buffer), id });
        } else {
            if (filenames.contains(currentRelPath)) {
                std::string buffer;
                if (!readFileIntoBuffer(currentAbsPath, buffer))
                    return {};

                result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), currentRelPath });
            } else {
                result.refFiles.push_back(StoredFileRef{ currentAbsPath, currentRelPath });
            }
        }
    }

    return result;
}

bool FolderStorage::prepareForWrite() const noexcept
{
    const auto rootpath = string2path(m_root);

    if (std_fs::exists(string2path(m_root) / "data"))
        removeRecursively(m_root + "data/");

    if (!createDirectories(m_root)) {
        Logger() << "Failed to create: " << m_root;
        return false;
    }
    if (m_storageType == StorageType::D2ResurrectedModFolder) {
        if (std_fs::exists(rootpath / "modinfo.json"))
            std_fs::remove(rootpath / "modinfo.json");
        PropertyTree modinfo;
        modinfo.convertToMap();
        modinfo["name"]     = PropertyTreeScalar(m_modName);
        modinfo["savepath"] = PropertyTreeScalar(m_modName + "/");
        std::string buffer;
        writeJsonToBuffer(buffer, modinfo);
        if (!writeFileFromBuffer(m_root + "modinfo.json", buffer)) {
            Logger() << "Failed to write: " << (m_root + "modinfo.json");
            return false;
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        const std::string excelRoot = m_root + s_tableSubfolder;
        if (!createDirectories(excelRoot))
            return false;
    }

    return true;
}

bool FolderStorage::writeData(const StoredData& data) const noexcept
{
    auto writeData = [](const std::string& data, const std::string& absPath) {
        if (!createDirectoriesForFile(absPath))
            return false;

        return writeFileFromBuffer(absPath, data);
    };
    auto copyFile = [](const std::string& absSrc, const std::string& absDest) {
        if (!createDirectoriesForFile(absDest))
            return false;

        std::error_code ec;
        std_fs::copy(string2path(absSrc), string2path(absDest), std_fs::copy_options::overwrite_existing, ec);
        return !ec;
    };
    for (const auto& tableData : data.tables) {
        const std::string relPath = m_storageType == StorageType::CsvFolder ? tableData.id + ".txt" : makeTableRelativePath(tableData.id, false);
        const std::string absPath = m_root + relPath;
        if (!writeData(tableData.data, absPath)) {
            Logger(Logger::Warning) << "failed to write to:" << absPath.c_str();
            return false;
        }
    }
    for (const auto& memoryData : data.inMemoryFiles) {
        const std::string absPath = m_root + memoryData.relFilepath;
        if (!writeData(memoryData.data, absPath)) {
            Logger(Logger::Warning) << "failed to write to:" << absPath.c_str();
            return false;
        }
    }
    for (const auto& refData : data.refFiles) {
        const std::string absPathDest = m_root + refData.relFilepath;
        if (!copyFile(refData.absSrcFilepath, absPathDest)) {
            Logger(Logger::Warning) << "failed to copy file:" << refData.absSrcFilepath.c_str() << " -> " << absPathDest.c_str();
            return false;
        }
    }

    return true;
}

}
