/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "RandoUtils.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {

static const std::vector<AttributeFlag> s_attFlags{
    AttributeFlag::Durability,
    AttributeFlag::Quantity,
    AttributeFlag::Sockets,
    AttributeFlag::Missile,
    AttributeFlag::PD2Map,
};

}

void MagicPropRawList::detectBounded()
{
    if (parsedProps.empty())
        return;

    static const std::vector<StringVector> s_bundledIds{
        StringVector{ "ltng-min", "ltng-max" },
        StringVector{ "fire-min", "fire-max" },
        StringVector{ "cold-min", "cold-max", "cold-len" },
        StringVector{ "pois-min", "pois-max", "pois-len" },
    };
    auto getBundledKey = [](const std::string& code) -> std::string {
        for (const auto& pack : s_bundledIds)
            if (std::find(pack.cbegin(), pack.cend(), code) != pack.cend())
                return pack[0];
        return "";
    };
    std::map<std::string, MagicPropList> listsByBundledProp;
    for (const auto& prop : parsedProps) {
        listsByBundledProp[getBundledKey(prop.code)].push_back(prop);
    }
    MagicPropList newProps = std::move(listsByBundledProp[""]);
    for (const StringVector& bundleIdPack : s_bundledIds) {
        const std::string& rootCode = bundleIdPack[0];
        if (!listsByBundledProp.contains(rootCode))
            continue;
        const MagicPropList& bundledProps = listsByBundledProp[rootCode];
        // Sometimes items have only one propery. Like 'Cathan's Rule' has only "fire-max".
        if (bundledProps.size() == 1) {
            newProps.push_back(bundledProps[0]);
            continue;
        }
        assert(bundledProps.size() == static_cast<size_t>(bundleIdPack.size()));
        MagicProp newProp;
        for (const MagicProp& p : bundledProps) {
            if (p.code == rootCode) {
                newProp = p;
            } else {
                newProp.bounded.push_back(p);
            }
        }
        assert(!newProp.code.empty());
        if (!newProp.code.empty())
            newProps.push_back(std::move(newProp));
    }
    parsedProps = std::move(newProps);
}

void MagicPropRawList::postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock)
{
    if (!replaceSkills && !replaceCharges && !skipKnock)
        return;

    MagicPropList newProps;
    for (MagicProp prop : parsedProps) {
        if (replaceSkills && prop.code == "skill") {
            if (prop.par == "132") // Leap
            {
                newProps.push_back(std::move(prop));
                continue;
            }
            prop.code = "oskill";
        }
        if (replaceCharges && prop.code == "charged") {
            if (prop.par == "132") // Leap
            {
                newProps.push_back(std::move(prop));
                continue;
            }
            prop.code = "oskill";
            if (prop.max.starts_with("-")) {
                prop.min = prop.max = prop.max.substr(1);
            }
        }
        if (skipKnock && (prop.code == "knock" || prop.code == "howl"))
            continue;

        newProps.push_back(std::move(prop));
    }
    parsedProps = std::move(newProps);
}

void MagicPropRawList::makePerfect()
{
    for (MagicProp& prop : parsedProps) {
        if (isMinMaxRange(prop.code))
            prop.min = prop.max;
        for (MagicProp& boundedProp : prop.bounded) {
            if (isMinMaxRange(boundedProp.code))
                boundedProp.min = boundedProp.max;
        }
    }
}

void MagicPropRawList::readFromRow(TableView::RowView& row, const ColumnsDesc& columns, const StringSet& extraKnownCodes)
{
    for (const auto& col : columns.m_cols) {
        MagicProp mp;
        mp.code = (row[col.code].str);
        mp.par  = (row[col.par].str);
        mp.min  = (row[col.min].str);
        mp.max  = (row[col.max].str);

        if (mp.code.empty())
            continue; // PD2 writes properties to the end of a list.
        if (!extraKnownCodes.contains(mp.code)) {
            auto cons = getAttributeConsume(mp.code);
            if (cons == AttributeConsume::Skip)
                continue;
            if (cons == AttributeConsume::Keep) {
                keptProps.push_back(std::move(mp));
                continue;
            }
        }
        parsedProps.push_back(std::move(mp));
    }
    detectBounded();
}

