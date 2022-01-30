/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageDropFiltering.hpp"

#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>

namespace D2ModGen {

namespace {
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

const QString s_hidden = QString("%1").arg(s_colorTransparent);

const QStringList s_locales{
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

ConfigPageDropFiltering::ConfigPageDropFiltering(QWidget* parent)
    : ConfigPageAbstract(parent)
    , m_items{
        { { "isc" }, "isc", tr("ID scroll") },
        { { "tsc" }, "tsc", tr("TP scroll") },
        { { "hp1", "hp2", "hp3" }, "hps", tr("Health pots 1-3") },
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa", tr("All Health pots") },
        { { "mp1", "mp2", "mp3" }, "mps", tr("Mana pots 1-3") },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa", tr("All Mana pots") },
        { { "rvs" }, "rvs", tr("Rejuv pots") },
        { { "rvl" }, "rvl", tr("Full Rejuv pots") },
        { { "aqv", "cqv" }, "ammo", tr("Bolts/Arrows") },
        { { "vps", "yps", "wms" }, "stam", tr("Stamina/Antidote/Thawing") },
        { { "key", "opl", "opm", "ops", "gpl", "gpm", "gps" }, "junks", tr("Keys,Fire/Poison pots") },
    }

{
    addWidget(new QLabel(tr("<b>Make item names compact</b>: this will make item names take less space, e.g. '!HP2' for health potion."),
                         this));
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Compact potion names"), "compact_pots", false, this)
               << new CheckboxWidget(tr("Compact TP/ID scrolls"), "compact_scrolls", false, this));

    addWidget(new QLabel(tr("<b>Hide items on the ground</b>: this will make item names transparent; <br>"
                            "you still can pickup them, but their labels will be invisible on Alt press."),
                         this));
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget(tr("Hide ") + item.title, "hide_" + item.settingKey, false, this));

    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Hide low quality/damaged/cracked items"), "hide_lowq", false, this));
    closeLayout();
}

QString ConfigPageDropFiltering::pageHelp() const
{
    return tr("This tab consists of two sections:\n"
              "1. Make some items have compact names, like \"!MP5\" for Super Mana Potion. \n"
              "2. Hide item labels on the ground (you still be able to pick them). ");
}

void ConfigPageDropFiltering::gatherInfo(PreGenerationContext& output, const GenerationEnvironment& env) const
{
    if (env.isLegacy || isAllDefault())
        return;

    output.m_extraJson << s_itemnamesJson << s_affixnamesJson;
}

void ConfigPageDropFiltering::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    auto replaceJson = [&output](const QMap<QString, QString>& replacements, const QString& name) {
        if (replacements.isEmpty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto  jsonArray = jsonDoc.array();
        for (QJsonValueRef lang : jsonArray) {
            QJsonObject   obj = lang.toObject();
            const QString key = obj["Key"].toString();
            if (replacements.contains(key)) {
                auto val = replacements[key];
                for (const QString& loc : s_locales)
                    obj[loc] = val;
            }
            lang = obj;
        }
        jsonDoc.setArray(jsonArray);
    };
    QMap<QString, QString> replacementsItems;
    QMap<QString, QString> replacementsAffix;
    if (getWidgetValue("compact_pots")) {
        replacementsItems.insert(QMap<QString, QString>{
            { "mp1", QString("%1!%2MP1").arg(s_colorBlue).arg(s_colorLightGray) },
            { "mp2", QString("%1!%2MP2").arg(s_colorBlue).arg(s_colorLightGray) },
            { "mp3", QString("%1!%2MP3").arg(s_colorBlue).arg(s_colorLightGray) },
            { "mp4", QString("%1!%2MP4").arg(s_colorBlue).arg(s_colorLightGray) },
            { "mp5", QString("%1!%2MP5").arg(s_colorBlue).arg(s_colorLightGray) },
            { "hp1", QString("%1!%2HP1").arg(s_colorRed).arg(s_colorLightGray) },
            { "hp2", QString("%1!%2HP2").arg(s_colorRed).arg(s_colorLightGray) },
            { "hp3", QString("%1!%2HP3").arg(s_colorRed).arg(s_colorLightGray) },
            { "hp4", QString("%1!%2HP4").arg(s_colorRed).arg(s_colorLightGray) },
            { "hp5", QString("%1!%2HP5").arg(s_colorRed).arg(s_colorLightGray) },
            { "rvs", QString("%1Rej%2").arg(s_colorPurple).arg(s_colorLightGray) },
            { "rvl", QString("%1Full%2").arg(s_colorPurple).arg(s_colorLightGray) },
        });
    }
    if (getWidgetValue("compact_scrolls")) {
        replacementsItems.insert(QMap<QString, QString>{
            { "tsc", QString("%1•%2TP").arg(s_colorBlue).arg(s_colorLightGray) },
            { "isc", QString("%1•%2ID").arg(s_colorRed).arg(s_colorLightGray) },
        });
    }
    if (getWidgetValue("hide_lowq")) {
        replacementsAffix.insert(QMap<QString, QString>{
            { "Low Quality", s_hidden },
            { "Damaged", s_hidden },
            { "Cracked", s_hidden },
            { "Crude", s_hidden },
        });
    }
    for (const auto& item : m_items) {
        if (getWidgetValue("hide_" + item.settingKey)) {
            for (auto& id : item.internalIds)
                replacementsItems.insert(id, s_hidden);
        }
    }

    replaceJson(replacementsItems, s_itemnamesJson);
    replaceJson(replacementsAffix, s_affixnamesJson);
}

}
