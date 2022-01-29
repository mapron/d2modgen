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
#include <QFile>
#include <QJsonDocument>

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
    : m_storage(storage)
    , m_modName(modname)
    , m_modRoot(storage == StorageType::D2ResurrectedModFolder ? storageRoot + s_modSubfolderTemplate.arg(modname) : storageRoot)
{}

bool FolderStorage::prepareForWrite() const noexcept
{
    qDebug() << "started generation in " << m_modRoot;
    if (QFileInfo::exists(m_modRoot + "data/"))
        QDir(m_modRoot + "data/").removeRecursively();

    if (!QFileInfo::exists(m_modRoot))
        QDir().mkpath(m_modRoot);
    if (!QFileInfo::exists(m_modRoot)) {
        qDebug() << "Failed to create: " << m_modRoot;
        return false;
    }
    if (m_storage == StorageType::D2ResurrectedModFolder) {
        if (QFileInfo::exists(m_modRoot + "modinfo.json"))
            QFile::remove(m_modRoot + "modinfo.json");
        QJsonObject modinfo;
        modinfo["name"]     = m_modName;
        modinfo["savepath"] = m_modName + "/";
        if (!writeJsonFile(m_modRoot + "modinfo.json", QJsonDocument(modinfo))) {
            qDebug() << "Failed to write: " << (m_modRoot + "modinfo.json");
            return false;
        }
    }
    const QString excelRoot = m_modRoot + s_tableSubfolder;
    if (!QFileInfo::exists(excelRoot))
        QDir().mkpath(excelRoot);

    return true;
}

bool FolderStorage::writeData(const OutFileList& files) const noexcept
{
    for (const auto& file : files) {
        assert(!file.relFilepath.isEmpty());
        const QString dest   = m_modRoot + file.relFilepath;
        const QString folder = QFileInfo(dest).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);

        if (!file.srcAbsFilepath.isEmpty()) {
            assert(file.data.isEmpty());
            QFile::copy(file.srcAbsFilepath, dest);
        } else if (!file.data.isEmpty()) {
            assert(file.srcAbsFilepath.isEmpty());
            QFile saveFile(dest);

            if (!saveFile.open(QIODevice::WriteOnly))
                return false;

            saveFile.write(file.data);
        } else {
            assert(!"Invalid logic, empty file?");
        }
    }
    return true;
}

}
