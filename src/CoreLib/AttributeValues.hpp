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
#include <set>

#include "AttributeHelper.hpp"
#include "Scale.hpp"
#include "ModgenCoreExport.hpp"

namespace D2ModGen {

constexpr const int DEFAULT_FREQ      = 100; // arbitrary number. not very large but large enough so 1-2-3 is negligible
constexpr const int FREQ_PER_ONE_PROP = 100;
constexpr const int LEVEL_GROUP_SIZE  = 18;
constexpr const int LEVEL_GROUP_COUNT = 5;
constexpr const int LEVEL_GROUP_MAX   = LEVEL_GROUP_COUNT * LEVEL_GROUP_SIZE - 1;

constexpr inline int makeTier(int level)
{
    return std::clamp(level, 0, LEVEL_GROUP_MAX) / LEVEL_GROUP_SIZE;
}

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
        int min = 0;
        int p10 = 0;
        int p50 = 0;
        int p90 = 0;
        int max = 0;

        std::string toString() const;
        void        makeMonotonic(const Stat& prev);
        void        rescaleValuesOnTotal(const Stat& total);

        bool isEmpty() const { return min == max && min == 0; }

    } low, high, par;

    void makeMonotonic(const NonParValueSummary& prev);

    void rescaleValuesOnTotal(const NonParValueSummary& total);

    std::string toString() const;
    std::string toStringTabbed() const;

    void make(const NonParValueList& list);
};

struct NonParValueByLevel {
    static constexpr const size_t s_segments = 5;
    NonParValueSummary            total;

    std::array<NonParValueSummary, s_segments> levels;

    std::string toString(bool printLevels = true) const;

    void makeMonotonic();
    void makeApprox(const NonParValueList& list, NonParValueByLevel* prev = nullptr);

    void rescaleFreq(Percent percent, int denum);
    void rescaleValuesOnTotal();

    size_t estimateSize() const;
};

using PermilleCurve = std::vector<Permille>;
using SlotPermMap   = TinyMap<SlotType, Permille>;
struct NonParRecordNew {
    static inline const auto s_defaultCurve = PermilleCurve(LEVEL_GROUP_COUNT, 1000_perm);
    static inline const auto s_emptyCurve   = PermilleCurve(LEVEL_GROUP_COUNT, 0_perm);

    int freq = 0;

    int minMedian = 0;
    int maxMedian = 0;
    int penMax    = 0;

    int minLevel = 0;

    AttributeGroup        group;
    std::set<AttributeId> ids;

    SlotSet     slotTypes;
    SlotPermMap slotScale; // empty key means = 1000_perm

    // max rolls for each level bracket, median
    PermilleCurve medCurve = s_defaultCurve;

    // max rolls relative to medCurve
    PermilleCurve ratioP000 = s_defaultCurve;
    PermilleCurve ratioP010 = s_defaultCurve;
    PermilleCurve ratioP090 = s_defaultCurve;
    PermilleCurve ratioP100 = s_defaultCurve;

    std::string toString() const;
};
using NonParRecordNewList     = std::vector<NonParRecordNew>;
using NonParRecordNewGroupMap = std::map<AttributeGroup, NonParRecordNew>;
using NonParRecordNewMap      = std::map<AttributeId, NonParRecordNew>;

struct NonParRecord {
    AttributeId code       = AttributeId::INVALID;
    int         freqFactor = DEFAULT_FREQ * 1; // see NonParValueSummary::add
    SlotSet     slotTypes;
    SlotIntMap  slotTypesScale; // if empty, means = 100%

    NonParValueByLevel comm;

    //std::map<SlotType, NonParValueByLevel> approxMapForSlot;

    void addScale(SlotType slot, const NonParValueSummary& slotApprox, const NonParValueSummary& globalApprox);
    void cleanupScale();

    size_t estimateSize() const;
    void   rescaleFreq(Percent percent, int denum);

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

    PropListByLevel comm;

    std::string toString() const;

    size_t estimateSize() const;

    void rescaleFreq(Percent percent, int denum);
};
using ParRecordList = std::vector<ParRecord>;

int smartRound(int value, int rounding);
int smartRoundDetect(int value);

struct AllAttributeData {
    NonParRecordList nonPar;
    ParRecordList    par;

    NonParRecordNewGroupMap groupMap;
    NonParRecordNewMap      idMap;

    std::map<AttributeId, NonParValueSummary> statistics;

    std::set<AttributeId> ids;

    void refreshIds();

    //void insert(const AllAttributeData& other);

    void erase(AttributeId id);

    void add(ParRecord rec);
    void add(NonParRecord rec);
    void addGrouped(NonParRecordNew rec);
    void addIdd(NonParRecordNew rec);

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

MODGENCORE_EXPORT AllAttributeData getPrecompiledData();

}
