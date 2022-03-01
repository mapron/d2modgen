/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageSkillRandomizer.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageSkillRandomizer>();
}

ConfigPageSkillRandomizer::ConfigPageSkillRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << addHelp(new CheckboxWidget(tr("Randomize skill tabs within each character"), "skillTree", true, this),
                          tr("That DOES not move skills between characters! What it can do:\n"
                             "-Move a skill to a different skill tab;\n"
                             "-Move a skill to a higher/lower level;\n"
                             "-Change skill requirements. \n"
                             "So, after level 50 there is not much of a difference. Synergies are also untouched."))
               << addHelp(new CheckboxWidget(tr("Randomize element types for some damage skills"), "skillDamage", true, this),
                          tr("For some skills, where it is easy to change damage type, it will randomize between 5 different elements:\n"
                             "Cold, Fire, Lightning, Magic and Poison\n"
                             "WARNING! Skill descriptions won't be updated and will be completely wrong!\n"
                             "Also, character info screen does not show difference between cold and magic correctly.\n"
                             "You need to find by yourself what damage (magic or cold) was rolled.\n"
                             "For poison, all damage is dealt over 2 seconds and slightly more than original."))
               << addHelp(new CheckboxWidget(tr("Ensure that element types are different from original"), "ensureDifferent", true, this),
                          tr("If the source element is Cold, then new randomized element never should be Cold.\n"
                             "If this turned off, ~20% of skills will have same behaviour."))

    );
    closeLayout();
}

QString ConfigPageSkillRandomizer::pageHelp() const
{
    return tr("This allow to bring some randomization into character skills.");
}

}
