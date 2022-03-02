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

const QString s_itemnamesJson  = "data\\local\\lng\\strings\\item-names.json";
const QString s_affixnamesJson = "data\\local\\lng\\strings\\item-nameaffixes.json";

const QLatin1String s_colorLightGray("\xC3\xBF\x63\x30");   // (Item Descriptions)
const QLatin1String s_colorRed("\xC3\xBF\x63\x31");         //
const QLatin1String s_colorBrightGreen("\xC3\xBF\x63\x32"); //  (Set Items)
const QLatin1String s_colorBlue("\xC3\xBF\x63\x33");        // (Magic Items)
const QLatin1String s_colorGold("\xC3\xBF\x63\x34");        // (Unique Items)
const QLatin1String s_colorDarkGray("\xC3\xBF\x63\x35");    // (Socketed/Ethereal Items)
const QLatin1String s_colorTransparent("\xC3\xBF\x63\x36"); //  (Text Doesn't Show)
const QLatin1String s_colorTan("\xC3\xBF\x63\x37");         //
const QLatin1String s_colorOrange("\xC3\xBF\x63\x38");      // (Crafted Items)
const QLatin1String s_colorYellow("\xC3\xBF\x63\x39");      // (Rare Items)
const QLatin1String s_colorDarkGreen("\xC3\xBF\x63\x3A");   //
const QLatin1String s_colorPurple("\xC3\xBF\x63\x3B");      //
const QLatin1String s_colorWhite("\xC3\xBF\x63\x2F");       //  (Brighter than Light Gray)

const std::string s_hidden = QString("%1").arg(s_colorTransparent).toStdString();

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

ModuleDropFiltering::ModuleDropFiltering()
    : m_items{
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

PropertyTreeScalarMap ModuleDropFiltering::defaultValues() const
{
    PropertyTreeScalarMap result{
        { "compact_pots", false },
        { "compact_scrolls", false },
        { "hide_lowq", false },
    };
    for (auto& item : m_items)
        result["hide_" + item.settingKey] = false;
    return result;
}

void ModuleDropFiltering::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy || input.isAllDefault())
        return;

    output.m_extraJson << s_itemnamesJson << s_affixnamesJson;
}

void ModuleDropFiltering::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto replaceJson = [&output](const QMap<std::string, std::string>& replacements, const QString& name) {
        if (replacements.isEmpty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto& jsonArray = jsonDoc.getList();
        for (PropertyTree& lang : jsonArray) {
            const std::string key = lang["Key"].toString();
            if (replacements.contains(key)) {
                auto val = replacements[key];
                for (const std::string& loc : s_locales)
                    lang[loc] = PropertyTreeScalar{ val };
            }
        }
    };
    QMap<std::string, std::string> replacementsItems;
    QMap<std::string, std::string> replacementsAffix;
    if (input.getInt("compact_pots")) {
        replacementsItems.insert(QMap<std::string, std::string>{
            { "mp1", QString("%1!%2MP1").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "mp2", QString("%1!%2MP2").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "mp3", QString("%1!%2MP3").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "mp4", QString("%1!%2MP4").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "mp5", QString("%1!%2MP5").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "hp1", QString("%1!%2HP1").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
            { "hp2", QString("%1!%2HP2").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
            { "hp3", QString("%1!%2HP3").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
            { "hp4", QString("%1!%2HP4").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
            { "hp5", QString("%1!%2HP5").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
            { "rvs", QString("%1Rej%2").arg(s_colorPurple).arg(s_colorLightGray).toStdString() },
            { "rvl", QString("%1Full%2").arg(s_colorPurple).arg(s_colorLightGray).toStdString() },
        });
    }
    if (input.getInt("compact_scrolls")) {
        replacementsItems.insert(QMap<std::string, std::string>{
            { "tsc", QString("%1•%2TP").arg(s_colorBlue).arg(s_colorLightGray).toStdString() },
            { "isc", QString("%1•%2ID").arg(s_colorRed).arg(s_colorLightGray).toStdString() },
        });
    }
    if (input.getInt("hide_lowq")) {
        replacementsAffix.insert(QMap<std::string, std::string>{
            { "Low Quality", s_hidden },
            { "Damaged", s_hidden },
            { "Cracked", s_hidden },
            { "Crude", s_hidden },
        });
    }
    for (const auto& item : m_items) {
        if (input.getInt("hide_" + item.settingKey)) {
            for (auto& id : item.internalIds)
                replacementsItems.insert(id, s_hidden);
        }
    }

    replaceJson(replacementsItems, s_itemnamesJson);
    replaceJson(replacementsAffix, s_affixnamesJson);
}

}
