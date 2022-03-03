/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QRandomGenerator>

#include <vector>
#include <set>

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

inline void appendToSet(std::set<std::string>& s, const std::set<std::string>& another)
{
    s.insert(another.cbegin(), another.cend());
}

using ItemCodeSet = std::set<std::string>;
struct ItemCodeFilter {
    ItemCodeSet include;
    ItemCodeSet exclude;
};
struct MagicProp;
using MagicPropList = std::vector<MagicProp>;
struct MagicProp {
    std::string   code;
    std::string   par;
    std::string   min;
    std::string   max;
    MagicPropList bounded; // for properties

    std::string toDebugString() const;
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
    MagicPropList keptProps;

    void detectBounded();
    void postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock);
    void makePerfect();

    void readFromRow(TableView::RowView& row, const ColumnsDesc& columns, const std::set<std::string>& extraKnownCodes);
    void writeToRow(TableView::RowView& row, const ColumnsDesc& columns) const;

    int                   getTotalSize() const;
    std::set<std::string> getAllCodes() const;

    std::string toDebugString() const;

    void append(MagicPropList added);

    void truncateRandom(QRandomGenerator& rng, const int newSize);
};

struct ItemTypeInfo {
    std::set<AttributeFlag> flags;
    ItemCodeSet             nested;
    ItemCodeSet             parents;
};

struct MagicPropUniverse {
    MagicPropList props;

    MagicPropIdxSet                          indexAll;
    std::map<int, MagicPropIdxSet>           indexByLevel;
    std::map<std::string, MagicPropIdxSet>   indexByPropCode;
    std::map<AttributeFlag, MagicPropIdxSet> indexByAttributeUnsupported;
    std::map<std::string, MagicPropIdxSet>   indexByIncludeType;
    std::map<std::string, MagicPropIdxSet>   indexByExcludeType;

    mutable std::map<int, std::map<int, MagicPropIdxSet>> indexByLevelCache;

    void add(MagicPropRawList propList, const ItemCodeFilter& itemTypes, const int level);
    void dump(const MagicPropIdxSet& idxs) const;

    std::map<std::string, ItemTypeInfo> itemTypeInfo;

    MagicPropList generate(QRandomGenerator&            rng,
                           const std::set<std::string>& existingCodes,
                           const AttributeFlagSet&      narrowingFlags,
                           const ItemCodeFilter&        specificTypeQuery,
                           int                          specificItemUsage,
                           int                          count,
                           int                          level,
                           int                          unbalance,
                           bool                         noDuplicateCode) const;

    ItemCodeSet expand(const ItemCodeSet& query, bool nested) const;

    MagicPropIdxSet getIndexByLevel(int levelMin,
                                    int levelMax) const;

    MagicPropIdxSet getIndexBySupportedAttributes(const AttributeFlagSet& supportedAttributes) const;

    MagicPropIdxSet getIndexByTypeFilter(const ItemCodeFilter& query) const;
};

}
