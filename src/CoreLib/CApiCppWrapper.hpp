/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CApi.h"

#include <string>

namespace Wrapper {

// Data Context

class Cell {
public:
    Cell(CCell ccell)
        : m_ccell(ccell)
    {}

    std::string getStr()
    {
        const char* value = cell_getDataStr(m_ccell);
        return value;
    }
    int getInt()
    {
        int value = cell_getDataInt(m_ccell);
        return value;
    }
    void setStr(const std::string& newValue)
    {
        cell_setDataStr(m_ccell, newValue.c_str());
    }
    void setInt(const int newValue)
    {
        cell_setDataInt(m_ccell, newValue);
    }

private:
    CCell m_ccell;
};

class Row {
public:
    Row(CRow crow)
        : m_crow(crow)
    {}

    Cell getCellByIndex(int column)
    {
        CCell ccell = row_getCellByIndex(m_crow, column);
        return Cell(ccell);
    }

    Cell getCell(const std::string& columnName)
    {
        CCell ccell = row_getCell(m_crow, columnName.c_str());
        return Cell(ccell);
    }

    Cell operator[](const std::string& columnName)
    {
        return getCell(columnName);
    }

private:
    CRow m_crow;
};

class Table {
public:
    Table(CTable ctable)
        : m_ctable(ctable)
    {}

    Row getRow(int row)
    {
        CRow crow = table_getRow(m_ctable, row);
        return Row(crow);
    }

    int getRowCount() { return table_getRowCount(m_ctable); }

private:
    CTable m_ctable;
};

class DataContext {
public:
    DataContext(CDataContext cdataContext)
        : m_cdataContext(cdataContext)
    {}

    Table getTable(const std::string& tableName, bool needWrite, bool needIndexes)
    {
        CTable ctable = dataContext_getTable(m_cdataContext, tableName.c_str(), needWrite, needIndexes);
        return Table(ctable);
    }

private:
    CDataContext m_cdataContext;
};

// Tree

class PropertyTreeScalar {
public:
    PropertyTreeScalar(CPropertyTreeScalar cscalar)
        : m_cscalar(cscalar)
    {}
    bool isNull() const { return treeScalar_isNull(m_cscalar); }
    bool isBool() const { return treeScalar_isBool(m_cscalar); }
    bool isInt() const { return treeScalar_isInt(m_cscalar); }
    bool isDouble() const { return treeScalar_isDouble(m_cscalar); }
    bool isString() const { return treeScalar_isString(m_cscalar); }

    bool        toBool() const { return treeScalar_toBool(m_cscalar); }
    int64_t     toInt() const { return treeScalar_toInt(m_cscalar); }
    std::string toString() const { return treeScalar_toString(m_cscalar); }
    double      toDouble() const { return treeScalar_toDouble(m_cscalar); }

private:
    CPropertyTreeScalar m_cscalar;
};

class PropertyTree;

class PropertyTreeList {
public:
    PropertyTreeList(CPropertyTreeList clist)
        : m_clist(clist)
    {}

private:
    CPropertyTreeList m_clist;
};

class PropertyTreeMap {
public:
    PropertyTreeMap(CPropertyTreeMap cmap)
        : m_cmap(cmap)
    {}

    bool contains(const std::string& key) const
    {
        CPropertyTree val = treeMap_value(m_cmap, key.c_str());
        return val.opaque != nullptr;
    }
    PropertyTree       value(const std::string& key) const;
    PropertyTreeScalar scalarValue(const std::string& key) const;

private:
    CPropertyTreeMap m_cmap;
};

class PropertyTree {
public:
    PropertyTree(CPropertyTree ctree)
        : m_ctree(ctree)
    {}

    bool isNull() const { return tree_isNull(m_ctree); }
    bool isScalar() const { return tree_isScalar(m_ctree); }
    bool isList() const { return tree_isList(m_ctree); }
    bool isMap() const { return tree_isMap(m_ctree); }

    PropertyTreeScalar getScalar() const
    {
        CPropertyTreeScalar cscalar = tree_getScalar(m_ctree);
        return PropertyTreeScalar(cscalar);
    }
    PropertyTreeList getList() const
    {
        CPropertyTreeList clist = tree_getList(m_ctree);
        return PropertyTreeList(clist);
    }
    PropertyTreeMap getMap() const
    {
        CPropertyTreeMap cmap = tree_getMap(m_ctree);
        return PropertyTreeMap(cmap);
    }

private:
    CPropertyTree m_ctree;
};

inline PropertyTree PropertyTreeMap::value(const std::string& key) const
{
    CPropertyTree val = treeMap_value(m_cmap, key.c_str());
    return PropertyTree(val);
}

inline PropertyTreeScalar PropertyTreeMap::scalarValue(const std::string& key) const
{
    CPropertyTree val = treeMap_value(m_cmap, key.c_str());
    return PropertyTree(val).getScalar();
}

}
