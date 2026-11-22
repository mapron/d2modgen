/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DynamicBitSet.hpp"
#include "MagicProp.hpp"
#include "Scale.hpp"

namespace D2ModGen {

struct MODGENCORE_EXPORT PropRandomizer {
    struct MODGENCORE_EXPORT PropGenerator {
        uint64_t frequency = 0;

        Permille powerMult = g_identityPerm;

        std::optional<NonParRecord> nonPar;

        std::optional<ParRecordParam> par;

        MagicProp          prop;
        std::set<SlotType> slotTypes;
        SlotPermMap        slotScale;

        int minLevel = 0;

        void set(const NonParRecord& rec, AttributeId id)
        {
            prop.id   = id;
            prop.desc = &getAttributeDesc(id);
            prop.updateProperties();

            powerMult = rec.powerMult;
            for (auto slot : rec.slotTypes.asVector())
                slotTypes.insert(slot);

            frequency = rec.freq * rec.freqMult;
            assert(frequency > 0);
            nonPar    = rec;
            minLevel  = rec.minLevel;
            slotScale = rec.slotScale;
        }
        void set(const ParRecord& rec, const ParRecordParam& param)
        {
            prop.id   = rec.id;
            prop.par  = param.par;
            prop.desc = &getAttributeDesc(rec.id);
            prop.updateProperties();

            powerMult = rec.powerMult;
            for (auto slot : rec.slotTypes.asVector())
                slotTypes.insert(slot);

            frequency = param.freq * rec.freqMult;
            assert(frequency > 0);
            par       = param;
            minLevel  = param.minLevel;
            slotScale = rec.slotScale;
        }

        MagicProp makeRoll(RandomGenerator& rng, const BasicProperties& basicProperties, Percent trashChance, Percent luckyChance) const;

        std::string toString() const;
    };
    using PropGeneratorPtr = const PropGenerator*;

    std::vector<PropGenerator>    gens;
    DynamicBitSet::FrequencyArray frequencies;

    struct {
        std::map<std::pair<AttributeId, int>, DynamicBitSet> byIdParam;

        std::map<int, DynamicBitSet>            minLevel;
        std::map<SlotType, DynamicBitSet>       slot;
        std::map<Character, DynamicBitSet>      clas;
        std::map<AttributeIdiom, DynamicBitSet> idiom;
        std::map<Element, DynamicBitSet>        element;

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
