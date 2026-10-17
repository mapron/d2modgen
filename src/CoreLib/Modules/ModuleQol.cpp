/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleQol.hpp"

namespace D2ModGen {

ModuleQol::ModuleQol()
    : PluginModule(Mernel::PropertyTreeMap{
          { "keySize", Mernel::PropertyTreeScalar{ false } },
          { "disableExpPenalty", Mernel::PropertyTreeScalar{ false } },
          { "quiverSize", Mernel::PropertyTreeScalar{ false } },
          { "reduceCost", Mernel::PropertyTreeScalar{ 100 } },
          { "showItemLevel", Mernel::PropertyTreeScalar{ false } },
          { "tomeSize", Mernel::PropertyTreeScalar{ false } },
          { "uniqueCharmLimit", Mernel::PropertyTreeScalar{ false } },
          { "weakenTownSkills", Mernel::PropertyTreeScalar{ false } },
          { "weakenTownSkillsExt", Mernel::PropertyTreeScalar{ false } },
      })
{}

void ModuleQol::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool tomeSize            = input.getInt("tomeSize");
    const bool keySize             = input.getInt("keySize");
    const bool quiverSize          = input.getInt("quiverSize");
    const bool uniqueCharmLimit    = input.getInt("uniqueCharmLimit");
    const bool weakenTownSkills    = input.getInt("weakenTownSkills");
    const bool weakenTownSkillsExt = input.getInt("weakenTownSkillsExt");
    const bool showItemLevel       = input.getInt("showItemLevel");
    const bool disableExpPenalty   = input.getInt("disableExpPenalty");
    const int  reduceCost          = input.getInt("reduceCost");
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
