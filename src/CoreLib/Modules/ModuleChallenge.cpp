/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleChallenge.hpp"

namespace D2ModGen {

ModuleChallenge::ModuleChallenge()
    : PluginModule(Mernel::PropertyTreeMap{
          { "hell_minus_res", Mernel::PropertyTreeScalar{ 100 } },
          { "levelIncreaseHell", Mernel::PropertyTreeScalar{ 0 } },
          { "levelIncreaseNightmare", Mernel::PropertyTreeScalar{ 0 } },
          { "levelIncreaseUltra", Mernel::PropertyTreeScalar{ false } },
          { "nightmare_minus_res", Mernel::PropertyTreeScalar{ 40 } },
          { "nodrop_hpsa", Mernel::PropertyTreeScalar{ false } },
          { "nodrop_mpsa", Mernel::PropertyTreeScalar{ false } },
          { "nodrop_rvs", Mernel::PropertyTreeScalar{ false } },
          { "normal_minus_res", Mernel::PropertyTreeScalar{ 0 } },
      })
    , m_items{
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa" },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa" },
        { { "rvs", "rvl" }, "rvs" },
    }
{
}

void ModuleChallenge::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    {
        StringSet disabledIds;
        for (auto& item : m_items) {
            const bool disable = input.getInt("nodrop_" + item.settingKey);
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
        auto checkPenalty = [&input, &output](const std::string& key, const std::string& name) {
            const int penalty = input.getInt(key);
            for (auto& row : output.difficultylevels.rows) {
                if (row.name == name) {
                    row.resistPenalty = -penalty;
                }
            }
        };
        checkPenalty("normal_minus_res", "Normal");
        checkPenalty("nightmare_minus_res", "Nightmare");
        checkPenalty("hell_minus_res", "Hell");
    }

    const int levelIncreaseNightmare = input.getInt("levelIncreaseNightmare");
    const int levelIncreaseHell      = input.getInt("levelIncreaseHell");

    if (levelIncreaseNightmare || levelIncreaseHell) {
        const int levelIncreaseUltra = input.getInt("levelIncreaseUltra") ? 105 : 85;

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
