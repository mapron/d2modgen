/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleTestConfig.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleTestConfig>();
}

PropertyTreeScalarMap ModuleTestConfig::defaultValues() const
{
    return {
        { "addKeys", true },
    };
}

void ModuleTestConfig::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (!input.getInt("addKeys"))
        return;

    Table&    charTable = output.tableSet.tables["charstats"];
    TableView charTableView(charTable, true);

    for (auto& rowView : charTableView) {
        if (rowView["class"] == "Expansion")
            continue;
        for (int i = 1; i <= 10; ++i) {
            const auto codeColumn  = QString("item%1").arg(i);
            const auto countColumn = QString("item%1count").arg(i);

            QString& code  = rowView[codeColumn];
            QString& count = rowView[countColumn];
            if (count == "0") {
                count = "1";
                code  = "key";
                break;
            }
        }
    }
}

}
