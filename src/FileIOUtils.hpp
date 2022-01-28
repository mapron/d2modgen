/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

bool readJsonFile(const QString& file, QJsonDocument& data);
bool writeJsonFile(const QString& file, const QJsonDocument& data, bool escape = false);

bool readCSV(const QString& csvData, Table& table);
bool writeCSV(QString& csvData, const Table& table);

bool readCSVfile(const QString& csvFilename, Table& table);
bool writeCSVfile(const QString& csvFilename, const Table& table);

}
