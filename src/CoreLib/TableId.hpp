/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <array>
#include <string>

namespace D2ModGen {

enum class TableId
{
    weapons,
    armor,
    automagic,
    charstats,
    cubemain,
    difficultylevels,
    experience,
    gamble,
    gems,
    hireling,
    itemstatcost,
    itemtypes,
    levels,
    magicprefix,
    magicsuffix,
    misc,
    monlvl,
    monstats,
    runes,
    setitems,
    sets,
    skilldesc,
    skills,
    treasureclassex,
    uniqueitems,

    count__,
};

inline auto getTableIds()
{
    std::array<TableId, size_t(TableId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<TableId>(i);
    return result;
}

std::string_view getTableIdString(TableId id);

}
