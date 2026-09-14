/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
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
    bool    modified = false;
    bool    valid    = false;
};

}
