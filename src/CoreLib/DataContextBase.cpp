/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextBase.hpp"
#include "DataContextEquipment.hpp"

namespace D2ModGen {

Character ItemCodeList::determineClass() const
{
    for (ItemType type : *this)
        if (type->classSpecific != Character::None)
            return type->classSpecific;

    return Character::None;
}

SlotType ItemCodeList::determineSlot() const
{
    for (ItemType type : *this) {
        auto slot = type->determineSlot();
        if (slot != SlotType::None)
            return slot;
    }

    return SlotType::None;
}

AttributeIdiom ItemCodeList::determineIdiom() const
{
    for (ItemType type : *this) {
        auto slot = type->determineIdiom();
        if (slot != AttributeIdiom::None)
            return slot;
    }

    return AttributeIdiom::None;
}

std::string ItemCodeList::toString() const
{
    std::string res = "";
    for (ItemType type : *this) {
        res += type->code + ",";
    }
    return res;
}

}
