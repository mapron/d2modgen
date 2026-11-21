/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "PropRandomizer.hpp"
#include "Logger.hpp"
#include "RandoUtils.hpp"

namespace D2ModGen {

namespace {

static const auto s_attFlags = std::array{
    AttributeFlag::Durability,
    AttributeFlag::Throwable,
    AttributeFlag::Sockets,
    AttributeFlag::Bow,
    AttributeFlag::ThrowableOrBow,
};

}

PropRandomizer::PropRandomizer(const AllAttributeData& attributeData, const SkillGetter& skillGetter)
{
    size_t estimatedSizeNonPar = 0;
    for (const NonParRecord& rec : attributeData.nonPar) {
        auto recEstimate = rec.estimateSize();
        assert(recEstimate);
        estimatedSizeNonPar += recEstimate;
    }
    size_t estimatedSizePar = 0;
    for (const ParRecord& rec : attributeData.par) {
        auto recEstimate = rec.estimateSize();
        assert(recEstimate);
        estimatedSizePar += recEstimate;
    }
    size_t estimatedSizeGroup = 0;
    for (const auto& [group, rec] : attributeData.groupMap) {
        estimatedSizeGroup += rec.ids.size();
    }
    const size_t estimatedSizeIdmap = attributeData.idMap.size();

    const auto totalEstimate = estimatedSizeNonPar + estimatedSizePar + estimatedSizeGroup + estimatedSizeIdmap;
    if (totalEstimate == 0)
        throw std::runtime_error("Randomizer attribute config is empty!");

    gens.reserve(totalEstimate);
    for (const auto& [group, rec] : attributeData.groupMap) {
        for (auto& id : rec.ids) {
            PropGenerator gen;
            MagicProp&    prop = gen.prop;
            prop.id            = id;
            prop.desc          = &getAttributeDesc(id);
            prop.updateProperties();

            gen.power = g_identityPct;
            for (auto slot : rec.slotTypes.asVector())
                gen.slotTypes.insert(slot);

            gen.frequency = rec.freq;
            gen.nonParNew = rec;
            gens.push_back(std::move(gen));
        }
    }

    // assert(gens.size() == estimatedSizeNonPar);

    for (const ParRecord& rec : attributeData.par) {
        MagicProp prop;
        prop.id   = rec.code;
        prop.desc = &getAttributeDesc(prop.id);

        for (SlotType slot : getSlotTypes()) {
            if (!rec.slotTypes.contains(slot))
                continue;
            prop.properties.slot = slot;

            //for (RollType rollType : getRollTypes()) {
            auto& rolls = rec.comm;
            if (!rolls.freq)
                continue;
            for (size_t bracket = 0; bracket < NonParValueByLevel::s_segments; ++bracket) {
                const PropWithRangeList& list = rolls.levels[bracket];
                if (!list.size())
                    continue;

                prop.properties.levelTier = bracket;

                for (const PropWithRange& propRange : list) {
                    prop.max = propRange.max;
                    prop.min = propRange.min;
                    prop.par = propRange.par;
                    if (prop.desc->parMode == ParMode::Skill) {
                        prop.parSkill = skillGetter(prop.par.value_or(-1));
                        assert(prop.parSkill);
                        if (!prop.parSkill)
                            continue;
                    }
                    prop.updateProperties();

                    assert(prop.properties.clas <= Character::War);

                    PropGenerator gen;
                    gen.frequency = 1 * rec.freqFactor;
                    gen.prop      = prop;
                    gens.push_back(std::move(gen));
                }
            }
            //}
        }
    }
    const auto size = gens.size();
    assert(size == totalEstimate);

    frequencies.resize((size + DynamicBitSet::BitsPerBlock) / DynamicBitSet::BitsPerBlock);
    for (size_t i = 0; i < size; ++i) {
        auto freq                                                                     = gens[i].frequency;
        frequencies[i / DynamicBitSet::BitsPerBlock][i % DynamicBitSet::BitsPerBlock] = freq;

        assert(freq > 0);
    }

    for (AttributeFlag flag : s_attFlags)
        index.notRequireFlag[flag].assign(size, true);

    index.slot[SlotType::None].assign(size);
    index.clas[Character::None].assign(size);
    index.idiom[AttributeIdiom::None].assign(size);
    index.element[Element::None].assign(size);

    for (auto key : getSlotTypes())
        index.slot[key].assign(size);
    for (auto key : getClassTypes())
        index.clas[key].assign(size);

    for (auto key : getIdiomTypes())
        index.idiom[key].assign(size);
    for (auto key : getElementTypes())
        index.element[key].assign(size);

    for (size_t i = 0; const PropGenerator& gen : gens) {
        auto& byIdParam = index.byIdParam[std::pair{ gen.prop.id, gen.prop.par.value_or(0) }];
        byIdParam.assignIfEmpty(size);
        byIdParam.set(i);
        for (AttributeFlag flag : s_attFlags) {
            if (gen.prop.desc->flags.contains(flag)) {
                index.notRequireFlag[flag].set(i, false);
            }
        }
        for (auto slot : gen.slotTypes)
            index.slot[slot].set(i);
        index.clas[gen.prop.properties.clas].set(i);
        index.idiom[gen.prop.properties.idiom].set(i);
        index.element[gen.prop.properties.element].set(i);

        i++;
    }

    for (auto key : getSlotTypes())
        index.slot[key] |= index.slot[SlotType::None];
    for (auto key : getClassTypes())
        index.clas[key] |= index.clas[Character::None];
    for (auto key : getIdiomTypes())
        index.idiom[key] |= index.idiom[AttributeIdiom::None];
    for (auto key : getElementTypes())
        index.element[key] |= index.element[Element::None];
}

void PropRandomizer::indexAnd(DynamicBitSet& result, const BasicProperties& filter, bool useSlot) const
{
    if (useSlot && filter.slot > SlotType::None)
        result &= index.slot.at(filter.slot);

    if (filter.clas > Character::None)
        result &= index.clas.at(filter.clas);
    if (filter.idiom > AttributeIdiom::None)
        result &= index.idiom.at(filter.idiom);
    if (filter.element > Element::None)
        result &= index.element.at(filter.element);
}

void PropRandomizer::indexAnd(DynamicBitSet& result, const AttributeFlagSet& supportedAttributes) const
{
    // Take AttributeFlag::Sockets. if supportedAttributes contains this flag,
    // this means item has socket support(it is chest or weapon).
    // that does mean we just use full index.
    // If however Sockets is not supported, than means we should intersect ( &= ) with indices
    // that contains propties that are NOT require Socket attribute.
    for (auto flag : s_attFlags) {
        if (!supportedAttributes.contains(flag)) {
            const DynamicBitSet& notRequireThisFlag = index.notRequireFlag.at(flag);

            result &= notRequireThisFlag;
        }
    }
}

void PropRandomizer::indexAnd(DynamicBitSet& result, AttributeId id, int par) const
{
    auto it = index.byIdParam.find(std::pair{ id, par });
    if (it != index.byIdParam.cend()) {
        result &= it->second;
    }
}

void PropRandomizer::indexAnd(DynamicBitSet& result, const MagicProp& prop) const
{
    indexAnd(result, prop.id, prop.getUniquePar());
}

void PropRandomizer::indexAndNot(DynamicBitSet& result, AttributeId id, int par) const
{
    auto it = index.byIdParam.find(std::pair{ id, par });
    if (it != index.byIdParam.cend()) {
        result.andNot(it->second);
    }
}

void PropRandomizer::indexAndNot(DynamicBitSet& result, const MagicProp& prop) const
{
    indexAndNot(result, prop.id, prop.getUniquePar());
}

void PropRandomizer::indexOr(DynamicBitSet& result, AttributeId id, int par) const
{
    auto it = index.byIdParam.find(std::pair{ id, par });
    if (it != index.byIdParam.cend()) {
        result |= it->second;
    }
}

void PropRandomizer::indexOr(DynamicBitSet& result, const MagicPropList& props) const
{
    for (const MagicProp& prop : props) {
        indexOr(result, prop);
    }
}

void PropRandomizer::indexOr(DynamicBitSet& result, const MagicProp& prop) const
{
    indexOr(result, prop.id, prop.getUniquePar());
}

std::string PropRandomizer::PropGenerator::toString() const
{
    return enumToString(prop.id) + "=" + std::to_string(frequency);
}

PropRandomizer::PropGeneratorPtr PropRandomizer::getRandomAttributeGenerator(const DynamicBitSet& filter, RandomGenerator& rng) const
{
    uint64_t totalFrequency = filter.getTotal(frequencies);
    if (totalFrequency == 0) [[unlikely]] {
        return nullptr;
    }
    if (totalFrequency == 1) [[unlikely]] {
        return &gens[0];
    }
    uint64_t mult = 1;
    while (totalFrequency >= uint64_t(std::numeric_limits<int>::max())) {
        totalFrequency /= 2;
        mult *= 2;
    }

    const uint64_t targetValue = uint64_t(rng(int(totalFrequency))) * mult;

    const size_t i = filter.findLowerBound(frequencies, targetValue);
    return &gens[i];
}

MagicProp PropRandomizer::PropGenerator::makeRoll(RandomGenerator& rng) const
{
    MagicProp result = prop;
    if (nonPar.has_value()) {
        if (result.desc->rangeMode == RangeMode::Flag) {
            result.min = result.max = 1;
            return result;
        }
        const NonParValueSummary& vs = nonPar.value();

        const int absLimit = result.desc->flags.contains(AttributeFlag::ZeroIsValid) ? 0 : 1;
        const int min      = scaleValue(genRng(rng, vs.low), Percent{ power }, absLimit);
        const int max      = scaleValue(genRng(rng, vs.high), Percent{ power }, absLimit);
        const int par      = genRng(rng, vs.par);

        result.min = min;
        result.max = max;
        result.par = par;

        if (!result.par && result.desc->defaultParameter.has_value())
            result.par = result.desc->defaultParameter;
    }
    return result;
}

MagicProp PropRandomizer::PropGenerator::makeRoll2(RandomGenerator& rng, const BasicProperties& basicProperties, Percent trashChance, Percent luckyChance) const
{
    MagicProp result            = prop;
    result.properties.levelTier = basicProperties.levelTier;
    result.properties.slot      = basicProperties.slot;

    const NonParRecordNew& vs = nonParNew.value();

    Permille scale     = 1000_perm;
    scale              = scale * vs.medCurve[basicProperties.levelTier];
    Permille slotScale = 1000_perm;
    if (auto it = vs.slotScale.get(basicProperties.slot)) {
        slotScale = it.value();
    }
    scale = scale * slotScale;

    Permille lowBound;
    Permille highBound;

    bool isHighPart = rng(2);
    if (isHighPart) {
        if (checkChance(rng, luckyChance)) {
            highBound = vs.ratioP100[basicProperties.levelTier] * scale;
            lowBound  = highBound;
        } else {
            lowBound  = scale;
            highBound = vs.ratioP090[basicProperties.levelTier] * scale;
        }
    } else {
        if (checkChance(rng, trashChance)) {
            lowBound  = vs.ratioP000[basicProperties.levelTier] * scale;
            highBound = lowBound;
        } else {
            lowBound  = vs.ratioP010[basicProperties.levelTier] * scale;
            highBound = scale;
        }
    }

    const int maxValue = genRandomScaledValue(rng, vs.maxMedian, lowBound.as<100>(), highBound.as<100>());
    const int minValue = maxValue * vs.minMedian / vs.maxMedian;
    const int absLimit = result.desc->flags.contains(AttributeFlag::ZeroIsValid) ? 0 : 1;
    result.min         = std::max(absLimit, minValue);
    result.max         = std::max(absLimit, maxValue);
    return result;
}

}
