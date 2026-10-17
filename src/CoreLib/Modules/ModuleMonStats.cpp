/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMonStats.hpp"

#include <algorithm>

namespace D2ModGen {

ModuleMonStats::ModuleMonStats()
    : PluginModule(Mernel::PropertyTreeMap{
          { "max_resist", Mernel::PropertyTreeScalar{ 250 } },
          { "mon_ar", Mernel::PropertyTreeScalar{ 100 } },
          { "mon_dam", Mernel::PropertyTreeScalar{ 100 } },
          { "mon_def", Mernel::PropertyTreeScalar{ 100 } },
          { "mon_hp", Mernel::PropertyTreeScalar{ 100 } },
          { "mon_xp", Mernel::PropertyTreeScalar{ 100 } },
          { "rel_resist", Mernel::PropertyTreeScalar{ 100 } },
          { "resist_apply_elem", Mernel::PropertyTreeScalar{ true } },
          { "resist_apply_magic", Mernel::PropertyTreeScalar{ false } },
          { "resist_apply_physical", Mernel::PropertyTreeScalar{ false } },
          { "resist_apply_poison", Mernel::PropertyTreeScalar{ true } },
      })
{}

void ModuleMonStats::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    {
        auto proceedMonParam = [&input](const std::string& key, int& val) {
            const int percent = input.getInt(key);
            val               = val * percent / 100;
        };
        for (auto& row : output.monlvl.rows) {
            if (row.level <= 1)
                continue;
            for (size_t i = 0; i < 6; ++i) {
                proceedMonParam("mon_ar", row.TH[i]);
                proceedMonParam("mon_def", row.AC[i]);
                proceedMonParam("mon_hp", row.HP[i]);
                proceedMonParam("mon_dam", row.DM[i]);
                proceedMonParam("mon_xp", row.XP[i]);
            }
        }
    }

    {
        std::vector<decltype(&DataRow_monstats::resCo)> resCols;

        if (input.getInt("resist_apply_elem")) {
            resCols.push_back(&DataRow_monstats::resCo);
            resCols.push_back(&DataRow_monstats::resLi);
            resCols.push_back(&DataRow_monstats::resFi);
        }

        if (input.getInt("resist_apply_poison"))
            resCols.push_back(&DataRow_monstats::resPo);

        if (input.getInt("resist_apply_magic"))
            resCols.push_back(&DataRow_monstats::resMa);

        if (input.getInt("resist_apply_physical"))
            resCols.push_back(&DataRow_monstats::resDm);

        const int maxResistCap = input.getInt("max_resist");
        const int relResist    = input.getInt("rel_resist");

        for (auto& row : output.monstats.rows) {
            if (!row.isRegularMob())
                continue;

            for (auto& col : resCols) {
                for (size_t i = 0; i < 3; ++i) {
                    std::optional<int>& value = (row.*col)[i];

                    int resist = value.value_or(0);
                    resist     = std::min(maxResistCap, resist);
                    if (resist < 100 && relResist != 100) {
                        const int damage    = 100 - resist;
                        const int newDamage = damage * relResist / 100;
                        resist              = std::clamp(100 - newDamage, 0, std::min(99, maxResistCap));
                    }
                    if (resist == 0)
                        value.reset();
                    else
                        value = resist;
                }
            }
        }
    }
}

}
