/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextEquipment.hpp"

namespace D2ModGen {

// =========================================================

SlotType DataRow_itemtypes::determineSlot() const
{
    if (code == "shld" || code == "pala")
        return SlotType::MainArmor;
    if (code == "weap")
        return SlotType::Weapon;
    if (code == "armo")
        return SlotType::OtherArmor;
    if (code == "pvpm" || code == "map" || code == "t4m" || code == "t5m")
        return SlotType::PD2Map;
    if (code == "misl")
        return SlotType::Weapon; // quiver
    if (bodyLoc1 == "head" || bodyLoc1 == "tors")
        return SlotType::MainArmor;
    if (bodyLoc1 == "feet" || bodyLoc1 == "glov" || bodyLoc1 == "belt")
        return SlotType::OtherArmor;
    if (bodyLoc1 == "rrin")
        return SlotType::Ring;
    if (bodyLoc1 == "neck")
        return SlotType::Amulet;
    if (bodyLoc1 == "rarm")
        return SlotType::Weapon;

    if (equiv1 == "char")
        return SlotType::Charm;
    if (equiv1 == "gem" || equiv1 == "sock")
        return SlotType::Socketable;
    if (equiv1 == "SET")
        return SlotType::Set;

    for (auto* parent : parents) {
        SlotType parentType = parent->determineSlot();
        if (parentType != SlotType::None)
            return parentType;
    }

    return SlotType::None;
}

}
