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

    const Percent mercHP            = module.m_mercHP;
    const Percent mercDam           = module.m_mercDam;
    const bool    disableExpPenalty = module.m_disableExpPenalty;

    auto addValue = [](std::optional<int>& value, int extra) {
        int val = value.value_or(0);
        val += extra;
        val   = std::max(0, val);
        value = val;
    };
    auto scaleValue = [](std::optional<int>& value, Percent scale) {
        int val = value.value_or(0);
        val     = val * scale;
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

    if (mercHP != g_identityPct) {
        output.hireling.applyIntTransform(&DataRow_hireling::hp, mercHP);
        output.hireling.applyIntTransform(&DataRow_hireling::hp_lvl, mercHP);
    }
    if (mercDam != g_identityPct) {
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_min, mercDam);
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_max, mercDam);
        output.hireling.applyIntTransform(&DataRow_hireling::dmg_lvl, mercDam);
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
