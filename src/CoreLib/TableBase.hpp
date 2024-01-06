/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "TableId.hpp"

#include "MernelPlatform/FileFormatCSVTable.hpp"

namespace D2ModGen {

using TableCell = Mernel::CSVTableCell;
using TableRow  = Mernel::CSVTableRow;

struct Table : public Mernel::CSVTable {
    TableId id;
    bool    modified    = false;
    bool    forceOutput = false;
};

}
