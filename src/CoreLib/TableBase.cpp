/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "TableBase.hpp"

namespace D2ModGen {

int TableCell::toInt() const
{
    return std::atoi(str.c_str());
}

void TableCell::setInt(int value)
{
    str = std::to_string(value);
}

std::string TableCell::toLower() const
{
    return ::D2ModGen::toLower(str);
}

bool TableCell::startsWith(const std::string& s) const
{
    return str.starts_with(s);
}

bool TableCell::endsWith(const std::string& s) const
{
    return str.ends_with(s);
}

bool TableCell::contains(const std::string& s) const
{
    return str.find(s) != std::string::npos;
}

int Table::indexOf(const std::string& col) const
{
    auto it = std::find(columns.cbegin(), columns.cend(), col);
    if (it == columns.cend())
        return -1;
    return it - columns.cbegin();
}

}
