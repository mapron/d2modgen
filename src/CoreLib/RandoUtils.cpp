/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "RandoUtils.hpp"

#include "AttributeValues.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {

static const std::vector<AttributeFlag> s_attFlags{
    AttributeFlag::Durability,
    AttributeFlag::Quantity,
    AttributeFlag::Sockets,
    AttributeFlag::Missile,
};

}

void MagicPropRawList::postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock)
{
    if (!replaceSkills && !replaceCharges && !skipKnock)
        return;

    MagicPropList newProps;
    for (MagicProp prop : parsedProps) {
        if (replaceSkills && prop.id == AttributeId::skill) {
            if (prop.par == "132") // Leap
            {
                newProps.push_back(std::move(prop));
                continue;
            }
            prop.id = AttributeId::oskill;
        }
        if (replaceCharges && prop.id == AttributeId::charged) {
            if (prop.par == "132") // Leap
            {
                newProps.push_back(std::move(prop));
                continue;
            }
            prop.id = AttributeId::oskill;
            if (prop.max < 0) {
                prop.max = -prop.max.value_or(0);
            }
            prop.min = prop.max;
        }
        if (skipKnock && (prop.id == AttributeId::knock || prop.id == AttributeId::howl))
            continue;

        newProps.push_back(std::move(prop));
    }
    parsedProps = std::move(newProps);
}

void MagicPropRawList::makePerfect()
{
    for (MagicProp& prop : parsedProps) {
        if (isMinMaxRange(prop.id))
            prop.min = prop.max;
    }
}

void MagicPropRawList::readFromRow(const AffixPropList& propList)
{
    for (const auto& col : propList) {
        MagicProp mp;
        mp.par = col.par;
        mp.min = col.min;
        mp.max = col.max;
        mp.id  = getAttributeId(col.code);

        if (mp.id == AttributeId::SKIP)
            continue;
        if (mp.id == AttributeId::INVALID) {
            keptProps.push_back(col);
            continue;
        }

        const auto& desc = getAttributeDesc(mp.id);
        if (desc.flags.contains(AttributeFlag::Keep)) {
            keptProps.push_back(col);
            continue;
        }

        parsedProps.push_back(std::move(mp));
    }
}

void MagicPropRawList::writeToRow(AffixPropList& propList) const
{
    propList.clear();

    auto isFull = [propList](size_t more) {
        return (propList.size() + more > propList.getLimit());
    };

    auto consume = [&propList](const MagicProp& prop) {
        propList.push_back(AffixProp{
            .code = idToString(prop.id),
            .par  = prop.par,
            .min  = prop.min,
            .max  = prop.max,
        });
    };
    for (const auto& prop : keptProps) {
        if (isFull(1))
            break;
        propList.push_back(prop);
    }
    for (const MagicProp& prop : parsedProps) {
        if (isFull(1))
            break;
        consume(prop);
    }
}

int MagicPropRawList::getTotalSize() const
{
    int result = static_cast<int>(parsedProps.size());
    result += keptProps.size();
    return result;
}

Character MagicPropRawList::determineClass(bool checkOskill) const
{
    for (const MagicProp& prop : parsedProps) {
        Character determined = prop.determineClass(checkOskill);
        if (determined > Character::none)
            return determined;
    }
    return Character::none;
}

void MagicPropRawList::append(MagicPropList added)
{
    if (parsedProps.empty())
        parsedProps = std::move(added);
    else {
        parsedProps.reserve(parsedProps.size() + added.size());
        std::move(std::begin(added), std::end(added), std::back_inserter(parsedProps));
    }
}

void MagicPropRawList::truncateRandom(RandomGenerator& rng, const int newSize)
{
    MagicPropList newProps;
    const int     kept = keptProps.size();
    for (int i = 0; i < newSize - kept; ++i) {
        auto r = rng(static_cast<int>(parsedProps.size()));
        newProps.push_back(parsedProps[r]);
        parsedProps.erase(parsedProps.begin() + r);
    }
    parsedProps = std::move(newProps);
}

void MagicPropRawList::fill(UniqueAttributeChecker& checker) const
{
    for (auto& prop : parsedProps)
        checker.add(prop.id, prop.par);
}

MagicPropUniverse::MagicPropUniverse(const MagicContext& magicContext)
{
    for (size_t i = 0; i < 5; ++i)
        levelGen[i] = CommonGen(magicContext, i);
}

