/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleCharacter.hpp"

namespace D2ModGen {

ModuleCharacter::ModuleCharacter()
    : PluginModule(Mernel::PropertyTreeMap{
          { "addCube", Mernel::PropertyTreeScalar{ false } },
          { "addTeleport", Mernel::PropertyTreeScalar{ false } },
          { "idTome", Mernel::PropertyTreeScalar{ false } },
          { "manaPots", Mernel::PropertyTreeScalar{ false } },
          { "mercDam", Mernel::PropertyTreeScalar{ 100 } },
          { "mercHP", Mernel::PropertyTreeScalar{ 100 } },
          { "skillPerLevel", Mernel::PropertyTreeScalar{ 1 } },
          { "skillPointLimit", Mernel::PropertyTreeScalar{ 20 } },
          { "statPerLevel", Mernel::PropertyTreeScalar{ 5 } },
      })
{}

void ModuleCharacter::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const int statPerLevel    = input.getInt("statPerLevel");
    const int skillPerLevel   = input.getInt("skillPerLevel");
    const int skillPointLimit = input.getInt("skillPointLimit");
    const int mercHPpercent   = input.getInt("mercHP");
    const int mercDampercent  = input.getInt("mercDam");

    for (auto& row : output.charstats.rows) {
        if (row.clas == "Expansion")
            continue;

        if (input.getInt("addCube"))
            row.items.push_back(DataRow_charstats::Item{ "box", 1 });
        if (input.getInt("idTome"))
            for (auto& item : row.items) {
                if (item.code == "isc")
                    item.code = "ibk";
            }
        if (input.getInt("manaPots"))
            for (auto& item : row.items) {
                if (item.code == "hp1")
                    item.code = "mp1";
            }

        if (input.getInt("addTeleport"))
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
