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
    
    QJsonParseError err;

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData, &err));
    if (loadDoc.isNull()) {
        qWarning() << "failed to parse json:" << file << ", " << err.errorString();
        return false;
    }

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


}
