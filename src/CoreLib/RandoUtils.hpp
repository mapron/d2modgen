/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"

namespace D2ModGen {

inline void appendToSet(StringSet& s, const StringSet& another)
{
    s.insert(another.cbegin(), another.cend());
}
inline void appendToSet(ItemTypeSet& s, const ItemTypeSet& another)
{
    s.insert(another.cbegin(), another.cend());
}
struct MagicProp;
using MagicPropList = std::vector<MagicProp>;
struct MagicProp {
    AttributeId        id;
    std::string        par;
    std::optional<int> min;
    std::optional<int> max;

    Character determineClass(bool checkOskill) const;
};

struct MagicPropRawList {
    MagicPropList          parsedProps;
    std::vector<AffixProp> keptProps;

    void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);
    void makePerfect();

    void readFromRow(const AffixPropList& propList);
    void writeToRow(AffixPropList& propList) const;

    int       getTotalSize() const;
    Character determineClass(bool checkOskill) const;

    void append(MagicPropList added);

    void truncateRandom(RandomGenerator& rng, const int newSize);

    void fill(UniqueAttributeChecker& checker) const;
};

struct MagicPropUniverse {
    struct PropGenerator {
        int frequencyCumulative = 0;
        int powerScalePercent   = 100;

        AttributeId id = AttributeId::INVALID;

        std::optional<ApproximationResult> ar;
        std::vector<AffixPropLeveled>      fixedLevelAffixes;

        MagicProp generate(RandomGenerator& rng) const;
    };
    struct PropGeneratorContainer {
        int                        frequency = 0;
        std::vector<PropGenerator> gens;
        std::set<AttributeId>      allIds;

        PropGeneratorContainer() = default;
        explicit PropGeneratorContainer(const PropAttributeRecordList& extra, const CodePowerMap& powerMap, SlotType slotType, size_t levelBracket20);
        const PropGenerator* getRandomAttributeGenerator(RandomGenerator& rng) const;
    };

    struct CommonGen {
        PropGeneratorContainer                     general;
        std::map<SlotType, PropGeneratorContainer> bySlot;

        CommonGen() = default;
        explicit CommonGen(const PropAttributeRecordList& extra, size_t levelBracket20);
    };
    std::map<size_t, CommonGen> levelGen;

    MagicPropUniverse(const PropAttributeRecordList& extra);

    MagicPropList generate(RandomGenerator&        rng,
                           UniqueAttributeChecker& existingCodes,
                           Character               existingClass,
                           const AttributeFlagSet& narrowingFlags,
                           SlotType                specificTypeQuery,
                           int                     specificItemUsage,
                           int                     count,
                           size_t                  levelBracket20) const;

    bool isHandled(AttributeId id) const;
};

}
