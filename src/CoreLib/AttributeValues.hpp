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
        bool        isEmpty() const { return min == max && min == 0; }

    } low, high, par;

    std::string toString() const;
    std::string toStringTabbed() const;

    void make(const NonParValueList& list);
};

using PermilleCurve = std::array<Permille, LEVEL_GROUP_COUNT>;
using SlotPermMap   = TinyMap<SlotType, Permille>;
struct NonParRecord {
    static inline const auto s_defaultCurve = PermilleCurve{ { 1000_perm, 1000_perm, 1000_perm, 1000_perm, 1000_perm } };
    static inline const auto s_emptyCurve   = PermilleCurve{ { 0_perm, 0_perm, 0_perm, 0_perm, 0_perm } };

    /// condition
    int freq = 0;

    AttributeGroup        group = AttributeGroup::None;
    std::set<AttributeId> ids; // only one dynamic-allocated memeber. Maybe tiny_flat_set<> or something?

    SlotSet     slotTypes;
    SlotPermMap slotScale; // empty key means = 1000_perm

    int minLevel = 0;

    /// model

    int minMedian = 0;
    int maxMedian = 0;
    int limit     = 0;
    int penMax    = 0;

    // max rolls for each level bracket, median
    PermilleCurve medCurve = s_defaultCurve;

    // max rolls relative to medCurve
    PermilleCurve ratioP000 = s_defaultCurve;
    PermilleCurve ratioP010 = s_defaultCurve;
    PermilleCurve ratioP090 = s_defaultCurve;
    PermilleCurve ratioP100 = s_defaultCurve;

    std::string toString() const;
};
using NonParRecordList     = std::vector<NonParRecord>;
using NonParRecordGroupMap = std::map<AttributeGroup, NonParRecord>;
using NonParRecordMap      = std::map<AttributeId, NonParRecord>;

struct ParRecordParam {
    static inline const auto s_defaultCurve = NonParRecord::s_defaultCurve;
    static inline const auto s_emptyCurve   = NonParRecord::s_emptyCurve;

    /// condition

    int freq     = 0;
    int par      = 0;
    int minLevel = 0;

    /// model

    int minMedian = 0;
    int maxMedian = 0;

    PermilleCurve minCurve = s_defaultCurve;
    PermilleCurve maxCurve = s_defaultCurve;

    std::string toString() const;
};
using ParRecordParamList = std::vector<ParRecordParam>;

struct ParRecord {
    /// condition

    AttributeId id = AttributeId::INVALID;
    SlotSet     slotTypes;
    SlotPermMap slotScale;
    Permille    freqMult = g_identityPerm;

    /// model
    ParRecordParamList params;

    std::string toString() const;
};

using ParRecordList = std::vector<ParRecord>;
using ParRecordMap  = std::map<AttributeId, ParRecord>;

int smartRound(int value, int rounding);
int smartRoundDetect(int value);

using AttributeStatistics = std::map<AttributeId, NonParValueSummary>;

struct AllAttributeData {
    NonParRecordGroupMap groupMap;
    NonParRecordMap      idMap;
    ParRecordMap         parMap;

    std::set<AttributeId> ids;

    void refreshIds();

    ParRecord* findExistingPar(AttributeId id)
    {
        auto it = parMap.find(id);
        return it == parMap.cend() ? nullptr : &it->second;
    }
    NonParRecord* findExistingNonPar(AttributeId id)
    {
        auto it = idMap.find(id);
        return it == idMap.cend() ? nullptr : &it->second;
    }
};

struct StatisticsTable {
    struct Row {
        std::string data;
        int         freq = 0;
    };
    std::set<int>                             versions;
    std::map<AttributeId, std::map<int, Row>> rows;

    void addSnapshot(const AttributeStatistics& allData, int ver);
    void traceStatistics() const;
};

MODGENCORE_EXPORT AllAttributeData getPrecompiledData();

}
