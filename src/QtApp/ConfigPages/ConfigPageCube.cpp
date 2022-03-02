/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCube.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageCube>();
}

ConfigPageCube::ConfigPageCube(const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(module, parent)
{
    addEditors(makeEditors({
        "noGemUpgrade",
        "quickPortals",
        "quickQuests",
        "socketing",
        "upgrading",
        "runeDowngrade",
    }));
    addWidget(new QLabel(tr("Simplyfy crafting:"), this));
    addEditors(makeEditors({
        "craftNoStrict",
        "craftNoRunes",

        "craftHighIlvl",
        "craftMultBonus",
    }));
    closeLayout();
}

QString ConfigPageCube::pageHelp() const
{
    return tr("Create some recipes to cheese/cut corners around the game. \n"
              "Input and results are self-explanatory.  ");
}

QMap<std::string, QString> ConfigPageCube::widgetTitles() const
{
    return {
        { "noGemUpgrade", tr("Remove gem component from Rune upgrade recipes (Rune x3,x2 => next Rune)") },
        { "quickPortals", tr("Add quick portal access recipes:") + "\n"
                                                                   "1. TP book + Id scroll = Cow Portal\n"
                                                                   "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                                                   "3. TP book + Id scroll x3 = Uber Tristram Portal" },
        { "quickQuests", tr("Add quick quest recipes:") + " "
                                                          "1. key + Id scroll = Horadric Staff "
                                                          "2. key + TP scroll = Khalim Will" },
        { "socketing", tr("Add socketing recipes:") + "\n"
                                                      "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                                      "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                                      "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                                      "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                                      "5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket\n"
                                                      "6. Socketed item + TP scroll x1 = Clear sockets" },
        { "upgrading", tr("Add upgrade recipes:") + "\n"
                                                    "1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item\n"
                                                    "2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item\n"
                                                    "3. Any item + Stamina x2 = Add Ethereal\n"
                                                    "4. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type" },
        { "runeDowngrade", tr("Add Rune downgrade:") + " Rune + key + key = Lower rune" },

        { "craftNoStrict", tr("Remove strict item types for Crafted (any helm, any gloves etc)") },
        { "craftNoRunes", tr("Remove Rune/Jewel requirement for Crafted\n"
                             "(So any recipe is 'item + gem')") },
        { "craftHighIlvl", tr("Make Item Level always 99") },
        { "craftMultBonus", tr("Multiply crafted item bonuses by") },
    };
}

}
