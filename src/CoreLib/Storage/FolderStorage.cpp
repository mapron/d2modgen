/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FolderStorage.hpp"

#include "FileIOUtils.hpp"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace D2ModGen {

namespace {
static const QString s_modSubfolderTemplate = "mods/%1/%1.mpq/";
static const QString s_tableSubfolder       = "data/global/excel/";
static const QString s_tableSubfolderBack   = "data\\global\\excel\\";
}

QString IStorage::makeTableRelativePath(const QString& id, bool backslash)
{
    const QString& base = backslash ? s_tableSubfolderBack : s_tableSubfolder;
    return base + id + ".txt";
}

FolderStorage::FolderStorage(const QString& storageRoot, StorageType storage, const QString& modname)
    : m_storageType(storage)
    , m_modName(modname)
    , m_root(storage == StorageType::D2ResurrectedModFolder ? storageRoot + s_modSubfolderTemplate.arg(modname) : storageRoot)
{
    assert(m_root.endsWith('/') || m_root.endsWith('\\'));
}

IStorage::StoredData FolderStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    StoredData result{ true };

    QDir         rootDir(m_root);
    QDirIterator it(m_root, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
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

                result.tables.push_back(StoredFileTable{ loadFile.readAll().toStdString(), id });
            } else {
                QString relPath = rootDir.relativeFilePath(fi.absoluteFilePath());
                if (filenames.contains(relPath)) {
                    QFile loadFile(absPath);
                    if (!loadFile.open(QIODevice::ReadOnly))
                        return {};

                    result.inMemoryFiles.push_back(StoredFileMemory{ loadFile.readAll().toStdString(), relPath });
                } else {
                    result.refFiles.push_back(StoredFileRef{ absPath, relPath });
                }
            }
        }
    }
    return result;
}

bool FolderStorage::prepareForWrite() const noexcept
{
    qDebug() << "started generation in " << m_root;
    if (QFileInfo::exists(m_root + "data/"))
        QDir(m_root + "data/").removeRecursively();

    if (!QFileInfo::exists(m_root))
        QDir().mkpath(m_root);
    if (!QFileInfo::exists(m_root)) {
        qDebug() << "Failed to create: " << m_root;
        return false;
    }
    if (m_storageType == StorageType::D2ResurrectedModFolder) {
        if (QFileInfo::exists(m_root + "modinfo.json"))
            QFile::remove(m_root + "modinfo.json");
        QJsonObject modinfo;
        modinfo["name"]     = m_modName;
        modinfo["savepath"] = m_modName + "/";
        if (!writeJsonFile(m_root + "modinfo.json", QJsonDocument(modinfo))) {
            qDebug() << "Failed to write: " << (m_root + "modinfo.json");
            return false;
        }
    }
    if (m_storageType != StorageType::CsvFolder) {
        const QString excelRoot = m_root + s_tableSubfolder;
        if (!QFileInfo::exists(excelRoot))
            QDir().mkpath(excelRoot);
    }

    return true;
}

bool FolderStorage::writeData(const StoredData& data) const noexcept
{
    auto writeData = [](const QByteArray& data, const QString& absPath) {
        const QString folder = QFileInfo(absPath).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);
        if (!QFileInfo::exists(folder))
            return false;
        QFile saveFile(absPath);
        if (!saveFile.open(QIODevice::WriteOnly))
            return false;

        if (!data.isEmpty()) {
            if (saveFile.write(data) <= 0)
                return false;
        }

        return true;
    };
    auto copyFile = [](const QString& absSrc, const QString& absDest) {
        const QString folder = QFileInfo(absDest).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);
        if (!QFileInfo::exists(folder))
            return false;

        if (QFileInfo::exists(absDest))
            QFile::remove(absDest);
        if (QFileInfo::exists(absDest))
            return false;

        if (!QFile::copy(absSrc, absDest))
            return false;

        return true;
    };
    for (const auto& tableData : data.tables) {
        const QString relPath = m_storageType == StorageType::CsvFolder ? tableData.id + ".txt" : makeTableRelativePath(tableData.id, false);
        const QString absPath = m_root + relPath;
        if (!writeData(QByteArray::fromStdString(tableData.data), absPath)) {
            qWarning() << "failed to write to:" << absPath;
            return false;
        }
    }
    for (const auto& memoryData : data.inMemoryFiles) {
        const QString absPath = m_root + memoryData.relFilepath;
        if (!writeData(QByteArray::fromStdString(memoryData.data), absPath)) {
            qWarning() << "failed to write to:" << absPath;
            return false;
        }
    }
    for (const auto& refData : data.refFiles) {
        const QString absPathDest = m_root + refData.relFilepath;
        if (!copyFile(refData.absSrcFilepath, absPathDest)) {
            qWarning() << "failed to copy file:" << refData.absSrcFilepath << " -> " << absPathDest;
            return false;
        }
    }

    return true;
}

}
