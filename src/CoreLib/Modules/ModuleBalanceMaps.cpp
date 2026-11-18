/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceMaps.hpp"

#include "RandoUtils.hpp"

namespace D2ModGen {

namespace {

const std::set s_buggy{ 113, 115, 118 }; // ice caverns don't like 1
}

void ModuleBalanceMaps::generate()
{
    rng = createRNG(config.m_globalSeed);

    const auto& module = config.m_balanceMaps;
    if (module.m_mazeMode > 0) {
        for (auto& row : output.lvlmaze.rows) {
            if (!row.isValid())
                continue;

            if (module.m_mazeMode == 1) {
                int id           = row.levelId.value_or(0);
                int compactCount = s_buggy.contains(id) ? 4 : 1;
                for (auto& room : row.rooms) {
                    room = compactCount;
                }
            } else if (module.m_mazeMode == 2) {
                // all to normal
                row.rooms[2] = row.rooms[0];
                row.rooms[1] = row.rooms[0];
            } else if (module.m_mazeMode == 3) {
                row.rooms[0] = row.rooms[2];
                row.rooms[1] = row.rooms[2];
            }
        }
    }

    const int levelIncreaseNightmare = module.m_levelIncreaseNightmare;
    const int levelIncreaseHell      = module.m_levelIncreaseHell;

    if (levelIncreaseNightmare || levelIncreaseHell) {
        const int levelIncreaseOverBound = module.m_levelIncreaseOverBound ? 105 : 85;

        auto adjustLevel = [levelIncreaseOverBound](std::optional<int>& value, const int levelIncrease) {
            if (!value.has_value() || value.value() > 85)
                return;
            value = std::min(levelIncreaseOverBound, value.value() + levelIncrease);
        };

        for (auto& row : output.levels.rows) {
            if (!row.useForRando)
                continue;

            adjustLevel(row.monLvlEx[1], levelIncreaseNightmare);
            adjustLevel(row.monLvlEx[2], levelIncreaseHell);
        }
    }

    if (const int groupIncrease = module.m_monGroupIncreaseSize; groupIncrease > 0) {
        for (auto& row : output.monstats.rows) {
            if (!row.isRegular)
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
                *value         = std::clamp(next, 1, 99);
            }
        }
    }

    auto densityAdjust = [&module, this](std::optional<int>& value, int min) {
        if (!value.has_value() || value.value() == 0)
            return;
        if (min > 0) {
            value = std::max(min, value.value_or(0));
        }

        if (module.m_densityMin == 100 && module.m_densityMax == 100)
            return;

        const int prev  = value.value_or(0);
        const int scale = genBetweenRng(rng, module.m_densityMin, module.m_densityMax);
        const int next  = scaleValue(prev, scale);
        value           = std::clamp(next, 200, 9900);
    };
    auto packsAdjust = [&module, this](std::optional<int>& value, int min) {
        if (min > 0) {
            value = std::max(min, value.value_or(0));
        }
        if (module.m_elitePacksMin == 100 && module.m_elitePacksMax == 100)
            return;
        const int prev  = value.value_or(0);
        const int scale = genBetweenRng(rng, module.m_elitePacksMin, module.m_elitePacksMax);
        const int next  = scaleValue(prev, scale);
        value           = (std::min(next, 255));
    };

    const int spawnedCount = module.m_numMonExtra;
    for (auto& row : output.levels.rows) {
        if (!row.useForRando)
            continue;

        if (module.m_hellPacks) {
            row.monUMin[0] = row.monUMin[1] = row.monUMin[2];
            row.monUMax[0] = row.monUMax[1] = row.monUMax[2];
        }

        for (size_t i = 0; i < 3; ++i) {
            packsAdjust(row.monUMin[i], module.m_minAbsPacks);
            packsAdjust(row.monUMax[i], module.m_minAbsPacks);
            if (row.monUMax[i].has_value()) {
                row.monUMax[i] = std::max(row.monUMin[i].value_or(0), row.monUMax[i].value_or(0));
                if (module.m_maxPacks)
                    row.monUMin[i] = row.monUMax[i];
            }
        }

        for (auto& val : row.monDen)
            densityAdjust(val, module.m_minAbsDensity);

        row.numMon = std::max(2, row.numMon.value() + spawnedCount);
    }
}

}
