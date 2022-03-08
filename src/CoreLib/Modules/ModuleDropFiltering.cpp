/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleDropFiltering.hpp"

#include "DataContext.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleDropFiltering>();

const std::string s_itemnamesJson  = "data\\local\\lng\\strings\\item-names.json";
const std::string s_affixnamesJson = "data\\local\\lng\\strings\\item-nameaffixes.json";

const std::string s_colorLightGray("\xC3\xBF\x63\x30");   // (Item Descriptions)
const std::string s_colorRed("\xC3\xBF\x63\x31");         //
const std::string s_colorBrightGreen("\xC3\xBF\x63\x32"); //  (Set Items)
const std::string s_colorBlue("\xC3\xBF\x63\x33");        // (Magic Items)
const std::string s_colorGold("\xC3\xBF\x63\x34");        // (Unique Items)
const std::string s_colorDarkGray("\xC3\xBF\x63\x35");    // (Socketed/Ethereal Items)
const std::string s_colorTransparent("\xC3\xBF\x63\x36"); //  (Text Doesn't Show)
const std::string s_colorTan("\xC3\xBF\x63\x37");         //
const std::string s_colorOrange("\xC3\xBF\x63\x38");      // (Crafted Items)
const std::string s_colorYellow("\xC3\xBF\x63\x39");      // (Rare Items)
const std::string s_colorDarkGreen("\xC3\xBF\x63\x3A");   //
const std::string s_colorPurple("\xC3\xBF\x63\x3B");      //
const std::string s_colorWhite("\xC3\xBF\x63\x2F");       //  (Brighter than Light Gray)

const std::string& s_hidden = s_colorTransparent;

const std::vector<std::string> s_locales{
    "deDE",
    "enUS",
    "esES",
    "esMX",
    "frFR",
    "itIT",
    "jaJP",
    "koKR",
    "plPL",
    "ptBR",
    "ruRU",
    "zhCN",
    "zhTW",
};

}

ModuleDropFiltering::ModuleDropFiltering(PropertyTree moduleMetadata, std::string id)
    : PluginModule(std::move(moduleMetadata), std::move(id))
    , m_items{
        { { "isc" }, "isc" },
        { { "tsc" }, "tsc" },
        { { "hp1", "hp2", "hp3" }, "hps" },
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa" },
        { { "mp1", "mp2", "mp3" }, "mps" },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa" },
        { { "rvs" }, "rvs" },
        { { "rvl" }, "rvl" },
        { { "aqv", "cqv" }, "ammo" },
        { { "vps", "yps", "wms" }, "stam" },
        { { "key", "opl", "opm", "ops", "gpl", "gpm", "gps" }, "junks" },
    }

{
}

void ModuleDropFiltering::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy || input.isAllDefault())
        return;

    output.m_extraJson.insert(s_itemnamesJson);
    output.m_extraJson.insert(s_affixnamesJson);
}

void ModuleDropFiltering::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto replaceJson = [&output](const std::map<std::string, std::string>& replacements, const std::string& name) {
        if (replacements.empty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto& jsonArray = jsonDoc.getList();
        for (PropertyTree& lang : jsonArray) {
            const std::string key = lang["Key"].getScalar().toString();
            if (replacements.contains(key)) {
                auto val = replacements.at(key);
                for (const std::string& loc : s_locales)
                    lang[loc] = PropertyTreeScalar{ val };
            }
        }
    };
    std::map<std::string, std::string> replacementsItems;
    std::map<std::string, std::string> replacementsAffix;
    auto                               twoColor = [](const std::string& color, const std::string& coloredText, const std::string& grayText) {
        return color + coloredText + s_colorLightGray + grayText;
    };
    if (input.getInt("compact_pots")) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "mp1", twoColor(s_colorBlue, "!", "MP1") },
            { "mp2", twoColor(s_colorBlue, "!", "MP2") },
            { "mp3", twoColor(s_colorBlue, "!", "MP3") },
            { "mp4", twoColor(s_colorBlue, "!", "MP4") },
            { "mp5", twoColor(s_colorBlue, "!", "MP5") },
            { "hp1", twoColor(s_colorRed, "!", "HP1") },
            { "hp2", twoColor(s_colorRed, "!", "HP2") },
            { "hp3", twoColor(s_colorRed, "!", "HP3") },
            { "hp4", twoColor(s_colorRed, "!", "HP4") },
            { "hp5", twoColor(s_colorRed, "!", "HP5") },
            { "rvs", twoColor(s_colorRed, "Rej", "") },
            { "rvl", twoColor(s_colorRed, "Full", "") },
        });
    }
    if (input.getInt("compact_scrolls")) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "tsc", twoColor(s_colorBlue, "•", "TP") },
            { "isc", twoColor(s_colorRed, "•", "ID") },
        });
    }
    if (input.getInt("hide_lowq")) {
        replacementsAffix.merge(std::map<std::string, std::string>{
            { "Low Quality", s_hidden },
            { "Damaged", s_hidden },
            { "Cracked", s_hidden },
            { "Crude", s_hidden },
        });
    }
    for (const auto& item : m_items) {
        if (input.getInt("hide_" + item.settingKey)) {
            for (auto& id : item.internalIds)
                replacementsItems[id] = s_hidden;
        }
    }

    replaceJson(replacementsItems, s_itemnamesJson);
    replaceJson(replacementsAffix, s_affixnamesJson);
}

}
