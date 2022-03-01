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
    , m_items{
        { "hpsa", tr("All Health pots") },
        { "mpsa", tr("All Mana pots") },
        { "rvs", tr("Rejuv and Full Rejuv pots") },
    }
{
    initModule();
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget(tr("Disable drop: ") + item.title, "nodrop_" + item.settingKey, false, this));

    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax(tr("Normal difficulty resistance penalty, -all%"), "normal_minus_res", 0, 250, 0, this)
               << new SliderWidgetMinMax(tr("Nightmare difficulty resistance penalty, -all%"), "nightmare_minus_res", 0, 250, 40, this)
               << new SliderWidgetMinMax(tr("Hell difficulty resistance penalty, -all%"), "hell_minus_res", 0, 250, 100, this)
               << new SliderWidgetMinMax(tr("Increase Nightmare area levels, +levels"), "levelIncreaseNightmare", 0, 20, 0, this)
               << new SliderWidgetMinMax(tr("Increase Hell area levels, +levels"), "levelIncreaseHell", 0, 30, 0, this)
               << new CheckboxWidget(tr("Go beyond 85 level for areas\nWarning! It's only briefly tested."), "levelIncreaseUltra", false, this));

    closeLayout();
}

QString ConfigPageChallenge::pageHelp() const
{
    return tr("First, you can disable potion drops entirely (that's not the same as drop filter).\n"
              "Second, you can change resistance penalty for each difficulty from default 0/40/100.\n"
              "And the last, you can adjust level area on all maps; \n"
              "note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).  ");
}


}
