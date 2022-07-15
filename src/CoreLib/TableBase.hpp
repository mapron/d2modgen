/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"
#include "TableId.hpp"

#include <deque>

namespace D2ModGen {

struct TableCell {
    std::string str;

    TableCell()                 = default;
    TableCell(const TableCell&) = default;
    TableCell(std::string s)
        : str(std::move(s))
    {}

    int  toInt() const;
    void setInt(int value);
    bool isEmpty() const { return str.empty(); }
    void clear() { str.clear(); }

    std::string toLower() const;

    bool startsWith(const std::string& s) const;
    bool endsWith(const std::string& s) const;
    bool contains(const std::string& s) const;

    bool operator==(const TableCell&) const = default;

    bool operator==(const std::string& s) const { return s == str; }
    bool operator!=(const std::string& s) const { return s != str; }
};

struct TableRow {
    std::vector<TableCell> data;
    TableRow() = default;
    explicit TableRow(int size)
    {
        data.resize(size);
    }
    explicit TableRow(std::vector<TableCell> data)
        : data(std::move(data))
    {
    }
    bool operator==(const TableRow&) const = default;
};

struct Table {
    TableId                  id;
    std::deque<TableRow>     rows;
    std::vector<std::string> columns;
    bool                     modified    = false;
    bool                     forceOutput = false;

    int indexOf(const std::string& col) const;
};

}
