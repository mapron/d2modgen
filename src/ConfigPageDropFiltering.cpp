/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageDropFiltering.hpp"

#include <QLabel>

DropFiltering::DropFiltering(QWidget* parent)
    : AbstractPage(parent)
    , m_items{
        { { "isc" }, "isc", "ID scroll" },
        { { "tsc" }, "tsc", "TP scroll" },
        { { "hp1", "hp2", "hp3" }, "hps", "Health pots 1-3" },
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa", "All Health pots" },
        { { "mp1", "mp2", "mp3" }, "mps", "Mana pots 1-3" },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa", "All Mana pots" },
        { { "rvs" }, "rvs", "Rejuv pots" },
        { { "rvl" }, "rvl", "Full Rejuv pots" },
        { { "Ammo" }, "ammo", "Bolts/Arrows" },
        { { "vps", "yps", "wms" }, "ammo", "Stamina/Antidote/Thawing" },
        { { "Misc 0", "Misc 1", "Misc 2" }, "junks", "Keys/Fire/Poison pots" },
    }

{
    addWidget(new QLabel("<b>Warning</b>: please note, there is no real way to filter drops in D2R;<br>"
                         "Instead of filtering loot, we just make it to not drop at all instead.<br>"
                         "In practice, it's the same for user, but you have no way to pick an item if you changed you mind.<br>"
                         "Other approach - is to change item tag to something really short - that solution require <br>"
                         "localization edits, which is possible, but not yet supported.<br>"
                         "All options below just <b>replace drops with NoDrop</b>, so no real affect on probabilities of other items."

                         ,
                         this));
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget("Disable " + item.title, "nodrop_" + item.settingKey, false, this));
    closeLayout();
}

KeySet DropFiltering::generate(TableSet& tableSet, QRandomGenerator& rng) const
{
    if (isAllDefault())
        return {};
    KeySet result;
    result << "treasureclassex";

    TableView view(tableSet.tables["treasureclassex"]);

    QSet<QString> disabledIds;
    for (auto& item : m_items) {
        const bool disable = getWidgetValue("nodrop_" + item.settingKey);
        if (disable)
            disabledIds += item.internalIds;
    }
    if (!disabledIds.empty()) {
        for (auto& row : view) {
            DropSet dropSet;
            dropSet.readRow(row);
            for (int i = dropSet.m_items.size() - 1; i >= 0; i--) {
                auto&    dropItem = dropSet.m_items[i];
                QString& tcName   = dropItem.tc;
                if (disabledIds.contains(tcName)) {
                    dropSet.m_noDrop += dropItem.prob;
                    dropSet.m_items.removeAt(i);
                }
            }
            dropSet.writeRow(row);
        }
    }

    return result;
}
