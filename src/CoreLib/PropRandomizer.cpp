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
    size_t estimatedSizeGroup = 0;
    for (const auto& [group, rec] : attributeData.groupMap) {
        estimatedSizeGroup += rec.ids.size();
    }
    const size_t estimatedSizeIdmap = attributeData.idMap.size();
    size_t       estimatedPar       = 0;

    for (const auto& [id, rec] : attributeData.parMap) {
        estimatedPar += rec.params.size();
    }

    const auto totalEstimate = estimatedSizeGroup + estimatedSizeIdmap + estimatedPar;
    if (totalEstimate == 0)
        throw std::runtime_error("Randomizer attribute config is empty!");

    gens.reserve(totalEstimate);

    for (const auto& [group, rec] : attributeData.groupMap) {
        for (auto& id : rec.ids) {
            PropGenerator gen;
            gen.set(rec, id);
            gens.push_back(std::move(gen));
        }
    }

    for (const auto& [group, rec] : attributeData.idMap) {
        assert(rec.ids.size() == 1);
        auto id = *rec.ids.begin();

        PropGenerator gen;
        gen.set(rec, id);
        gens.push_back(std::move(gen));
    }

    for (const auto& [group, rec] : attributeData.parMap) {
        for (auto& param : rec.params) {
            PropGenerator gen;
            gen.set(rec, param);
            gens.push_back(std::move(gen));
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
    for (int lvl = 0; lvl < LEVEL_GROUP_MAX + 1; ++lvl)
        index.minLevel[lvl].assign(size);

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
        for (int lvl = gen.minLevel; lvl < LEVEL_GROUP_MAX + 1; ++lvl) {
            index.minLevel[lvl].set(i);
        }

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

    if (filter.targetLevel > -1)
        result &= index.minLevel.at(std::min(filter.targetLevel, LEVEL_GROUP_MAX));
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

MagicProp PropRandomizer::PropGenerator::makeRoll(RandomGenerator& rng, const BasicProperties& basicProperties, Percent trashChance, Percent luckyChance) const
{
    MagicProp result              = prop;
    result.properties.targetLevel = basicProperties.targetLevel;
    result.properties.slot        = basicProperties.slot;

    const Permille slotScaleVal = slotScale.get(basicProperties.slot).value_or(g_identityPerm);

    int levelTier = makeTier(basicProperties.targetLevel);
    if (nonPar.has_value()) {
        assert(!par.has_value());
        const NonParRecord& vs = nonPar.value();

        Permille scale = vs.medCurve[levelTier] * slotScaleVal * powerMult;

        Permille lowBound;
        Permille highBound;

        bool isHighPart = rng(2);
        if (isHighPart) {
            if (checkChance(rng, luckyChance)) {
                highBound = vs.ratioP100[levelTier] * scale;
                lowBound  = highBound;
            } else {
                lowBound  = scale;
                highBound = vs.ratioP090[levelTier] * scale;
            }
        } else {
            if (checkChance(rng, trashChance)) {
                lowBound  = vs.ratioP000[levelTier] * scale;
                highBound = lowBound;
            } else {
                lowBound  = vs.ratioP010[levelTier] * scale;
                highBound = scale;
            }
        }

        const int maxValue = genRandomScaledValue(rng, vs.maxMedian, lowBound.as<100>(), highBound.as<100>());
        const int minValue = maxValue * vs.minMedian / vs.maxMedian;

        result.min = std::min(minValue, vs.limit);
        result.max = std::min(maxValue, vs.limit);

        if (!result.par && result.desc->defaultParameter.has_value())
            result.par = result.desc->defaultParameter;

        result.desc->clampLimits(result.min, false);
        result.desc->clampLimits(result.max, false);
    } else if (par.has_value()) {
        const ParRecordParam& vs = par.value();
        result.par               = vs.par;

        Permille scaleMin = vs.minCurve[levelTier] * slotScaleVal * powerMult;
        Permille scaleMax = vs.maxCurve[levelTier] * slotScaleVal * powerMult;

        result.min = vs.minMedian * scaleMin;
        result.max = vs.maxMedian * scaleMax;

        result.desc->clampLimits(result.min);
        result.desc->clampLimits(result.max);
    }
    return result;
}

}
