/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "DataContext.hpp"

namespace D2ModGen {

using SkillCodeResolver = std::function<int(const std::string&)>;
using SkillGetter       = std::function<DataRow_skills_ptr(int)>;

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

struct BasicProperties {
    SlotType       slot    = SlotType::None;
    Character      clas    = Character::None;
    RollType       roll    = RollType::None;
    AttributeIdiom idiom   = AttributeIdiom::None;
    Element        element = Element::None;

    size_t levelBracket20 = size_t(-1);

    template<class T>
    bool checkWithDefault(const T& next, T current, T def) const
    {
        if (current != def && next != def && next != current)
            return false;

        return true;
    }
    bool checkClass(Character next) const
    {
        return checkWithDefault(next, clas, Character::None);
    }

    template<class T>
    void propagate(const T& current, T& parent, T def) const
    {
        if (current != def && parent == def)
            parent = current;
    }
    void propagateDataToParent(BasicProperties& parent) const;

    std::string toString() const noexcept;
};

struct MagicProp {
    AttributeId        id = AttributeId::INVALID;
    std::optional<int> par;
    std::optional<int> min;
    std::optional<int> max;

    const AttributeDesc* desc     = nullptr;
    DataRow_skills_ptr   parSkill = nullptr;

    BasicProperties properties;

    void updateProperties();
    int  getUniquePar() const
    {
        if (desc->parMode == ParMode::None || desc->parMode == ParMode::PureValue)
            return 0;
        return par.value_or(0);
    }

    std::string toString() const;
};
using MagicPropList = std::vector<MagicProp>;

struct MagicPropRawList {
    RollType               rollTypeHighestGrab = RollType::Common;
    RollType               rollTypeHighestGen  = RollType::Common;
    MagicPropList          parsedProps;
    std::vector<AffixProp> keptProps;

    int affixLimit   = 0;
    int needGenerate = 0;
    int removed      = 0;

    void removeKnock(const RandoParams& randoParams);
    void replaceToOskill(const RandoParams& randoParams);
    void removeNonClass(Character target);
    void makePerfect();

    void readFromRow(const AffixPropList&     propList,
                     const SkillCodeResolver& codeResolver,
                     const SkillGetter&       skillGetter,
                     const BasicProperties&   parentProperties,
                     std::set<std::string>*   unknownList,
                     std::set<std::string>*   skippedList);
    void writeToRow(AffixPropList& propList) const;

    int getTotalSize() const;

    void propagateDataToParent(BasicProperties& parent) const;

    void append(MagicPropList added);

    void truncateRandom(RandomGenerator& rng, const int newSize);
};

}
