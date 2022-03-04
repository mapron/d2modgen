/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonStats.hpp"
#include "TableUtils.hpp"

#include <algorithm>

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleMonStats>();
}

PropertyTreeScalarMap ModuleMonStats::defaultValues() const
{
    return {
        { "mon_ar", 100 },
        { "mon_def", 100 },
        { "mon_hp", 100 },
        { "mon_dam", 100 },
        { "mon_xp", 100 },
        { "max_resist", 250 },
        { "rel_resist", 100 },
        { "resist_apply_elem", true },
        { "resist_apply_poison", true },
        { "resist_apply_magic", false },
        { "resist_apply_physical", false },
    };
}

IModule::UiControlHintMap ModuleMonStats::uiHints() const
{
    return {
        { "mon_ar", UiControlHintSlider(10, 10) },
        { "mon_def", UiControlHintSlider(10, 10) },
        { "mon_hp", UiControlHintSlider(10, 10) },
        { "mon_dam", UiControlHintSlider(10, 10) },
        { "mon_xp", UiControlHintSlider(10, 10) },

        { "max_resist", UiControlHintSliderMinMax(90, 250) },
        { "rel_resist", UiControlHintSlider(10, 10) },
    };
}

void ModuleMonStats::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    {
        TableView view(output.tableSet.tables["monlvl"], true);
        auto      proceedMonParam = [&view, &input](const std::string& key, const StringVector& cols) {
            if (input.isDefault(key))
                return;
            const int percent = input.getInt(key);
            for (auto& row : view) {
                if (row["Level"] == "0")
                    continue;
                for (auto& col : cols) {
                    if (!row.hasColumn(col))
                        continue;

                    auto& value = row[col];
                    value.setInt(value.toInt() * percent / 100);
                }
            }
        };

        proceedMonParam("mon_ar", { "TH", "TH(N)", "TH(H)", "L-TH", "L-TH(N)", "L-TH(H)" });
        proceedMonParam("mon_def", { "AC", "AC(N)", "AC(H)", "L-AC", "L-AC(N)", "L-AC(H)" });
        proceedMonParam("mon_hp", { "HP", "HP(N)", "HP(H)", "L-HP", "L-HP(N)", "L-HP(H)" });
        proceedMonParam("mon_dam", { "DM", "DM(N)", "DM(H)", "L-DM", "L-DM(N)", "L-DM(H)" });
        proceedMonParam("mon_xp", { "XP", "XP(N)", "XP(H)", "L-XP", "L-XP(N)", "L-XP(H)" });
    }

    {
        StringVector resColumns;
        if (input.getInt("resist_apply_elem"))
            resColumns << StringVector{ "ResFi", "ResLi", "ResCo", "ResFi(N)", "ResLi(N)", "ResCo(N)", "ResFi(H)", "ResLi(H)", "ResCo(H)" };

        if (input.getInt("resist_apply_poison"))
            resColumns << StringVector{ "ResPo", "ResPo(N)", "ResPo(H)" };

        if (input.getInt("resist_apply_magic"))
            resColumns << StringVector{ "ResMa", "ResMa(N)", "ResMa(H)" };

        if (input.getInt("resist_apply_physical"))
            resColumns << StringVector{ "ResDm", "ResDm(N)", "ResDm(H)" };

        TableView view(output.tableSet.tables["monstats"], true);
        const int maxResistCap = input.getInt("max_resist");
        const int relResist    = input.getInt("rel_resist");
        for (auto& col : resColumns) {
            if (!view.hasColumn(col))
                continue;

            for (auto& row : view) {
                const bool isRegular = row["isSpawn"] == "1" && row["killable"] == "1" && row["npc"] == "" && row["boss"] == "";
                if (!isRegular)
                    continue;
                auto& res    = row[col];
                int   resist = res.toInt();
                resist       = std::min(maxResistCap, resist);
                if (resist < 100 && relResist != 100) {
                    const int damage    = 100 - resist;
                    const int newDamage = damage * relResist / 100;
                    resist              = std::clamp(100 - newDamage, 0, std::min(99, maxResistCap));
                }
                if (resist == 0)
                    res.str = "";
                else
                    res.setInt(resist);
            }
        }
    }
}

}
