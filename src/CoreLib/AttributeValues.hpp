/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <string>
#include <array>
#include <map>
#include <vector>
#include <optional>

#include <AttributeHelper.hpp>

namespace D2ModGen {

constexpr const int DEFAULT_FREQ      = 100; // arbitrary number. not very large but large enough so 1-2-3 is negligible
constexpr const int FREQ_PER_ONE_PROP = 100;

struct NonParValue {
    int level = 0;
    int min   = 0;
    int max   = 0;
    int par   = 0;
};
using NonParValueList = std::vector<NonParValue>;

struct NonParValueSummary {
    int freq = 0;

    struct Stat {
        int min    = 0;
        int median = 0;
        int max    = 0;

        void add(int val)
        {
            min = std::min(min, val);
            max = std::max(max, val);
        }
        void add(Stat stat, int currentFreq, int newFreq)
        {
            if (!currentFreq) {
                median = stat.median;
                min    = stat.min;
                max    = stat.max;
            } else {
                min    = std::min(min, stat.min);
                max    = std::max(max, stat.max);
                median = (median * currentFreq + stat.median * newFreq) / (currentFreq + newFreq);
            }
        }
        std::string toString() const;
        void        setMinMaxForCalculation();
        void        cleanMinMax();
        void        makeMonotonic(const Stat& prev);

        bool isEmpty() const { return min == max && min == 0; }

    } low, high, par;

    void add(const NonParValue& value)
    {
        low.add(value.min);
        high.add(value.max);
        par.add(value.par);
        freq += FREQ_PER_ONE_PROP;
    }

    void add(const NonParValueSummary& summary)
    {
        if (!summary.freq)
            return;
        low.add(summary.low, freq, summary.freq);
        high.add(summary.high, freq, summary.freq);
        par.add(summary.par, freq, summary.freq);
        freq += summary.freq;
    }

    void setMinMaxForCalculation();
    void cleanMinMax();
    void makeMonotonic(const NonParValueSummary& prev);

    std::string toString() const;
    std::string toStringTabbed() const;

    void make(const NonParValueList& list);
};

struct NonParValueByLevel {
    static constexpr const size_t s_segments = 5;
    NonParValueSummary            total;

    std::array<NonParValueSummary, s_segments> levels;

    std::string toString() const;

    void makeMonotonic();
    void makeApprox(const NonParValueList& list, NonParValueByLevel* prev = nullptr);
    void add(const NonParValueByLevel& other);
    void rescaleFreq(int percent, int denum);

    size_t estimateSize() const;
};

struct NonParRecord {
    AttributeId code       = AttributeId::INVALID;
    int         freqFactor = DEFAULT_FREQ * 1; // see NonParValueSummary::add
    SlotSet     slotTypes;
    SlotIntMap  slotTypesScale; // if empty, means = 100%

    NonParValueByLevel badd;
    NonParValueByLevel comm;
    NonParValueByLevel rare;
    NonParValueByLevel epic;

    void addScale(SlotType slot, const NonParValueSummary& slotApprox, const NonParValueSummary& globalApprox);
    void cleanupScale();

    size_t estimateSize() const;
    void   doHomogen(const std::vector<const NonParRecord*>& sources);
    void   rescaleFreq(int percent, int denum);

    NonParValueByLevel& get(RollType roll)
    {
        switch (roll) {
                // clang-format off
            case RollType::Negative: return badd;
            case RollType::Common: return comm;
            case RollType::Rare: return rare;
            case RollType::Epic: return epic;
                // clang-format on
            default:
                throw 0;
        }
    }
    const NonParValueByLevel& get(RollType roll) const
    {
        switch (roll) {
                // clang-format off
            case RollType::Negative: return badd;
            case RollType::Common: return comm;
            case RollType::Rare: return rare;
            case RollType::Epic: return epic;
                // clang-format on
            default:
                throw 0;
        }
    }

    std::string toString() const;
};

using NonParRecordList = std::vector<NonParRecord>;

struct PropWithRange {
    std::optional<int> par;
    std::optional<int> min;
    std::optional<int> max;

    bool hasRange() const { return min.has_value() && max.has_value(); }

    std::string toString() const;
};
using PropWithRangeList = std::vector<PropWithRange>;

struct PropListByLevel {
    static constexpr const size_t s_segments = NonParValueByLevel::s_segments;
    int                           freq       = 0;

    std::array<PropWithRangeList, s_segments> levels;

    std::string toString() const;

    size_t estimateSize() const;
};

struct ParRecord {
    AttributeId code       = AttributeId::INVALID;
    int         freqFactor = DEFAULT_FREQ * FREQ_PER_ONE_PROP;
    SlotSet     slotTypes;

    PropListByLevel badd;
    PropListByLevel comm;
    PropListByLevel rare;
    PropListByLevel epic;

    std::string toString() const;

    size_t estimateSize() const;

    void rescaleFreq(int percent, int denum);

    PropListByLevel& get(RollType roll)
    {
        switch (roll) {
                // clang-format off
            case RollType::Negative: return badd;
            case RollType::Common: return comm;
            case RollType::Rare: return rare;
            case RollType::Epic: return epic;
                // clang-format on
            default:
                throw 0;
        }
    }
    const PropListByLevel& get(RollType roll) const
    {
        switch (roll) {
                // clang-format off
            case RollType::Negative: return badd;
            case RollType::Common: return comm;
            case RollType::Rare: return rare;
            case RollType::Epic: return epic;
                // clang-format on
            default:
                throw 0;
        }
    }
};
using ParRecordList = std::vector<ParRecord>;

int smartRound(int value, int rounding);
int smartRoundDetect(int value);

struct AllAttributeData {
    NonParRecordList nonPar;
    ParRecordList    par;

    std::map<AttributeId, NonParValueSummary> statistics;

    std::set<AttributeId> ids;

    void refreshIds();

    void insert(const AllAttributeData& other);

    void erase(AttributeId id);

    void add(ParRecord rec);
    void add(NonParRecord rec);

    ParRecord*    findExistingPar(AttributeId id);
    NonParRecord* findExistingNonPar(AttributeId id);
};

struct StatisticsTable {
    struct Row {
        std::string data;
        int         freq = 0;
    };
    std::set<int>                             versions;
    std::map<AttributeId, std::map<int, Row>> rows;

    void addSnapshot(const AllAttributeData& allData, int ver);
    void traceStatistics() const;
};

AllAttributeData getPrecompiledData();

}
