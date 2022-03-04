/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "TableId.hpp"

#include <map>

namespace D2ModGen {

namespace {
const std::vector<std::string> s_tableNames{
    "weapons",
    "actinfo",
    "armor",
    "armtype",
    "automagic",
    "automap",
    "belts",
    "bodylocs",
    "books",
    "charstats",
    "colors",
    "compcode",
    "composit",
    "cubemain",
    "cubemod",
    "difficultylevels",
    "elemtypes",
    "events",
    "experience",
    "gamble",
    "gems",
    "hireling",
    "hitclass",
    "inventory",
    "itemratio",
    "itemstatcost",
    "itemtypes",
    "levels",
    "lowqualityitems",
    "lvlmaze",
    "lvlprest",
    "lvlsub",
    "lvltypes",
    "lvlwarp",
    "magicprefix",
    "magicsuffix",
    "misc",
    "misscalc",
    "missiles",
    "monai",
    "monequip",
    "monlvl",
    "monmode",
    "monplace",
    "monpreset",
    "monprop",
    "monseq",
    "monsounds",
    "monstats",
    "monstats2",
    "montype",
    "monumod",
    "npc",
    "objects",
    "objgroup",
    "objmode",
    "objpreset",
    "objtype",
    "overlay",
    "pettype",
    "playerclass",
    "plrmode",
    "plrtype",
    "properties",
    "qualityitems",
    "rareprefix",
    "raresuffix",
    "runes",
    "setitems",
    "sets",
    "shrines",
    "skillcalc",
    "skilldesc",
    "skills",
    "soundenviron",
    "sounds",
    "states",
    "storepage",
    "superuniques",
    "treasureclassex",
    "uniqueappellation",
    "uniqueitems",
    "uniqueprefix",
    "uniquesuffix",
    "wanderingmon",
};

const std::map<std::string, TableId> s_tableIdCache = []() {
    std::map<std::string, TableId> res;
    for (size_t i = 0; i < s_tableNames.size(); ++i) {
        res[s_tableNames[i]] = static_cast<TableId>(i);
    }
    return res;
}();

}

const StringVector& getTableNames()
{
    return s_tableNames;
}

std::string getTableIdString(TableId id)
{
    const int index = static_cast<int>(id);
    return s_tableNames[index];
}

bool findTableId(const std::string& name, TableId& id)
{
    auto it = s_tableIdCache.find(name);
    if (it == s_tableIdCache.cend())
        return false;

    id = it->second;
    return true;
}

}
