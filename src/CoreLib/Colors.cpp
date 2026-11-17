/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "Colors.hpp"

#include <vector>
#include <map>
#include <cassert>

namespace D2ModGen {

const std::string s_binaryPrefixCode{ "\xC3\xBF\x63" };

namespace {

static const std::vector<ColorDesc::RGB> s_RGB{
    { 255, 255, 255 },
    { 0xc0, 0xc0, 0xc0 },
    { 255, 0, 0 },
    { 0, 255, 0 },
    { 80, 80, 200 },
    { 160, 145, 105 },
    { 0x80, 0x80, 0x80 },
    { 0, 0, 0 },
    { 170, 160, 120 },
    { 250, 170, 35 },
    { 255, 255, 0 },
    { 0, 0x80, 0 },
    { 150, 90, 250 },
    { 0, 0, 0 },
};

static const std::vector<char> s_shortCodes{
    0x2F,
    0x30,
    0x31,
    0x32,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x38,
    0x39,
    0x3A,
    0x3B,
    0x00,
};
static const std::vector<std::string> s_user{
    "white",
    "grey",
    "red",
    "green",
    "blue",
    "gold",
    "dgrey",
    "black",
    "tan",
    "orange",
    "yellow",
    "dgreen",
    "purple",
    "colorcode",
};
static const std::vector<ColorDesc> s_colors = []() {
    std::vector<ColorDesc> result(s_shortCodes.size());
    for (size_t i = 0; i < result.size(); i++) {
        result[i].m_color          = static_cast<Color>(i);
        result[i].m_rgb            = s_RGB[i];
        result[i].m_shortCode      = s_shortCodes[i];
        result[i].m_fullBinaryCode = s_binaryPrefixCode + s_shortCodes[i];
        result[i].m_userReadable   = s_user[i];
    }
    return result;
}();

}

const ColorDesc& getColorDesc(Color color)
{
    return s_colors[static_cast<size_t>(color)];
}

}
