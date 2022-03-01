/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageRuneDrops.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageRuneDrops>();
}

ConfigPageRuneDrops::ConfigPageRuneDrops(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidgetMinMax(tr("Increase Rare Rune drops"), "zod_factor", 1, 1000, 1, this),
                          tr("That will drastically improve High Rune drop rates! You probably don't need more than 50x.\n"
                             "Value here increases chance of dropping Zod in 'Runes 17' TC\n"
                             "Rarity of other runes will change proportionally \n"
                             "(so High Runes still be more rare in the same manner)."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Runes chance in Good TC"), "rune_factor", 1, 10, 1, this),
                          tr("When dropping from Good TC, that will make Runes drop more often \n"
                             "compared to other jewellery (Gems/Rings/Amulets/Charms)"))
               << addHelp(new CheckboxWidget(tr("Switch (Ber,Jah) with (Cham,Zod) in rarity"), "highrune_switch", false, this),
                          tr("When game desides to drop Jah rune, it drops Zod instead. And vice versa."))
               << new CheckboxWidget(tr("Make Countess Drop higher runes (up to Runes 17=Zod on Hell)"), "countess_rune_higher", false, this)
               << new CheckboxWidget(tr("Increase Countess Runes drop count to 5"), "countess_rune_more", false, this)
               << new CheckboxWidget(tr("Increase Wraiths Runes drop"), "wraith_runes", false, this));
    closeLayout();
}

QString ConfigPageRuneDrops::pageHelp() const
{
    return tr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.\n"
              "Basically all options here modifying TC pick weights - so one preferred option become less rare.");
}

IConfigPage::PresetList ConfigPageRuneDrops::pagePresets() const
{
    return {
        tr("Gimme pile of Ber runes!"),
        tr("I want to make Infinity at least once in my life"),
        tr("I want high runes to be rare, but want more rune drops overall"),
    };
}

}
