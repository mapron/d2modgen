/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"

#include <unordered_map>

namespace D2ModGen {

inline std::string argCompat(std::string tpl, const std::string& arg)
{
    size_t pos = tpl.find("%1");
    if (pos == std::string::npos)
        return tpl;
    tpl.replace(pos, 2, arg);
    return tpl;
}

inline std::string argCompat(std::string tpl, int arg)
{
    return argCompat(std::move(tpl), std::to_string(arg));
}

class TableView {
public:
    TableView(Table& table, bool markModified = false);
    TableView(const Table& table);

    void markModified();
    bool createRowIndex();

    using RowValues = std::map<std::string, std::string>;

    class RowView {
    public:
        RowView(int index, TableView& parent)
            : i(index)
            , parent(parent)
        {}

        TableCell& operator[](int index)
        {
            assert(parent.isWriteable);
            return parent.m_table.rows[i].data[index];
        }
        TableCell& operator[](const std::string& index)
        {
            assert(parent.isWriteable);
            return parent.m_table.rows[i].data[ind(index)];
        }

        const TableCell& operator[](int index) const { return parent.m_table.rows[i].data[index]; }
        const TableCell& operator[](const std::string& colName) const { return parent.m_table.rows[i].data[ind(colName)]; }

        bool hasColumn(const std::string& colName) const { return parent.ind(colName) >= 0; }

        int index() const { return i; }

        RowValues toValues() const
        {
            RowValues result;
            for (int c = 0; c < parent.m_table.columns.size(); ++c)
                result[parent.m_table.columns[c]] = parent.m_table.rows[i].data[c].str;
            return result;
        }

        void setValues(const RowValues& values)
        {
            for (const auto& p : values)
                (*this)[p.first].str = p.second;
        }

        std::string makeKey() const
        {
            std::string v;
            bool        started = false;
            for (int col : parent.m_primaryKey) {
                if (started)
                    v += "###";
                v += (*this)[col].str;
                started = true;
            }
            return v;
        }

        template<class T>
        void applyIntTransform(const int index, const T& transform)
        {
            TableCell& value = (*this)[index];
            if (value.str.empty())
                return;

            const int newValue = transform(value.toInt());
            value.setInt(newValue);
        }

        template<class T>
        void applyIntTransform(const std::string& colName, const T& transform)
        {
            const int index = ind(colName);
            if (index < 0)
                return;

            applyIntTransform(index, transform);
        }

    private:
        int        ind(const std::string& colName) const { return parent.ind(colName); }
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

    bool hasColumn(const std::string& name) const { return m_columnIndex.contains(name); }
    void appendRow(const RowValues& values);

    void merge(const TableView& source, bool appendNew, bool updateExisting);
    void concat(const TableView& source);

    void clear();

    int rowCount() const { return m_table.rows.size(); }

    template<class T>
    void applyIntTransform(const std::vector<int>& cols, const T& transform)
    {
        for (RowView& row : m_rows) {
            for (int index : cols)
                row.applyIntTransform(index, transform);
        }
    }

    template<class T>
    void applyIntTransform(const int index, const T& transform)
    {
        applyIntTransform({ index }, transform);
    }

    template<class T>
    void applyIntTransform(const StringVector& colNames, const T& transform)
    {
        std::vector<int> cols;
        for (const std::string& colName : colNames) {
            const int index = ind(colName);
            if (index < 0)
                continue;
            cols.push_back(index);
        }

        applyIntTransform(cols, transform);
    }

    template<class T>
    void applyIntTransform(const std::string& key, const T& transform)
    {
        applyIntTransform(StringVector{ key }, transform);
    }

    int ind(const std::string& colName) const noexcept
    {
        auto it = m_columnIndex.find(colName);
        return it == m_columnIndex.cend() ? -1 : it->second;
    }
    int indPK(const std::string& index) const noexcept
    {
        auto it = m_pkIndex.find(index);
        return it == m_pkIndex.cend() ? -1 : it->second;
    }

private:
    Table&                               m_table;
    std::vector<RowView>                 m_rows;
    std::vector<int>                     m_primaryKey;
    std::unordered_map<std::string, int> m_columnIndex;
    std::unordered_map<std::string, int> m_pkIndex;
    bool                                 isWriteable = true;
};

struct ColumnsDesc {
    struct Col {
        std::string code;
        std::string par;
        std::string min;
        std::string max;
    };
    std::vector<Col> m_cols;
    int              m_start = 0;
    ColumnsDesc()            = default;
    ColumnsDesc(const std::string& codeTpl,
                const std::string& parTpl,
                const std::string& minTpl,
                const std::string& maxTpl,
                int                end,
                int                start = 1)
        : m_start(start)
    {
        for (int i = start; i <= end; ++i) {
            Col col{
                argCompat(codeTpl, i),
                argCompat(parTpl, i),
                argCompat(minTpl, i),
                argCompat(maxTpl, i)
            };
            m_cols.push_back(std::move(col));
        }
    }
};
using ColumnsDescList = std::vector<ColumnsDesc>;

struct DropSet {
    struct Item {
        TableCell tc;
        int       prob;
    };
    int               m_noDrop = 0;
    std::vector<Item> m_items;

    void readRow(const TableView::RowView& row);
    void writeRow(TableView::RowView& row) const;

    int getDropSomethingProb() const;
};

namespace Tables {
extern const ColumnsDescList s_descUniques;
extern const ColumnsDescList s_descRunewords;
extern const ColumnsDescList s_descSetItems;

extern const ColumnsDescList s_descGems;

extern const ColumnsDescList s_descAffix;

extern const ColumnsDescList s_descSets;

}

}
