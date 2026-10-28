/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <vector>
#include <bitset>
#include <array>
#include <stdexcept>

namespace D2ModGen {

class DynamicBitSet {
public:
    // empyrically selected value
    // BitsPerBlock =256  => full process time (us.) =177206 (search=19003 , getGenerator=148421)
    // BitsPerBlock = 64  => full process time (us.) =143211 (search=29265 , getGenerator=103684)
    // BitsPerBlock = 32  => full process time (us.) =133222 (search=51743 , getGenerator=69153 )
    // BitsPerBlock = 16  => full process time (us.) =229508 (search=159746, getGenerator=53281 )
    static constexpr size_t BitsPerBlock = 32;

private:
    using BlockType = std::bitset<BitsPerBlock>;
    std::vector<BlockType> m_blocks;
    size_t                 m_totalBits = 0;

    void check(const DynamicBitSet& other) noexcept(false)
    {
        if (!m_totalBits || !other.m_totalBits)
            throw std::runtime_error("DynamicBitSet was not initialized");
        if (m_totalBits != other.m_totalBits)
            throw std::runtime_error("DynamicBitSet sizes do not match!");
    }

public:
    using FrequencyBucket = std::array<uint64_t, BitsPerBlock>;
    using FrequencyArray  = std::vector<std::array<uint64_t, BitsPerBlock>>;

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

    void set(size_t index, bool value = true) noexcept
    {
        m_blocks[index / BitsPerBlock].set(index % BitsPerBlock, value);
    }
    DynamicBitSet& operator&=(const DynamicBitSet& other) noexcept(false)
    {
        check(other);

        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i] &= other.m_blocks[i];

        return *this;
    }

    DynamicBitSet& operator|=(const DynamicBitSet& other) noexcept(false)
    {
        check(other);

        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i] |= other.m_blocks[i];

        return *this;
    }

    DynamicBitSet& andNot(const DynamicBitSet& other) noexcept(false)
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

    void flip() noexcept
    {
        for (size_t i = 0; i < m_blocks.size(); ++i)
            m_blocks[i].flip();
    }

    uint64_t getTotal(const FrequencyArray& frequencies) const noexcept
    {
        uint64_t total = 0;
        for (size_t blockIndex = 0; blockIndex < m_blocks.size(); ++blockIndex) {
            const auto& block = m_blocks[blockIndex];
            if (block.none())
                continue;

            const FrequencyBucket& bucket = frequencies[blockIndex];
            for (size_t bitIndex = 0; bitIndex < BitsPerBlock; ++bitIndex) {
                if (block.test(bitIndex)) {
                    total += bucket[bitIndex];
                }
            }
        }
        return total;
    }

    size_t findLowerBound(const FrequencyArray& frequencies, uint64_t value) const noexcept
    {
        uint64_t currentSum = 0;

        for (size_t blockIndex = 0; blockIndex < m_blocks.size(); ++blockIndex) {
            const auto& block = m_blocks[blockIndex];
            if (block.none())
                continue;

            const FrequencyBucket& bucket = frequencies[blockIndex];

            for (size_t bitIndex = 0; bitIndex < BitsPerBlock; ++bitIndex) {
                if (block.test(bitIndex)) {
                    currentSum += bucket[bitIndex];
                    if (currentSum > value) {
                        const size_t offset    = blockIndex * BitsPerBlock;
                        const size_t globalIdx = offset + bitIndex;
                        return globalIdx;
                    }
                }
            }
        }

        return 0;
    }
};

}
