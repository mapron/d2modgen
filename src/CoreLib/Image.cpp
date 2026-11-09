/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "Image.hpp"

#include "MernelPlatform/ByteOrderStream.hpp"

#include <numeric>

namespace D2ModGen::Image {

namespace {

class StoneTextureGenerator {
private:
    int p[512];

    // Standard Fade function for smooth transitions
    double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

    // Linear interpolation
    double lerp(double t, double a, double b) { return a + t * (b - a); }

    // Gradient calculation
    double grad(int hash, double x, double y)
    {
        int    h = hash & 7;
        double u = h < 4 ? x : y;
        double v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
    }

public:
    StoneTextureGenerator(unsigned int seed = 42)
    {
        // Initialize permutation table with a standard shuffle sequence
        int base_p[256];
        std::iota(std::begin(base_p), std::end(base_p), 0);

        // Simple LCG deterministic shuffle using the seed
        for (int i = 255; i > 0; --i) {
            seed  = seed * 1103515245 + 12345;
            int j = (seed / 65536) % (i + 1);
            std::swap(base_p[i], base_p[j]);
        }

        // Duplicate the array to avoid buffer overflows during interpolation
        for (int i = 0; i < 256; i++) {
            p[i]       = base_p[i];
            p[i + 256] = base_p[i];
        }
    }

    // 2D Perlin Noise implementation returning range [-1.0, 1.0]
    double noise(double x, double y)
    {
        int X = (int) std::floor(x) & 255;
        int Y = (int) std::floor(y) & 255;

        x -= std::floor(x);
        y -= std::floor(y);

        double u = fade(x);
        double v = fade(y);

        int A = p[X] + Y;
        int B = p[X + 1] + Y;

        return lerp(v, lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)), lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));
    }

    // Generates a 32-bit RGBA color buffer matching the UI element dimensions
    void generateStoneTexture(Bitmap& bitmap, double scale = 0.03)
    {
        int width  = bitmap.size.width;
        int height = bitmap.size.height;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                double sampleX = x * scale;
                double sampleY = y * scale;

                double totalHeight = 0.0;
                double slopeX      = 0.0;
                double slopeY      = 0.0;

                double amplitude = 1.0;
                double frequency = 1.0;
                double maxValue  = 0.0;

                // 3 octaves for high speed while preserving micro-cracks
                for (int octave = 0; octave < 3; ++octave) {
                    double nx = sampleX * frequency;
                    double ny = sampleY * frequency;

                    // --- FAST ANALYTICAL INLINE DERIVATIVE ---
                    int    X  = (int) std::floor(nx) & 255;
                    int    Y  = (int) std::floor(ny) & 255;
                    double fx = nx - std::floor(nx);
                    double fy = ny - std::floor(ny);

                    // Inline fade curves and their exact derivatives
                    double u  = fx * fx * fx * (fx * (fx * 6 - 15) + 10);
                    double v  = fy * fy * fy * (fy * (fy * 6 - 15) + 10);
                    double du = 30.0 * fx * fx * (fx * (fx - 2.0) + 1.0);
                    double dv = 30.0 * fy * fy * (fy * (fy - 2.0) + 1.0);

                    int    A   = p[X] + Y;
                    int    B   = p[X + 1] + Y;
                    double g00 = grad(p[A], fx, fy);
                    double g10 = grad(p[B], fx - 1, fy);
                    double g01 = grad(p[A + 1], fx, fy - 1);
                    double g11 = grad(p[B + 1], fx - 1, fy - 1);

                    // Calculate smooth height
                    double r = lerp(v, lerp(u, g00, g10), lerp(u, g01, g11));

                    // Transform to Ridged/Craggy profile mathematically
                    double crag   = 1.0 - std::abs(r);
                    double cragSq = crag * crag;

                    totalHeight += cragSq * amplitude;
                    maxValue += amplitude;

                    // Chain rule to extract matching slopes instantly without extra noise calls
                    double dNdx = lerp(v, g10 - g00, g11 - g01) * du;
                    double dNdy = (lerp(u, g01, g11) - lerp(u, g00, g10)) * dv;

                    // Sign correction based on the absolute value flip
                    double sign = (r < 0.0) ? 1.0 : -1.0;
                    slopeX += 2.0 * crag * sign * dNdx * amplitude * frequency;
                    slopeY += 2.0 * crag * sign * dNdy * amplitude * frequency;

                    amplitude *= 0.45;
                    frequency *= 2.2;
                }

                totalHeight /= maxValue;

                // --- FAKE LIGHTING CALCULATIONS ---
                double lightX  = -0.5;
                double lightY  = -0.5;
                double lightZ  = 0.8;
                double rsq     = slopeX * slopeX + slopeY * slopeY + 1.0;
                double inv_len = 1.0 / std::sqrt(rsq);

                double light = (slopeX * inv_len) * lightX + (slopeY * inv_len) * lightY + inv_len * lightZ;
                light        = std::max(0.0, light);

                double rockPattern = std::pow(totalHeight, 2.5);
                double finalShade  = rockPattern * (0.3 + 0.7 * light);

                // --- COMPATIBILITY WITH YOUR BITMAP API ---
                Color c;
                c.r = static_cast<int>(12 + finalShade * 40); // Range ~12 to 52 (Dark Charcoal)
                c.g = static_cast<int>(14 + finalShade * 42);
                c.b = static_cast<int>(16 + finalShade * 45);
                c.a = 255;

                bitmap.putDot({ x, y }, c);
            }
        }
    }
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

