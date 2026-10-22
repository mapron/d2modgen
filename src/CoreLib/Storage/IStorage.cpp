/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "IStorage.hpp"

namespace D2ModGen {

std::string_view IStorage::s_dataVersionPath = "data\\global\\dataversionbuild.txt";

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

const std::array<std::string_view, size_t(JsonId::count__)> s_jsonpaths{
    "data\\local\\lng\\strings\\item-names.json",
    "data\\local\\lng\\strings\\item-nameaffixes.json",
    "data\\hd\\character\\monsters.json",
};

const std::array<std::string_view, size_t(BlobId::count__)> s_blobpaths{
    "data\\local\\lng\\eng\\patchstring.tbl",
};

}

std::string_view getTableIdString(TableId id)
{
    return s_tableNames[static_cast<size_t>(id)];
}

std::string_view getJsonRelPath(JsonId id)
{
    return s_jsonpaths[static_cast<size_t>(id)];
}

std::string_view getBlobRelPath(BlobId id)
{
    return s_blobpaths[static_cast<size_t>(id)];
}

}
