/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "FileIOUtils.hpp"
#include "TableBase.hpp"

#include "CoreExport.hpp"

namespace D2ModGen {

CORE_EXPORT bool writeCSVToBuffer(std::string& csvData, const Table& table);
CORE_EXPORT bool readCSVFromBuffer(const std::string& csvData, Table& table);

}
