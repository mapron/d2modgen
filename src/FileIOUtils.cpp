/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FileIOUtils.hpp"

#include <QFile>
#include <QJsonDocument>

namespace D2ModGen {

bool readJsonFile(const QString& file, QJsonDocument& data)
{
    QFile loadFile(file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    data = loadDoc;

    return true;
}

bool writeJsonFile(const QString& file, const QJsonDocument& data, bool escape)
{
    QFile saveFile(file);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QByteArray datastr = data.toJson(QJsonDocument::Indented);
    if (escape)
        datastr.replace(QByteArray("/"), QByteArray("\\/")); // weird battlenet format.

    saveFile.write(datastr);

    return true;
}

bool readCSV(const QString& csvData, Table& table)
{
    QStringList rows = csvData.split("\r\n", Qt::SkipEmptyParts);
    if (rows.isEmpty())
        return false;
    const QString header = rows.takeFirst();
    table.columns        = header.split('\t', Qt::KeepEmptyParts);
    for (const QString& row : rows) {
        const QStringList cells = row.split('\t', Qt::KeepEmptyParts);
        table.rows << TableRow{ cells };
    }
#ifndef NDEBUG
    {
        QString check;
        writeCSV(check, table);
        Q_ASSERT(check == csvData);
    }
#endif
    return true;
}

bool writeCSV(QString& csvData, const Table& table)
{
    csvData += table.columns.join('\t') + "\r\n";
    for (const auto& row : table.rows)
        csvData += row.data.join('\t') + "\r\n";
    return true;
}

}
