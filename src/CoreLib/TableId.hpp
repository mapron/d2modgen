/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

const StringVector& getTableNames();

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
    monstats2,
    runes,
    setitems,
    sets,
    skilldesc,
    skills,
    treasureclassex,
    uniqueitems,
};

std::string getTableIdString(TableId id);
bool        findTableId(const std::string& name, TableId& id);

}
