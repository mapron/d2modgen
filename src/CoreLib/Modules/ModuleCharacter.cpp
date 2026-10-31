/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleCharacter.hpp"

namespace D2ModGen {

void ModuleCharacter::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleCharacter)
        return;

    const auto& module = config.m_moduleCharacter;

    const int statPerLevel    = module.m_statPerLevel;
    const int skillPerLevel   = module.m_skillPerLevel;
    const int skillPointLimit = module.m_skillPointLimit;
    const int mercHPpercent   = module.m_mercHP;
    const int mercDampercent  = module.m_mercDam;

    for (auto& row : output.charstats.rows) {
        if (row.clas == "Expansion")
            continue;

        if (module.m_addCube)
            row.items.push_back(DataRow_charstats::Item{ "box", 1 });
        if (module.m_idTome)
            for (auto& item : row.items) {
                if (item.code == "isc")
                    item.code = "ibk";
            }
        if (module.m_manaPots)
            for (auto& item : row.items) {
                if (item.code == "hp1")
                    item.code = "mp1";
            }

        if (module.m_addTeleport)
            row.skills.push_back("Teleport");

        row.statPerLevel   = statPerLevel;
        row.skillsPerLevel = skillPerLevel;
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
}

}
