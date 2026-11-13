/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "IStorage.hpp"

namespace D2ModGen {

std::string_view IStorage::s_dataVersionPath = "data\\global\\dataversionbuild.txt";

namespace {

static const std::string s_tableSubfolder = "data\\global\\excel\\";
static const std::string s_tableBase      = s_tableSubfolder + "base\\";

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
    "inventory",
    "itemstatcost",
    "itemtypes",
    "levels",
    "lvlmaze",
    "magicprefix",
    "magicsuffix",
    "misc",
    "missiles",
    "monlvl",
    "monseq",
    "monstats",
    "monstats2",
    "runes",
    "setitems",
    "sets",
    "skilldesc",
    "skills",
    "superuniques",
    "treasureclassex",
    "uniqueitems",
};

const std::array<std::string_view, size_t(JsonId::count__)> s_jsonpaths{
    "data\\local\\lng\\strings\\item-names.json",
    "data\\local\\lng\\strings\\item-nameaffixes.json",
    "data\\hd\\character\\monsters.json",
    "data\\hd\\missiles\\missiles.json",

    "data\\global\\ui\\layouts\\_profilehd.json",
    "data\\global\\ui\\layouts\\_profilelv.json",
    "data\\global\\ui\\layouts\\playerinventoryoriginallayouthd.json",
    "data\\global\\ui\\layouts\\playerinventoryexpansionlayouthd.json",
    "data\\global\\ui\\layouts\\controller\\playerinventoryoriginallayouthd.json",
    "data\\global\\ui\\layouts\\controller\\playerinventoryexpansionlayouthd.json",

};

const std::array<bool, size_t(JsonId::count__)> s_jsonPretty{
    false,
    false,
    false,
    false,

    true,
    true,
    true,
    true,
    true,
    true,
};

const std::array<std::string_view, size_t(BlobId::count__)> s_blobpaths{
    "data\\local\\lng\\eng\\patchstring.tbl",

    "data\\global\\ui\\spells\\amskillicon.dc6",
    "data\\global\\ui\\spells\\soskillicon.dc6",
    "data\\global\\ui\\spells\\neskillicon.dc6",
    "data\\global\\ui\\spells\\paskillicon.dc6",
    "data\\global\\ui\\spells\\baskillicon.dc6",
    "data\\global\\ui\\spells\\drskillicon.dc6",
    "data\\global\\ui\\spells\\asskillicon.dc6",

    /// "data\\hd\\global\\ui\\panel\\docking_bar\\sidepanel_r.sprite",
    /// "data\\hd\\global\\ui\\panel\\docking_bar\\sidepanel_r.lowend.sprite",

    "data\\hd\\global\\ui\\panel\\inventory\\classic_background.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\classic_background.lowend.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\background.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\background.lowend.sprite",

    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_classic.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_classic.lowend.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg.lowend.sprite",

    "data\\hd\\global\\ui\\controller\\panel\\mainpanelbg.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\mainpanelbg.lowend.sprite",

    "data\\hd\\global\\ui\\spells\\amazon\\amskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\amazon\\amskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\sorceress\\soskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\sorceress\\soskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\necromancer\\neskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\necromancer\\neskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\paladin\\paskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\paladin\\paskillicon.lowend.sprite",

    "data\\hd\\global\\ui\\spells\\barbarian\\baskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\barbarian\\baskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\druid\\drskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\druid\\drskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\assassin\\asskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\assassin\\asskillicon.lowend.sprite",
    "data\\hd\\global\\ui\\spells\\warlock\\waskillicon.sprite",
    "data\\hd\\global\\ui\\spells\\warlock\\waskillicon.lowend.sprite",

    "data\\hd\\global\\ui\\panel\\inventory\\classic_background_expanded.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\classic_background_expanded.lowend.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\background_expanded.sprite",
    "data\\hd\\global\\ui\\panel\\inventory\\background_expanded.lowend.sprite",

    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_classic_expanded.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_classic_expanded.lowend.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_expanded.sprite",
    "data\\hd\\global\\ui\\controller\\panel\\inventorypanel\\v2\\inventorybg_expanded.lowend.sprite",

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

bool isPrettyPrintable(JsonId id)
{
    return s_jsonPretty[static_cast<size_t>(id)];
}

std::string_view getBlobRelPath(BlobId id)
{
    return s_blobpaths[static_cast<size_t>(id)];
}
std::string IStorage::makeTableRelativePath(TableId id, bool baseSub)
{
    const std::string& base = baseSub ? s_tableBase : s_tableSubfolder;
    return base + std::string(getTableIdString(id)) + ".txt";
}

}
