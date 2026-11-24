/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <cstddef>
#include <optional>

namespace D2ModGen {

struct GeneratedTableSchema {
    // Canonical lowercase names, including expanded array columns.
    std::vector<std::string> columns;

    // Lowercase alias -> lowercase canonical name.
    std::map<std::string, std::string> aliases;
};

inline const std::string& missingCsvCell()
{
    static const std::string value("\0\0", 2);
    return value;
}

class CsvRowReader {
public:
    explicit CsvRowReader(const std::vector<std::string>& cells)
        : m_cells(cells)
    {
    }

    bool available(std::size_t column) const noexcept
    {
        return column < m_cells.size()
               && m_cells[column] != missingCsvCell();
    }

    void read(std::size_t column, std::string& destination) const
    {
        if (available(column))
            destination = m_cells[column];
        else
            destination.clear();
    }

    void read(std::size_t column, std::optional<int>& destination) const
    {
        if (!available(column) || m_cells[column].empty()) {
            destination.reset();
            return;
        }

        destination = std::atoi(m_cells[column].c_str());
    }

    void read(std::size_t column, std::optional<bool>& destination) const
    {
        if (!available(column) || m_cells[column].empty()) {
            destination.reset();
            return;
        }

        destination = std::atoi(m_cells[column].c_str()) != 0;
    }

    void read(
        std::size_t          column,
        std::optional<bool>& destination,
        bool                 valueIfNoColumn) const
    {
        if (!available(column)) {
            destination = valueIfNoColumn;
            return;
        }

        if (m_cells[column].empty()) {
            destination.reset();
            return;
        }

        destination = std::atoi(m_cells[column].c_str()) != 0;
    }

private:
    const std::vector<std::string>& m_cells;
};

class CsvRowWriter {
public:
    explicit CsvRowWriter(std::vector<std::string>& cells)
        : m_cells(cells)
    {
    }

    void write(std::size_t column, const std::string& value)
    {
        m_cells[column] = value;
    }

    void write(std::size_t column, const std::optional<int>& value)
    {
        if (value.has_value())
            m_cells[column] = std::to_string(value.value());
        else
            m_cells[column].clear();
    }

    void write(std::size_t column, const std::optional<bool>& value)
    {
        if (value.has_value())
            m_cells[column] = std::to_string(static_cast<int>(value.value()));
        else
            m_cells[column].clear();
    }

private:
    std::vector<std::string>& m_cells;
};

}
