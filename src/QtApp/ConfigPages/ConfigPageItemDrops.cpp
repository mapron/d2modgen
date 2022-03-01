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
               << new SliderWidgetMinMax(tr("Increase Unique Chance"), "chance_uni", 1, 50, 1, this)
               << new SliderWidgetMinMax(tr("Increase Set Chance"), "chance_set", 1, 30, 1, this)
               << new SliderWidgetMinMax(tr("Increase Rare Chance"), "chance_rare", 1, 15, 1, this)
               << addHelp(new SliderWidgetMinMax(tr("Increase Chance of High-level equipment"), "chance_highlevel", 1, 15, 1, this),
                          tr("This will reduce the reduce the chance that equipment of lower level will drop as a fallback.\n"
                             "For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on."))
               << addHelp(new CheckboxWidget(tr("Increase Champion/Unique item count"), "high_elite_drops", false, this),
                          tr("This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.\n"
                             "Note that potion drops are slightly reduced.")));

    addWidget(new QLabel(tr("Adjust proportion of main drop classes"), this));
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidget(tr("NoDrop (higher=LESS drops)"), "nodrop_percent", 20, 5, 100, true, this),
                          tr("NoDrop slider provides ability to gradually reduce NoDrop picks \n"
                             "(it basically similar to increasing players count in the game)"))
               << addHelp(new SliderWidget(tr("Good TC (Runes/Gems/Jewellery)"), "good_percent", 2, 10, 100, true, this),
                          tr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often \n"
                             "compared to equipment (armor/weapons)."))
               << addHelp(new SliderWidget(tr("Equipment (armor/weapons)"), "equip_percent", 5, 10, 100, true, this),
                          tr("Relative chance of equipment dropping compared to other items (or NoDrop)."))
               << addHelp(new SliderWidget(tr("Gold (higher=MORE gold)"), "gold_percent", 20, 10, 100, true, this),
                          tr("Relative chance of gold compared to other items."))
               << addHelp(new SliderWidget(tr("Junk (keys/bolts/etc) (higher=MORE junk)"), "junk_percent", 20, 10, 100, true, this),
                          tr("Relative chance of junk items compared to other items.")));

    addEditors(QList<IValueWidget*>()
               << addHelp(new CheckboxWidget(tr("Make all Uniques have equal rarity on same base"), "equal_uniques", false, this),
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
