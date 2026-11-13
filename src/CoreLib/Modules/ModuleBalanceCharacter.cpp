/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceCharacter.hpp"

namespace D2ModGen {

void ModuleBalanceCharacter::generate()
{
    const auto& module = config.m_balanceCharacter;

    const int  mercHPpercent     = module.m_mercHP;
    const int  mercDampercent    = module.m_mercDam;
    const bool disableExpPenalty = module.m_disableExpPenalty;

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

    {
        auto checkPenalty = [this](int penalty, const std::string& name) {
            for (auto& row : output.difficultylevels.rows) {
                if (row.name == name) {
                    row.resistPenalty = -penalty;
                }
            }
        };
        checkPenalty(module.m_normalResPenalty, "Normal");
        checkPenalty(module.m_nightmareResPenalty, "Nightmare");
        checkPenalty(module.m_hellResPenalty, "Hell");
    }
    if (disableExpPenalty) {
        for (auto& row : output.experience.rows)
            row.expRatio = 1024;
    }
}

}
