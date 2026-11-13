/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "Image.hpp"

#include "MernelPlatform/ByteOrderStream.hpp"

#include <cmath>

namespace D2ModGen::Image {

namespace {

uint8_t s_defaultPaletteGBR[768] = {
    // clang-format off
    0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x08, 0x18, 0x1C, 0x10, 0x24, 0x2C, 0x18, 0x34, 0x3C, 0x00,
    0x00, 0x5C, 0x20, 0x40, 0x48, 0x28, 0x48, 0x54, 0x00, 0x00, 0x90, 0x10, 0x48, 0x8C, 0x00, 0x00,
    0xBC, 0x20, 0x84, 0xD0, 0x6C, 0xC4, 0xF4, 0x50, 0x7C, 0x8C, 0x64, 0x9C, 0xAC, 0x08, 0x0C, 0x0C,
    0x10, 0x10, 0x14, 0x1C, 0x1C, 0x1C, 0x1C, 0x24, 0x28, 0x2C, 0x2C, 0x2C, 0x30, 0x38, 0x3C, 0x38,
    0x38, 0x38, 0x48, 0x48, 0x48, 0x48, 0x50, 0x58, 0x34, 0x58, 0x64, 0x58, 0x58, 0x58, 0x3C, 0x64,
    0x74, 0x64, 0x64, 0x64, 0x70, 0x74, 0x7C, 0x84, 0x84, 0x84, 0x94, 0x98, 0x9C, 0xC4, 0xC4, 0xC4,
    0xF4, 0xF4, 0xF4, 0x04, 0x04, 0x08, 0x00, 0x04, 0x10, 0x04, 0x08, 0x18, 0x08, 0x10, 0x18, 0x10,
    0x14, 0x1C, 0x04, 0x0C, 0x24, 0x0C, 0x18, 0x20, 0x14, 0x18, 0x20, 0x08, 0x10, 0x2C, 0x10, 0x1C,
    0x24, 0x0C, 0x20, 0x28, 0x04, 0x08, 0x38, 0x10, 0x1C, 0x30, 0x14, 0x28, 0x30, 0x0C, 0x14, 0x40,
    0x10, 0x28, 0x38, 0x04, 0x0C, 0x48, 0x1C, 0x28, 0x38, 0x0C, 0x20, 0x4C, 0x14, 0x2C, 0x44, 0x20,
    0x2C, 0x40, 0x08, 0x10, 0x58, 0x20, 0x34, 0x48, 0x28, 0x34, 0x44, 0x1C, 0x28, 0x54, 0x14, 0x1C,
    0x5C, 0x08, 0x24, 0x5C, 0x18, 0x38, 0x54, 0x24, 0x38, 0x54, 0x10, 0x18, 0x6C, 0x10, 0x2C, 0x68,
    0x24, 0x44, 0x5C, 0x18, 0x24, 0x70, 0x24, 0x3C, 0x68, 0x0C, 0x2C, 0x7C, 0x2C, 0x4C, 0x64, 0x08,
    0x48, 0x70, 0x18, 0x28, 0x80, 0x28, 0x50, 0x74, 0x24, 0x30, 0x88, 0x40, 0x50, 0x6C, 0x0C, 0x34,
    0x8C, 0x68, 0x20, 0x70, 0x44, 0x58, 0x78, 0x40, 0x40, 0x8C, 0x30, 0x5C, 0x84, 0x24, 0x34, 0x9C,
    0x18, 0x28, 0xA4, 0x14, 0x48, 0xA4, 0x40, 0x64, 0x8C, 0x38, 0x70, 0x8C, 0x50, 0x68, 0x8C, 0x34,
    0x44, 0xB0, 0x20, 0x4C, 0xB4, 0x58, 0x70, 0x98, 0x40, 0x78, 0xA0, 0x1C, 0x60, 0xBC, 0x48, 0x84,
    0x9C, 0x54, 0x54, 0xC4, 0x20, 0x6C, 0xC8, 0x58, 0x7C, 0xAC, 0x48, 0x88, 0xB0, 0x54, 0x7C, 0xC8,
    0x20, 0x70, 0xE0, 0x54, 0x9C, 0xB8, 0x00, 0x2C, 0xFC, 0x70, 0x8C, 0xC0, 0x50, 0x98, 0xCC, 0x30,
    0x84, 0xE4, 0x70, 0x70, 0xE0, 0x78, 0x98, 0xD0, 0x3C, 0x88, 0xF8, 0x38, 0xA0, 0xEC, 0x64, 0xB8,
    0xD8, 0x84, 0xA4, 0xE0, 0x44, 0xB4, 0xF0, 0x4C, 0xC0, 0xF4, 0x8C, 0xB0, 0xF0, 0x5C, 0xD4, 0xFC,
    0xB0, 0xB0, 0xFC, 0x10, 0x24, 0x04, 0x18, 0x24, 0x14, 0x18, 0x3C, 0x20, 0x0C, 0x48, 0x18, 0x30,
    0x44, 0x00, 0x08, 0x64, 0x18, 0x24, 0x5C, 0x24, 0x24, 0x5C, 0x38, 0x44, 0x6C, 0x08, 0x14, 0x7C,
    0x28, 0x34, 0x74, 0x40, 0x30, 0x78, 0x58, 0x1C, 0x9C, 0x34, 0x38, 0x84, 0x70, 0x34, 0xA0, 0x48,
    0x4C, 0x90, 0x58, 0x28, 0xBC, 0x44, 0x48, 0x98, 0x84, 0x4C, 0xB8, 0x60, 0x00, 0xFC, 0x18, 0x5C,
    0xDC, 0x74, 0x7C, 0xD0, 0x8C, 0x88, 0xFC, 0xA0, 0x28, 0x0C, 0x0C, 0x48, 0x18, 0x18, 0x58, 0x00,
    0x00, 0x44, 0x14, 0x38, 0x68, 0x24, 0x10, 0x64, 0x3C, 0x28, 0x78, 0x28, 0x28, 0x80, 0x10, 0x4C,
    0x84, 0x4C, 0x38, 0x94, 0x30, 0x30, 0x8C, 0x60, 0x48, 0xA0, 0x5C, 0x38, 0xAC, 0x50, 0x50, 0xAC,
    0x6C, 0x4C, 0xBC, 0x78, 0x54, 0xD8, 0x60, 0x24, 0xD0, 0x78, 0x64, 0xE0, 0x90, 0x64, 0xDC, 0xA0,
    0x80, 0xFC, 0x20, 0xA4, 0xF0, 0x84, 0x84, 0xFC, 0xA0, 0xA0, 0xFC, 0xB8, 0x90, 0x90, 0x8C, 0x58,
    0xA4, 0xA0, 0x68, 0xC4, 0xC0, 0x84, 0xD4, 0xD0, 0x98, 0xFC, 0xCC, 0xA8, 0xF4, 0xF4, 0xCC, 0x80,
    0xA0, 0xC0, 0xA8, 0xC0, 0xC4, 0x94, 0xC4, 0xE0, 0x74, 0xE8, 0xFC, 0xB0, 0xFC, 0xC4, 0xA4, 0xE4,
    0xFC, 0xC4, 0xFC, 0xFC, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x0C, 0x0C, 0x0C, 0x10, 0x10, 0x10,
    0x14, 0x14, 0x14, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x24, 0x20, 0x20, 0x20, 0x24, 0x24, 0x24, 0x28,
    0x28, 0x28, 0x20, 0x28, 0x30, 0x30, 0x30, 0x30, 0x28, 0x30, 0x38, 0x34, 0x34, 0x34, 0x3C, 0x38,
    0x34, 0x34, 0x38, 0x44, 0x3C, 0x3C, 0x3C, 0x30, 0x3C, 0x4C, 0x40, 0x40, 0x40, 0x3C, 0x40, 0x44,
    0x44, 0x44, 0x44, 0x3C, 0x48, 0x50, 0x38, 0x44, 0x58, 0x4C, 0x4C, 0x4C, 0x3C, 0x4C, 0x60, 0x5C,
    0x58, 0x34, 0x50, 0x50, 0x50, 0x54, 0x54, 0x54, 0x5C, 0x5C, 0x5C, 0x50, 0x5C, 0x68, 0x60, 0x60,
    0x60, 0x74, 0x70, 0x44, 0x50, 0x64, 0x7C, 0x68, 0x68, 0x68, 0x6C, 0x6C, 0x6C, 0x60, 0x6C, 0x78,
    0x70, 0x70, 0x70, 0x74, 0x74, 0x74, 0x7C, 0x7C, 0x7C, 0x64, 0x80, 0x94, 0x74, 0x84, 0x90, 0x70,
    0x88, 0xAC, 0x90, 0x90, 0x90, 0x84, 0x94, 0x9C, 0xB8, 0x94, 0x80, 0xA0, 0xA0, 0xA0, 0x98, 0xAC,
    0xB0, 0xAC, 0xAC, 0xAC, 0xB8, 0xB8, 0xB8, 0xCC, 0xCC, 0xCC, 0xD8, 0xD8, 0xD8, 0xFC, 0xCC, 0xCC,
    0xE4, 0xE4, 0xE4, 0x80, 0x00, 0xFC, 0x08, 0x14, 0x10, 0x08, 0x1C, 0x14, 0x08, 0x20, 0x14, 0x0C,
    0x24, 0x18, 0x0C, 0x2C, 0x20, 0x18, 0x34, 0x20, 0x1C, 0x38, 0x30, 0x44, 0x20, 0x00, 0x68, 0x00,
    0x18, 0x04, 0x08, 0x08, 0x10, 0x14, 0x14, 0x14, 0x18, 0x18, 0x14, 0x1C, 0x1C, 0x10, 0x20, 0x1C,
    0x18, 0x20, 0x24, 0x14, 0x28, 0x24, 0x20, 0x28, 0x28, 0x24, 0x28, 0x2C, 0x30, 0x2C, 0x20, 0x24,
    0x2C, 0x30, 0x28, 0x30, 0x34, 0x2C, 0x34, 0x38, 0x40, 0x3C, 0x30, 0x34, 0x3C, 0x40, 0x38, 0x44,
    0x48, 0x44, 0x4C, 0x50, 0x4C, 0x58, 0x5C, 0x54, 0x5C, 0x60, 0x58, 0x64, 0x68, 0xFF, 0xFF, 0xFF,
    // clang-format on
};

void makeBorderImage(Bitmap& dest, const Bitmap& src, Size cornerSize)
{
    const int srcW  = src.size.width;
    const int srcH  = src.size.height;
    const int destW = dest.size.width;
    const int destH = dest.size.height;

    const int cW = cornerSize.width;
    const int cH = cornerSize.height;

    const int srcRightX   = srcW - cW;
    const int srcBottomY  = srcH - cH;
    const int destRightX  = destW - cW;
    const int destBottomY = destH - cH;

    const int srcCenterW = srcRightX - cW;
    const int srcCenterH = srcBottomY - cH;

    if (srcCenterW <= 0 || srcCenterH <= 0 || destRightX < cW || destBottomY < cH) {
        return;
    }

    for (int y = 0; y < cH; ++y) {
        for (int x = 0; x < cW; ++x) {
            // Top-Left
            dest.drawDot({ x, y }, src, { x, y });
            dest.drawDot({ destRightX + x, y }, src, { srcRightX + x, y });
            dest.drawDot({ x, destBottomY + y }, src, { x, srcBottomY + y });
            dest.drawDot({ destRightX + x, destBottomY + y }, src, { srcRightX + x, srcBottomY + y });
        }
    }

    for (int x = cW; x < destRightX; ++x) {
        int srcX = cW + ((x - cW) % srcCenterW);
        for (int y = 0; y < cH; ++y) {
            dest.drawDot({ x, y }, src, { srcX, y });
            dest.drawDot({ x, destBottomY + y }, src, { srcX, srcBottomY + y });
        }
    }

    for (int y = cH; y < destBottomY; ++y) {
        int srcY = cH + ((y - cH) % srcCenterH);
        for (int x = 0; x < cW; ++x) {
            dest.drawDot({ x, y }, src, { x, srcY });
            dest.drawDot({ destRightX + x, y }, src, { srcRightX + x, srcY });
        }
    }
}
}

const Palette Palette::s_defaultPalette = []() -> Palette {
    Palette result;
    for (int i = 0; i < result.colors.size(); i++) {
        Color& c = result.colors[i];
        c.r      = s_defaultPaletteGBR[i * 3 + 2];
        c.g      = s_defaultPaletteGBR[i * 3 + 0];
        c.b      = s_defaultPaletteGBR[i * 3 + 1];
        c.a      = i == 0 ? 0 : 255;
    }
    return result;
}();

void Point::read(const Mernel::PropertyTree& data)
{
    x = data.value("x", Mernel::PropertyTreeScalar(0)).toInt();
    y = data.value("y", Mernel::PropertyTreeScalar(0)).toInt();
}

void Point::write(Mernel::PropertyTree& data) const
{
    data["x"] = Mernel::PropertyTreeScalar(x);
    data["y"] = Mernel::PropertyTreeScalar(y);
}

Point Point::operator+(const Point& s)
{
    Point result = *this;
    result.x += s.x;
    result.y += s.y;
    return result;
}

Point& Point::operator+=(const Point& r)
{
    x += r.x;
    y += r.y;
    return *this;
}

Point& Point::operator/=(int downscale)
{
    x /= downscale;
    y /= downscale;
    return *this;
}

std::string Point::toString() const
{
    return "Point{" + std::to_string(x) + ", " + std::to_string(y) + "}";
}

void Size::read(const Mernel::PropertyTree& data)
{
    width  = data.value("width", Mernel::PropertyTreeScalar(0)).toInt();
    height = data.value("height", Mernel::PropertyTreeScalar(0)).toInt();
}

void Size::write(Mernel::PropertyTree& data) const
{
    data["width"]  = Mernel::PropertyTreeScalar(width);
    data["height"] = Mernel::PropertyTreeScalar(height);
}

Size& Size::operator/=(int downscale)
{
    width /= downscale;
    height /= downscale;
    return *this;
}

std::string Size::toString() const
{
    return "Size{ " + std::to_string(width) + ", " + std::to_string(height) + " }";
}

Color Bitmap::getColor(Point point) const
{
    const uint8_t* pix = getPixel(point);
    if (!pix)
        return Color{};
    return Color{ .r = pix[0], .g = pix[1], .b = pix[2], .a = pix[3] };
}

void Bitmap::alloc()
{
    rgba.resize(size.width * size.height * 4);
}

void Bitmap::resize(Size size_)
{
    size = size_;
    alloc();
}

uint8_t* Bitmap::getPixel(Point point)
{
    if (point.x < 0 || point.x >= size.width || point.y < 0 || point.y >= size.height)
        return nullptr;
    size_t offset = size.width * point.y * 4 + point.x * 4;
    if (offset > rgba.size())
        return nullptr;

    return rgba.data() + offset;
}

const uint8_t* Bitmap::getPixel(Point point) const
{
    if (point.x < 0 || point.x >= size.width || point.y < 0 || point.y >= size.height)
        return nullptr;
    size_t offset = size.width * point.y * 4 + point.x * 4;
    if (offset > rgba.size())
        return nullptr;

    return rgba.data() + offset;
}

void Bitmap::putDot(Point point)
{
    putDot(point, brush);
}

void Bitmap::putDot(Point point, Color c)
{
    uint8_t* pix = getPixel(point);
    if (!pix)
        return;
    pix[0] = c.r;
    pix[1] = c.b;
    pix[2] = c.g;
    pix[3] = c.a;
}

void Bitmap::dumpToBuffer(std::string& buffer) const
{
    using namespace Mernel;
    ByteArrayHolder binaryBuffer;

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamWriter stream(bobuffer, ByteOrderDataStream::s_littleEndian);

    const uint32_t imageSize = static_cast<uint32_t>(size.width * size.height * 4);
    const uint32_t fileSize  = 122 + imageSize; // 14 (FileHeader) + 108 (BITMAPV4HEADER)

    // --- BITMAPFILEHEADER (14 bytes) ---
    stream.writeBlock("BM", 2);  // Signature
    stream << uint32_t(fileSize) // File size
           << uint16_t(0)        // Reserved 1
           << uint16_t(0)        // Reserved 2
           << uint32_t(122);     // Offset to pixel data (0x7A)

    // --- BITMAPV4HEADER (108 bytes to support Alpha Channel) ---
    stream << uint32_t(108)       // Header size
           << size.width          // Width
           << size.height         // Height (Positive = bottom-up)
           << uint16_t(1)         // Planes
           << uint16_t(32)        // Bits per pixel (RGBA)
           << uint32_t(3)         // BI_BITFIELDS compression
           << uint32_t(imageSize) // Image size
           << int32_t(2835)       // H-Resolution (72 DPI)
           << int32_t(2835)       // V-Resolution (72 DPI)
           << uint32_t(0)         // Colors items in palette
           << uint32_t(0);        // Important colors

    stream << uint32_t(0x00FF0000)  // Red mask   (Byte 2)
           << uint32_t(0x0000FF00)  // Green mask (Byte 1)
           << uint32_t(0x000000FF)  // Blue mask  (Byte 0)
           << uint32_t(0xFF000000); // Alpha mask (Byte 3)

    stream.writeBlock("BGRs", 4); // bV4CSType: "sRGB"

    stream.zeroPadding(9 * 4);
    stream.zeroPadding(3 * 4); // bV4Gamma

    std::vector<uint8_t> rowBuffer(size.width * 4);

    for (size_t y = 0; y < size.height; ++y) {
        const uint8_t* rowIn = rgba.data() + (size.height - 1 - y) * size.width * 4;

        for (size_t x = 0; x < size.width; ++x) {
            size_t idx         = x * 4;
            rowBuffer[idx + 0] = rowIn[idx + 2]; // B
            rowBuffer[idx + 1] = rowIn[idx + 1]; // G
            rowBuffer[idx + 2] = rowIn[idx + 0]; // R
            rowBuffer[idx + 3] = rowIn[idx + 3]; // A
        }

        stream.writeBlock(reinterpret_cast<const char*>(rowBuffer.data()), rowBuffer.size());
    }

    buffer.resize(bobuffer.getSize());
    memcpy(buffer.data(), binaryBuffer.data(), binaryBuffer.size());
}

bool Bitmap::readFromSprite(const std::string& spriteBuffer)
{
    if (spriteBuffer.empty())
        return false;

    using namespace Mernel;
    ByteArrayHolder binaryBuffer;
    binaryBuffer.resize(spriteBuffer.size());
    memcpy(binaryBuffer.data(), spriteBuffer.data(), spriteBuffer.size());

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamReader stream(bobuffer, ByteOrderDataStream::s_littleEndian);

    uint32_t magicCode{};
    uint16_t version{};
    uint16_t frameWidth16{};
    int32_t  width{};
    int32_t  height{};
    int32_t  frameCount32{};
    uint32_t dataSize{};
    uint32_t channels{}; // == 4

    stream >> magicCode; // "SpA1"
    stream >> version;
    stream >> frameWidth16;
    stream >> width;
    stream >> height;
    // offset: 16 / 0x10

    if (version != 31)
        return false;

    stream.zeroPadding(4);
    stream >> frameCount32;
    stream.zeroPadding(8);
    // offset: 32 / 0x20

    stream >> dataSize;
    stream >> channels;
    // offset: 40 / 0x28

    frameWidth = frameWidth16;
    frameCount = frameCount32;

    resize({ width, height });

    assert(dataSize == rgba.size());
    stream.readBlock(rgba.data(), dataSize);
    auto remaining = stream.getBuffer().getRemainRead();
    assert(remaining == 0);

    return true;
}

void Bitmap::writeToSprite(std::string& spriteBuffer) const
{
    using namespace Mernel;
    ByteArrayHolder binaryBuffer;

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamWriter stream(bobuffer, ByteOrderDataStream::s_littleEndian);

    const uint16_t frameWidth16 = frameWidth ? frameWidth : size.width;
    const uint16_t version{ 31 };

    stream.writeBlock("SpA1", 4); // magicCode
    stream << version;
    stream << frameWidth16;
    stream << uint32_t(size.width);
    stream << uint32_t(size.height);
    // offset: 16 / 0x10

    stream.zeroPadding(4);
    stream << uint32_t(frameCount);
    stream.zeroPadding(8);
    // offset: 32 / 0x20

    stream << uint32_t(rgba.size());
    stream << uint32_t(4);
    stream.writeBlock(rgba.data(), rgba.size());
    // offset: 40 / 0x28

    spriteBuffer.resize(bobuffer.getSize());

    memcpy(spriteBuffer.data(), binaryBuffer.data(), binaryBuffer.size());
}

void Bitmap::readFromPal8(const Pal8Bitmap& palBitmap, const Palette& palette)
{
    resize(palBitmap.size);
    const int stride = size.width;
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            const uint8_t index = palBitmap.pixels[stride * y + x];
            Color         c     = palette.colors[index];
            putDot({ x, y }, c);
        }
    }
}

