/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"

#include "PropRandomizer.hpp"

namespace D2ModGen {

struct PropFilter {
    DynamicBitSet availableIdPar;
    DynamicBitSet attributeUnrestricted;

    BasicProperties props;

    bool canBeEpic = false;
    bool canBeRare = false;

    bool generatedEpic = false;
    bool generatedNeg  = false;

    void indexAnd(DynamicBitSet& result, const PropRandomizer* propRandomizer, bool addIdFilter) const;
};

struct ItemStatistics {
    using S          = ItemStatistics;
    size_t countRows = 0;

    size_t src  = 0; // source prop count
    size_t dest = 0; // dest prop count
    size_t kept = 0; // how many original kept

    size_t genReq      = 0; // count of requested generated properties
    size_t genFit      = 0;
    size_t genCommon   = 0;
    size_t countSearch = 0; // how many times we access index search (approx == kept + genReq)

    size_t rollRare = 0;
    size_t rollEpic = 0;
    size_t rollBadd = 0;

    int64_t process      = 0; // MagicPropItem::process
    int64_t indexSearch  = 0; // time on DynamicBitSet operations.
    int64_t getGenerator = 0; // time on getGenerator() calls.
    int64_t makeRoll     = 0; // time on gen->makeRoll() calls.

    static auto intMembers() { return std::array{ &S::countRows, &S::src, &S::dest, &S::kept, &S::genReq, &S::genFit, &S::genCommon, &S::countSearch, &S::rollRare, &S::rollEpic, &S::rollBadd }; }
    static auto timeMembers() { return std::array{ &S::process, &S::indexSearch, &S::getGenerator, &S::makeRoll }; }

    ItemStatistics& operator+=(const ItemStatistics& other)
    {
        for (auto&& m : intMembers()) {
            this->*m += other.*m;
        }
        for (auto&& m : timeMembers()) {
            this->*m += other.*m;
        }
        return *this;
    }

    enum class Mode
    {
        Item,
        Table,
        All,
    };

    std::string toString(Mode mode) const;
};

struct MagicPropItem {
    RandomGenerator*      rng;
    const PropRandomizer* propRandomizer = nullptr;
    const RandoParams*    randoParams    = nullptr;

    // const during generation
    std::string                   id;
    int                           originalLevel = 0;
    std::vector<MagicPropRawList> affixlists;

    AttributeFlagSet supportedAttributes;

    // mutable during generation
    PropFilter filter;

    ItemStatistics stats;

    template<class Row>
    void readParams(const Row& row)
    {
        id                  = row.getId();
        originalLevel       = row.getLevel();
        supportedAttributes = row.getAttributes();

        auto types         = row.getItemFilter();
        filter.props.slot  = types.determineSlot();
        filter.props.clas  = types.determineClass();
        filter.props.idiom = types.determineIdiom();
    }

    template<class Row>
    void readAffixes(const Row&               row,
                     const SkillCodeResolver& codeResolver,
                     const SkillGetter&       skillGetter,
                     std::set<std::string>*   unknownList,
                     std::set<std::string>*   skippedList)
    {
        auto rowAffixLists    = row.affixLists();
        auto rowRollTypesGrab = row.rollTypesGrab();
        auto rowRollTypesGen  = row.rollTypesGen();

        affixlists.clear();
        for (size_t i = 0, cnt = rowAffixLists.size(); i < cnt; ++i) {
            const AffixPropList* affixList = rowAffixLists[i];
            MagicPropRawList     rawList;
            rawList.rollTypeHighestGrab = rowRollTypesGrab[i];
            rawList.rollTypeHighestGen  = rowRollTypesGen[i];
            rawList.readFromRow(*affixList, codeResolver, skillGetter, filter.props, unknownList, skippedList);
            affixlists.push_back(std::move(rawList));
        }
    }

    void process();

    MagicPropList processList(int count);

    void update(MagicPropRawList& propList, bool noGenerate);
    void added(const MagicProp& prop);

    template<class Row>
    void writeTo(Row& row)
    {
        for (size_t i = 0; AffixPropList* affixList : row.affixLists()) {
            affixlists[i++].writeToRow(*affixList);
        }
    }

    std::string toString(bool withProperties) const;
};

}
