/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QStringList>
#include <QMap>
#include <QSet>
#include <QList>
#include <QWidget>
#include <QJsonObject>
#include <QRandomGenerator>

#include <vector>

namespace D2ModGen {

struct TableRow {
    QStringList data;
    TableRow() = default;
    explicit TableRow(int size)
    {
        for (int i = 0; i < size; ++i)
            data << "";
    }
    explicit TableRow(QStringList data)
        : data(std::move(data))
    {
    }
};

struct Table {
    QString         id;
    QList<TableRow> rows;
    QStringList     columns;
};

struct TableSet {
    QMap<QString, Table> tables;
};

enum class StorageType {
    D2ResurrectedInternal, // using CASC storage from D2R Installation,
    D2LegacyInternal, // using Storm storage from D2 Legacy Installation (patch_d2.mpq)
    D2ResurrectedModFolder, // folder containing D2R mod structure (usually in mods/name/name.mpq)
    D2LegacyFolder, // Folder containing 'data' folder where data can be extracted.
    CsvFolder, // Folder containing plain 'txt' files. Can not be selected as output.
};

struct GenerationEnvironment {
    QString  modName;
    QString  d2rPath;
    QString  appData;
    QString  outPath;
    bool     exportAllTables;
    bool     isLegacy;
    uint32_t seed;
};

using KeySet      = QSet<QString>;
using JsonFileSet = QSet<QString>;

struct CopyFileInfo {
    QString srcModRoot;
    QString relativePath;
};

struct DataContext {
    TableSet                     tableSet;  // txt tables parsed into csv-like tables.
    QMap<QString, QJsonDocument> jsonFiles; // data of extra json files
    QList<CopyFileInfo>          copyFiles; // extra files - not loaded into memory, just plain copy to the destination.
};
using DataContextPtr = std::shared_ptr<DataContext>;

}