void Bitmap::writeToPal8(Pal8Bitmap& palBitmap, const Palette& palette)
{
    palBitmap.pixels.resize(size.width * size.height);
    palBitmap.size   = size;
    palBitmap.trans  = 0;
    const int stride = size.width;
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            uint8_t& index = palBitmap.pixels[stride * y + x];
            Color    c     = getColor({ x, y });
            auto     it    = std::find(palette.colors.cbegin(), palette.colors.cend(), c);
            if (it != palette.colors.cend()) {
                index = static_cast<uint8_t>(std::distance(palette.colors.cbegin(), it));
            } else {
                index = 0;
            }
        }
    }
}

void Bitmap::paintLineH(Point point, int width)
{
    for (int x = point.x; x <= point.x + width; x++) {
        putDot(Point{ x, point.y });
    }
}

void Bitmap::paintLineV(Point point, int height)
{
    for (int y = point.y; y <= point.y + height; y++) {
        putDot(Point{ point.x, y });
    }
}

void Bitmap::fill()
{
    for (size_t i = 0; i < rgba.size(); i += 4) {
        rgba[i + 0] = brush.r;
        rgba[i + 1] = brush.g;
        rgba[i + 2] = brush.b;
        rgba[i + 3] = brush.a;
    }
}

void Bitmap::copyBorders(const Bitmap& src, Size cornerSize)
{
    makeBorderImage(*this, src, cornerSize);
}

