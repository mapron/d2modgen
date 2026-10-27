/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <vector>
#include <bitset>
#include <climits>
#include <new>
#include <stdexcept>

namespace D2ModGen {

class DynamicBitSet {
private:
    static constexpr size_t BitsPerBlock = std::hardware_constructive_interference_size * CHAR_BIT;

private:
    using BlockType = std::bitset<BitsPerBlock>;
    std::vector<BlockType> m_blocks;
    size_t                 m_totalBits = 0;

    void check(const DynamicBitSet& other)
    {
        if (!m_totalBits || !other.m_totalBits)
            throw std::runtime_error("DynamicBitSet was not initialized");
        if (m_totalBits != other.m_totalBits)
            throw std::runtime_error("DynamicBitSet sizes do not match!");
    }

public:
    void assign(size_t bitSize, bool val = false)
    {
        m_totalBits              = bitSize;
        const size_t blocksCount = (bitSize + BitsPerBlock - 1) / BitsPerBlock;
        m_blocks.assign(blocksCount, val ? BlockType().set() : BlockType());
    }
    void assignIfEmpty(size_t bitSize, bool val = false)
    {
        if (!m_totalBits)
            assign(bitSize, val);
    }

    void set(size_t index, bool value = true)
    {
        if (index < m_totalBits)
            m_blocks[index / BitsPerBlock].set(index % BitsPerBlock, value);
    }

    size_t capacity() const { return m_totalBits; }

    size_t countNonZero() const
    {
        size_t result = 0;
        visitSetBits([&result](size_t) -> bool { result++; return false; });
        return result;
    }

    bool isNonEmpty() const
    {
        bool result = false;
        visitSetBits([&result](size_t) -> bool { result = true; return true; });
        return result;
    }

    DynamicBitSet& operator&=(const DynamicBitSet& other)
    {
        check(other);

        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i] &= other.m_blocks[i];

        return *this;
    }

    DynamicBitSet& operator|=(const DynamicBitSet& other)
    {
        check(other);

        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i] |= other.m_blocks[i];

        return *this;
    }

    DynamicBitSet& andNot(const DynamicBitSet& other)
    {
        check(other);

        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i] &= ~other.m_blocks[i];

        return *this;
    }

    DynamicBitSet operator~() const noexcept
    {
        DynamicBitSet t = *this;
        t.flip();
        return t;
    }

    void flip()
    {
        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i].flip();
    }

    template<typename Func>
    void visitSetBits(Func&& func) const
    {
        for (size_t blockIndex = 0; blockIndex < m_blocks.size(); ++blockIndex) {
            const auto& block = m_blocks[blockIndex];
            if (block.none())
                continue;

            const size_t offset = blockIndex * BitsPerBlock;

            for (size_t bitIndex = 0; bitIndex < BitsPerBlock; ++bitIndex) {
                if (block.test(bitIndex)) {
                    size_t globalIdx = offset + bitIndex;

                    if (globalIdx >= m_totalBits || func(globalIdx))
                        return;
                }
            }
        }
    }
};

}
