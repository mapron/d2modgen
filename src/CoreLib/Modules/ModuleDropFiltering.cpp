/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleDropFiltering.hpp"

#include "DataContext.hpp"
#include "Colors.hpp"

namespace D2ModGen {

void ModuleDropFiltering::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleDropFiltering)
        return;
    const auto& module = config.m_moduleDropFiltering;

    struct Item {
        StringSet internalIds;
        bool      hide;
    };
    const std::vector<Item> m_items{
        { { "isc" }, module.m_hide_isc },
        { { "tsc" }, module.m_hide_tsc },
        { { "hp1", "hp2", "hp3" }, module.m_hide_hps },
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, module.m_hide_hpsa },
        { { "mp1", "mp2", "mp3" }, module.m_hide_mps },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, module.m_hide_mpsa },
        { { "rvs" }, module.m_hide_rvs },
        { { "rvl" }, module.m_hide_rvl },
        { { "aqv", "cqv" }, module.m_hide_ammo },
        { { "vps", "yps", "wms" }, module.m_hide_stam },
        { { "key", "opl", "opm", "ops", "gpl", "gpm", "gps" }, module.m_hide_junks },
    };

    auto replaceJson = [&output, &env](const std::map<std::string, std::string>& replacements, const JsonId& name) {
        if (replacements.empty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto& jsonArray = jsonDoc.getList();
        for (Mernel::PropertyTree& lang : jsonArray) {
            const std::string key = lang["Key"].getScalar().toString();
            if (replacements.contains(key)) {
                auto val              = replacements.at(key);
                lang[env.d2rLocaleId] = Mernel::PropertyTreeScalar{ val };
            }
        }
    };
    std::map<std::string, std::string> replacementsItems;
    std::map<std::string, std::string> replacementsAffix;
    auto                               twoColor = [](const Color& color, const std::string& coloredText, const std::string& grayText) {
        return getColorDesc(color).m_fullBinaryCode + coloredText + getColorDesc(Color::grey).m_fullBinaryCode + grayText;
    };
    if (module.m_compact_pots) {
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
    if (module.m_compact_scrolls) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "tsc", twoColor(Color::blue, "•", "TP") },
            { "isc", twoColor(Color::red, "•", "ID") },
        });
    }
    const std::string& hidden = getColorDesc(Color::transparent).m_fullBinaryCode;
    if (module.m_hide_lowq) {
        replacementsAffix.merge(std::map<std::string, std::string>{
            { "Low Quality", hidden },
            { "Damaged", hidden },
            { "Cracked", hidden },
            { "Crude", hidden },
        });
    }
    for (const auto& item : m_items) {
        if (item.hide) {
            for (auto& id : item.internalIds)
                replacementsItems[id] = hidden;
        }
    }

    replaceJson(replacementsItems, JsonId::ItemNames);
    replaceJson(replacementsAffix, JsonId::ItemNameAffixes);
}

}
