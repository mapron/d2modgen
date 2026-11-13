/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "MernelPlatform/PropertyTree.hpp"
#include "ModgenCoreExport.hpp"

#include <cstdint>
#include <array>

namespace D2ModGen::Image {
struct Size;
struct Point {
    int x{};
    int y{};

    void  read(const Mernel::PropertyTree& data);
    void  write(Mernel::PropertyTree& data) const;
    Point operator+(const Point& s);

    Point&      operator+=(const Point& r);
    Point&      operator/=(int downscale);
    std::string toString() const;
};

struct Size {
    int width{};
    int height{};

    void        read(const Mernel::PropertyTree& data);
    void        write(Mernel::PropertyTree& data) const;
    Size&       operator/=(int downscale);
    std::string toString() const;
};

struct Rect {
    Point topLeft;
    Size  size;

    int getTop() const { return topLeft.y; }
    int getBottom() const { return topLeft.y + size.height - 1; }
    int getLeft() const { return topLeft.x; }
    int getRight() const { return topLeft.x + size.width - 1; }

    Point getTopRight() const
    {
        return { getRight(), getTop() };
    }
    Point getBottomRight() const
    {
        return { getRight(), getBottom() };
    }
    Point getBottomLeft() const
    {
        return { getLeft(), getBottom() };
    }
    int getHeight() const { return size.height; }
    int getWidth() const { return size.width; }

    bool isValid() const
    {
        return size.width > 0 && size.height > 0;
    }

    bool read(const Mernel::PropertyTree& data)
    {
        topLeft = {};
        size    = {};
        topLeft.read(data);
        size.read(data);
        return true;
    }
    void write(Mernel::PropertyTree& data, bool writeSize = false) const
    {
        if (!data.isMap())
            data = {};

        topLeft.write(data);
        if (writeSize)
            size.write(data);
    }
    Rect& operator/=(int downscale)
    {
        topLeft /= downscale;
        size /= downscale;
        return *this;
    }
    std::string toString() const { return "Rect{ " + topLeft.toString() + ", " + size.toString() + " }"; }

    void expand(int thickness)
    {
        topLeft.x -= thickness;
        topLeft.y -= thickness;
        size.width += thickness * 2;
        size.height += thickness * 2;
    }
};

struct Color {
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};

    bool operator==(const Color&) const noexcept = default;
};

struct Pal8Bitmap {
    std::vector<uint8_t> pixels;
    Size                 size;
    Point                offset;
    uint8_t              trans = 0;
};
using Pal8BitmapList = std::vector<Pal8Bitmap>;

struct MODGENCORE_EXPORT Palette {
    std::array<Color, 256> colors{};

    static const Palette s_defaultPalette;
};

struct MODGENCORE_EXPORT Bitmap {
    std::vector<uint8_t> rgba;
    Size                 size;

    int frameWidth = 0;
    int frameCount = 1;

    Color brush;

    void           alloc();
    void           resize(Size size);
    uint8_t*       getPixel(Point point);
    const uint8_t* getPixel(Point point) const;
    Color          getColor(Point point) const;

    void putDot(Point point);
    void putDot(Point point, Color c);

    void dumpToBuffer(std::string& buffer) const;
    bool readFromSprite(const std::string& spriteBuffer);
    void writeToSprite(std::string& spriteBuffer) const;
    void readFromPal8(const Pal8Bitmap& palBitmap, const Palette& palette);
    void writeToPal8(Pal8Bitmap& palBitmap, const Palette& palette);

    void paintLineH(Point point, int width);
    void paintLineV(Point point, int height);

    void fill();
    void copyBorders(const Bitmap& src, Size cornerSize);

    void fillRect(Rect rect);
    void linedRect(Rect rect);

    void draw(Point destPoint, const Bitmap& src, Point srcPoint, Size size);

    void drawDot(Point destPoint, const Bitmap& src, Point srcPoint);
};

}
