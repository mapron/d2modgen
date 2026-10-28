/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"
#include "DynamicBitSet.hpp"
#include "MagicProp.hpp"

namespace D2ModGen {

struct PropRandomizer {
    struct PropGenerator {
        uint64_t frequency = 0;

        int                               power = 100;
        std::optional<NonParValueSummary> nonPar;

        MagicProp prop;

        MagicProp makeRoll(RandomGenerator& rng) const;

        std::string toString() const;
    };
    using PropGeneratorPtr = const PropGenerator*;

    std::vector<PropGenerator>    gens;
    DynamicBitSet::FrequencyArray frequencies;

    struct {
        std::map<std::pair<AttributeId, int>, DynamicBitSet> byIdParam;

        std::map<SlotType, DynamicBitSet>       slot;
        std::map<Character, DynamicBitSet>      clas;
        std::map<RollType, DynamicBitSet>       roll;
        std::map<AttributeIdiom, DynamicBitSet> idiom;
        std::map<Element, DynamicBitSet>        element;

        std::array<DynamicBitSet, NonParValueByLevel::s_segments> levelBracket20;

        std::map<AttributeFlag, DynamicBitSet> notRequireFlag;
    } index;

    DynamicBitSet createIndexAll() const
    {
        DynamicBitSet result;
        result.assign(gens.size(), true);
        return result;
    }
    DynamicBitSet createIndexNone() const
    {
        DynamicBitSet result;
        result.assign(gens.size(), false);
        return result;
    }

    void indexAnd(DynamicBitSet& result, const BasicProperties& filter, bool useSlot) const;
    void indexAnd(DynamicBitSet& result, const AttributeFlagSet& supportedAttributes) const;

    void indexAnd(DynamicBitSet& result, AttributeId id, int par) const;
    void indexAnd(DynamicBitSet& result, const MagicProp& prop) const;

    void indexAndNot(DynamicBitSet& result, AttributeId id, int par) const;
    void indexAndNot(DynamicBitSet& result, const MagicProp& prop) const;

    void indexOr(DynamicBitSet& result, AttributeId id, int par) const;
    void indexOr(DynamicBitSet& result, const MagicPropList& props) const;
    void indexOr(DynamicBitSet& result, const MagicProp& prop) const;

    PropGeneratorPtr getRandomAttributeGenerator(const DynamicBitSet& filter, RandomGenerator& rng) const;

    PropRandomizer(const AllAttributeData& attributeData, const SkillGetter& skillGetter);
};

}
