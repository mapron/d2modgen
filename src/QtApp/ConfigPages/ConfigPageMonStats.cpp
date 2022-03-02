/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonStats.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageMonStats>();
}

ConfigPageMonStats::ConfigPageMonStats(const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(module, parent)
{
    addEditors(makeEditors({
        "mon_ar",
        "mon_def",
        "mon_hp",
        "mon_dam",
        "mon_xp",
        "max_resist",
        "rel_resist",
    }));
    addWidget(new QLabel(tr("Apply resistance options to following:"), this));
    addEditors(makeEditors({
        "resist_apply_elem",
        "resist_apply_poison",
        "resist_apply_magic",
        "resist_apply_physical",
    }));
    closeLayout();
}

QString ConfigPageMonStats::pageHelp() const
{
    return tr("Sliders allow you to change monster stats on all difficulties.\n"
              "You can either lower or raise stats up 10x or 10x times.\n"
              "That is orthogonal to /playersX setting - it will multiply stats independent.");
}

QMap<std::string, QString> ConfigPageMonStats::widgetTitles() const
{
    return {
        { "mon_ar", tr("Monster Attack Rating, multiply by") },
        { "mon_def", tr("Monster Defense, multiply by") },
        { "mon_hp", tr("Monster HP, multiply by") },
        { "mon_dam", tr("Monster Damage, multiply by") },
        { "mon_xp", tr("Monster EXP gain, multiply by") },
        { "max_resist", tr("Maximum resistance allowed for base monsters, percent:") },
        { "rel_resist", tr("Change monster resistances, by multiplying damage from magic they take") },
        { "resist_apply_elem", tr("Elemental (Fire/Cold/Lightning)") },
        { "resist_apply_poison", tr("Poison") },
        { "resist_apply_magic", tr("Magic (Non-elemental)") },
        { "resist_apply_physical", tr("Physical") },
    };
}

QMap<std::string, QString> ConfigPageMonStats::widgetHelps() const
{
    return {
        { "max_resist", tr("If you make this below 100, then regular monsters will have NO IMMUNITIES at all!\n"
                           "Use with caution. You can set this to 110-115, to make cold immunes breakable.\n"
                           "Also this do not prevent Enchanted monsters to have immunities.") },
        { "rel_resist", tr("DO NOT affect IMMUNE monsters and monsters with 0 resistance!\n"
                           "It affects resistance perent indirectly, through this calculation:\n"
                           "Lets say you have 200% selected for multiply, and monster has 80% basic Fire resist.\n"
                           "That means, he has 20% damage taken from fire, and new value will be 40% damage, or 60% resist.\n"
                           "Damage is capped at 100%, so you won't get negative reistance.\n"
                           "Also monster can't become immune to element, maximum resistance is 99%") },
    };
}

}
