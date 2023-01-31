/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "FileIOUtils.hpp"
#include "TableBase.hpp"

#include "ModgenCoreExport.hpp"

namespace D2ModGen {

MODGENCORE_EXPORT bool writeCSVToBuffer(std::string& csvData, const Table& table);
MODGENCORE_EXPORT bool readCSVFromBuffer(const std::string& csvData, Table& table);

}
