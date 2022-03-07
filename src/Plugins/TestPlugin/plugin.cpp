/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "CApi.h"

#include "PluginExport.hpp"

#include <string>

namespace Wrapper {

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

}

extern "C" PLUGIN_EXPORT void generate(CDataContext cdataContext)
{
    Wrapper::DataContext ctx(cdataContext);

    Wrapper::Table charTable = ctx.getTable("charstats", true, false);
    const int      size      = charTable.getRowCount();
    for (int i = 0; i < size; ++i) {
        Wrapper::Row row = charTable.getRow(i);

        if (row["class"].getStr() == "Expansion")
            continue;
        for (int i = 1; i <= 10; ++i) {
            const std::string codeColumn  = "item" + std::to_string(i);
            const std::string countColumn = "item" + std::to_string(i) + "count";

            Wrapper::Cell code  = row[codeColumn];
            Wrapper::Cell count = row[countColumn];
            if (count.getInt() == 0) {
                count.setInt(1);
                code.setStr("key");
                break;
            }
        }
    }

    //return 1;
}

CHECK_GENERATE(generate)
