/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace D2ModGen::Test {

inline const std::string g_png = std::string("\x89PNG\r\n\x1a\n", 8) + "png-frame";
inline const std::string g_bmp = std::string("\x28\0\0\0", 4) + "bmp-frame";

inline void putLe(std::string& data, size_t offset, uint32_t value, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        data[offset + i] = static_cast<char>(value >> (8 * i));
}

struct Frame {
    uint16_t    id;
    std::string bytes;
};

/// Minimal PE: headers, one .rsrc section at file offset/RVA 0x200 holding one icon group.
inline std::string buildPe(bool pe32Plus, const std::vector<uint16_t>& groupIds, const std::vector<Frame>& frames)
{
    constexpr uint32_t rsrc = 0x200;
    std::string        tree(0x400, '\0');
    size_t             pos = 0;

    auto directory = [&](const std::vector<std::pair<uint32_t, uint32_t>>& entries) {
        const size_t at = pos;
        putLe(tree, at + 14, static_cast<uint32_t>(entries.size()), 2);
        for (size_t i = 0; i < entries.size(); ++i) {
            putLe(tree, at + 16 + i * 8, entries[i].first, 4);
            putLe(tree, at + 20 + i * 8, entries[i].second, 4);
        }
        pos += 16 + entries.size() * 8;
        return static_cast<uint32_t>(at);
    };
    std::string payload;
    auto        leaf = [&](const std::string& data) {
        const uint32_t entry = directory({ { 0x409, static_cast<uint32_t>(pos + 24) } });
        putLe(tree, pos, rsrc + 0x400 + static_cast<uint32_t>(payload.size()), 4);
        putLe(tree, pos + 4, static_cast<uint32_t>(data.size()), 4);
        pos += 16;
        payload += data;
        return entry | 0x80000000U;
    };

    std::string group(6 + groupIds.size() * 14, '\0');
    putLe(group, 2, 1, 2);
    putLe(group, 4, static_cast<uint32_t>(groupIds.size()), 2);
    for (size_t i = 0; i < groupIds.size(); ++i)
        putLe(group, 6 + i * 14 + 12, groupIds[i], 2);

    pos                                                  = 0x100;
    const uint32_t                             groupLeaf = leaf(group);
    const uint32_t                             groupDir  = directory({ { 1, groupLeaf } });
    std::vector<std::pair<uint32_t, uint32_t>> icons;
    for (const auto& frame : frames)
        icons.push_back({ frame.id, leaf(frame.bytes) });
    const uint32_t iconDir = directory(icons);
    pos                    = 0;
    directory({ { 3, iconDir | 0x80000000U }, { 14, groupDir | 0x80000000U } });

    const std::string section = tree + payload;
    std::string       image(rsrc, '\0');
    image[0]            = 'M';
    image[1]            = 'Z';
    constexpr size_t pe = 0x40;
    putLe(image, 0x3C, pe, 4);
    image.replace(pe, 4, std::string("PE\0\0", 4));
    const uint16_t optionalSize = pe32Plus ? 240 : 224;
    putLe(image, pe + 6, 1, 2);
    putLe(image, pe + 20, optionalSize, 2);
    const size_t optional = pe + 24;
    putLe(image, optional, pe32Plus ? 0x20B : 0x10B, 2);
    const size_t dataDirs = optional + (pe32Plus ? 112 : 96);
    putLe(image, dataDirs - 4, 16, 4);
    putLe(image, dataDirs + 16, rsrc, 4);
    putLe(image, dataDirs + 20, static_cast<uint32_t>(section.size()), 4);
    const size_t header = optional + optionalSize;
    image.replace(header, 5, ".rsrc");
    putLe(image, header + 8, static_cast<uint32_t>(section.size()), 4);
    putLe(image, header + 12, rsrc, 4);
    putLe(image, header + 16, static_cast<uint32_t>(section.size()), 4);
    putLe(image, header + 20, rsrc, 4);
    return image + section;
}

}
