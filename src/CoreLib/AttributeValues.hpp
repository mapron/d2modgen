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

enum class SlotType
{
    Undefined,
    MainArmor, // Torso/Helm/Shield
    OtherArmor,
    Ring,
    Amulet,
    Weapon,
    Charm,
    Socketable,
    Set,
    PD2Map,
};

std::string slotTypeName(SlotType slotType);

struct ApproximationResult {
    int avg   = 0;
    int plus  = 0;
    int minus = 0;

    std::string toString() const;
};

struct LevelValue {
    int level = 0;
    int min   = 0;
    int max   = 0;

    std::string toString() const;
};
using LevelValueList = std::vector<LevelValue>;

struct ApproximationResults {
    static constexpr const size_t               s_segments = 5;
    size_t                                      frequency  = 0;
    std::array<ApproximationResult, s_segments> results;

    std::string toString() const;

    void makeMonotonic(bool positive);

    void linearApprox(const LevelValueList& values);
};

int buildResultsSimple(const ApproximationResults& slotApprox, const ApproximationResults& globalApprox);

using ApproximationResultsMap       = std::map<SlotType, ApproximationResults>;
using CodePowerMap                  = std::map<AttributeId, int>;
using ApproximationResultsSimpleMap = std::map<SlotType, CodePowerMap>;

struct NonPropAttributeRecord {
    AttributeId          code      = AttributeId::INVALID;
    size_t               frequency = 0;
    ApproximationResults goodValues;
    ApproximationResults badValues;

    std::string toString() const;
};

using NonPropAttributeRecordList = std::vector<NonPropAttributeRecord>;

struct AffixPropLeveled {
    int                level = 0;
    std::string        par;
    std::optional<int> min;
    std::optional<int> max;

    bool hasRange() const { return min.has_value() && max.has_value(); }

    std::string toString() const;
};
using AffixPropLeveledList = std::vector<AffixPropLeveled>;
using AffixPropLeveledMap  = std::map<SlotType, AffixPropLeveledList>;

struct PropAttributeRecord {
    AttributeId         code = AttributeId::INVALID;
    AffixPropLeveledMap slotValues;
    int                 freqPercent = 100;

    std::string toString() const;
};
using PropAttributeRecordList = std::vector<PropAttributeRecord>;

std::string printMap(const ApproximationResultsSimpleMap& data);

int smartRound(int value, int rounding);
int smartRoundDetect(int value);

const NonPropAttributeRecordList&    globalNonPropAttributes();
const PropAttributeRecordList&       globalPropAttributes();
const ApproximationResultsSimpleMap& globalSlotScale();

}