void MagicPropRawList::writeToRow(TableView::RowView& row, const ColumnsDesc& columns) const
{
    int  col     = 0;
    auto consume = [&row, &col, &columns](const MagicProp& prop) {
        const auto& colDesc   = columns.m_cols[col];
        row[colDesc.code].str = prop.code;
        row[colDesc.par].str  = prop.par;
        row[colDesc.min].str  = prop.min;
        row[colDesc.max].str  = prop.max;
        col++;
    };
    for (const MagicProp& prop : keptProps) {
        if (col + 1 > columns.m_cols.size())
            break;
        consume(prop);
    }
    for (const MagicProp& prop : parsedProps) {
        const int propSize = 1 + prop.bounded.size();
        if (col + propSize > columns.m_cols.size())
            break;
        consume(prop);
        for (const MagicProp& propBounded : prop.bounded)
            consume(propBounded);
    }
    while (col < columns.m_cols.size())
        consume({});
}

int MagicPropRawList::getTotalSize() const
{
    int result = static_cast<int>(parsedProps.size());
    for (const MagicProp& prop : parsedProps)
        result += static_cast<int>(prop.bounded.size());
    result += keptProps.size();
    return result;
}

StringSet MagicPropRawList::getAllCodes() const
{
    StringSet result;
    for (const MagicProp& prop : parsedProps)
        result.insert(prop.code); // don't need bounded, we do not index them.
    return result;
}

std::string MagicPropRawList::toDebugString() const
{
    std::string result = "";
    for (const MagicProp& prop : parsedProps)
        result += prop.toDebugString() + "; ";
    return result;
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
        i += parsedProps[r].bounded.size();
        newProps.push_back(parsedProps[r]);
        parsedProps.erase(parsedProps.begin() + r);
    }
    parsedProps = std::move(newProps);
}

void MagicPropUniverse::add(MagicPropRawList propList, const ItemCodeFilter& itemTypes, const int level)
{
    if (propList.parsedProps.empty())
        return;

    const ItemCodeSet include = expand(itemTypes.include, true);
    const ItemCodeSet exclude = expand(itemTypes.exclude, true);

    for (MagicProp& prop : propList.parsedProps) {
        const auto i = static_cast<uint32_t>(props.size());
        indexByLevel[level] << i;
        indexByPropCode[prop.code] << i;

        const auto& desc = getAttributeDesc(prop.code);
        for (AttributeFlag f : s_attFlags)
            if (!desc.flags.contains(f))
                indexByAttributeUnsupported[f] << i;

        for (const auto& type : include)
            indexByIncludeType[type] << i;

        for (const auto& type : exclude)
            indexByExcludeType[type] << i;

        indexAll << i;

        props.push_back(std::move(prop));
    }
}

void MagicPropUniverse::dump(const MagicPropIdxSet& idxs) const
{
    for (const auto idx : idxs) {
        Logger() << "[" << idx << "] " << props[idx].toDebugString();
    }
}

