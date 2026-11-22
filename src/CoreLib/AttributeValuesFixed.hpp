/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX_License_Identifier: BSL_1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeValues.hpp"

namespace D2ModGen {

static inline const NonParRecordList g_generatedNonParRecordsGrouped{
    NonParRecord{
        .freq      = 1,
        .group     = AttributeGroup::AllClassSkills,
        .ids       = { AttributeId::ama, AttributeId::pal, AttributeId::nec, AttributeId::sor, AttributeId::bar, AttributeId::dru, AttributeId::ass, AttributeId::war },
        .slotTypes = { SlotType::MainArmor, SlotType::Amulet, SlotType::Weapon, SlotType::Set },
        .slotScale = { { SlotType::Set, 1500_perm } },
        .minLevel  = 5,
        .minMedian = 1,
        .maxMedian = 2,
        .medCurve  = { 500_perm, 500_perm, 1000_perm, 1000_perm, 1000_perm }, // 1-1-1, 1-1-2, 2-2-3, 2-3-3, 2-3-4
        .ratioP000 = { 1000_perm, 1000_perm, 500_perm, 500_perm, 500_perm },
        .ratioP090 = { 1000_perm, 1000_perm, 1000_perm, 1500_perm, 1500_perm },
        .ratioP100 = { 1000_perm, 2000_perm, 1500_perm, 1500_perm, 2000_perm },
    },
    NonParRecord{
        .freq      = 1,
        .group     = AttributeGroup::Res,
        .ids       = { AttributeId::res_fire, AttributeId::res_ltng, AttributeId::res_cold, AttributeId::res_mag, AttributeId::res_pois },
        .slotTypes = { SlotType::MainArmor, SlotType::OtherArmor, SlotType::Ring, SlotType::Amulet, SlotType::Weapon, SlotType::Charm, SlotType::Socketable, SlotType::Set },
        .slotScale = { { SlotType::MainArmor, 1200_perm }, { SlotType::OtherArmor, 1200_perm }, { SlotType::Ring, 900_perm }, { SlotType::Amulet, 1600_perm }, { SlotType::Weapon, 1200_perm }, { SlotType::Charm, 500_perm } },
        .minLevel  = 0,
        .minMedian = 10,
        .maxMedian = 15,
        .penMax    = -90,
        .medCurve  = { 600_perm, 800_perm, 1000_perm, 1100_perm, 1200_perm },
        .ratioP090 = { 1500_perm, 1400_perm, 1400_perm, 1300_perm, 1300_perm },
        .ratioP100 = { 2500_perm, 2200_perm, 2000_perm, 1800_perm, 1600_perm },
    },
    NonParRecord{
        .freq      = 1,
        .group     = AttributeGroup::ResMax,
        .ids       = { AttributeId::res_fire_max, AttributeId::res_ltng_max, AttributeId::res_cold_max, AttributeId::res_pois_max },
        .slotTypes = { SlotType::MainArmor, SlotType::OtherArmor, SlotType::Amulet, SlotType::Weapon, SlotType::Socketable, SlotType::Set },
        .slotScale = { { SlotType::MainArmor, 1250_perm }, { SlotType::Socketable, 500_perm } },
        .minLevel  = 7,
        .minMedian = 8,
        .maxMedian = 10,
        .medCurve  = { 500_perm, 800_perm, 1000_perm, 1100_perm, 1200_perm },
        .ratioP000 = { 500_perm, 500_perm, 500_perm, 500_perm, 500_perm },
        .ratioP010 = { 800_perm, 800_perm, 800_perm, 800_perm, 800_perm },
        .ratioP090 = { 666_perm, 1000_perm, 1000_perm, 1000_perm, 1000_perm },
        .ratioP100 = { 666_perm, 1333_perm, 1333_perm, 1333_perm, 1333_perm },
    },
};

}
