/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"

namespace D2ModGen {

class TableView {
public:
    TableView(Table& table, bool markModified = false);
    TableView(const Table& table);

    void markModified();
    bool createRowIndex();

    using RowValues = std::map<QString, QString>;

    class RowView {
    public:
        RowView(int index, TableView& parent)
            : i(index)
            , parent(parent)
        {}

        QString& operator[](int index)
        {
            assert(parent.isWriteable);
            return parent.m_table.rows[i].data[index];
        }
        QString& operator[](QString index)
        {
            assert(parent.isWriteable);
            return parent.m_table.rows[i].data[ind(index)];
        }

        const QString& operator[](int index) const { return parent.m_table.rows[i].data[index]; }
        const QString& operator[](QString index) const { return parent.m_table.rows[i].data[ind(index)]; }

        bool hasColumn(const QString& name) const { return parent.m_columnIndex.contains(name); }

        int index() const { return i; }

        RowValues toValues() const
        {
            RowValues result;
            for (int c = 0; c < parent.m_table.columns.size(); ++c)
                result[parent.m_table.columns[c]] = parent.m_table.rows[i].data[c];
            return result;
        }

        void setValues(const RowValues& values)
        {
            for (const auto& p : values)
                (*this)[p.first] = p.second;
        }

        QString makeKey() const
        {
            QStringList v;
            for (int col : parent.m_primaryKey)
                v << (*this)[col];
            return v.join("###");
        }

    private:
        int        ind(const QString& index) const { return parent.m_columnIndex.value(index, -1); }
        int        i;
        TableView& parent;
    };

    auto begin()
    {
        assert(isWriteable);
        return m_rows.begin();
    }
    auto end()
    {
        assert(isWriteable);
        return m_rows.end();
    }
    auto cbegin() const { return m_rows.begin(); }
    auto cend() const { return m_rows.end(); }
    auto begin() const { return m_rows.begin(); }
    auto end() const { return m_rows.end(); }

    RowView& operator[](int index)
    {
        assert(isWriteable);
        return m_rows[index];
    }
    const RowView& operator[](int index) const
    {
        return m_rows[index];
    }

    bool hasColumn(const QString& name) const { return m_columnIndex.contains(name); }
    void appendRow(const RowValues& values);

    void merge(const TableView& source, bool appendNew, bool updateExisting);
    void concat(const TableView& source);

    bool updateRows(const TableView& source, const QList<int>& specificRows);
    void clear();

    int rowCount() const { return m_table.rows.size(); }

private:
    Table&               m_table;
    std::vector<RowView> m_rows;
    QList<int>           m_primaryKey;
    QHash<QString, int>  m_columnIndex;
    QHash<QString, int>  m_pkIndex;
    bool                 isWriteable = true;
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
