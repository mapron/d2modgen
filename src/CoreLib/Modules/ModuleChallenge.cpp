/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleChallenge.hpp"

#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleChallenge>();
}

ModuleChallenge::ModuleChallenge()
    : m_items{
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa" },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa" },
        { { "rvs", "rvl" }, "rvs" },
    }
{
}

PropertyTreeScalarMap ModuleChallenge::defaultValues() const
{
    PropertyTreeScalarMap result{
        { "normal_minus_res", 0 },
        { "nightmare_minus_res", 40 },
        { "hell_minus_res", 100 },
        { "levelIncreaseNightmare", 0 },
        { "levelIncreaseHell", 0 },
        { "levelIncreaseUltra", false },
    };
    for (auto& item : m_items) {
        result["nodrop_" + item.settingKey] = false;
    }
    return result;
}

IModule::UiControlHintMap ModuleChallenge::uiHints() const
{
    return {
        { "normal_minus_res", UiControlHintSliderMinMax(0, 250) },
        { "nightmare_minus_res", UiControlHintSliderMinMax(0, 250) },
        { "hell_minus_res", UiControlHintSliderMinMax(0, 250) },
        { "levelIncreaseNightmare", UiControlHintSliderMinMax(0, 20) },
        { "levelIncreaseHell", UiControlHintSliderMinMax(0, 30) },
    };
}

void ModuleChallenge::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    {
        TableView view(output.tableSet.tables[TableId::treasureclassex]);

        StringSet disabledIds;
        for (auto& item : m_items) {
            const bool disable = input.getInt("nodrop_" + item.settingKey);
            if (disable)
                for (const auto& id : item.internalIds)
                    disabledIds.insert(id);
        }
        if (!disabledIds.empty()) {
            view.markModified();
            for (auto& row : view) {
                DropSet dropSet;
                dropSet.readRow(row);
                for (int i = dropSet.m_items.size() - 1; i >= 0; i--) {
                    auto&       dropItem = dropSet.m_items[i];
                    std::string tcName   = dropItem.tc.str;
                    if (disabledIds.contains(tcName)) {
                        dropSet.m_noDrop += dropItem.prob;
                        dropSet.m_items.erase(dropSet.m_items.begin() + i);
                    }
                }
                dropSet.writeRow(row);
            }
        }
    }
    {
        TableView view(output.tableSet.tables[TableId::difficultylevels]);
        auto      checkPenalty = [&input, &output, &view](const std::string& key, const std::string& name) {
            if (input.isAllDefault({ key }))
                return;
            const int penalty = input.getInt(key);
            view.markModified();
            for (auto& row : view) {
                if (row["Name"].str == name) {
                    row["ResistPenalty"].setInt(penalty);
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
        TableView view(output.tableSet.tables[TableId::levels], true);
        const int levelIncreaseUltra = input.getInt("levelIncreaseUltra") ? 105 : 85;

        auto adjustLevel = [levelIncreaseUltra](TableView::RowView& row, const std::string& key, const int levelIncrease) {
            auto& lev   = row[key];
            int   level = lev.toInt();
            if (!level || level > 85)
                return;
            lev.setInt(std::min(levelIncreaseUltra, level + levelIncrease));
        };

        for (auto& row : view) {
            adjustLevel(row, "MonLvlEx(N)", levelIncreaseNightmare);
            adjustLevel(row, "MonLvlEx(H)", levelIncreaseHell);
        }
    }
}

}
