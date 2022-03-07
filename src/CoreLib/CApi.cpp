/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "CApi.h"

#include "TableUtils.hpp"
#include "PropertyTree.hpp"

using namespace D2ModGen;

CTable dataContext_getTable(CDataContext cdataContext, const char* tableName, int32_t needWrite, int32_t needIndexes)
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

CRow table_getRow(CTable ctable, int32_t row)
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

CCell row_getCellByIndex(CRow crow, int32_t column)
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

void cell_setDataInt(CCell ccell, int32_t newValue)
{
    auto* cppCell = static_cast<TableCell*>(ccell.opaque);
    cppCell->setInt(newValue);
}

const char* cell_getDataStr(CCell ccell)
{
    auto* pcell = static_cast<TableCell*>(ccell.opaque);
    return pcell->str.c_str();
}

int32_t cell_getDataInt(CCell ccell)
{
    auto* pcell = static_cast<TableCell*>(ccell.opaque);
    return pcell->toInt();
}

// Tree

int8_t tree_isNull(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    return ptree->isNull();
}

int8_t tree_isScalar(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    return ptree->isScalar();
}

int8_t tree_isList(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    return ptree->isList();
}

int8_t tree_isMap(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    return ptree->isMap();
}

CPropertyTreeScalar tree_getScalar(CPropertyTree ctree)
{
    const auto* ptree   = static_cast<const PropertyTree*>(ctree.opaque);
    const auto& pscalar = ptree->getScalar();
    return { &pscalar };
}

CPropertyTreeList tree_getList(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    const auto& plist = ptree->getList();
    return { &plist };
}

CPropertyTreeMap tree_getMap(CPropertyTree ctree)
{
    const auto* ptree = static_cast<const PropertyTree*>(ctree.opaque);
    const auto& pmap  = ptree->getMap();
    return { &pmap };
}

CPropertyTree treeMap_value(CPropertyTreeMap ctreemap, const char* key)
{
    const auto* pmap = static_cast<const PropertyTreeMap*>(ctreemap.opaque);
    if (!pmap->contains(key))
        return { nullptr };
    const PropertyTree& child = pmap->at(key);
    return { &child };
}

int8_t treeScalar_isNull(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->isNull();
}

int8_t treeScalar_isBool(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->isBool();
}

int8_t treeScalar_isInt(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->isInt();
}

int8_t treeScalar_isDouble(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->isDouble();
}

int8_t treeScalar_isString(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->isString();
}

int8_t treeScalar_toBool(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->toBool();
}

int64_t treeScalar_toInt(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->toInt();
}

const char* treeScalar_toString(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->toCString();
}

double treeScalar_toDouble(CPropertyTreeScalar cscalar)
{
    const auto* pscalar = static_cast<const PropertyTreeScalar*>(cscalar.opaque);
    return pscalar->toDouble();
}
