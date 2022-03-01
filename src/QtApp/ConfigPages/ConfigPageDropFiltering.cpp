/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageDropFiltering.hpp"

#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageDropFiltering>();
}

ConfigPageDropFiltering::ConfigPageDropFiltering(QWidget* parent)
    : ConfigPageAbstract(parent)
    , m_items{
        { "isc", tr("ID scroll") },
        { "tsc", tr("TP scroll") },
        { "hps", tr("Health pots 1-3") },
        { "hpsa", tr("All Health pots") },
        { "mps", tr("Mana pots 1-3") },
        { "mpsa", tr("All Mana pots") },
        { "rvs", tr("Rejuv pots") },
        { "rvl", tr("Full Rejuv pots") },
        { "ammo", tr("Bolts/Arrows") },
        { "stam", tr("Stamina/Antidote/Thawing") },
        { "junks", tr("Keys,Fire/Poison pots") },
    }

{
    initModule();
    addWidget(new QLabel(tr("<b>Make item names compact</b>: this will make item names take less space, e.g. '!HP2' for health potion."),
                         this));
    addEditors(QList<IValueWidget*>()
               << makeEditor("compact_pots", tr("Compact potion names"))
               << makeEditor("compact_scrolls", tr("Compact TP/ID scrolls")));

    addWidget(new QLabel(tr("<b>Hide items on the ground</b>: this will make item names transparent; <br>"
                            "you still can pickup them, but their labels will be invisible on Alt press."),
                         this));
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << makeEditor("hide_" + item.settingKey, tr("Hide ") + item.title));

    addEditors(QList<IValueWidget*>()
               << makeEditor("hide_lowq", tr("Hide low quality/damaged/cracked items")));
    closeLayout();
}

QString ConfigPageDropFiltering::pageHelp() const
{
    return tr("This tab consists of two sections:\n"
              "1. Make some items have compact names, like \"!MP5\" for Super Mana Potion. \n"
              "2. Hide item labels on the ground (you still be able to pick them). ");
}

}
