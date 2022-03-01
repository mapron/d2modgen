/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonDensity.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageMonDensity>();
}

ConfigPageMonDensity::ConfigPageMonDensity(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << makeEditor("density", tr("Increase density, times"))
               << makeEditor("packs", tr("Increase Boss packs count, times"))
               << makeEditor("mon_groups", tr("Increase monster groups population, +count<br>"
                                              "<b>Beware! This setting have the most impact on getting screen laggy!<b><br>"
                                              "<b>Having value above +3 will work well only if previous options are not maxed!</b>"))
               << makeEditor("hellPacks", tr("Use Hell elite pack count on Nightmare and Normal")));
    closeLayout();
}

QString ConfigPageMonDensity::pageHelp() const
{
    return tr("Density slider - that will affect increase of density of regular monsters \n"
              "(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)\n"
              "Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.\n"
              "Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.");
}

}
