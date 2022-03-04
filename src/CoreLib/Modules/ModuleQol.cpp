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

PropertyTreeScalarMap ModuleQol::defaultValues() const
{
    return {
        { "tomeSize", false },
        { "keySize", false },
        { "quiverSize", false },
        { "uniqueCharmLimit", false },
        { "weakenTownSkills", false },
        { "reduceCost", 100 },
    };
}

IModule::UiControlHintMap ModuleQol::uiHints() const
{
    return {
        { "reduceCost", UiControlHintSliderMinMax(5, 100) },
    };
}

void ModuleQol::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    const bool tomeSize         = input.getInt("tomeSize");
    const bool keySize          = input.getInt("keySize");
    const bool quiverSize       = input.getInt("quiverSize");
    const bool uniqueCharmLimit = input.getInt("uniqueCharmLimit");
    const bool weakenTownSkills = input.getInt("weakenTownSkills");
    const int  reduceCost       = input.getInt("reduceCost");
    if (tomeSize || keySize || quiverSize) {
        Table&    table = output.tableSet.tables["misc"];
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
        Table&    table = output.tableSet.tables["uniqueitems"];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            row["carry1"].str = "";
        }
    }
    if (weakenTownSkills) {
        static const StringSet s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        Table&                             table = output.tableSet.tables["skills"];
        TableView                          tableView(table, true);
        for (auto& row : tableView) {
            if (s_exceptions.contains(row["skill"].str))
                row["InTown"].str = "1";
        }
    }
    if (reduceCost != 100) {
        {
            Table&    table = output.tableSet.tables["skills"];
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
            Table&    table = output.tableSet.tables["itemstatcost"];
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
}

}