MagicPropList MagicPropUniverse::generate(RandomGenerator&        rng,
                                          UniqueAttributeChecker& existingCodes,
                                          Character               existingClass,
                                          const AttributeFlagSet& supportedAttributes,
                                          SlotType                specificTypeQuery,
                                          int                     specificItemUsage,
                                          int                     count,
                                          size_t                  levelBracket20) const
{
    if (count <= 0)
        return {};

    const bool noTypeSlot = specificTypeQuery == SlotType::Undefined;

    const int typefitCount = noTypeSlot ? 0 : [count, specificItemUsage, &rng]() -> int {
        if (specificItemUsage == 100)
            return count;
        if (specificItemUsage == 0)
            return 0;
        int specificCount = 0;
        for (int i = 0; i < count; ++i) {
            if (rng(100) < specificItemUsage)
                specificCount++;
        }
        return specificCount;
    }();
    MagicPropList result;

    Character currentClass = existingClass;

    auto tryAdd = [&existingCodes, &result, &supportedAttributes, &currentClass, &rng, this](const PropGeneratorContainer& container, bool isLastAttempt) -> bool {
        auto* someGenerator = container.getRandomAttributeGenerator(rng);

        const MagicProp prop = someGenerator->generate(rng);
        const auto&     desc = getAttributeDesc(prop.id);

        Character nextClass = prop.determineClass(true);
        if (currentClass != Character::none && nextClass != Character::none) {
            if (nextClass != currentClass)
                return false;
        }
        for (auto flag : s_attFlags) {
            if (desc.flags.contains(flag) && !supportedAttributes.contains(flag))
                return false;
        }

        if (!existingCodes.add(prop.id, prop.par))
            return false;
        if (nextClass != Character::none)
            currentClass = nextClass;

        result.push_back(prop);
        return true;
    };
    auto tryAddWrap = [&tryAdd](const PropGeneratorContainer& container, int totalCount, int retries) {
        for (int i = 0; i < totalCount; ++i) {
            for (int r = 0; r <= retries; ++r)
                if (tryAdd(container, r == retries))
                    break;
        }
    };

    assert(levelBracket20 >= 0 && levelBracket20 <= 4);

    const CommonGen& commonGen = levelGen.at(levelBracket20);
    if (!noTypeSlot) {
        if (commonGen.bySlot.contains(specificTypeQuery))
            tryAddWrap(commonGen.bySlot.at(specificTypeQuery), typefitCount, 5);
    }

    tryAddWrap(commonGen.general, count - result.size(), 5);

    return result;
}

void MagicPropUniverse::update(MagicPropRawList& propList, RandomGenerator& rng, SlotType specificTypeQuery, size_t levelBracket20) const
{
    const CommonGen& commonGen = levelGen.at(levelBracket20);

    const bool noTypeSlot = specificTypeQuery == SlotType::Undefined;

    for (auto& prop : propList.parsedProps) {
        const MagicPropUniverse::PropGenerator* gen = nullptr;
        if (!noTypeSlot) {
            if (commonGen.bySlot.contains(specificTypeQuery))
                gen = commonGen.bySlot.at(specificTypeQuery).findAttributeGenerator(prop.id);
        }
        if (!gen)
            gen = commonGen.general.findAttributeGenerator(prop.id);
        if (!gen)
            continue;
        prop = gen->generate(rng);
    }
}

Character MagicProp::determineClass(bool checkOskill) const
{
    Character cs = getAttributeDesc(id).classSpecific;
    if (cs != Character::none)
        return cs;

    if (id == AttributeId::skilltab) {
        int tab = std::atoi(par.c_str());
        return static_cast<Character>(tab / 3);
    }
    if (checkOskill && id == AttributeId::oskill || id == AttributeId::skill) {
        int skillId = std::atoi(par.c_str());
        if (skillId == 0)
            return Character::none;
        // [6..155] = [ama...bar] = [0...4]
        if (skillId >= 6 && skillId <= 155)
            return static_cast<Character>((skillId - 6) / 30);
        // [221...280] = [dru,ass] = [5,6]
        if (skillId >= 221 && skillId <= 280)
            return static_cast<Character>(((skillId - 221) / 30) + 5);

        // [373...402] = [war] = [7]
        if (skillId >= 373 && skillId <= 402)
            return Character::war;
    }
    return Character::none;
}

