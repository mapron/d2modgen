/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QSet>
#include <QString>
#include <QMap>
#include <QRandomGenerator>

#include <vector>

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

using ItemCodeSet = QSet<QString>;
struct ItemCodeFilter {
    ItemCodeSet include;
    ItemCodeSet exclude;
};
struct MagicProp;
using MagicPropList = std::vector<MagicProp>;
struct MagicProp {
    QString       code;
    QString       par;
    QString       min;
    QString       max;
    MagicPropList bounded; // for properties

    QString toDebugString() const;
};
struct MagicPropIdxSet {
    std::vector<uint32_t> data;
    using T = uint32_t;

    void insert(const T& value)
    {
        if (data.empty())
            data.push_back(value);
        else
            data.insert(std::upper_bound(data.begin(), data.end(), value),
                        value);
    }
    MagicPropIdxSet& unite(const MagicPropIdxSet& other)
    {
        std::vector<uint32_t> resultData;
        resultData.reserve(data.size() + other.data.size());
        std::set_union(data.cbegin(), data.cend(), other.data.cbegin(), other.data.cend(), std::back_inserter(resultData));
        std::swap(data, resultData);
        return *this;
    }
    MagicPropIdxSet& intersect(const MagicPropIdxSet& other)
    {
        std::vector<uint32_t> resultData;
        resultData.reserve(std::min(data.size(), other.data.size()));
        std::set_intersection(data.cbegin(), data.cend(), other.data.cbegin(), other.data.cend(), std::back_inserter(resultData));
        std::swap(data, resultData);
        return *this;
    }
    MagicPropIdxSet& subtract(const MagicPropIdxSet& other)
    {
        std::vector<uint32_t> resultData;
        resultData.reserve(data.size());
        std::set_difference(data.cbegin(), data.cend(), other.data.cbegin(), other.data.cend(), std::back_inserter(resultData));
        std::swap(data, resultData);
        return *this;
    }
    bool contains(const T& value);

    // STL compatibility
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto cbegin() const { return data.begin(); }
    auto cend() const { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    bool   empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    // comfort
    inline MagicPropIdxSet& operator<<(const T& value)
    {
        insert(value);
        return *this;
    }
    inline MagicPropIdxSet& operator|=(const MagicPropIdxSet& other)
    {
        unite(other);
        return *this;
    }
    inline MagicPropIdxSet& operator&=(const MagicPropIdxSet& other)
    {
        intersect(other);
        return *this;
    }
    inline MagicPropIdxSet& operator+=(const MagicPropIdxSet& other)
    {
        unite(other);
        return *this;
    }
    inline MagicPropIdxSet& operator-=(const MagicPropIdxSet& other)
    {
        subtract(other);
        return *this;
    }
};

struct MagicPropRawList {
    MagicPropList parsedProps;

    void detectBounded();
    void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);
    void makePerfect();

    void readFromRow(TableView::RowView& row, const ColumnsDesc& columns);
    void writeToRow(TableView::RowView& row, const ColumnsDesc& columns) const;

    int           getTotalSize() const;
    QSet<QString> getAllCodes() const;

    QString toDebugString() const;

    void append(MagicPropList added);
    
    void truncateRandom(QRandomGenerator&       rng, const int newSize);
};

struct ItemTypeInfo {
    QSet<AttributeFlag> flags;
    ItemCodeSet         nested;
    ItemCodeSet         parents;
};

struct MagicPropUniverse {
    MagicPropList props;

    MagicPropIdxSet                      indexAll;
    QMap<int, MagicPropIdxSet>           indexByLevel;
    QMap<QString, MagicPropIdxSet>       indexByPropCode;
    QMap<AttributeFlag, MagicPropIdxSet> indexByAttributeUnsupported;
    QMap<QString, MagicPropIdxSet>       indexByIncludeType;
    QMap<QString, MagicPropIdxSet>       indexByExcludeType;

    mutable std::map<int, std::map<int, MagicPropIdxSet>> indexByLevelCache;

    void add(MagicPropRawList propList, const ItemCodeFilter& itemTypes, const int level);
    void dump(const MagicPropIdxSet& idxs) const;

    std::map<QString, ItemTypeInfo> itemTypeInfo;

    MagicPropList generate(QRandomGenerator&       rng,
                           const QSet<QString>&    existingCodes,
                           const AttributeFlagSet& narrowingFlags,
                           const ItemCodeFilter&   specificTypeQuery,
                           int                     specificItemUsage,
                           int                     count,
                           int                     level,
                           int                     unbalance,
                           bool                    noDuplicateCode) const;

    ItemCodeSet expand(const ItemCodeSet& query, bool nested) const;

    MagicPropIdxSet getIndexByLevel(int levelMin,
                                    int levelMax) const;

    MagicPropIdxSet getIndexBySupportedAttributes(const AttributeFlagSet& supportedAttributes) const;

    MagicPropIdxSet getIndexByTypeFilter(const ItemCodeFilter& query) const;
};

}
