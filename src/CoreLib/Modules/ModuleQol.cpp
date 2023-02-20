/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleQol.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleQol>();
}

void ModuleQol::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

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
        Table&    table = output.tableSet.tables[TableId::misc];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            auto& code     = row["code"];
            auto& maxstack = row["maxstack"];
            if (tomeSize && (code == "ibk" || code == "tbk"))
                maxstack.str = "60";
            if (keySize && code == "key")
                maxstack.str = "50";
            if (quiverSize && (code == "aqv" || code == "cqv"))
                maxstack.str = "511";
        }
    }
    if (uniqueCharmLimit) {
        Table&    table = output.tableSet.tables[TableId::uniqueitems];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            row["carry1"].str = "";
        }
    }
    if (weakenTownSkills || weakenTownSkillsExt) {
        static const StringSet s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        Table&                 table = output.tableSet.tables[TableId::skills];
        TableView              tableView(table, true);
        for (auto& row : tableView) {
            if (s_exceptions.contains(row["skill"].str) || weakenTownSkillsExt)
                row["InTown"].str = "1";
        }
    }
    if (reduceCost != 100) {
        {
            Table&    table = output.tableSet.tables[TableId::skills];
            TableView tableView(table, true);
            for (auto& row : tableView) {
                auto& mult = row["cost mult"];
                auto& add  = row["cost add"];
                if (mult.isEmpty())
                    continue;
                mult.setInt(mult.toInt() * reduceCost / 100);
                add.setInt(add.toInt() * reduceCost / 100);
            }
        }
        {
            Table&    table = output.tableSet.tables[TableId::itemstatcost];
            TableView tableView(table, true);
            for (auto& row : tableView) {
                auto& mult = row["Multiply"];
                auto& add  = row["Add"];
                if (mult.isEmpty())
                    continue;
                mult.setInt(mult.toInt() * reduceCost / 100);
                add.setInt(add.toInt() * reduceCost / 100);
            }
        }
    }
    if (showItemLevel && !input.m_env.isLegacy) {
        for (TableId tableid : { TableId::armor, TableId::weapons }) {
            TableView tableView(output.tableSet.tables[tableid], true);
            for (auto& row : tableView) {
                row["ShowLevel"].setInt(1);
            }
        }
    }
    if (disableExpPenalty) {
        Table&    table = output.tableSet.tables[TableId::experience];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            row["ExpRatio"].setInt(1024);
        }
    }
}

}
