/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleGambling.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleGambling>();
}

void ModuleGambling::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool charmGamble = input.getInt("charmGamble") && input.m_env.isLegacy; // disabled for D2R now; it crashes the game.
    if (charmGamble) {
        auto& table    = output.tableSet.tables[TableId::gamble];
        table.modified = true;
        table.rows.emplace_back(TableRow({ TableCell("Charm Small"), TableCell("cm1") }));
        table.rows.emplace_back(TableRow({ TableCell("Charm Medium"), TableCell("cm2") }));
        table.rows.emplace_back(TableRow({ TableCell("Charm Large"), TableCell("cm3") }));
        table.rows.emplace_back(TableRow({ TableCell("Jewel"), TableCell("jew") }));
    }
    if (input.isAllDefault({ "ratioUnique", "ratioSet", "ratioRare", "ratioExc", "ratioElite" }))
        return;

    static const std::vector<std::pair<std::string, std::string>> s_columns{
        { "GambleRare", "ratioRare" },
        { "GambleSet", "ratioSet" },
        { "GambleUnique", "ratioUnique" },
        { "GambleUber", "ratioExc" },
        { "GambleUltra", "ratioElite" },
    };

    {
        TableView view(output.tableSet.tables[TableId::difficultylevels], true);
        for (auto& row : view) {
            for (auto& p : s_columns) {
                auto&     col      = row[p.first];
                const int multiply = input.getInt(p.second);
                col.setInt(col.toInt() * multiply);
            }
        }
    }
}

}
