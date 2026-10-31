/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMonDensity.hpp"

#include <algorithm>

namespace D2ModGen {

void ModuleMonDensity::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleMonDensity)
        return;
    const auto& module = config.m_moduleMonDensity;
    {
        const int groupIncrease = module.m_mon_groups;
        if (groupIncrease > 0) {
            for (auto& row : output.monstats.rows) {
                if (!row.isRegularMob())
                    continue;
                auto& valueMin = row.minGrp;
                auto& valueMax = row.maxGrp;
                if (!valueMin.has_value() || !valueMax.has_value())
                    continue;
                if (valueMin == valueMax && valueMax == 1)
                    continue;
                for (auto* value : { &valueMin, &valueMax }) {
                    const int prev = value->value();
                    const int next = prev + groupIncrease;
                    *value         = std::min(next, 99);
                }
            }
        }
    }
    {
        const int density       = module.m_density;
        auto      densityAdjust = [density](std::optional<int>& value) {
            if (density == 1)
                return;
            const int prev = value.value();
            const int next = prev * density;
            value          = std::clamp(next, 100, 9900);
        };
        const int packs       = module.m_packs;
        auto      packsAdjust = [packs](std::optional<int>& value) {
            if (packs == 1)
                return;
            const int prev = value.value();
            const int next = prev * packs;
            value          = (std::min(next, 255));
        };
        for (auto& row : output.levels.rows) {
            if (!row.hasSpawns())
                continue;

            if (module.m_hellPacks) {
                row.monUMin[0] = row.monUMin[1] = row.monUMin[2];
                row.monUMax[0] = row.monUMax[1] = row.monUMax[2];
            }

            for (auto& val : row.monUMin)
                packsAdjust(val);
            for (auto& val : row.monUMax)
                packsAdjust(val);

            for (auto& val : row.monDen)
                densityAdjust(val);
        }
    }
}

}
