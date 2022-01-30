/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "Storage/IStorage.hpp"
#include "CommonTypes.hpp"

#include <set>

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
    bool            modified    = false;
    bool            forceOutput = false;
};

struct TableSet {
    std::map<QString, Table> tables;
    std::set<QString>        relativeNames;
};

struct DataContext {
    TableSet                                   tableSet;  // txt tables parsed into csv-like tables.
    std::map<QString, QJsonDocument>           jsonFiles; // data of extra json files
    std::map<QString, IStorage::StoredFileRef> copyFiles; // files for plain copy.

    bool readData(const IStorage::StoredData& data);
    bool writeData(IStorage::StoredData& data) const;

    bool mergeWith(const DataContext& source, ConflictPolicy policy);
};

}
