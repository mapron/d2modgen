/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "MagicProp.hpp"

namespace D2ModGen {

void BasicProperties::propagateDataToParent(BasicProperties& parent) const
{
    //propagate(slot, parent.slot, SlotType::None);
    propagate(clas, parent.clas, Character::None);
    propagate(roll, parent.roll, RollType::None);
    propagate(idiom, parent.idiom, AttributeIdiom::None);
    propagate(element, parent.element, Element::None);
    //propagate(levelBracket20, parent.levelBracket20, size_t(-1));
}

std::string BasicProperties::toString() const noexcept
{
    std::string result = "{ ";
    result += "s=" + enumToString(slot);
    if (clas != Character::None)
        result += ", c=" + enumToString(clas);
    if (idiom != AttributeIdiom::None)
        result += ", i=" + enumToString(idiom);
    if (element != Element::None)
        result += ", e=" + enumToString(element);
    if (levelBracket20 != size_t(-1))
        result += ", l=" + std::to_string(levelBracket20);
    result += "}";
    return result;
}

void MagicProp::updateProperties()
{
    assert(desc);
    properties.clas = [this]() {
        if (desc->classSpecific != Character::None)
            return desc->classSpecific;

        if (id == AttributeId::skilltab) {
            int tab = par.value_or(0);
            return static_cast<Character>(tab / 3);
        }
        if (id == AttributeId::skill && parSkill) {
            return parSkill->classSpecific;
        }
        return Character::None;
    }();
    assert(properties.clas <= Character::War);
    properties.idiom   = desc->idiom;
    properties.element = desc->element;
}

std::string MagicProp::toString() const
{
    std::string result;
    result += "[" + enumToString(id) + "] ";
    if (par.has_value())
        result += std::to_string(par.value());
    else
        result += "~";
    if (min.has_value() && max.has_value())
        result += " " + std::to_string(min.value()) + ".." + std::to_string(max.value());

    result += properties.toString();
    return result;
}

void MagicPropRawList::removeKnock(const RandoParams& randoParams)
{
    if (!randoParams.removeKnock)
        return;

    auto pred = [&randoParams](const MagicProp& prop) {
        return (prop.id == AttributeId::knock || prop.id == AttributeId::howl);
    };
    removed += std::erase_if(parsedProps, pred);
}

void MagicPropRawList::replaceToOskill(const RandoParams& randoParams)
{
    for (MagicProp& prop : parsedProps) {
        if (randoParams.replaceSkills && prop.id == AttributeId::skill) {
            if (prop.parSkill && prop.parSkill->canBeOskill)
                prop.id = AttributeId::oskill;
        }
        if (randoParams.replaceCharges && prop.id == AttributeId::charged) {
            if (prop.parSkill && prop.parSkill->canBeOskill) {
                prop.id = AttributeId::oskill;
                if (prop.max < 0) {
                    prop.max = -prop.max.value_or(0);
                }
                prop.min = prop.max;
            }
        }
    }
}

void MagicPropRawList::removeNonClass(Character target)
{
    if (target == Character::None)
        return;

    removed += std::erase_if(parsedProps, [target](const MagicProp& prop) {
        return prop.properties.checkClass(target);
    });
}

void MagicPropRawList::makePerfect()
{
    for (MagicProp& prop : parsedProps) {
        if (isMinMaxRange(prop.id))
            prop.min = prop.max;
    }
}

void MagicPropRawList::readFromRow(const AffixPropList&     propList,
                                   const SkillCodeResolver& codeResolver,
                                   const SkillGetter&       skillGetter,
                                   const BasicProperties&   parentProperties,
                                   std::set<std::string>*   unknownList,
                                   std::set<std::string>*   skippedList)
{
    affixLimit = propList.getLimit();
    for (const auto& col : propList) {
        MagicProp prop;
        prop.id              = getAttributeId(col.code);
        prop.properties      = parentProperties;
        prop.properties.roll = rollTypeHighestGrab;

        if (prop.id == AttributeId::SKIP) {
            if (skippedList)
                skippedList->insert(col.code);
            continue;
        }
        if (prop.id == AttributeId::INVALID) {
            if (unknownList)
                unknownList->insert(col.code);
            keptProps.push_back(col);
            affixLimit--;
            removed++;
            continue;
        }

        prop.desc = &getAttributeDesc(prop.id);
        if (prop.desc->flags.contains(AttributeFlag::Keep)) {
            keptProps.push_back(col);
            affixLimit--;
            removed++;
            continue;
        }
        prop.min = col.min;
        prop.max = col.max;
        if (!col.par.empty()) {
            if (prop.desc->parMode != ParMode::None) {
                if (prop.desc->parMode == ParMode::Skill) {
                    prop.par = codeResolver(col.par);
                    if (prop.par.value() == -1)
                        prop.par.reset();
                    else
                        prop.parSkill = skillGetter(prop.par.value());
                } else {
                    prop.par = std::atoi(col.par.c_str());
                }
            }
        }
        if (prop.desc->parMode == ParMode::PureValue && prop.desc->rangeMode == RangeMode::None) {
            if (!prop.par.has_value()) {
                if (prop.min.has_value())
                    prop.par = prop.min;
            }

            prop.min = prop.par.value_or(0);
            prop.max = prop.par.value_or(0);
            prop.par.reset();
        }
        if (prop.desc->flags.contains(AttributeFlag::Negative)) {
            prop.min = -prop.min.value_or(0);
            prop.max = -prop.max.value_or(0);
        }

        prop.updateProperties();
        parsedProps.push_back(std::move(prop));
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

        AffixProp affix{
            .code = enumToString(prop.id),
            .par  = prop.par.has_value() ? std::to_string(prop.par.value()) : "",
            .min  = prop.min,
            .max  = prop.max,
        };
        if (prop.desc->flags.contains(AttributeFlag::Negative)) {
            affix.min = -affix.min.value_or(0);
            affix.max = -affix.max.value_or(0);
        }
        if (prop.desc->parMode == ParMode::PureValue && prop.desc->rangeMode == RangeMode::None) {
            affix.par = std::to_string(prop.min.value());
            affix.min.reset();
            affix.max.reset();
        }
        if (prop.desc->parMode == ParMode::None)
            affix.par.clear();

        propList.push_back(affix);
    }
}

int MagicPropRawList::getTotalSize() const
{
    return static_cast<int>(parsedProps.size());
}

void MagicPropRawList::propagateDataToParent(BasicProperties& parent) const
{
    for (const MagicProp& prop : parsedProps)
        prop.properties.propagateDataToParent(parent);
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

}
