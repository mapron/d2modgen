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

struct GenOutput {
    TableSet                     tableSet;
    QMap<QString, QJsonDocument> jsonFiles;
    QList<CopyFileInfo>          copyFiles;
};

