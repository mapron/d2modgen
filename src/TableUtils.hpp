/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

class TableView {
public:
    TableView(Table& table)
        : m_table(table)
    {
        for (int i = 0; i < m_table.columns.size(); ++i)
            m_columnIndex[m_table.columns[i]] = i;
        for (int i = 0; i < m_table.rows.size(); ++i)
            m_rows.emplace_back(i, *this);
    }

    class RowView {
    public:
        RowView(int index, TableView& parent)
            : i(index)
            , parent(parent)
        {}

        QString& operator[](int index) { return parent.m_table.rows[i].data[index]; }
        QString& operator[](QString index) { return parent.m_table.rows[i].data[ind(index)]; }

        const QString& operator[](int index) const { return parent.m_table.rows[i].data[index]; }
        const QString& operator[](QString index) const { return parent.m_table.rows[i].data[ind(index)]; }

        bool hasColumn(const QString& name) const { return parent.m_columnIndex.contains(name); }

        int index() const { return i; }

    private:
        int        ind(const QString& index) const { return parent.m_columnIndex.value(index, -1); }
        int        i;
        TableView& parent;
    };

    auto begin() { return m_rows.begin(); }
    auto end() { return m_rows.end(); }
    auto cbegin() const { return m_rows.begin(); }
    auto cend() const { return m_rows.end(); }
    auto begin() const { return m_rows.begin(); }
    auto end() const { return m_rows.end(); }

    bool hasColumn(const QString& name) const { return m_columnIndex.contains(name); }
    void appendRow(const QMap<QString, QString>& values)
    {
        m_table.rows << TableRow(m_table.columns.size());
        m_rows.emplace_back(static_cast<int>(m_rows.size()), *this);
        RowView&                       row = *m_rows.rbegin();
        QMapIterator<QString, QString> i(values);
        while (i.hasNext()) {
            i.next();
            row[i.key()] = i.value();
        }
    }

private:
    Table&               m_table;
    std::vector<RowView> m_rows;
    QHash<QString, int>  m_columnIndex;
};

struct ColumnsDesc {
    struct Col {
        QString code;
        QString par;
        QString min;
        QString max;
    };
    QList<Col> m_cols;
    int        m_start = 0;
    ColumnsDesc()      = default;
    ColumnsDesc(const QString& codeTpl,
                const QString& parTpl,
                const QString& minTpl,
                const QString& maxTpl,
                int            end,
                int            start = 1)
        : m_start(start)
    {
        for (int i = start; i <= end; ++i) {
            Col col{
                codeTpl.arg(i),
                parTpl.arg(i),
                minTpl.arg(i),
                maxTpl.arg(i)
            };
            m_cols << col;
        }
    }
};

}
