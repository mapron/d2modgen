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

void MagicPropRawList::preProcess(const SkillResolver& skillResolver, const RandoParams& randoParams)
{
    MagicPropList newProps;
    for (MagicProp prop : parsedProps) {
        if (randoParams.replaceSkills && prop.id == AttributeId::skill) {
            if (auto* skill = skillResolver(prop.par); skill && skill->canBeOskill)
                prop.id = AttributeId::oskill;
        }
        if (randoParams.replaceCharges && prop.id == AttributeId::charged) {
            if (auto* skill = skillResolver(prop.par); skill && skill->canBeOskill) {
                prop.id = AttributeId::oskill;
                if (prop.max < 0) {
                    prop.max = -prop.max.value_or(0);
                }
                prop.min = prop.max;
            }
        }
        if (randoParams.removeKnock && (prop.id == AttributeId::knock || prop.id == AttributeId::howl))
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
    affixLimit = propList.getLimit();
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
            affixLimit--;
            continue;
        }

        const auto& desc = getAttributeDesc(mp.id);
        if (desc.flags.contains(AttributeFlag::Keep)) {
            keptProps.push_back(col);
            affixLimit--;
            continue;
        }

        parsedProps.push_back(std::move(mp));
    }
}

void MagicPropRawList::writeToRow(AffixPropList& propList) const
{
    propList.clear();

    for (const auto& prop : keptProps) {
        propList.push_back(prop);
    }
    for (const MagicProp& prop : parsedProps) {
        if (propList.size() + 1 > propList.getLimit())
            break;
        propList.push_back(AffixProp{
            .code = idToString(prop.id),
            .par  = prop.par,
            .min  = prop.min,
            .max  = prop.max,
        });
    }
}

int MagicPropRawList::getTotalSize() const
{
    return static_cast<int>(parsedProps.size());
}

Character MagicPropRawList::determineClass(const SkillResolver& skillResolver, bool checkOskill) const
{
    for (const MagicProp& prop : parsedProps) {
        Character determined = prop.determineClass(skillResolver, checkOskill);
        if (determined != Character::none)
            return determined;
    }
    return Character::none;
}

