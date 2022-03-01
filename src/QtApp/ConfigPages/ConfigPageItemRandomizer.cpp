/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageItemRandomizer.hpp"
#include "RandoUtils.hpp"

#include "HelpToolButton.hpp"

#include <QBoxLayout>
#include <QLabel>

namespace D2ModGen {

namespace {
constexpr const int s_maxUnbalanceLevel = 100;

}

namespace {
const bool s_init = pageRegisterHelper<ConfigPageItemRandomizer>();
}

ConfigPageItemRandomizer::ConfigPageItemRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << makeEditor("crazyLevel",
                             tr("Crazy-ness (or 'NON-balance level', lower = more balance, 100=chaos)"),
                             tr("Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.\n"
                                "With '10' it will select between level-10 and level+10 at first, if there are <50 candidates,\n"
                                "then it will select level-30..level+30, and finally it will try fully random. \n"
                                "In short, lower value = more balance in terms of original affix level and item level."))
               << makeEditor("itemFitPercent",
                             tr("Item type fit percent (0% = fully random, 100% = all according to item type)"),
                             tr("Item fit slider allow you to select how much item affixes will be related to original item type.\n"
                                "For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type\n"
                                "Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.\n"
                                "Item type-specific properties will be picked in proportion to all types."))
               << makeEditor("keepOriginalPercent",
                             tr("How many original properties to keep, percent"),
                             tr("You can select how many properties of original item you want to keep.\n"
                                "If 0, then every item will be fully randomized.\n"
                                "If 50, then half of genereted properties will be original, and half randomized.\n"
                                "If 100, then every property will be property of original item. (you can reduce an amount of props)"))
               << makeEditor("relativeCountMin",
                             tr("Minimum relative property count, compared to the original"),
                             tr("This and the next options determine new property count will be after generation.\n"
                                "If Min=Max=100%%, then property count will be exactly as original (except rare corner cases).\n"
                                "If Min=50%% and Max=200%%, then new property count will be at least half as original, and twice at best.\n"
                                "For example, if item has 5 properties, then worst case is 2, and best case is 9 (because 9 is maximum for Uniques)"))
               << makeEditor("relativeCountMax",
                             tr("Maximum relative property count, compared to the original"))
               << makeEditor("repeat_uniques",
                             tr("Number of versions of each unique"),
                             tr("allow you to have different uniques with same name and level, but different properties, \n"
                                "you will have N different uniques with differnet stats;\n"
                                "so you have an opportunity to pick same item again to check it out.\n"
                                "This works only with Uniques, not Sets.")));

    addEditors(QList<IValueWidget*>()
               << makeEditor("noDuplicates", tr("Prevent duplicate properties on items"))
               << makeEditor("affixRandom",
                             tr("Randomize magix/rare affixes"),
                             tr("This will modify rare and magic suffixes - \n"
                                "so they can include properties of any other item in the game. \n"
                                "Note that their properties are read even without this option."))
               << makeEditor("gemsRandom",
                             tr("Randomize gem and runes properties"),
                             tr("This will modify gem and rune properties - \n"
                                "so they can include properties of any other item in the game. \n"
                                "Note that their properties are read even without this option."))
               << makeEditor("replaceSkills",
                             tr("Replace skills with oskills"))
               << makeEditor("replaceCharges", tr("Replace charges with oskills"))
               << makeEditor("removeKnock", tr("Remove Knockback/Monster flee"))
               << makeEditor("extraKnown", tr("Add extra attributes to randomize (comma-separated)")));
    closeLayout();
}

QString ConfigPageItemRandomizer::pageHelp() const
{
    return tr("What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc, \n"
              "And then reassign properties back, but in random order (also it does not mean every original will be used).\n"
              "For details, check descriptions of every option.");
}

IConfigPage::PresetList ConfigPageItemRandomizer::pagePresets() const
{
    return {
        tr("I want to be overpowered machine!"),
        tr("Want to have some fun without flying to space"),
        tr("Want to have fresh experience but balanced if possible"),
    };
}

}
