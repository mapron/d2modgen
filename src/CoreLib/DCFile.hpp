/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Image.hpp"

#include "MernelPlatform/ByteOrderStream.hpp"

namespace D2ModGen::Image {

class MODGENCORE_EXPORT DC6File {
public:
    using ByteOrderDataStreamReader = Mernel::ByteOrderDataStreamReader;
    using ByteOrderDataStreamWriter = Mernel::ByteOrderDataStreamWriter;

    struct DC6FileHeader {
        int32_t version{};           // 06000000
        int32_t unknown01{};         // 01000000
        int32_t unknown02{};         // 00000000
        uint8_t termination[4] = {}; // EEEEEEEE or CDCDCDCD
        int32_t unknown03{};         // 10000000
        int32_t blockCount{};        // total frames
    };

    struct DC6BlockHeader {
        int32_t unknown01{}; // 00000000
        int32_t width{};
        int32_t height{};
        int32_t offsetX{};
        int32_t offsetY{};
        int32_t unknown04{};
        int32_t nextBlock{};
        int32_t length{};
    };

    struct FrameHeader {
        int32_t flip{};
        int32_t width{};
        int32_t height{};
        int32_t offsetX{};
        int32_t offsetY{};
        int32_t allocSize{}; // size of frame payload
        int32_t unknown02{};
        int32_t unknown03{};
        int32_t nextBlock{}; // unused pointer at file runtime
    };

    bool read(ByteOrderDataStreamReader& reader) noexcept;
    bool readFromSprite(const std::string& spriteBuffer) noexcept;

    const Pal8BitmapList& getFrames() const { return m_frames; }
    void                  setFrames(const Pal8BitmapList& frames) { m_frames = frames; }

    void write(ByteOrderDataStreamWriter& writer) const;
    void writeToSprite(std::string& spriteBuffer) noexcept;

private:
    // Private properties representing original global state properties
    int32_t m_version{ 6 };
    uint8_t m_termination[4]{ 0xcd, 0xcd, 0xcd, 0xcd };
    int32_t m_unknown01{ 1 };
    int32_t m_unknown02{ 0 };
    int32_t m_unknown03{ 1 };

    Pal8BitmapList m_frames;
};

}
