/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleRequirements.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleRequirements>();
}

void ModuleRequirements::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto&     tableSet  = output.tableSet;
    Table&    charTable = tableSet.tables[TableId::charstats];
    TableView charTableView(charTable, true);

    const int reqStr = input.getInt("reqStr");
    const int reqDex = input.getInt("reqDex");

    const int equipReqLevel   = input.getInt("equipReqLevel");
    const int uniquesReqLevel = input.getInt("uniquesReqLevel");
    const int affixReqLevel   = input.getInt("affixReqLevel");
    const int skillsReqLevel  = input.getInt("skillsReqLevel");

    const int equipItemLevel   = input.getInt("equipItemLevel");
    const int uniquesItemLevel = input.getInt("uniquesItemLevel");
    const int affixItemLevel   = input.getInt("affixItemLevel");

    auto makeLevelModifier = [](int percent) {
        return [percent](const int value) {
            if (value <= 0 || percent == 100)
                return value;
            const int newLevel = value * percent / 100;
            return std::clamp(newLevel, 1, 110);
        };
    };

    for (const auto tableName : { TableId::armor, TableId::weapons }) {
        Table&    table = tableSet.tables[tableName];
        TableView view(table, true);
        view.applyIntTransform("reqstr", [reqStr](const int value) {
            const int newStat = value * reqStr / 100;
            return newStat < 10 ? 0 : newStat;
        });
        view.applyIntTransform("reqdex", [reqDex](const int value) {
            const int newStat = value * reqDex / 100;
            return newStat < 10 ? 0 : newStat;
        });
        view.applyIntTransform("level", makeLevelModifier(equipItemLevel));
        view.applyIntTransform("levelreq", makeLevelModifier(equipReqLevel));
    }
}

}
