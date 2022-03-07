/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FileFormatCSV.hpp"

#include <cassert>

namespace D2ModGen {

namespace {

class FastCsvTable {
    const char* begin;
    const char* end;
    const char* curr;

public:
    FastCsvTable(const char* begin, size_t length)
    {
        this->begin = begin;
        this->curr  = begin;
        this->end   = begin + length;
    }
    bool scanLine()
    {
        if (curr >= end)
            return false;

        const char* peek = curr;
        size_t      tabs = 0;
        while (peek < end) {
            if (*peek == '\t')
                tabs++;
            if (*peek == '\r' || *peek == '\n')
                break;
            peek++;
        }
        const char* i       = curr;
        const char* lineEnd = peek;
        line                = std::string_view(curr, peek - curr);
        if (*peek == '\r')
            ++peek;
        if (*peek == '\n')
            ++peek;

        curr = peek;
        row.resize(tabs + 1);
        size_t      index = 0;
        const char* prevI = i;
        while (i < lineEnd) {
            if (*i == '\t') {
                row[index] = i == prevI ? std::string_view() : std::string_view(prevI, i - prevI);
                prevI      = i + 1;
                index++;
            }
            i++;
        }
        row[index] = i == prevI ? std::string_view() : std::string_view(prevI, i - prevI);

        return true;
    }

    std::vector<std::string_view> row;
    std::string_view              line;
};

}

bool writeCSVToBuffer(std::string& csvData, const Table& table)
{
    for (size_t i = 0; i < table.columns.size(); ++i) {
        if (i > 0)
            csvData += '\t';
        csvData += table.columns[i];
    }
    csvData += "\r\n";
    for (const auto& row : table.rows) {
        for (size_t i = 0; i < row.data.size(); ++i) {
            if (i > 0)
                csvData += '\t';
            csvData += row.data[i].str;
        }
        csvData += "\r\n";
    }
    return true;
}

bool readCSVFromBuffer(const std::string& csvData, Table& table)
{
    FastCsvTable csvTable(csvData.data(), csvData.size());
    if (!csvTable.scanLine())
        return false;
    table.columns.resize(csvTable.row.size());
    for (size_t i = 0; i < csvTable.row.size(); ++i)
        table.columns[i] = std::string(csvTable.row[i]);

    std::vector<TableCell> data;
    while (csvTable.scanLine()) {
        data.resize(csvTable.row.size());
        for (size_t i = 0; i < csvTable.row.size(); ++i)
            data[i].str = std::string(csvTable.row[i]);
        table.rows.push_back(TableRow(data));
    }

#ifndef NDEBUG
    {
        std::string check;
        writeCSVToBuffer(check, table);
        assert(check == csvData);
    }
#endif
    return true;
}

}
