/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "RandoUtils.hpp"

#include "AttributeValues.hpp"

#include "Logger.hpp"

#include "MernelPlatform/ChronoPoint.hpp"
#include <numeric>

namespace D2ModGen {

int genRng(RandomGenerator& rng, const NonParValueSummary::Stat& stat)
{
    const auto bounds = rng(2) == 0 ? std::pair{ stat.min, stat.median } : std::pair{ stat.median, stat.max };
    return genBetweenRng(rng, bounds.first, bounds.second);
}

int scaleValue(int val, int percent, int absLimit)
{
    if (percent == 100)
        return val;
    if (val == 0)
        return 0;

    const bool isPositive = val > 0;
    const int  scaledVal  = val * percent / 100;
    const int  limitsMin  = isPositive ? absLimit : std::numeric_limits<int>::min();
    const int  limitsMax  = isPositive ? std::numeric_limits<int>::max() : -absLimit;
    return std::clamp(scaledVal, limitsMin, limitsMax);
}

int genBetweenRng(RandomGenerator& rng, int first, int second)
{
    const int lowerBound = std::min(first, second);
    const int range      = std::abs(first - second);
    const int randomDiff = range > 0 ? rng(range + 1) : 0;

    return lowerBound + randomDiff;
}

int genRandomScaledValue(RandomGenerator& rng, int value, int minPercent, int maxPercent)
{
    const int absLimit = minPercent > 0 ? 1 : 0;
    if (minPercent == maxPercent) {
        if (minPercent == 0)
            return 0;
        if (minPercent == 100)
            return value;
        return scaleValue(value, minPercent, absLimit);
    }

    const int lowerBound       = std::min(minPercent, maxPercent);
    const int upperBound       = std::max(minPercent, maxPercent);
    const int valueScaledLower = scaleValue(value, lowerBound, absLimit);
    const int valueScaledUpper = scaleValue(value, upperBound, absLimit);
    return genBetweenRng(rng, valueScaledLower, valueScaledUpper);
}

std::vector<size_t> genRandomIndexes(RandomGenerator& rng, size_t size, size_t count)
{
    if (count == 0) {
        return {};
    }
    assert(count <= size);

    std::vector<size_t> result(size);
    std::iota(result.begin(), result.end(), 0);
    if (count >= size) {
        return result;
    }
    while (result.size() > count) {
        auto r = rng(static_cast<int>(result.size()));
        result.erase(result.begin() + r);
    }
    return result;
}

void MagicPropItem::process()
{
    stats.countRows++;
    Mernel::ChronoPoint start(true);

    const int targetLevel = [this]() {
        if ((*rng)(2)) {
            return randoParams->levelPlus ? originalLevel + (*rng)(randoParams->levelPlus) : originalLevel;
        } else {
            return randoParams->levelMinus ? originalLevel - (*rng)(randoParams->levelMinus) : originalLevel;
        }
    }();
    filter.props.levelBracket20 = std::clamp(targetLevel, 0, 99) / 20;

    const int genCountRel = genBetweenRng(*rng, randoParams->common.m_randoGenerateMin, randoParams->common.m_randoGenerateMin);

    const bool forceCaster = [this]() -> bool {
        if (randoParams->forceCasterType == 100)
            return true;
        if (randoParams->forceCasterType == 0)
            return false;

        return (*rng)(100) < randoParams->typeFitPercent;
    }();

    assert(filter.props.clas <= Character::War);

    if (filter.props.idiom != AttributeIdiom::Brawler && forceCaster)
        filter.props.idiom = AttributeIdiom::Caster;

    filter.attributeUnrestricted = propRandomizer->createIndexAll();
    propRandomizer->indexAnd(filter.attributeUnrestricted, supportedAttributes);

    // preproces:: replace skills, potentially remove some attrs;
    for (MagicPropRawList& rawList : affixlists) {
        rawList.removeKnock(*randoParams);
        rawList.replaceToOskill(*randoParams);
        rawList.removeNonClass(filter.props.clas);
    }

    // determine how much keep and how much generate
    int genCountRemainder = 0;
    for (MagicPropRawList& rawList : affixlists) {
        const int originalCount = rawList.getTotalSize() + rawList.removed;
        const int keepCnt       = scaleValue(originalCount, randoParams->common.m_randoKeepOriginalPercent, 0);

        const int genCountUnbound = scaleValue(originalCount, genCountRel, 0) + genCountRemainder;
        const int genCountMin     = genCountRel > 0 ? 1 : 0;
        const int genCountMax     = std::max(1, rawList.affixLimit - keepCnt);
        const int genCount        = std::clamp(genCountUnbound, genCountMin, genCountMax);

        genCountRemainder = randoParams->uniqueCheckAcross ? genCountUnbound - genCount : 0;

        stats.src += originalCount;
        stats.kept += keepCnt;
        rawList.needGenerate = genCount;
        rawList.truncateRandom(*rng, keepCnt);
    }

    DynamicBitSet existingIdPar = propRandomizer->createIndexNone();

    // Feed existing data to consistency checker
    for (MagicPropRawList& rawList : affixlists) {
        rawList.propagateDataToParent(filter.props);
        if (randoParams->uniqueCheckAcross) {
            propRandomizer->indexOr(existingIdPar, rawList.parsedProps);
        }
    }

    // Actually generate new stuff
    for (MagicPropRawList& rawList : affixlists) {
        if (!randoParams->uniqueCheckAcross) {
            existingIdPar = propRandomizer->createIndexNone();
            propRandomizer->indexOr(existingIdPar, rawList.parsedProps);
        }
        filter.availableIdPar = ~existingIdPar;
        filter.canBeEpic      = rawList.rollTypeHighestGen >= RollType::Epic;
        filter.canBeRare      = rawList.rollTypeHighestGen >= RollType::Rare;

        if (randoParams->updateExisting)
            update(rawList, rawList.needGenerate <= 0);

        rawList.append(processList(rawList.needGenerate));

        stats.dest += rawList.getTotalSize();

        if (!randoParams->supportRandomRange)
            rawList.makePerfect();

        rawList.replaceToOskill(*randoParams);
    }

    stats.process += start.GetElapsedTime().GetUS();
}

MagicPropList MagicPropItem::processList(int count)
{
    if (count <= 0)
        return {};
    stats.genReq += count;

    const bool hasTypeSlot = filter.props.slot != SlotType::None;

    MagicPropList result;

    auto tryAdd = [&result, this](bool useSlot) -> bool {
        // build index based struct with filter values

        Mernel::ChronoPoint timer(true);

        DynamicBitSet index = propRandomizer->createIndexAll();
        filter.indexAnd(index, propRandomizer, true);
        propRandomizer->indexAnd(index, filter.props, useSlot);

        stats.countSearch++;
        stats.indexSearch += timer.GetElapsedTime().GetUS();

        timer                                = Mernel::ChronoPoint(true);
        PropRandomizer::PropGeneratorPtr gen = propRandomizer->getRandomAttributeGenerator(index, *rng);
        stats.getGenerator += timer.GetElapsedTime().GetUS();

        if (!gen)
            return false; // no more suitable property left; resistance is futile

        timer          = Mernel::ChronoPoint(true);
        MagicProp prop = gen->makeRoll(*rng);
        stats.makeRoll += timer.GetElapsedTime().GetUS();
        added(prop);

        // update filters and indexes after generating data
        prop.properties.propagateDataToParent(filter.props);

        timer = Mernel::ChronoPoint(true);
        propRandomizer->indexAndNot(filter.availableIdPar, prop);
        stats.indexSearch += timer.GetElapsedTime().GetUS();
        if (useSlot)
            stats.genFit++;
        else
            stats.genCommon++;

        result.push_back(prop);
        return true;
    };
    auto addList = [&tryAdd, this](bool useSlot, const std::string& descr, int totalCount) {
        int added = 0;
        for (; added < totalCount && tryAdd(useSlot); ++added) {
            ;
        }
        if (added != totalCount)
            Logger(Logger::Warning) << "For item '" << id << "' requested " << descr << "=" << totalCount << ", actual=" << added;

        return added;
    };

    if (hasTypeSlot) {
        const int typefitCount = scaleValue(count, randoParams->typeFitPercent);
        addList(true, "typefit", typefitCount);
    }
    addList(false, "common", count - result.size());

    return result;
}

void MagicPropItem::update(MagicPropRawList& propList, bool noGenerate)
{
    for (auto& prop : propList.parsedProps) {
        Mernel::ChronoPoint timer(true);

        stats.countSearch++;
        auto index = propRandomizer->createIndexAll();
        filter.indexAnd(index, propRandomizer, false);
        propRandomizer->indexAnd(index, prop);
        propRandomizer->indexAnd(index, filter.props, true);
        stats.indexSearch += timer.GetElapsedTime().GetUS();
        timer = Mernel::ChronoPoint(true);

        PropRandomizer::PropGeneratorPtr gen = propRandomizer->getRandomAttributeGenerator(index, *rng);

        stats.getGenerator += timer.GetElapsedTime().GetUS();
        if (!gen) {
            continue;
        }
        timer = Mernel::ChronoPoint(true);
        prop  = gen->makeRoll(*rng);
        stats.makeRoll += timer.GetElapsedTime().GetUS();

        if (noGenerate)
            added(prop);
    }
}

void MagicPropItem::added(const MagicProp& prop)
{
    if (prop.properties.roll == RollType::Epic) {
        stats.rollEpic++;
        filter.generatedEpic = true;
    }
    if (prop.properties.roll == RollType::Rare) {
        stats.rollRare++;
    }
    if (prop.properties.roll == RollType::Negative) {
        stats.rollBadd++;
        filter.generatedNeg = true;
    }
}

std::string MagicPropItem::toString(bool withProperties) const
{
    std::string result = "'" + id + "': lvl=" + std::to_string(originalLevel)
                         + ", props=" + filter.props.toString()
                         + ", stats" + stats.toString(ItemStatistics::Mode::Item);
    if (withProperties) {
        for (auto& affixlist : affixlists) {
            for (auto& prop : affixlist.parsedProps) {
                result += "\n    " + prop.toString();
            }
        }
    }
    return result;
}

std::string ItemStatistics::toString(Mode mode) const
{
    std::string result;

    if (mode == Mode::Item) {
        result += "{ " + std::to_string(src)
                  + " -> " + std::to_string(dest)
                  + " : kept=" + std::to_string(kept)
                  + " + gen=" + std::to_string(genReq)
                  + " (fit=" + std::to_string(genFit)
                  + ", com=" + std::to_string(genCommon)
                  + ") roll[";

        if (rollRare)
            result += "rare=" + std::to_string(rollRare) + ", ";
        if (rollEpic)
            result += "epic=" + std::to_string(rollEpic) + ", ";
        if (rollBadd)
            result += "badd=" + std::to_string(rollBadd) + ", ";
        if (result.ends_with(", "))
            result.resize(result.size() - 2);

        result += "] }";
    } else if (mode == Mode::Table) {
        result += "rows=" + std::to_string(countRows);
        result += ", kept affixes=" + std::to_string(kept);
        result += ", generated=" + std::to_string(genReq);
        result += ", process time (us.): " + std::to_string(process);
    } else if (mode == Mode::All) {
        result += "fill properties finished; total tows=" + std::to_string(countRows);
        result += ", total properties=" + std::to_string(countSearch);
        result += ", full process time (us.) =" + std::to_string(process);
        result += " (search=" + std::to_string(indexSearch);
        result += ", getGenerator=" + std::to_string(getGenerator);
        result += ", makeRoll=" + std::to_string(makeRoll);
        result += ")";
    }
    return result;
}

void PropFilter::indexAnd(DynamicBitSet& result, const PropRandomizer* propRandomizer, bool addIdFilter) const
{
    DynamicBitSet rollIndex = propRandomizer->createIndexNone();
    rollIndex |= propRandomizer->index.roll.at(RollType::Common);
    if (canBeEpic && !generatedEpic) {
        rollIndex |= propRandomizer->index.roll.at(RollType::Epic);
    }
    if (canBeRare) {
        rollIndex |= propRandomizer->index.roll.at(RollType::Rare);
    }
    if (!generatedNeg) {
        rollIndex |= propRandomizer->index.roll.at(RollType::Negative);
    }
    result &= rollIndex;

    result &= attributeUnrestricted;
    if (addIdFilter)
        result &= availableIdPar;
}

}
