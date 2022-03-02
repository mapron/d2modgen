/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageChallenge.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageChallenge>();
}

ConfigPageChallenge::ConfigPageChallenge(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(makeEditors({
        "nodrop_hpsa",
        "nodrop_mpsa",
        "nodrop_rvs",

        "normal_minus_res",
        "nightmare_minus_res",
        "hell_minus_res",
        "levelIncreaseNightmare",
        "levelIncreaseHell",
        "levelIncreaseUltra",
    }));

    closeLayout();
}

QString ConfigPageChallenge::pageHelp() const
{
    return tr("First, you can disable potion drops entirely (that's not the same as drop filter).\n"
              "Second, you can change resistance penalty for each difficulty from default 0/40/100.\n"
              "And the last, you can adjust level area on all maps; \n"
              "note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).  ");
}

QMap<std::string, QString> ConfigPageChallenge::widgetTitles() const
{
    QMap<std::string, QString> result;
    QString                    prefix = tr("Disable drop: ");
    result.insert(QMap<std::string, QString>{
        { "nodrop_hpsa", prefix + tr("All Health pots") },
        { "nodrop_mpsa", prefix + tr("All Mana pots") },
        { "nodrop_rvs", prefix + tr("Rejuv and Full Rejuv pots") },
        { "normal_minus_res", tr("Normal difficulty resistance penalty, -all%") },
        { "nightmare_minus_res", tr("Nightmare difficulty resistance penalty, -all%") },
        { "hell_minus_res", tr("Hell difficulty resistance penalty, -all%") },
        { "levelIncreaseNightmare", tr("Increase Nightmare area levels, +levels") },
        { "levelIncreaseHell", tr("Increase Hell area levels, +levels") },
        { "levelIncreaseUltra", tr("Go beyond 85 level for areas\nWarning! It's only briefly tested.") },
    });
    return result;
}

}
