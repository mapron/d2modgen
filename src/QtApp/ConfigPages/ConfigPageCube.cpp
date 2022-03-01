/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCube.hpp"

#include "AttributeHelper.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageCube>();
}

ConfigPageCube::ConfigPageCube(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Remove gem component from Rune upgrade recipes (Rune x3,x2 => next Rune)"), "noGemUpgrade", false, this)
               << new CheckboxWidget(tr("Add quick portal access recipes:") + "\n"
                                                                              "1. TP book + Id scroll = Cow Portal\n"
                                                                              "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                                                              "3. TP book + Id scroll x3 = Uber Tristram Portal",
                                     "quickPortals",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add quick quest recipes:") + " "
                                                                      "1. key + Id scroll = Horadric Staff "
                                                                      "2. key + TP scroll = Khalim Will",
                                     "quickQuests",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add socketing recipes:") + "\n"
                                                                    "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                                                    "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                                                    "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                                                    "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                                                    "5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket\n"
                                                                    "6. Socketed item + TP scroll x1 = Clear sockets",
                                     "socketing",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add upgrade recipes:") + "\n"
                                                                  "1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item\n"
                                                                  "2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item\n"
                                                                  "3. Any item + Stamina x2 = Add Ethereal\n"
                                                                  "4. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type",
                                     "upgrading",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add Rune downgrade:") + " Rune + key + key = Lower rune",
                                     "runeDowngrade",
                                     false,
                                     this));
    addWidget(new QLabel(tr("Simplyfy crafting:"), this));
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Remove strict item types for Crafted (any helm, any gloves etc)"), "craftNoStrict", false, this)
               << new CheckboxWidget(tr("Remove Rune/Jewel requirement for Crafted\n"
                                        "(So any recipe is 'item + gem')"),
                                     "craftNoRunes",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Make Item Level always 99"), "craftHighIlvl", false, this)
               << new SliderWidgetMinMax(tr("Multiply crafted item bonuses by"), "craftMultBonus", 1, 5, 1, true, this));
    closeLayout();
}

QString ConfigPageCube::pageHelp() const
{
    return tr("Create some recipes to cheese/cut corners around the game. \n"
              "Input and results are self-explanatory.  ");
}

}
