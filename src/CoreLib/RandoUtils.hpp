/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"
#include "FlatSet.hpp"

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

    // statistics
    struct Statistics {
        int src       = 0;
        int dest      = 0;
        int kept      = 0;
        int genReq    = 0;
        int genFit    = 0;
        int genCommon = 0;

        int rollRare = 0;
        int rollEpic = 0;
        int rollBadd = 0;

        std::string toString() const;
    } stats;

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

    void generate();

    MagicPropList generate(int count);

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