MagicProp MagicPropUniverse::PropGenerator::generate(RandomGenerator& rng) const
{
    MagicProp result;
    result.id = id;
    if (ar.has_value()) {
        const ApproximationResult& are   = ar.value();
        int                        value = [&are, &rng]() -> int {
            if (!are.minus && !are.plus)
                return are.avg;
            if (rng(2)) {
                return are.avg + rng(are.plus);
            } else {
                return are.avg - rng(-are.minus);
            }
        }();

        value = value * powerScalePercent / 100;
        if (are.avg > 0)
            value = std::max(1, value);
        else
            value = std::min(-1, value);

        auto desc     = getAttributeDesc(id);
        int  rounding = desc.rounding;
        if (rounding <= 1)
            rounding = smartRoundDetect(value);
        value = smartRound(value, rounding);

        result.par = "";
        if (result.id == AttributeId::dmg_cold || result.id == AttributeId::dmg_pois)
            result.par = "100";
        if (desc.flags.contains(AttributeFlag::SinglePar)) {
            result.par = std::to_string(value);
            result.min.reset();
            result.max.reset();
        } else {
            result.min = result.max = value;
        }
    } else if (fixedLevelAffixes.size()) {
        int  idx   = rng(fixedLevelAffixes.size());
        auto prop  = fixedLevelAffixes[idx];
        result.min = prop.min;
        result.max = prop.max;
        result.par = prop.par;
    }
    return result;
}

MagicPropUniverse::PropGeneratorContainer::PropGeneratorContainer(const MagicContext& magicContext, const CodePowerMap& powerMap, SlotType slotType, size_t levelBracket20)
{
    for (const NonPropAttributeRecord& rec : magicContext.generatedNonPropAttributes) {
        if (!rec.goodValues.results[levelBracket20].avg)
            continue;

        PropGenerator gen;
        gen.id = rec.code;
        gen.ar = rec.goodValues.results[levelBracket20]; /// @todo: bad values?
        if (powerMap.empty()) {
            frequency += rec.frequency;
            gen.frequencyCumulative = frequency;
            gens.push_back(gen);
            allIds.insert(rec.code);
        } else {
            if (auto it = powerMap.find(rec.code); it != powerMap.cend()) {
                frequency += rec.frequency;
                gen.frequencyCumulative = frequency;
                gen.powerScalePercent   = it->second;
                gens.push_back(gen);
                allIds.insert(rec.code);
            }
        }
    }

    for (const PropAttributeRecord& rec : magicContext.generatedPropAttributes) {
        PropGenerator gen;
        gen.id       = rec.code;
        auto& filter = gen.fixedLevelAffixes;

        for (const auto& [slot, slotValues] : rec.slotValues) {
            if (slotType != SlotType::Undefined && slotType != slot)
                continue;
            for (const auto& propValue : slotValues) {
                const size_t bracket = std::min(4, propValue.level / 20);
                if (bracket == levelBracket20) {
                    filter.push_back(propValue);
                }
            }
        }
        if (!filter.empty()) {
            frequency += filter.size() * rec.freqPercent / 100;
            gen.frequencyCumulative = frequency;
            gens.push_back(gen);
            allIds.insert(rec.code);
        }
    }
}

const MagicPropUniverse::PropGenerator* MagicPropUniverse::PropGeneratorContainer::findAttributeGenerator(AttributeId id) const
{
    auto it = std::find_if(gens.begin(), gens.end(), [id](const PropGenerator& entry) {
        return entry.id == id;
    });

    if (it != gens.end()) {
        return &*it;
    }
    return nullptr;
}

const MagicPropUniverse::PropGenerator* MagicPropUniverse::PropGeneratorContainer::getRandomAttributeGenerator(RandomGenerator& rng) const
{
    int value = rng(frequency);

    auto it = std::lower_bound(gens.begin(), gens.end(), value, [](const PropGenerator& entry, int val) {
        return entry.frequencyCumulative <= val;
    });

    if (it != gens.end()) {
        return &*it;
    }
    throw std::runtime_error("WTF");
    return nullptr;
}

MagicPropUniverse::CommonGen::CommonGen(const MagicContext& magicContext, size_t levelBracket20)
{
    for (const auto& [slot, power] : magicContext.generatedSlotScale) {
        bySlot[slot] = PropGeneratorContainer(magicContext, power, slot, levelBracket20);
    }
    general = PropGeneratorContainer(magicContext, CodePowerMap{}, SlotType::Undefined, levelBracket20);
}

void MagicContext::calculateConfigAttributes()
{
    for (const auto& rec : generatedNonPropAttributes) {
        configAttributes.insert(rec.code);
    }
    for (const auto& rec : generatedPropAttributes) {
        configAttributes.insert(rec.code);
    }
}

void MagicContext::eraseNonGame()
{
    std::erase_if(generatedNonPropAttributes, [this](const NonPropAttributeRecord& rec) {
        return !existingGameAttributes.contains(rec.code);
    });
    std::erase_if(generatedPropAttributes, [this](const PropAttributeRecord& rec) {
        return !existingGameAttributes.contains(rec.code);
    });
}

}
