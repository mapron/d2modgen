/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleQol.hpp"

namespace D2ModGen {

void ModuleQol::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleQol)
        return;
    const auto& module = config.m_moduleQol;

    const bool tomeSize            = module.m_tomeSize;
    const bool keySize             = module.m_keySize;
    const bool quiverSize          = module.m_quiverSize;
    const bool uniqueCharmLimit    = module.m_uniqueCharmLimit;
    const bool weakenTownSkills    = module.m_weakenTownSkills;
    const bool weakenTownSkillsExt = module.m_weakenTownSkillsExt;
    const bool showItemLevel       = module.m_showItemLevel;
    const bool disableExpPenalty   = module.m_disableExpPenalty;
    const int  reduceCost          = module.m_reduceCost;
    if (tomeSize || keySize || quiverSize) {
        for (auto& row : output.misc.rows) {
            auto& code = row.code;
            if (tomeSize && (row.code == "ibk" || row.code == "tbk"))
                row.maxstack = 60;
            if (keySize && code == "key")
                row.maxstack = 50;
            if (quiverSize && (code == "aqv" || code == "cqv"))
                row.maxstack = 511;
        }
    }
    if (uniqueCharmLimit) {
        for (auto& row : output.uniqueitems.rows) {
            row.carry1.reset();
        }
    }
    if (weakenTownSkills || weakenTownSkillsExt) {
        static const StringSet s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        for (auto& row : output.skills.rows) {
            if (s_exceptions.contains(row.skill) || weakenTownSkillsExt)
                row.inTown = true;
        }
    }
    if (reduceCost != 100) {
        for (auto& row : output.skills.rows) {
            auto& mult = row.costMult;
            auto& add  = row.costAdd;
            if (!mult.has_value())
                continue;
            mult = (mult.value() * reduceCost / 100);
            add  = (add.value_or(0) * reduceCost / 100);
        }

        for (auto& row : output.itemstatcost.rows) {
            auto& mult = row.multiply;
            auto& add  = row.add;
            if (!mult.has_value())
                continue;
            mult = (mult.value() * reduceCost / 100);
            add  = (add.value_or(0) * reduceCost / 100);
        }
    }
    if (showItemLevel) {
        for (auto& row : output.armor.rows)
            row.showLevel = true;
        for (auto& row : output.weapons.rows)
            row.showLevel = true;
    }
    if (disableExpPenalty) {
        for (auto& row : output.experience.rows)
            row.expRatio = 1024;
    }
}

}