void Bitmap::resize(Size size_)
{
    size = size_;
    rgba.resize(size.width * size.height * 4);
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
    uint32_t                  unused1{};
    uint16_t                  version{};
    uint16_t                  unused2{};
    int32_t                   width{};
    int32_t                   height{};
    stream >> unused1;
    stream >> version;
    stream >> unused2;
    stream >> width;
    stream >> height;

    if (version != 31)
        return false;

    resize({ width, height });
    size_t rgbaOffset   = 0x28;
    size_t expectedSize = rgba.size() + rgbaOffset;
    assert(expectedSize == spriteBuffer.size());

    if (expectedSize != spriteBuffer.size())
        return false;
    memcpy(rgba.data(), spriteBuffer.data() + rgbaOffset, rgba.size());

    return true;
}

void Bitmap::writeToSprite(std::string& spriteBuffer)
{
    using namespace Mernel;
    ByteArrayHolder binaryBuffer;

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamWriter stream(bobuffer, ByteOrderDataStream::s_littleEndian);
    stream.writeBlock("SpA1", 4);
    uint16_t frameWidth = size.width;
    uint32_t frameCount = 1;
    uint16_t version{ 31 };
    stream << version;
    stream << uint16_t(frameWidth);
    stream << uint32_t(size.width);
    stream << uint32_t(size.height);
    stream << uint32_t(0);
    stream << uint32_t(frameCount);
    stream.zeroPadding(16);
    stream.writeBlock(rgba.data(), rgba.size());

    spriteBuffer.resize(bobuffer.getSize());

    memcpy(spriteBuffer.data(), binaryBuffer.data(), binaryBuffer.size());
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

void Bitmap::fillWithStone()
{
    StoneTextureGenerator gen;
    gen.generateStoneTexture(*this);
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

Bitmap Bitmap::cropAndMirror(Point srcPoint, Size size) const
{
    Bitmap result;
    result.resize({ size.width * 2 - 1, size.height * 2 - 1 });
    int w = size.width;
    int h = size.height;

    for (int x = 0; x < size.width; ++x) {
        const int r = size.width * 2 - 2 - x;
        for (int y = 0; y < size.height; ++y) {
            const int b = size.height * 2 - 2 - y;
            Color     c = this->getColor(srcPoint + Point{ x, y });

            result.putDot({ 0 + x, 0 + y }, c);
            result.putDot({ 0 + r, 0 + y }, c);
            result.putDot({ 0 + x, 0 + b }, c);
            result.putDot({ 0 + r, 0 + b }, c);
        }
    }
    return result;
}

void Bitmap::tileWith(const Bitmap& tile)
{
    const int w = tile.size.width - 1;
    const int h = tile.size.height - 1;
    for (int x = 0; x < size.width; x++) {
        const int srcX = x % w;
        for (int y = 0; y < size.height; y++) {
            const int srcY = y % h;

            drawDot({ x, y }, tile, { srcX, srcY });
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
