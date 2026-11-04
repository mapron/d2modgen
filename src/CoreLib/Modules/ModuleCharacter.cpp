/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleCharacter.hpp"

namespace D2ModGen {

void ModuleCharacter::generate()
{
    const auto& module = config.m_character;

    const int skillPointLimit = module.m_skillPointLimit;
    const int mercHPpercent   = module.m_mercHP;
    const int mercDampercent  = module.m_mercDam;

    auto addValue = [](std::optional<int>& value, int extra) {
        int val = value.value_or(0);
        val += extra;
        val   = std::max(0, val);
        value = val;
    };
    auto scaleValue = [](std::optional<int>& value, int scale) {
        int val = value.value_or(0);
        val     = val * scale / 100;
        val     = std::max(1, val);
        value   = val;
    };

    for (auto& row : output.charstats.rows) {
        if (row.clas == "Expansion")
            continue;

        if (module.m_addCube)
            row.items.push_back(DataRow_charstats::Item{ "box", 1 });

        for (auto& item : row.items) {
            if (item.code == "isc" && module.m_idTome)
                item.code = "ibk";
            if (item.code == "tsc" && module.m_tpTome)
                item.code = "tbk";
            if (item.code == "hp1" && module.m_manaPots)
                item.code = "mp1";
        }

        if (module.m_addTeleport)
            row.skills.push_back("Teleport");

        addValue(row.statPerLevel, module.m_extraStatPerLevel);
        addValue(row.skillsPerLevel, module.m_extraSkillPerLevel);
        addValue(row.hpPerLevel, module.m_extraHpPerLevel * 4);
        addValue(row.manaPerLevel, module.m_extraManaPerLevel * 4);
        scaleValue(row.hpPotEfficacy, module.m_hpPotEfficacy);
        scaleValue(row.manaPotEfficacy, module.m_mpPotEfficacy);
    }

    if (mercHPpercent != 100 || mercDampercent != 100) {
        output.hireling.applyIntTransform(&DataRow_hireling::hp, mercHPpercent);
        output.hireling.applyIntTransform(&DataRow_hireling::hp_lvl, mercHPpercent);
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_min, mercDampercent);
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_max, mercDampercent);
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_lvl, mercDampercent);
    }

    if (skillPointLimit != 20) {
        output.skills.applyTransform(
            &DataRow_skills::maxlvl, [skillPointLimit](std::optional<int>& data) {
                if (!data.has_value() || data.value() != 20)
                    return;
                data = skillPointLimit;
            });
    }

    const int skillsReqLevel = module.m_skillsReqLevel;
    output.skills.applyTransform(&DataRow_skills::lvlreq, [skillsReqLevel](std::optional<int>& data) {
        if (!data.has_value())
            return;
        data = data.value() * skillsReqLevel / 100;
    });
}

}
