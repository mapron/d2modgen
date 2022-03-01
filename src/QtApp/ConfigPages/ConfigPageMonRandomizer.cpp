/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonRandomizer.hpp"

#include <QJsonDocument>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageMonRandomizer>();
}

ConfigPageMonRandomizer::ConfigPageMonRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidgetMinMax(tr("Spawned different types in one area"), "spawnedCount", 2, 10, 5, this),
                          tr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).  \n"
                             "Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized."))
               << addHelp(new SliderWidgetMinMax(tr("Maximum possible monster types in one area (population variety)"), "maxTypes", 3, 25, 25, this),
                          tr("This option defines how many possibilites for monster types are exist in one zone; \n"
                             "Make this value lower if you want less variety between game restarts.\n"
                             "Note: D2 legacy support only 12 max, so value will be lowered implicitly.")));
    closeLayout();
}

QString ConfigPageMonRandomizer::pageHelp() const
{
    return tr("When this feature enabled, now any area in the game can spawn any basic monsters (Super Uniques, Act bosses etc not affected). \n"
              "For normal difficulty, each monster will have a \"level-adjusted\" copy which corresponds \n"
              "to area's level (so you will have adequate monster stats in Act 1 for guest from Act 5).  \n"
              "For Nightmare/Hell, adjustment is done by area level.  \n"
              "Monster will have adjusted their minion spawns and skill levels, too. ");
}

}