MagicPropList MagicPropUniverse::generate(RandomGenerator&            rng,
                                          const StringSet& existingCodes,
                                          const AttributeFlagSet&      supportedAttributes,
                                          const ItemCodeFilter&        specificTypeQuery,
                                          int                          specificItemUsage,
                                          int                          count,
                                          int                          level,
                                          int                          unbalance,
                                          bool                         noDuplicateCode) const
{
    if (count <= 0)
        return {};

    const MagicPropIdxSet flagsIndexAnd = getIndexBySupportedAttributes(supportedAttributes);
    auto                  makeIndex     = [this, &flagsIndexAnd, &specificTypeQuery, unbalance, level](int balanceMult, bool typeFit) -> MagicPropIdxSet {
        MagicPropIdxSet idx = (unbalance < 0 || balanceMult == 0) ? indexAll : getIndexByLevel(level - unbalance * balanceMult, level + unbalance * balanceMult);
        idx &= flagsIndexAnd;
        if (typeFit)
            idx &= getIndexByTypeFilter(specificTypeQuery);
        return idx;
    };

    auto makeIndexWrap = [&makeIndex](bool typeFit) -> MagicPropIdxSet {
        const int varietyMinimum = 50; // arbitrary value from nowhere;

        auto idx = makeIndex(1, typeFit);
        if (idx.size() > varietyMinimum)
            return idx;

        idx = makeIndex(3, typeFit);
        if (idx.size() > varietyMinimum)
            return idx;
        idx = makeIndex(0, typeFit);
        return idx;
    };

    const MagicPropIdxSet generalIndexes = makeIndexWrap(false);
    const MagicPropIdxSet typefitIndexes = makeIndexWrap(true);

    const std::vector<uint32_t> generalIndexesVec(generalIndexes.cbegin(), generalIndexes.cend());
    const std::vector<uint32_t> typefitIndexesVec(typefitIndexes.cbegin(), typefitIndexes.cend());

    assert(generalIndexes.size() > 1);
    //assert(typefitIndexes.size() > 1);

    if (typefitIndexesVec.size() < 2)
        specificItemUsage = 0;

    const int typefitCount = [count, specificItemUsage, &rng]() -> int {
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
    const int              generalCount = count - typefitCount;
    MagicPropIdxSet        totalIndices;
    UniqueAttributeChecker attrCheck;
    if (noDuplicateCode)
        attrCheck.add(existingCodes);

    auto tryAdd = [&attrCheck, &totalIndices, &rng, noDuplicateCode, this](const std::vector<uint32_t>& indices, bool force) -> bool {
        const uint32_t indexOfIndex = rng(static_cast<int>(indices.size()));
        const uint32_t index        = indices[indexOfIndex];
        const auto&    code         = props[index].code;
        if (noDuplicateCode && attrCheck.contains(code))
            return false;
        attrCheck.add(code);
        totalIndices << index;
        return true;
    };
    auto tryAddWrap = [&tryAdd](const std::vector<uint32_t>& indices, int totalCount, int retries) {
        for (int i = 0; i < totalCount; ++i) {
            for (int r = 0; r <= retries; ++r)
                if (tryAdd(indices, r == retries))
                    break;
        }
    };
    tryAddWrap(typefitIndexesVec, typefitCount, 5);
    tryAddWrap(generalIndexesVec, generalCount, 5);

    MagicPropList result;
    result.reserve(totalIndices.size());
    for (const auto idx : totalIndices)
        result.push_back(props[idx]);
    return result;
}

ItemCodeSet MagicPropUniverse::expand(const ItemCodeSet& query, bool nested) const
{
    ItemCodeSet codeSet = query;
    for (const std::string& code : query) {
        if (code.empty())
            continue;
        const auto& typeInfo = itemTypeInfo.at(code.substr(0, 4));
        appendToSet(codeSet, nested ? typeInfo.nested : typeInfo.parents);
    }
    return codeSet;
}

MagicPropIdxSet MagicPropUniverse::getIndexByLevel(int levelMin, int levelMax) const
{
    auto cacheMinIt = indexByLevelCache.find(levelMin);
    if (cacheMinIt != indexByLevelCache.end()) {
        auto cacheMaxit = cacheMinIt->second.find(levelMax);
        if (cacheMaxit != cacheMinIt->second.end()) {
            return cacheMaxit->second;
        }
    }
    MagicPropIdxSet result;
    for (int l = levelMin; l <= levelMax; ++l) {
        auto it = indexByLevel.find(l);
        if (it == indexByLevel.cend())
            continue;
        result += it->second;
    }
    indexByLevelCache[levelMin][levelMax] = result;
    return result;
}

MagicPropIdxSet MagicPropUniverse::getIndexBySupportedAttributes(const AttributeFlagSet& supportedAttributes) const
{
    MagicPropIdxSet result = indexAll;
    for (AttributeFlag flag : s_attFlags) {
        if (supportedAttributes.contains(flag))
            continue;
        // so, requested item does not support one of our attrubutes - exclude them.
        auto it = indexByAttributeUnsupported.find(flag);
        if (it == indexByAttributeUnsupported.cend())
            continue;
        result &= it->second;
    }
    return result;
}

MagicPropIdxSet MagicPropUniverse::getIndexByTypeFilter(const ItemCodeFilter& query) const
{
    MagicPropIdxSet   result;
    const ItemCodeSet include = expand(query.include, true);
    const ItemCodeSet exclude = expand(query.exclude, true);
    for (const std::string& type : include) {
        auto it = indexByIncludeType.find(type);
        if (it == indexByIncludeType.cend())
            continue;
        result += it->second;
    }
    for (const std::string& type : exclude) {
        auto it = indexByExcludeType.find(type);
        if (it == indexByExcludeType.cend())
            continue;
        result -= it->second; // important -=
    }
    return result;
}

std::string MagicProp::toDebugString() const
{
    std::string result;
    if (isMinMaxRange(code)) {
        result = code + ": " + min + "-" + max;
        if (!par.empty())
            result += " (" + par + ")";
    } else {
        result = code + ": " + par + "," + min + "," + max;
    }
    if (!bounded.empty())
        result += argCompat(" +[%1]", bounded.size());
    return result;
}

}
