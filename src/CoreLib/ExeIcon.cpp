/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ExeIcon.hpp"

#include <cstdint>
#include <fstream>
#include <string_view>
#include <vector>

namespace D2ModGen {

namespace {

constexpr uint32_t s_rtIcon      = 3;
constexpr uint32_t s_rtGroupIcon = 14;
constexpr uint32_t s_subdirFlag  = 0x80000000U;
constexpr char     s_pngMagic[]  = "\x89PNG\r\n\x1a\n";

uint32_t readLe(std::string_view data, size_t offset, size_t length)
{
    uint32_t value = 0;
    for (size_t i = length; i > 0; --i)
        value = (value << 8) | static_cast<uint8_t>(data[offset + i - 1]);
    return value;
}

class PeReader {
public:
    explicit PeReader(const Mernel::std_path& path)
        : m_file(path, std::ios::binary | std::ios::ate)
    {
        if (m_file)
            m_size = static_cast<uint64_t>(m_file.tellg());
    }

    std::optional<std::string> read(uint64_t offset, uint64_t length)
    {
        if (!m_file || offset > m_size || length > m_size - offset)
            return std::nullopt;
        std::string result(length, '\0');
        m_file.seekg(static_cast<std::streamoff>(offset));
        if (!m_file.read(result.data(), static_cast<std::streamsize>(length)))
            return std::nullopt;
        return result;
    }

    std::optional<uint32_t> u16(uint64_t offset) { return readInt(offset, 2); }
    std::optional<uint32_t> u32(uint64_t offset) { return readInt(offset, 4); }

private:
    std::optional<uint32_t> readInt(uint64_t offset, uint64_t length)
    {
        const auto bytes = read(offset, length);
        if (!bytes)
            return std::nullopt;
        return readLe(*bytes, 0, length);
    }

    std::ifstream m_file;
    uint64_t      m_size = 0;
};

struct Section {
    uint32_t virtualAddress;
    uint32_t virtualSize;
    uint32_t rawOffset;
    uint32_t rawSize;
};

class PeResources {
public:
    explicit PeResources(const Mernel::std_path& path)
        : m_reader(path)
    {
    }

    bool parseHeaders()
    {
        if (m_reader.u16(0) != 0x5A4D) // "MZ"
            return false;
        const auto peOffset = m_reader.u32(0x3C);
        if (!peOffset || m_reader.u32(*peOffset) != 0x00004550) // "PE\0\0"
            return false;
        const uint64_t coff         = *peOffset + 4;
        const auto     sectionCount = m_reader.u16(coff + 2);
        const auto     optionalSize = m_reader.u16(coff + 16);
        const uint64_t optional     = coff + 20;
        const auto     magic        = m_reader.u16(optional);
        if (!sectionCount || !optionalSize || !magic)
            return false;
        // Data directories follow the fixed part of the optional header, preceded by their count.
        uint64_t dataDirs = 0;
        if (*magic == 0x10B)
            dataDirs = optional + 96;
        else if (*magic == 0x20B)
            dataDirs = optional + 112;
        else
            return false;
        const auto dirCount = m_reader.u32(dataDirs - 4);
        const auto rsrcRva  = m_reader.u32(dataDirs + 2 * 8);
        if (!dirCount || *dirCount <= 2 || !rsrcRva || *rsrcRva == 0)
            return false;

        uint64_t header = optional + *optionalSize;
        for (uint32_t i = 0; i < *sectionCount; ++i, header += 40) {
            const auto table = m_reader.read(header, 40);
            if (!table)
                return false;
            m_sections.push_back({ readLe(*table, 12, 4), readLe(*table, 8, 4), readLe(*table, 20, 4), readLe(*table, 16, 4) });
        }
        const auto base = rvaToOffset(*rsrcRva, 16);
        if (!base)
            return false;
        m_rsrcBase = *base;
        return true;
    }

    std::optional<std::string> findPngIcon()
    {
        const auto group = findData(s_rtGroupIcon, std::nullopt);
        if (!group || group->size() < 6)
            return std::nullopt;
        const auto count = readLe(*group, 4, 2);
        // GRPICONDIR is 6 bytes; each GRPICONDIRENTRY is 14 bytes ending with the RT_ICON id.
        for (size_t i = 0; i < count && 6 + (i + 1) * 14 <= group->size(); ++i) {
            const auto frame = findData(s_rtIcon, readLe(*group, 6 + i * 14 + 12, 2));
            if (frame && frame->starts_with(std::string_view(s_pngMagic, sizeof(s_pngMagic) - 1)))
                return frame;
        }
        return std::nullopt;
    }

private:
    std::optional<uint64_t> rvaToOffset(uint32_t rva, uint32_t length) const
    {
        for (const auto& section : m_sections) {
            if (rva < section.virtualAddress)
                continue;
            const uint64_t delta = rva - section.virtualAddress;
            if (delta < section.rawSize && length <= section.rawSize - delta)
                return section.rawOffset + delta;
        }
        return std::nullopt;
    }

    struct Entry {
        uint32_t id;
        uint32_t offset;
    };

    std::vector<Entry> directoryEntries(uint32_t dirOffset)
    {
        const uint64_t dir   = m_rsrcBase + dirOffset;
        const auto     named = m_reader.u16(dir + 12);
        const auto     ids   = m_reader.u16(dir + 14);
        const auto     table = named && ids ? m_reader.read(dir + 16, (*named + *ids) * 8ULL) : std::nullopt;
        if (!table)
            return {};
        std::vector<Entry> entries;
        for (uint32_t i = 0; i < *named + *ids; ++i)
            entries.push_back({ readLe(*table, i * 8, 4), readLe(*table, i * 8 + 4, 4) });
        return entries;
    }

    std::optional<Entry> findEntry(uint32_t dirOffset, std::optional<uint32_t> id)
    {
        for (const auto& entry : directoryEntries(dirOffset))
            if (!id || entry.id == *id)
                return entry;
        return std::nullopt;
    }

    // Resource tree levels: type -> name/id -> language -> data entry.
    std::optional<std::string> findData(uint32_t type, std::optional<uint32_t> id)
    {
        const auto typeEntry = findEntry(0, type);
        if (!typeEntry || !(typeEntry->offset & s_subdirFlag))
            return std::nullopt;
        const auto nameEntry = findEntry(typeEntry->offset & ~s_subdirFlag, id);
        if (!nameEntry || !(nameEntry->offset & s_subdirFlag))
            return std::nullopt;
        const auto langEntry = findEntry(nameEntry->offset & ~s_subdirFlag, std::nullopt);
        if (!langEntry || (langEntry->offset & s_subdirFlag))
            return std::nullopt;
        const auto dataRva  = m_reader.u32(m_rsrcBase + langEntry->offset);
        const auto dataSize = m_reader.u32(m_rsrcBase + langEntry->offset + 4);
        if (!dataRva || !dataSize)
            return std::nullopt;
        const auto offset = rvaToOffset(*dataRva, *dataSize);
        if (!offset)
            return std::nullopt;
        return m_reader.read(*offset, *dataSize);
    }

    PeReader             m_reader;
    std::vector<Section> m_sections;
    uint64_t             m_rsrcBase = 0;
};

}

std::optional<std::string> extractExePngIcon(const Mernel::std_path& exe)
{
    PeResources resources(exe);
    if (!resources.parseHeaders())
        return std::nullopt;
    return resources.findPngIcon();
}

}
