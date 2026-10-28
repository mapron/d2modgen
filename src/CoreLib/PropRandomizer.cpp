/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "PropRandomizer.hpp"
#include "Logger.hpp"

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
    Logger() << "estimatedSize: nonPar=" << estimatedSizeNonPar << ", par=" << estimatedSizePar;
    const auto totalEstimate = estimatedSizeNonPar + estimatedSizePar;
    if (totalEstimate == 0)
        throw std::runtime_error("Randomizer attribute config is empty!");

    gens.reserve(totalEstimate);
    for (const NonParRecord& rec : attributeData.nonPar) {
        MagicProp prop;
        prop.id   = rec.code;
        prop.desc = &getAttributeDesc(prop.id);
        prop.updateProperties();

        for (SlotType slot : getSlotTypes()) {
            if (!rec.slotTypes.contains(slot))
                continue;
            prop.properties.slot = slot;
            int power            = rec.slotTypesScale.get(slot).value_or(100);
            for (RollType rollType : getRollTypes()) {
                auto& rolls = rec.get(rollType);
                if (!rolls.total.freq)
                    continue;
                prop.properties.roll = rollType;
                for (size_t bracket = 0; bracket < NonParValueByLevel::s_segments; ++bracket) {
                    const NonParValueSummary& summary = rolls.levels[bracket];
                    if (!summary.freq)
                        continue;
                    prop.properties.levelBracket20 = bracket;

                    assert(prop.properties.clas <= Character::War);

                    PropGenerator gen;
                    gen.power     = power;
                    gen.prop      = prop;
                    gen.frequency = uint64_t(summary.freq) * rec.freqFactor;
                    gen.nonPar    = summary;
                    gens.push_back(std::move(gen));
                }
            }
        }
    }

    for (const ParRecord& rec : attributeData.par) {
        MagicProp prop;
        prop.id   = rec.code;
        prop.desc = &getAttributeDesc(prop.id);

        for (SlotType slot : getSlotTypes()) {
            if (!rec.slotTypes.contains(slot))
                continue;
            prop.properties.slot = slot;

            for (RollType rollType : getRollTypes()) {
                auto& rolls = rec.get(rollType);
                if (!rolls.freq)
                    continue;
                prop.properties.roll = rollType;
                for (size_t bracket = 0; bracket < NonParValueByLevel::s_segments; ++bracket) {
                    const PropWithRangeList& list = rolls.levels[bracket];
                    if (!list.size())
                        continue;

                    prop.properties.levelBracket20 = bracket;

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
            }
        }
    }
    const auto size = gens.size();
    frequencies.resize((size + DynamicBitSet::BitsPerBlock) / DynamicBitSet::BitsPerBlock);
    for (size_t i = 0; i < size; ++i) {
        auto freq                                                                     = gens[i].frequency;
        frequencies[i / DynamicBitSet::BitsPerBlock][i % DynamicBitSet::BitsPerBlock] = freq;

        assert(freq > 0);
    }
    assert(size == totalEstimate);
    for (int i = 0, cnt = NonParValueByLevel::s_segments; i < cnt; ++i)
        index.levelBracket20[i].assign(size);
    for (AttributeFlag flag : s_attFlags)
        index.notRequireFlag[flag].assign(size, true);

    index.slot[SlotType::None].assign(size);
    index.clas[Character::None].assign(size);
    index.roll[RollType::None].assign(size);
    index.idiom[AttributeIdiom::None].assign(size);
    index.element[Element::None].assign(size);

    for (auto key : getSlotTypes())
        index.slot[key].assign(size);
    for (auto key : getClassTypes())
        index.clas[key].assign(size);
    for (auto key : getRollTypes())
        index.roll[key].assign(size);
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

        index.slot[gen.prop.properties.slot].set(i);
        index.clas[gen.prop.properties.clas].set(i);
        index.roll[gen.prop.properties.roll].set(i);
        index.idiom[gen.prop.properties.idiom].set(i);
        index.element[gen.prop.properties.element].set(i);

        index.levelBracket20[gen.prop.properties.levelBracket20].set(i);

        i++;
    }

    for (auto key : getSlotTypes())
        index.slot[key] |= index.slot[SlotType::None];
    for (auto key : getClassTypes())
        index.clas[key] |= index.clas[Character::None];
    for (auto key : getRollTypes())
        index.roll[key] |= index.roll[RollType::None];
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

    if (filter.levelBracket20 >= 0)
        result &= index.levelBracket20[filter.levelBracket20];
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
    const uint64_t totalFrequency = filter.getTotal(frequencies);
    if (totalFrequency == 0) [[unlikely]] {
        return nullptr;
    }
    if (totalFrequency == 1) [[unlikely]] {
        return &gens[0];
    }
    assert(totalFrequency < uint64_t(std::numeric_limits<int>::max()));
    const uint64_t targetValue = rng(int(totalFrequency));

    const size_t i = filter.findLowerBound(frequencies, targetValue);
    return &gens[i];
}

}
