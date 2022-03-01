/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageGambling.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageGambling>();
}

ConfigPageGambling::ConfigPageGambling(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Allow gambling for charms and jewels (Legacy only)\nNote: crashes D2R - so enabled only for D2."), "charmGamble", false, this)
               << new SliderWidgetMinMax(tr("Increase Unique chance, times<br>Note: you still can get a lot of failed uniques for no known reason."), "ratioUnique", 1, 500, 1, this)
               << new SliderWidgetMinMax(tr("Increase Set chance, times"), "ratioSet", 1, 250, 1, this)
               << new SliderWidgetMinMax(tr("Increase Rare chance, times"), "ratioRare", 1, 4, 1, this)
               << new SliderWidgetMinMax(tr("Increase Exceptional upgrade chance, times"), "ratioExc", 1, 4, 1, this)
               << new SliderWidgetMinMax(tr("Increase Elite upgrade chance, times<br>Note: seems like Exc. check is done beforehead, so if it's high enough, elite won't generate."), "ratioElite", 1, 10, 1, this));
    closeLayout();
}

QString ConfigPageGambling::pageHelp() const
{
    return tr("First option enables Charms in Gambling window, but only for D2 Legacy.\n"
              "Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.  ");
}

}
