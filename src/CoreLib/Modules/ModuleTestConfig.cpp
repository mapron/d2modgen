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

PropertyTreeScalarMap ModuleTestConfig::defaultValuesScalar() const
{
    return {
        { "addKeys", true },
    };
}

void ModuleTestConfig::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (!input.getInt("addKeys"))
        return;

    Table&    charTable = output.tableSet.tables[TableId::charstats];
    TableView charTableView(charTable, true);

    for (auto& rowView : charTableView) {
        if (rowView["class"] == "Expansion")
            continue;
        for (int i = 1; i <= 10; ++i) {
            const auto codeColumn  = argCompat("item%1", i);
            const auto countColumn = argCompat("item%1count", i);

            auto& code  = rowView[codeColumn];
            auto& count = rowView[countColumn];
            if (count == "0" || count == "") {
                count.str = "1";
                code.str  = "key";
                break;
            }
        }
    }
}

}
