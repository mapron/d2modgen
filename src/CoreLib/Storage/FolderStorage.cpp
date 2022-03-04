/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FolderStorage.hpp"

#include "FileIOUtils.hpp"

#include "Logger.hpp"
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QFile>

namespace D2ModGen {

namespace {
static const std::string s_tableSubfolder     = "data/global/excel/";
static const std::string s_tableSubfolderBack = "data\\global\\excel\\";
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

    QDir         rootDir(QString::fromStdString(m_root));
    QDirIterator it(QString::fromStdString(m_root), QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        auto fi = it.fileInfo();
        if (fi.isFile()) {
            const QString absPath = fi.absoluteFilePath();
            if (fi.completeSuffix().toLower() == "txt" && (fi.dir().dirName().toLower() == "excel" || fi.dir() == rootDir)) { // not a best guess, but however...
                auto  id = fi.completeBaseName().toLower();
                QFile loadFile(absPath);
                if (!loadFile.open(QIODevice::ReadOnly))
                    return {};

                result.tables.push_back(StoredFileTable{ loadFile.readAll().toStdString(), id.toStdString() });
            } else {
                const std::string relPath = rootDir.relativeFilePath(fi.absoluteFilePath()).toStdString();
                if (filenames.contains(relPath)) {
                    QFile loadFile(absPath);
                    if (!loadFile.open(QIODevice::ReadOnly))
                        return {};

                    result.inMemoryFiles.push_back(StoredFileMemory{ loadFile.readAll().toStdString(), relPath });
                } else {
                    result.refFiles.push_back(StoredFileRef{ absPath.toStdString(), relPath });
                }
            }
        }
    }
    return result;
}

bool FolderStorage::prepareForWrite() const noexcept
{
    Logger() << "started generation in " << m_root;
    QString root = QString::fromStdString(m_root);
    if (QFileInfo::exists(root + "data/"))
        QDir(root + "data/").removeRecursively();

    if (!QFileInfo::exists(root))
        QDir().mkpath(root);
    if (!QFileInfo::exists(root)) {
        Logger() << "Failed to create: " << m_root;
        return false;
    }
    if (m_storageType == StorageType::D2ResurrectedModFolder) {
        if (QFileInfo::exists(root + "modinfo.json"))
            QFile::remove(root + "modinfo.json");
        PropertyTree modinfo;
        modinfo.convertToMap();
        modinfo["name"]     = PropertyTreeScalar(m_modName);
        modinfo["savepath"] = PropertyTreeScalar(m_modName+ "/") ;
        std::string buffer;
        writeJsonToBuffer(buffer, modinfo);
        if (!writeFileFromBuffer(m_root + "modinfo.json", buffer)) {
            Logger() << "Failed to write: " << (m_root + "modinfo.json");
            return false;
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        const std::string excelRoot = m_root + s_tableSubfolder;
        if (!QFileInfo::exists(QString::fromStdString(excelRoot)))
            QDir().mkpath(QString::fromStdString(excelRoot));
    }

    return true;
}

bool FolderStorage::writeData(const StoredData& data) const noexcept
{
    auto writeData = [](const std::string& data, const std::string& absPath) {
        const QString folder = QFileInfo(QString::fromStdString(absPath)).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);
        if (!QFileInfo::exists(folder))
            return false;
        QFile saveFile(QString::fromStdString(absPath));
        if (!saveFile.open(QIODevice::WriteOnly))
            return false;

        if (!data.empty()) {
            if (saveFile.write(data.data(), data.size()) <= 0)
                return false;
        }

        return true;
    };
    auto copyFile = [](const std::string& absSrc, const std::string& absDest) {
        const QString folder = QFileInfo(QString::fromStdString(absDest)).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);
        if (!QFileInfo::exists(folder))
            return false;

        if (QFileInfo::exists(QString::fromStdString(absDest)))
            QFile::remove(QString::fromStdString(absDest));
        if (QFileInfo::exists(QString::fromStdString(absDest)))
            return false;

        if (!QFile::copy(QString::fromStdString(absSrc), QString::fromStdString(absDest)))
            return false;

        return true;
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
