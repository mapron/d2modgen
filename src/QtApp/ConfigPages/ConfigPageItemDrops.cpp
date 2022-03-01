/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageItemDrops.hpp"
#include "AttributeHelper.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageItemDrops>();
}

ConfigPageItemDrops::ConfigPageItemDrops(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << makeEditor("chance_uni", tr("Increase Unique Chance"))
               << makeEditor("chance_set", tr("Increase Set Chance"))
               << makeEditor("chance_rare", tr("Increase Rare Chance"))
               << makeEditor("chance_highlevel",
                             tr("Increase Chance of High-level equipment"),
                             tr("This will reduce the reduce the chance that equipment of lower level will drop as a fallback.\n"
                                "For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on."))
               << makeEditor("high_elite_drops",
                             tr("Increase Champion/Unique item count"),
                             tr("This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.\n"
                                "Note that potion drops are slightly reduced.")));

    addWidget(new QLabel(tr("Adjust proportion of main drop classes"), this));
    addEditors(QList<IValueWidget*>()
               << makeEditor("nodrop_percent",
                             tr("NoDrop (higher=LESS drops)"),
                             tr("NoDrop slider provides ability to gradually reduce NoDrop picks \n"
                                "(it basically similar to increasing players count in the game)"))
               << makeEditor("good_percent",
                             tr("Good TC (Runes/Gems/Jewellery)"),
                             tr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often \n"
                                "compared to equipment (armor/weapons)."))
               << makeEditor("equip_percent",
                             tr("Equipment (armor/weapons)"),
                             tr("Relative chance of equipment dropping compared to other items (or NoDrop)."))
               << makeEditor("gold_percent",
                             tr("Gold (higher=MORE gold)"),
                             tr("Relative chance of gold compared to other items."))
               << makeEditor("junk_percent",
                             tr("Junk (keys/bolts/etc) (higher=MORE junk)"),
                             tr("Relative chance of junk items compared to other items.")));

    addEditors(QList<IValueWidget*>()
               << makeEditor("equal_uniques",
                             tr("Make all Uniques have equal rarity on same base"),
                             tr("Now Uniques with equal item base will have equal chance to drop.\n"
                                "For example Tyrael's and Templar's will have equal chance. (and all rings too)")));
    closeLayout();
}

QString ConfigPageItemDrops::pageHelp() const
{
    return tr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.\n"
              "Basically all options here modifying TC pick weights - so one preferred option become less rare.\n"
              "First 3 sliders are for increasing chance for Uniques/Sets/Rares. \n"
              "Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.");
}

IConfigPage::PresetList ConfigPageItemDrops::pagePresets() const
{
    return {
        tr("I want to find Tyrael's today!"),
        tr("Just tired of finding my first Shako"),
        tr("A little bit more of good stuff, not crazy"),
    };
}

}
