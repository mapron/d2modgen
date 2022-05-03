/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleDropFiltering.hpp"

#include "DataContext.hpp"
#include "Colors.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleDropFiltering>();

const std::string s_itemnamesJson  = "data\\local\\lng\\strings\\item-names.json";
const std::string s_affixnamesJson = "data\\local\\lng\\strings\\item-nameaffixes.json";

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
    auto                               twoColor = [](const Color& color, const std::string& coloredText, const std::string& grayText) {
        return getColorDesc(color).m_fullBinaryCode + coloredText + getColorDesc(Color::grey).m_fullBinaryCode + grayText;
    };
    if (input.getInt("compact_pots")) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "mp1", twoColor(Color::blue, "!", "MP1") },
            { "mp2", twoColor(Color::blue, "!", "MP2") },
            { "mp3", twoColor(Color::blue, "!", "MP3") },
            { "mp4", twoColor(Color::blue, "!", "MP4") },
            { "mp5", twoColor(Color::blue, "!", "MP5") },
            { "hp1", twoColor(Color::red, "!", "HP1") },
            { "hp2", twoColor(Color::red, "!", "HP2") },
            { "hp3", twoColor(Color::red, "!", "HP3") },
            { "hp4", twoColor(Color::red, "!", "HP4") },
            { "hp5", twoColor(Color::red, "!", "HP5") },
            { "rvs", twoColor(Color::purple, "Rej", "") },
            { "rvl", twoColor(Color::purple, "Full", "") },
        });
    }
    if (input.getInt("compact_scrolls")) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "tsc", twoColor(Color::blue, "•", "TP") },
            { "isc", twoColor(Color::red, "•", "ID") },
        });
    }
    const std::string& hidden = getColorDesc(Color::transparent).m_fullBinaryCode;
    if (input.getInt("hide_lowq")) {
        replacementsAffix.merge(std::map<std::string, std::string>{
            { "Low Quality", hidden },
            { "Damaged", hidden },
            { "Cracked", hidden },
            { "Crude", hidden },
        });
    }
    for (const auto& item : m_items) {
        if (input.getInt("hide_" + item.settingKey)) {
            for (auto& id : item.internalIds)
                replacementsItems[id] = hidden;
        }
    }

    replaceJson(replacementsItems, s_itemnamesJson);
    replaceJson(replacementsAffix, s_affixnamesJson);
}

}
