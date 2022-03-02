/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "RandoUtils.hpp"

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

    static const QList<QStringList> s_bundledIds{
        QStringList{ "ltng-min", "ltng-max" },
        QStringList{ "fire-min", "fire-max" },
        QStringList{ "cold-min", "cold-max", "cold-len" },
        QStringList{ "pois-min", "pois-max", "pois-len" },
    };
    auto getBundledKey = [](const QString& code) -> QString {
        for (const auto& pack : s_bundledIds)
            if (pack.contains(code))
                return pack[0];
        return "";
    };
    QMap<QString, MagicPropList> listsByBundledProp;
    for (const auto& prop : parsedProps) {
        listsByBundledProp[getBundledKey(prop.code)].push_back(prop);
    }
    MagicPropList newProps = std::move(listsByBundledProp[""]);
    for (const QStringList& bundleIdPack : s_bundledIds) {
        const QString& rootCode = bundleIdPack[0];
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
        assert(!newProp.code.isEmpty());
        if (!newProp.code.isEmpty())
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
            if (prop.max.startsWith("-")) {
                prop.min = prop.max = prop.max.mid(1);
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

void MagicPropRawList::readFromRow(TableView::RowView& row, const ColumnsDesc& columns, const QSet<QString>& extraKnownCodes)
{
    for (const auto& col : columns.m_cols) {
        MagicProp mp;
        mp.code = row[col.code];
        mp.par  = row[col.par];
        mp.min  = row[col.min];
        mp.max  = row[col.max];

        if (mp.code.isEmpty())
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
        const auto& colDesc = columns.m_cols[col];
        row[colDesc.code]   = prop.code;
        row[colDesc.par]    = prop.par;
        row[colDesc.min]    = prop.min;
        row[colDesc.max]    = prop.max;
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

QSet<QString> MagicPropRawList::getAllCodes() const
{
    QSet<QString> result;
    for (const MagicProp& prop : parsedProps)
        result << prop.code; // don't need bounded, we do not index them.
    return result;
}

QString MagicPropRawList::toDebugString() const
{
    QString result = "";
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

void MagicPropRawList::truncateRandom(QRandomGenerator& rng, const int newSize)
{
    MagicPropList newProps;
    const int     kept = keptProps.size();
    for (int i = 0; i < newSize - kept; ++i) {
        auto r = rng.bounded(static_cast<uint32_t>(parsedProps.size()));
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
        qDebug() << "[" << idx << "] " << props[idx].toDebugString();
    }
}

MagicPropList MagicPropUniverse::generate(QRandomGenerator&       rng,
                                          const QSet<QString>&    existingCodes,
                                          const AttributeFlagSet& supportedAttributes,
                                          const ItemCodeFilter&   specificTypeQuery,
                                          int                     specificItemUsage,
                                          int                     count,
                                          int                     level,
                                          int                     unbalance,
                                          bool                    noDuplicateCode) const
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
            if (rng.bounded(100) < specificItemUsage)
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
        const uint32_t indexOfIndex = rng.bounded(static_cast<uint32_t>(indices.size()));
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
    for (const auto& code : query) {
        const auto& typeInfo = itemTypeInfo.at(code.left(4));
        codeSet += nested ? typeInfo.nested : typeInfo.parents;
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
        result += it.value();
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
        result &= it.value();
    }
    return result;
}

MagicPropIdxSet MagicPropUniverse::getIndexByTypeFilter(const ItemCodeFilter& query) const
{
    MagicPropIdxSet   result;
    const ItemCodeSet include = expand(query.include, true);
    const ItemCodeSet exclude = expand(query.exclude, true);
    for (const QString& type : include) {
        auto it = indexByIncludeType.find(type);
        if (it == indexByIncludeType.cend())
            continue;
        result += it.value();
    }
    for (const QString& type : exclude) {
        auto it = indexByExcludeType.find(type);
        if (it == indexByExcludeType.cend())
            continue;
        result -= it.value(); // important -=
    }
    return result;
}

QString MagicProp::toDebugString() const
{
    QString result;
    if (isMinMaxRange(code)) {
        result = QString("%1: %2-%3").arg(code, min, max);
        if (!par.isEmpty())
            result += " (" + par + ")";
    } else {
        result = QString("%1: %2,%3,%4").arg(code, par, min, max);
    }
    if (!bounded.empty())
        result += QString(" +[%1]").arg(bounded.size());
    return result;
}

}