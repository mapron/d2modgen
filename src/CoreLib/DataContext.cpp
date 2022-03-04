/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "TableUtils.hpp"
#include "FileIOUtils.hpp"

#include <QFile>
#include "Logger.hpp"

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

bool writeCSV(std::string& csvData, const Table& table)
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

bool readCSV(const std::string& csvData, Table& table)
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
        writeCSV(check, table);
        assert(check == csvData);
    }
#endif
    return true;
}

}

bool DataContext::readData(const IStorage::StoredData& data)
{
    for (const auto& fileData : data.tables) {
        if (fileData.id.empty()) {
            Logger(Logger::Warning) << "Empty csv id found!";
            return false;
        }

        Table table;
        table.id = fileData.id;
        if (!readCSV(fileData.data, table)) {
            Logger(Logger::Warning) << "failed to parse csv:" << fileData.id.c_str();
            return false;
        }
        tableSet.tables[fileData.id] = std::move(table);
        tableSet.relativeNames.insert(IStorage::makeTableRelativePath(fileData.id, false));
    }
    for (const auto& fileData : data.inMemoryFiles) {
        if (fileData.data.empty()) {
            Logger(Logger::Warning) << "Json data is empty:" << fileData.relFilepath.c_str();
            return false;
        }

        PropertyTree doc;
        if (!readJsonFromBuffer(fileData.data, doc)) {
            Logger(Logger::Warning) << "failed to parse json:" << fileData.relFilepath.c_str();
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "duplicate json file found:" << fileData.relFilepath.c_str();
            return false;
        }

        jsonFiles[fileData.relFilepath] = doc;
    }
    for (const auto& fileData : data.refFiles) {
        if (!QFile::exists(QString::fromStdString(fileData.absSrcFilepath))) {
            Logger(Logger::Warning) << "Non-existent file:" << fileData.absSrcFilepath.c_str();
            return false;
        }
        if (jsonFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "File for plain copy already exists in json list:" << fileData.relFilepath.c_str();
            return false;
        }
        if (tableSet.relativeNames.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "File for plain copy already exists in CSV list:" << fileData.relFilepath.c_str();
            return false;
        }
        if (copyFiles.contains(fileData.relFilepath)) {
            Logger(Logger::Warning) << "duplicate copy file found:" << fileData.relFilepath.c_str();
            return false;
        }
        copyFiles[fileData.relFilepath] = fileData;
    }

    return true;
}

bool DataContext::writeData(IStorage::StoredData& data) const
{
    for (const auto& p : tableSet.tables) {
        const Table& table = p.second;
        if (!table.modified && !table.forceOutput)
            continue;

        std::string tableStr;
        if (!writeCSV(tableStr, table))
            return false;
        data.tables.push_back(IStorage::StoredFileTable{ std::move(tableStr), table.id });
    }
    for (const auto& p : jsonFiles) {
        std::string buffer;
        writeJsonToBuffer(buffer, p.second);
        //datastr.replace(QByteArray("\xC3\x83\xC2\xBF\x63"), QByteArray("\xC3\xBF\x63")); // hack: replace color codes converter to UTF-8 from latin-1.
        data.inMemoryFiles.push_back(IStorage::StoredFileMemory{ buffer, p.first });
    }
    for (const auto& p : copyFiles) {
        data.refFiles.push_back(p.second);
    }
    return true;
}

bool DataContext::mergeWith(const DataContext& source, ConflictPolicy policy)
{
    for (const auto& p : source.tableSet.tables) {
        const Table& sourceTable = p.second;
        if (!tableSet.tables.contains(sourceTable.id)) {
            Logger(Logger::Warning) << "unknown table id:" << sourceTable.id.c_str();
            continue;
        }
        Table& destTable = tableSet.tables[sourceTable.id];
        if (destTable.modified) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }

        const TableView sourceView(sourceTable);
        TableView       destView(destTable);
        destTable.modified = true;

        // already modified, need to resolve conflicts:
        switch (policy) {
            case ConflictPolicy::Skip:
            case ConflictPolicy::RaiseError:
            case ConflictPolicy::Replace: // clear previous data
            {
                destView.clear();
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Append: // place all data at the end
            {
                destView.concat(sourceView);
                break;
            }
            case ConflictPolicy::Update: // overwrite records with same key
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, false, true);
                break;
            }
            case ConflictPolicy::AppendNew: // place new data at the end
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, false);
                break;
            }
            case ConflictPolicy::Merge: // try Update, then AppendNew
            {
                if (!destView.createRowIndex())
                    return false;

                destView.merge(sourceView, true, true);
                break;
            }
        };
    }

    for (const auto& p : source.jsonFiles) {
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        jsonFiles[p.first] = p.second;
    }

    for (const auto& p : source.copyFiles) {
        if (copyFiles.contains(p.first)) {
            if (policy == ConflictPolicy::RaiseError)
                return false;
            if (policy == ConflictPolicy::Skip)
                continue;
        }
        if (jsonFiles.contains(p.first)) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        if (tableSet.relativeNames.contains(toLower(p.first))) {
            if (policy == ConflictPolicy::Skip)
                continue;
            return false;
        }
        copyFiles[p.first] = p.second;
    }

    return true;
}

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
