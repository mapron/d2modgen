/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "TableId.hpp"

#include <array>

namespace D2ModGen {

namespace {
const std::array<std::string_view, size_t(TableId::count__)> s_tableNames{
    "weapons",
    "armor",
    "automagic",
    "charstats",
    "cubemain",
    "difficultylevels",
    "experience",
    "gamble",
    "gems",
    "hireling",
    "itemstatcost",
    "itemtypes",
    "levels",
    "magicprefix",
    "magicsuffix",
    "misc",
    "monlvl",
    "monstats",
    "runes",
    "setitems",
    "sets",
    "skilldesc",
    "skills",
    "treasureclassex",
    "uniqueitems",
};

}

std::string_view getTableIdString(TableId id)
{
    return s_tableNames[static_cast<size_t>(id)];
}

}
