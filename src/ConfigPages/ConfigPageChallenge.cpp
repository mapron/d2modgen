/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageChallenge.hpp"

namespace D2ModGen {

ConfigPageChallenge::ConfigPageChallenge(QWidget* parent)
    : ConfigPageAbstract(parent)
    , m_items{
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa", "All Health pots" },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa", "All Mana pots" },
        { { "rvs", "rvl" }, "rvs", "Rejuv & Full Rejuv pots" },
    }
{
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget("Disable drop: " + item.title, "nodrop_" + item.settingKey, false, this));

    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax("Normal difficulty resistance penalty, -all%", "normal_minus_res", 0, 250, 0, this)
               << new SliderWidgetMinMax("Nightmare difficulty resistance penalty, -all%", "nightmare_minus_res", 0, 250, 40, this)
               << new SliderWidgetMinMax("Hell difficulty resistance penalty, -all%", "hell_minus_res", 0, 250, 100, this));

    closeLayout();
}

KeySet ConfigPageChallenge::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return {};

    KeySet result;

    {
        TableView view(output.tableSet.tables["treasureclassex"]);

        QSet<QString> disabledIds;
        for (auto& item : m_items) {
            const bool disable = getWidgetValue("nodrop_" + item.settingKey);
            if (disable)
                disabledIds += item.internalIds;
        }
        if (!disabledIds.empty()) {
            result << "treasureclassex";
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
    }
    {
        TableView view(output.tableSet.tables["difficultylevels"]);
        auto      checkPenalty = [this, &result, &view](const QString& key, const QString& name) {
            if (isAllDefault({ key }))
                return;
            const int penalty = getWidgetValue(key);
            result << "difficultylevels";
            for (auto& row : view) {
                if (row["Name"] == name) {
                    row["ResistPenalty"] = QString("-%1").arg(penalty);
                }
            }
        };
        checkPenalty("normal_minus_res", "Normal");
        checkPenalty("nightmare_minus_res", "Nightmare");
        checkPenalty("hell_minus_res", "Hell");
    }

    return result;
}

}
