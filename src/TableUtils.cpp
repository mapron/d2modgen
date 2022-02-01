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

using AliasConfig = std::map<QString, QStringList>;
static const std::map<QString, AliasConfig> s_tableColumnAliases{
    { "itemtypes", AliasConfig{ { "MaxSockets3", { "MaxSock40" } } } },
    { "levels", AliasConfig{
                    { "MonLvlEx(N)", { "MonLvl2Ex" } },
                    { "MonLvlEx(H)", { "MonLvl3Ex" } },
                } },
};

}

TableView::TableView(Table& table, bool markModified)
    : m_table(table)
{
    auto itAlias = s_tableColumnAliases.find(table.id);
    for (int i = 0; i < m_table.columns.size(); ++i) {
        const QString& col = m_table.columns[i];
        m_columnIndex[col] = i;

        if (itAlias != s_tableColumnAliases.cend()) {
            auto itCol = itAlias->second.find(col);
            if (itCol != itAlias->second.cend()) {
                for (const QString& aliasCol : itCol->second)
                    m_columnIndex[aliasCol] = i;
            }
        }
    }
    for (int i = 0; i < m_table.rows.size(); ++i)
        m_rows.emplace_back(i, *this);
    if (markModified)
        m_table.modified = true;

    auto itKey = s_tableKeys.find(table.id);
    if (itKey != s_tableKeys.cend()) {
        for (const QString& col : itKey->second) {
            const int colIndex = m_columnIndex.value(col, -1);
            assert(colIndex != -1);
            if (colIndex < 0)
                continue;
            m_primaryKey.push_back(colIndex);
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
    if (m_primaryKey.empty())
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