void Bitmap::draw(Point destPoint, const Bitmap& src, Point srcPoint, Size size)
{
    for (int y = 0; y < size.height; ++y) {
        for (int x = 0; x < size.width; ++x) {
            drawDot(destPoint + Point{ x, y }, src, srcPoint + Point{ x, y });
        }
    }
}

void Bitmap::drawDot(Point destPoint, const Bitmap& src, Point srcPoint)
{
    auto*       destPixel = getPixel(destPoint);
    const auto* srcPixel  = src.getPixel(srcPoint);

    if (destPixel && srcPixel)
        memcpy(destPixel, srcPixel, 4);
}

void Bitmap::fillRect(Rect rect)
{
    for (int x = rect.getLeft(); x <= rect.getRight(); x++) {
        for (int y = rect.getTop(); y <= rect.getBottom(); y++) {
            putDot({ x, y });
        }
    }
}

void Bitmap::linedRect(Rect rect)
{
    if (!rect.isValid())
        return;

    paintLineH(rect.topLeft, rect.getWidth() - 1);
    paintLineH(rect.getBottomLeft(), rect.getWidth() - 1);
    paintLineV(rect.topLeft, rect.getHeight() - 1);
    paintLineV(rect.getTopRight(), rect.getHeight() - 1);
}

}
