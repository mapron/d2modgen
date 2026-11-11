/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DCFile.hpp"
#include "Logger.hpp"

namespace D2ModGen::Image {

// Assuming your stream class supports explicit absolute positioning,
// if not, adapt seek paths relative to implementation.
bool DC6File::read(ByteOrderDataStreamReader& reader) noexcept
{
    DC6FileHeader header;
    reader >> header.version;
    if (header.version != 6)
        return false;

    reader >> header.unknown01;
    reader >> header.unknown02;
    for (int i = 0; i < 4; ++i)
        reader >> header.termination[i];
    reader >> header.unknown03;
    reader >> header.blockCount;

    m_version   = header.version;
    m_unknown01 = header.unknown01;
    m_unknown02 = header.unknown02;
    std::copy(std::begin(header.termination), std::end(header.termination), std::begin(m_termination));
    m_unknown03 = header.unknown03;

    if (header.blockCount < 1)
        return false;

    std::vector<int32_t> pointerTable(header.blockCount);
    for (int i = 0; i < header.blockCount; ++i) {
        reader >> pointerTable[i];
    }

    m_frames.clear();
    m_frames.resize(header.blockCount);

    for (int i = 0; i < header.blockCount; ++i) {
        reader.getBuffer().setOffsetRead(pointerTable[i]);

        DC6BlockHeader bh;
        reader >> bh.unknown01;
        reader >> bh.width;
        reader >> bh.height;
        reader >> bh.offsetX;
        reader >> bh.offsetY;
        reader >> bh.unknown04;
        reader >> bh.nextBlock;
        reader >> bh.length;

        auto& frame       = m_frames[i];
        frame.size.width  = bh.width;
        frame.size.height = bh.height;
        frame.offset.x    = bh.offsetX;
        frame.offset.y    = bh.offsetY;
        frame.pixels.resize(static_cast<size_t>(bh.width * bh.height), 0);
        frame.trans = 0;

        int32_t line      = bh.height - 1;
        int32_t pixels    = 0;
        int32_t bytesRead = 0;

        while (bytesRead < bh.length && line >= 0) {
            uint8_t c1;
            reader >> c1;
            bytesRead++;

            if (c1 == 0x80) {
                line--;
                pixels = 0;
            } else if (c1 > 0x80) {
                int32_t skipCount = c1 - 0x80;
                while (skipCount > 0 && pixels < bh.width) {
                    skipCount--;
                    pixels++;
                }
            } else {
                int32_t rawCount = c1;
                while (rawCount > 0 && pixels < bh.width) {
                    uint8_t rawPixel;
                    reader >> rawPixel;
                    bytesRead++;

                    size_t destIdx = static_cast<size_t>(pixels) + (static_cast<size_t>(line) * bh.width);
                    if (destIdx < frame.pixels.size()) {
                        frame.pixels[destIdx] = rawPixel;
                    }

                    rawCount--;
                    pixels++;
                }
            }
        }
    }
    return true;
}

bool DC6File::readFromSprite(const std::string& spriteBuffer) noexcept
{
    if (spriteBuffer.empty())
        return false;

    using namespace Mernel;
    ByteArrayHolder binaryBuffer;
    binaryBuffer.resize(spriteBuffer.size());
    memcpy(binaryBuffer.data(), spriteBuffer.data(), spriteBuffer.size());

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamReader stream(bobuffer, ByteOrderDataStream::s_littleEndian);
    return read(stream);
}

// Main coordinator matching WriteDC6 structure setup parameters
void DC6File::write(ByteOrderDataStreamWriter& writer) const
{
    int32_t blockCount = static_cast<int32_t>(m_frames.size());

    // Local struct to keep track of compiled block properties in memory prior to sequential streaming
    struct CompiledOutput {
        std::vector<uint8_t> rlePayload;
        int32_t              length;
        int32_t              nextBlockPos;
    };

    std::vector<CompiledOutput> compiled(blockCount);

    // 1. Run RLE compression engine across individual isolated frames
    for (int32_t i = 0; i < blockCount; ++i) {
        const auto& frame = m_frames[i];
        auto&       cp    = compiled[i];

        int32_t line   = frame.size.height - 1;
        int32_t pixels = 0;

        while (line >= 0) {
            const uint8_t* rowStart = frame.pixels.data() + (line * frame.size.width);
            while (pixels < frame.size.width) {
                int32_t pwidth = frame.size.width - pixels;
                if (pwidth > 0x7F)
                    pwidth = 0x7F;

                if (rowStart[pixels] == frame.trans) {
                    int32_t p = 0;
                    for (int32_t k = pixels; k < frame.size.width; ++k) {
                        if (rowStart[k] == frame.trans)
                            p++;
                        else
                            break;
                    }

                    if ((p + pixels) == frame.size.width) {
                        pixels += p;
                        continue;
                    }

                    if (p > 0x7F)
                        p = 0x7F;
                    pixels += p;
                    cp.rlePayload.push_back(static_cast<uint8_t>(p + 0x80));
                } else {
                    for (int32_t k = 0; k < pwidth; ++k) {
                        if (rowStart[pixels + k] == frame.trans) {
                            pwidth = k;
                            break;
                        }
                    }

                    cp.rlePayload.push_back(static_cast<uint8_t>(pwidth));
                    for (int32_t k = 0; k < pwidth; ++k) {
                        cp.rlePayload.push_back(rowStart[pixels]);
                        pixels++;
                    }
                }
            }
            pixels = 0;
            line--;
            cp.rlePayload.push_back(0x80); // EOL marker
        }
        cp.length = static_cast<int32_t>(cp.rlePayload.size());
    }

    // 2. Pre-calculate precise offset tables sequentially
    int32_t              currentOffset = 24 + (blockCount * 4);
    std::vector<int32_t> pointerTable(blockCount);

    for (int32_t i = 0; i < blockCount; ++i) {
        pointerTable[i]              = currentOffset;
        int32_t totalBlockAllocation = 32 + compiled[i].length + 3; // 32 byte block header + RLE data + 3 padding bytes
        compiled[i].nextBlockPos     = currentOffset + totalBlockAllocation;
        currentOffset += totalBlockAllocation;
    }

    // 3. Write purely sequentially down the Stream
    writer << m_version;
    writer << m_unknown01;
    writer << m_unknown02;
    for (int i = 0; i < 4; ++i)
        writer << m_termination[i];
    writer << m_unknown03;
    writer << blockCount;

    for (int32_t i = 0; i < blockCount; ++i) {
        writer << pointerTable[i];
    }

    for (int i = 0; i < blockCount; ++i) {
        const auto& frame = m_frames[i];
        const auto& cp    = compiled[i];

        writer << static_cast<int32_t>(0); // unknown01
        writer << frame.size.width;
        writer << frame.size.height;
        writer << frame.offset.x;
        writer << frame.offset.y;
        writer << static_cast<int32_t>(0); // unknown04
        writer << cp.nextBlockPos;
        writer << cp.length;

        for (uint8_t rleByte : cp.rlePayload) {
            writer << rleByte;
        }

        // 3 bytes post-block alignment padding
        writer << static_cast<uint8_t>(0xcd);
        writer << static_cast<uint8_t>(0xcd);
        writer << static_cast<uint8_t>(0xcd);
    }
}

void DC6File::writeToSprite(std::string& spriteBuffer) noexcept
{
    using namespace Mernel;
    ByteArrayHolder binaryBuffer;

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamWriter stream(bobuffer, ByteOrderDataStream::s_littleEndian);

    write(stream);

    spriteBuffer.resize(bobuffer.getSize());

    memcpy(spriteBuffer.data(), binaryBuffer.data(), binaryBuffer.size());
}

}
