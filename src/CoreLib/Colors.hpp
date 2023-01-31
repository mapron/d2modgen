/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <string>

namespace D2ModGen {

enum class Color
{
    white,  // (Brighter than Light Gray)
    grey,   // (Item Descriptions)
    red,    //
    green,  //  (Set Items)
    blue,   // (Magic Items)
    gold,   // (Unique Items)
    dgrey,  // (Socketed/Ethereal Items)
    black,  //  (Text Doesn't Show)
    tan,    //
    orange, // (Crafted Items)
    yellow, // (Rare Items)
    dgreen, //
    purple, //

    colorcode,

    transparent = black,
};

struct ColorDesc {
    struct RGB {
        int r, g, b;
    };
    Color       m_color;
    char        m_shortCode;
    std::string m_fullBinaryCode;
    RGB         m_rgb;
    std::string m_userReadable;
};

const ColorDesc& getColorDesc(Color color);

std::string replaceColorsToUser(std::string s);
std::string replaceColorsToBinary(std::string s);

}
