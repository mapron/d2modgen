/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {

static const std::map<QString, QStringList> s_tableKeys{
    { "gamble", { "code" } },
    { "misc", { "code" } },
    { "gems", { "code" } },
    { "uniqueitems", { "index" } },
    { "setitems", { "index" } },
    { "sets", { "index" } },
    { "runes", { "Name" } },
    { "skills", { "skill" } },
    { "monstats", { "Id" } },
    { "monstats2", { "Id" } },
    { "difficultylevels", { "Name" } },
    { "levels", { "Name" } },
    { "monlvl", { "Level" } },
    { "charstats", { "class" } },
    { "weapons", { "code" } },
    { "armor", { "code" } },
};

}

TableView::TableView(Table& table, bool markModified)
    : m_table(table)
{
    for (int i = 0; i < m_table.columns.size(); ++i)
        m_columnIndex[m_table.columns[i]] = i;
    for (int i = 0; i < m_table.rows.size(); ++i)
        m_rows.emplace_back(i, *this);
    if (markModified)
        m_table.modified = true;

    auto it = s_tableKeys.find(table.id);
    if (it != s_tableKeys.cend()) {
        for (const QString& col : it->second) {
            const int colIndex = m_columnIndex.value(col, -1);
            assert(colIndex != -1);
            if (colIndex < 0)
                continue;
            m_primaryKey << colIndex;
        }
    }
}

TableView::TableView(const Table& table)
    : TableView(const_cast<Table&>(table)) // it's not that casting away const = UB, its writing this data when it's not const = UB.
{
    isWriteable = false;
}

void D2ModGen::TableView::markModified()
{
    assert(isWriteable);
    m_table.modified = true;
}

bool TableView::createRowIndex()
{
    if (m_primaryKey.isEmpty())
        return false;
    for (int i = 0; i < m_rows.size(); ++i)
        m_pkIndex[m_rows[i].makeKey()] = i;
    return true;
}

void TableView::appendRow(const RowValues& values)
{
    assert(isWriteable);
    m_table.rows << TableRow(m_table.columns.size());
    m_rows.emplace_back(static_cast<int>(m_rows.size()), *this);
    RowView& row = *m_rows.rbegin();
    row.setValues(values);
}

void TableView::merge(const TableView& source, bool appendNew, bool updateExisting)
{
    QList<RowValues> newValues;
    for (const auto& sourceRow : source) {
        const auto pk       = sourceRow.makeKey();
        int        rowIndex = m_pkIndex.value(pk, -1);
        if (rowIndex == -1) {
            if (appendNew)
                newValues << sourceRow.toValues();
        } else {
            if (updateExisting) {
                RowView& row = m_rows[rowIndex];
                row.setValues(sourceRow.toValues());
            }
        }
    }
    for (const auto& values : newValues)
        appendRow(values);
}

void TableView::concat(const TableView& source)
{
    for (const auto& sourceRow : source) {
        appendRow(sourceRow.toValues());
    }
}

void TableView::clear()
{
    assert(isWriteable);
    m_rows.clear();
    m_table.rows.clear();
    m_table.modified = true;
}

}
