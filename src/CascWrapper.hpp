/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

bool ExtractTables(const QString& d2rpath, GenOutput& output, const JsonFileSet& extraFiles);
bool ExtractTablesLegacy(const QString& d2rpath, GenOutput& output);
