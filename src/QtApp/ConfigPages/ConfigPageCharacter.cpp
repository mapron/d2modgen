/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCharacter.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageCharacter>();
}

ConfigPageCharacter::ConfigPageCharacter(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Add Horadric Cube as starting item"), "addCube", false, this)
               << new CheckboxWidget(tr("Replace ID scroll with ID tome"), "idTome", false, this)
               << new CheckboxWidget(tr("Replace starting Health pots with Mana"), "manaPots", false, this)
               << new SliderWidgetMinMax(tr("Stat points gain per level"), "statPerLevel", 1, 25, 5, this)
               << new SliderWidgetMinMax(tr("Skill points gain per level (D2R only!)"), "skillPerLevel", 1, 5, 1, this)
               << new SliderWidgetMinMax(tr("Lower Strength/Dexterity requirements on items, %"), "statLower", 10, 100, 100, this)
               << new SliderWidget(tr("Change mercenaries Health, multiply by"), "mercHP", 5, 10, this)
               << new SliderWidget(tr("Change mercenaries Damage, multiply by"), "mercDam", 5, 10, this));
    closeLayout();
}

QString ConfigPageCharacter::pageHelp() const
{
    return tr("First three options allow you to change starting items for characters: \n"
              "add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. \n"
              "Next two sliders allow you to change how many skill and stat poits you get on levelup. \n "
              "Last slider allow you to lower Strength and Dexterity requirements on all items.  ");
}

}
