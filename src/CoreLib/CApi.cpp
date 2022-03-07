/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "CApi.h"

#include "TableUtils.hpp"

using namespace D2ModGen;

CTable dataContext_getTable(CDataContext cdataContext, const char* tableName, int needWrite, int needIndexes)
{
    auto*   cppContext = static_cast<DataContext*>(cdataContext.opaque);
    TableId id;
    if (!findTableId(tableName, id))
        return { nullptr };
    Table& table     = cppContext->tableSet.tables[id];
    auto   tableView = std::make_shared<TableView>(table, needWrite);
    if (needIndexes)
        tableView->createRowIndex();

    cppContext->pluginTables[id] = tableView;

    return { tableView.get() };
}

CRow table_getRow(CTable ctable, int row)
{
    auto*               cppTable = static_cast<TableView*>(ctable.opaque);
    TableView::RowView& prow     = (*cppTable)[row];
    return { &prow };
}

int table_getRowCount(CTable ctable)
{
    auto* cppTable = static_cast<TableView*>(ctable.opaque);
    return cppTable->rowCount();
}

CCell row_getCellByIndex(CRow crow, int column)
{
    auto*      cppRow = static_cast<TableView::RowView*>(crow.opaque);
    TableCell& pcell  = (*cppRow)[column];
    return { &pcell };
}

CCell row_getCell(CRow crow, const char* columnName)
{
    auto*      cppRow = static_cast<TableView::RowView*>(crow.opaque);
    TableCell& pcell  = (*cppRow)[columnName];
    return { &pcell };
}

void cell_setDataStr(CCell ccell, const char* newValue)
{
    auto* cppCell = static_cast<TableCell*>(ccell.opaque);
    cppCell->str  = newValue;
}

void cell_setDataInt(CCell ccell, int newValue)
{
    auto* cppCell = static_cast<TableCell*>(ccell.opaque);
    cppCell->setInt(newValue);
}

const char* cell_getDataStr(CCell ccell)
{
    TableCell* pcell = static_cast<TableCell*>(ccell.opaque);
    return pcell->str.c_str();
}

int cell_getDataInt(CCell ccell)
{
    TableCell* pcell = static_cast<TableCell*>(ccell.opaque);
    return pcell->toInt();
}
