/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMonStats.hpp"

#include <algorithm>

namespace D2ModGen {

void ModuleMonStats::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    const auto& module = config.m_monsterStats;
    {
        auto proceedMonParam = [](int64_t percent, int& val) {
            val = val * percent / 100;
        };
        for (auto& row : output.monlvl.rows) {
            if (row.level <= 1)
                continue;
            for (size_t i = 0; i < 6; ++i) {
                proceedMonParam(module.m_monsterAttack, row.TH[i]);
                proceedMonParam(module.m_monsterDefense, row.AC[i]);
                proceedMonParam(module.m_monsterHealth, row.HP[i]);
                proceedMonParam(module.m_monsterDamage, row.DM[i]);
                proceedMonParam(module.m_monsterExperience, row.XP[i]);
            }
        }
    }

    const int  maxResistCap   = module.m_immunityLimit;
    const bool applyResistCap = maxResistCap < 250;
    if (applyResistCap) {
        std::vector<decltype(&DataRow_monstats::resCo)> resCols;

        if (module.m_applyCold)
            resCols.push_back(&DataRow_monstats::resCo);
        if (module.m_applyLight)
            resCols.push_back(&DataRow_monstats::resLi);
        if (module.m_applyFire)
            resCols.push_back(&DataRow_monstats::resFi);
        if (module.m_applyPois)
            resCols.push_back(&DataRow_monstats::resPo);
        if (module.m_applyMag)
            resCols.push_back(&DataRow_monstats::resMa);
        if (module.m_applyPhys)
            resCols.push_back(&DataRow_monstats::resDm);

        const int relResist = module.m_relativeResistDmg;

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