AttributeIdiom MagicPropRawList::determineIdiom() const
{
    for (const MagicProp& prop : parsedProps) {
        AttributeIdiom determined = prop.determineIdiom();
        if (determined != AttributeIdiom::Any)
            return determined;
    }
    return AttributeIdiom::Any;
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
    if (!newSize) {
        parsedProps.clear();
        return;
    }

    while (parsedProps.size() > newSize) {
        auto r = rng(static_cast<int>(parsedProps.size()));
        parsedProps.erase(parsedProps.begin() + r);
    }
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

Character MagicProp::determineClass(const SkillResolver& skillResolver, bool checkOskill) const
{
    Character cs = getAttributeDesc(id).classSpecific;
    if (cs != Character::none)
        return cs;

    if (id == AttributeId::skilltab) {
        int tab = std::atoi(par.c_str());
        return static_cast<Character>(tab / 3);
    }
    if (checkOskill && id == AttributeId::oskill || id == AttributeId::skill) {
        if (auto* skill = skillResolver(par))
            return skill->classSpecific;
    }
    return Character::none;
}

AttributeIdiom MagicProp::determineIdiom() const
{
    return getAttributeDesc(id).idiom;
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

void MagicPropItem::generate(RandomGenerator& rng, const MagicPropUniverse& props, const RandoParams& randoParams)
{
    const int targetLevel = [this, &rng, &randoParams]() {
        if (rng(2)) {
            return randoParams.levelPlus ? originalLevel + rng(randoParams.levelPlus) : originalLevel;
        } else {
            return randoParams.levelMinus ? originalLevel - rng(randoParams.levelMinus) : originalLevel;
        }
    }();
    const size_t levelBracket20 = std::clamp(targetLevel, 0, 99) / 20;

    assert(levelBracket20 >= 0 && levelBracket20 <= 4);

    const MagicPropUniverse::CommonGen& commonGen = props.levelGen.at(levelBracket20);

    auto calcNewCount = [&rng,
                         &randoParams](const int originalCount) -> std::pair<int, int> {
        if (randoParams.relativeCountMin == 100 && randoParams.relativeCountMax == 100) {
            if (randoParams.keepOriginalPercent == 100)
                return { originalCount, 0 };
            if (randoParams.keepOriginalPercent == 0)
                return { 0, originalCount };
        }

        const int relativeCountBase     = std::min(randoParams.relativeCountMin, randoParams.relativeCountMax);
        const int relativeCountBonusRng = std::max(randoParams.relativeCountMin, randoParams.relativeCountMax) - relativeCountBase;
        const int relativeCount         = relativeCountBase + (relativeCountBonusRng > 0 ? rng(relativeCountBonusRng) : 0);
        const int absCountInHundreds    = originalCount * relativeCount;
        if (randoParams.keepOriginalPercent == 0)
            return { 0, absCountInHundreds / 100 };
        const int  keepCountInHundreds = randoParams.keepOriginalPercent * originalCount;
        const auto rngFraction         = [&rng](const int hundredsCount) -> int {
            const int fraction = hundredsCount % 100;
            const int whole    = hundredsCount / 100;
            if (fraction < 2)
                return whole;
            return whole + (0 == rng(fraction));
        };
        const int keepCount          = rngFraction(keepCountInHundreds);
        const int genCountInHundreds = absCountInHundreds - keepCount * 100;
        const int generatedCount     = rngFraction(genCountInHundreds);
        return { keepCount, generatedCount };
    };

    const bool forceCaster = [&randoParams, &rng]() -> bool {
        if (randoParams.casterTypePercent == 100)
            return true;
        if (randoParams.casterTypePercent == 0)
            return false;

        return rng(100) < randoParams.typeFitPercent;
    }();

    if (currentIdiom == AttributeIdiom::Any && forceCaster)
        currentIdiom = AttributeIdiom::Caster;

    // preproces:: replace skills, potentially remove some attrs;
    for (MagicPropRawList& rawList : affixlists) {
        rawList.preProcess(skillResolver, randoParams);
    }

    // determine how much keep and how much generate
    int genCountPrev = 0;
    for (MagicPropRawList& rawList : affixlists) {
        const int originalCount = rawList.getTotalSize();
        propSrc += originalCount;
        auto [keepCnt, genCount] = calcNewCount(originalCount);

        if (keepCnt + genCount == 0) {
            genCount = 1;
        }
        const int maxGenCount = rawList.affixLimit - keepCnt;
        genCount += genCountPrev;
        if (genCount > maxGenCount) {
            genCountPrev = genCount - maxGenCount;
            genCount     = maxGenCount;
        }

        propKept += keepCnt;
        rawList.needGenerate = genCount;
        rawList.truncateRandom(rng, keepCnt);
    }

    // Feed existing data to consistency checker
    for (MagicPropRawList& rawList : affixlists) {
        if (currentClass == Character::none)
            currentClass = rawList.determineClass(skillResolver, true);
        if (currentIdiom == AttributeIdiom::Any)
            currentIdiom = rawList.determineIdiom();
        rawList.fill(uniqueChecker);
    }

    // Actually generate new stuff
    for (MagicPropRawList& rawList : affixlists) {
        if (!randoParams.uniqueCheckAcross) {
            uniqueChecker = {};
            rawList.fill(uniqueChecker);
        }

        if (randoParams.updateExisting)
            update(rawList, rng, commonGen);

        MagicPropList newProps = generate(rng, commonGen, randoParams, rawList.needGenerate);

        rawList.append(std::move(newProps));

        propDest += rawList.getTotalSize();

        if (!randoParams.supportRandomRange)
            rawList.makePerfect();

        rawList.preProcess(skillResolver, randoParams);
    }
}

MagicPropList MagicPropItem::generate(RandomGenerator&                    rng,
                                      const MagicPropUniverse::CommonGen& commonGen,
                                      const RandoParams&                  randoParams,
                                      int                                 count)
{
    if (count <= 0)
        return {};
    propGenReq += count;

    const bool noTypeSlot = slot == SlotType::Undefined;

    const int typefitCount = noTypeSlot ? 0 : [count, &randoParams, &rng]() -> int {
        if (randoParams.typeFitPercent == 100)
            return count;
        if (randoParams.typeFitPercent == 0)
            return 0;
        int specificCount = 0;
        for (int i = 0; i < count; ++i) {
            if (rng(100) < randoParams.typeFitPercent)
                specificCount++;
        }
        return specificCount;
    }();

    MagicPropList result;

    auto tryAdd = [&result, &rng, this](const MagicPropUniverse::PropGeneratorContainer& container, bool isLastAttempt) -> bool {
        auto* someGenerator = container.getRandomAttributeGenerator(rng);

        const MagicProp prop = someGenerator->generate(rng);
        const auto&     desc = getAttributeDesc(prop.id);

        Character nextClass = prop.determineClass(skillResolver, false);
        if (currentClass != Character::none && nextClass != Character::none) {
            if (nextClass != currentClass)
                return false;
        }
        AttributeIdiom nextIdiom = prop.determineIdiom();
        if (currentIdiom != AttributeIdiom::Any && nextIdiom != AttributeIdiom::Any) {
            if (nextIdiom != currentIdiom)
                return false;
        }
        for (auto flag : s_attFlags) {
            if (desc.flags.contains(flag) && !supportedAttributes.contains(flag))
                return false;
        }

        if (!uniqueChecker.add(prop.id, prop.par))
            return false;

        if (nextClass != Character::none)
            currentClass = nextClass;
        if (nextIdiom != AttributeIdiom::Any)
            currentIdiom = nextIdiom;

        result.push_back(prop);
        return true;
    };
    auto tryAddWrap = [&tryAdd, this](const MagicPropUniverse::PropGeneratorContainer& container, const std::string& descr, int totalCount, int retries) -> int {
        int added = 0;
        for (int i = 0; i < totalCount; ++i) {
            for (int r = 0; r <= retries; ++r)
                if (tryAdd(container, r == retries)) {
                    added++;
                    break;
                }
        }
        if (added != totalCount)
            Logger(Logger::Warning) << "For item '" << id << "' requested " << descr << "=" << totalCount << ", actual=" << added;

        return added;
    };

    if (!noTypeSlot) {
        if (commonGen.bySlot.contains(slot)) {
            propGenFit += tryAddWrap(commonGen.bySlot.at(slot), "typefit", typefitCount, 10);
        }
    }

    propGenCommon += tryAddWrap(commonGen.general, "common", count - result.size(), 10);

    return result;
}

void MagicPropItem::update(MagicPropRawList& propList, RandomGenerator& rng, const MagicPropUniverse::CommonGen& commonGen)
{
    const bool noTypeSlot = slot == SlotType::Undefined;

    for (auto& prop : propList.parsedProps) {
        const MagicPropUniverse::PropGenerator* gen = nullptr;
        if (!noTypeSlot) {
            if (commonGen.bySlot.contains(slot))
                gen = commonGen.bySlot.at(slot).findAttributeGenerator(prop.id);
        }
        if (!gen)
            gen = commonGen.general.findAttributeGenerator(prop.id);
        if (!gen)
            continue;

        const MagicProp nextProp  = gen->generate(rng);
        const Character nextClass = nextProp.determineClass(skillResolver, false);
        if (currentClass != Character::none && nextClass != Character::none) {
            if (nextClass != currentClass)
                continue;
        }
        prop = nextProp;
    }
}

std::string MagicPropItem::toString() const
{
    return "'" + id + "': lvl=" + std::to_string(originalLevel)
           + ", slot=" + slotTypeName(slot)
           + ", class=" + classTypeName(currentClass)
           + ", idiom=" + idiomTypeName(currentIdiom)
           + ", src=" + std::to_string(propSrc)
           + ", req=" + std::to_string(propGenReq)
           + ", dest=" + std::to_string(propDest)
           + " (og=" + std::to_string(propKept)
           + ", fit=" + std::to_string(propGenFit)
           + ", com=" + std::to_string(propGenCommon) + ")";
}

}
