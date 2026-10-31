/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleChallenge.hpp"

namespace D2ModGen {

void ModuleChallenge::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleChallenge)
        return;

    const auto& module = config.m_moduleChallenge;
    struct Item {
        StringSet internalIds;
        bool      disableDrop;
    };
    const std::vector<Item> m_items{
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, module.m_nodrop_hpsa },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, module.m_nodrop_mpsa },
        { { "rvs", "rvl" }, module.m_nodrop_rvs },
    };

    {
        StringSet disabledIds;
        for (auto& item : m_items) {
            const bool disable = item.disableDrop;
            if (disable)
                for (const auto& id : item.internalIds)
                    disabledIds.insert(id);
        }
        if (!disabledIds.empty()) {
            for (auto& row : output.treasureclassex.rows) {
                for (int i = row.items.size() - 1; i >= 0; i--) {
                    auto& dropItem = row.items[i];
                    if (disabledIds.contains(dropItem.tc)) {
                        row.noDrop = row.noDrop.value_or(0) + dropItem.prob.value_or(0);
                        row.items.erase(row.items.begin() + i);
                    }
                }
            }
        }
    }
    {
        auto checkPenalty = [&output](int penalty, const std::string& name) {
            for (auto& row : output.difficultylevels.rows) {
                if (row.name == name) {
                    row.resistPenalty = -penalty;
                }
            }
        };
        checkPenalty(module.m_normal_minus_res, "Normal");
        checkPenalty(module.m_nightmare_minus_res, "Nightmare");
        checkPenalty(module.m_hell_minus_res, "Hell");
    }

    const int levelIncreaseNightmare = module.m_levelIncreaseNightmare;
    const int levelIncreaseHell      = module.m_levelIncreaseHell;

    if (levelIncreaseNightmare || levelIncreaseHell) {
        const int levelIncreaseUltra = module.m_levelIncreaseUltra ? 105 : 85;

        auto adjustLevel = [levelIncreaseUltra](std::optional<int>& value, const int levelIncrease) {
            if (!value.has_value() || value.value() > 85)
                return;
            value = std::min(levelIncreaseUltra, value.value() + levelIncrease);
        };

        for (auto& row : output.levels.rows) {
            adjustLevel(row.monLvlEx[1], levelIncreaseNightmare);
            adjustLevel(row.monLvlEx[2], levelIncreaseHell);
        }
    }
}

}
