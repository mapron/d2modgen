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
    { "itemtypes", AliasConfig{
                       { "MaxSockets3", { "MaxSock40" } },
                       { "MaxSock40", { "MaxSockets3" } },
                   } },
    { "levels", AliasConfig{
                    { "MonLvlEx(N)", { "MonLvl2Ex" } },
                    { "MonLvlEx(H)", { "MonLvl3Ex" } },
                    { "MonLvl2Ex", { "MonLvlEx(N)" } },
                    { "MonLvl3Ex", { "MonLvlEx(H)" } },
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

void DropSet::readRow(const TableView::RowView& row)
{
    const QString& noDrop = row["NoDrop"];
    m_noDrop              = noDrop.toInt();

    m_items.clear();
    for (int i = 1; i <= 10; ++i) {
        const QString& prob = row[QString("Prob%1").arg(i)];
        if (prob.isEmpty())
            break;
        const QString& tcName = row[QString("Item%1").arg(i)];
        m_items << Item{ tcName, prob.toInt() };
    }
}

void DropSet::writeRow(TableView::RowView& row) const
{
    QString& noDrop = row["NoDrop"];
    noDrop          = m_noDrop ? QString("%1").arg(m_noDrop) : QString();
    for (int i = 1; i <= 10; ++i) {
        QString& prob   = row[QString("Prob%1").arg(i)];
        QString& tcName = row[QString("Item%1").arg(i)];
        prob.clear();
        tcName.clear();
        if (i - 1 >= m_items.size())
            continue;
        auto& item = m_items[i - 1];
        prob       = QString("%1").arg(item.prob);
        tcName     = item.tc;
    }
}

int DropSet::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : m_items)
        totalOther += item.prob;
    return totalOther;
}

namespace Tables {
const ColumnsDescList s_descUniques{ ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12) };
const ColumnsDescList s_descRunewords{ ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7) };
const ColumnsDescList s_descSetItems{
    ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9),
    ColumnsDesc("aprop%1a", "apar%1a", "amin%1a", "amax%1a", 5),
    ColumnsDesc("aprop%1b", "apar%1b", "amin%1b", "amax%1b", 5),
};

const ColumnsDescList s_descGems{
    ColumnsDesc("weaponMod%1Code", "weaponMod%1Param", "weaponMod%1Min", "weaponMod%1Max", 3),
    ColumnsDesc("helmMod%1Code", "helmMod%1Param", "helmMod%1Min", "helmMod%1Max", 3),
    ColumnsDesc("shieldMod%1Code", "shieldMod%1Param", "shieldMod%1Min", "shieldMod%1Max", 3),
};

const ColumnsDescList s_descAffix{ ColumnsDesc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3) };

const ColumnsDescList s_descSets{
    ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2),
    ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2),
    ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8),
};
}

}
