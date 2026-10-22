/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"

namespace D2ModGen {

using SkillResolver = std::function<Character(const std::string&)>;

struct RandoParams {
    int levelPlus           = 0;
    int levelMinus          = 0;
    int relativeCountMin    = 0;
    int relativeCountMax    = 0;
    int keepOriginalPercent = 0;
    int typeFitPercent      = 0;
    int casterTypePercent   = 0;

    bool enableGeneration   = true;
    bool uniqueCheckAcross  = false;
    bool updateExisting     = false;
    bool supportRandomRange = true; // gems can't have random variance

    bool replaceSkills  = false; // skill => oskill
    bool replaceCharges = false; // charge => oskill
    bool removeKnock    = false;
};

inline void appendToSet(StringSet& s, const StringSet& another)
{
    s.insert(another.cbegin(), another.cend());
}
inline void appendToSet(ItemTypeSet& s, const ItemTypeSet& another)
{
    s.insert(another.cbegin(), another.cend());
}
struct MagicProp;
using MagicPropList = std::vector<MagicProp>;
struct MagicProp {
    AttributeId        id;
    std::string        par;
    std::optional<int> min;
    std::optional<int> max;

    Character      determineClass(const SkillResolver& skillResolver, bool checkOskill) const;
    AttributeIdiom determineIdiom() const;
};

struct MagicPropRawList {
    MagicPropList          parsedProps;
    std::vector<AffixProp> keptProps;
    int                    affixLimit   = 0;
    int                    needGenerate = 0;

    void preProcess(const RandoParams& randoParams);
    void makePerfect();

    void readFromRow(const AffixPropList& propList);
    void writeToRow(AffixPropList& propList) const;

    int            getTotalSize() const;
    Character      determineClass(const SkillResolver& skillResolver, bool checkOskill) const;
    AttributeIdiom determineIdiom() const;

    void append(MagicPropList added);

    void truncateRandom(RandomGenerator& rng, const int newSize);

    void fill(UniqueAttributeChecker& checker) const;
};

struct MagicContext {
    NonPropAttributeRecordList    generatedNonPropAttributes;
    PropAttributeRecordList       generatedPropAttributes;
    ApproximationResultsSimpleMap generatedSlotScale;

    std::set<AttributeId> existingGameAttributes;
    std::set<AttributeId> configAttributes;

    void calculateConfigAttributes();
    void eraseNonGame();
};

struct MagicPropUniverse {
    struct PropGenerator {
        int frequencyCumulative = 0;
        int powerScalePercent   = 100;

        AttributeId id = AttributeId::INVALID;

        std::optional<ApproximationResult> ar;
        std::vector<AffixPropLeveled>      fixedLevelAffixes;

        MagicProp generate(RandomGenerator& rng) const;
    };
    struct PropGeneratorContainer {
        int                        frequency = 0;
        std::vector<PropGenerator> gens;
        std::set<AttributeId>      allIds;

        PropGeneratorContainer() = default;
        explicit PropGeneratorContainer(const MagicContext& magicContext, const CodePowerMap& powerMap, SlotType slotType, size_t levelBracket20);
        const PropGenerator* getRandomAttributeGenerator(RandomGenerator& rng) const;
        const PropGenerator* findAttributeGenerator(AttributeId id) const;
    };

    struct CommonGen {
        PropGeneratorContainer                     general;
        std::map<SlotType, PropGeneratorContainer> bySlot;

        CommonGen() = default;
        explicit CommonGen(const MagicContext& magicContext, size_t levelBracket20);
    };
    std::map<size_t, CommonGen> levelGen;

    MagicPropUniverse(const MagicContext& magicContext);
};

struct MagicPropItem {
    // const during generation
    std::string                   id;
    int                           originalLevel = 0;
    AttributeFlagSet              supportedAttributes;
    ItemCodeList                  types;
    std::vector<MagicPropRawList> affixlists;
    SlotType                      slot           = SlotType::Undefined;
    size_t                        levelBracket20 = 0;
    SkillResolver                 skillResolver;

    // mutable during generation
    UniqueAttributeChecker uniqueChecker;
    Character              currentClass = Character::none;
    AttributeIdiom         currentIdiom = AttributeIdiom::Any;

    // statistics
    int propSrc       = 0;
    int propDest      = 0;
    int propKept      = 0;
    int propGenReq    = 0;
    int propGenFit    = 0;
    int propGenCommon = 0;

    template<class Row>
    void readFrom(const Row& row)
    {
        id                  = row.getId();
        originalLevel       = row.getLevel();
        supportedAttributes = row.getAttributes();
        types               = row.getItemFilter();

        for (const AffixPropList* affixList : row.affixLists()) {
            MagicPropRawList rawList;
            rawList.readFromRow(*affixList);
            affixlists.push_back(std::move(rawList));
        }

        slot         = types.determineSlot();
        currentClass = types.determineClass();
        currentIdiom = types.determineIdiom();
    }

    void generate(RandomGenerator&         rng,
                  const MagicPropUniverse& props,
                  const RandoParams&       randoParams);

    MagicPropList generate(RandomGenerator&                    rng,
                           const MagicPropUniverse::CommonGen& commonGen,
                           const RandoParams&                  randoParams,
                           int                                 count);

    void update(MagicPropRawList& propList, RandomGenerator& rng, const MagicPropUniverse::CommonGen& commonGen);

    template<class Row>
    void writeTo(Row& row)
    {
        for (size_t i = 0; AffixPropList* affixList : row.affixLists()) {
            affixlists[i++].writeToRow(*affixList);
        }
    }

    std::string toString() const;
};

}
