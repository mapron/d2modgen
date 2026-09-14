/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "TableId.hpp"

#include <map>

namespace D2ModGen {

namespace {
const std::vector<std::string> s_tableNames{
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
    "monstats2",
    "runes",
    "setitems",
    "sets",
    "skilldesc",
    "skills",
    "treasureclassex",
    "uniqueitems",
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
