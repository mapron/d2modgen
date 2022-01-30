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
        { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa", tr("All Health pots") },
        { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa", tr("All Mana pots") },
        { { "rvs", "rvl" }, "rvs", tr("Rejuv and Full Rejuv pots") },
    }
{
    for (auto& item : m_items)
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget(tr("Disable drop: ") + item.title, "nodrop_" + item.settingKey, false, this));

    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax(tr("Normal difficulty resistance penalty, -all%"), "normal_minus_res", 0, 250, 0, this)
               << new SliderWidgetMinMax(tr("Nightmare difficulty resistance penalty, -all%"), "nightmare_minus_res", 0, 250, 40, this)
               << new SliderWidgetMinMax(tr("Hell difficulty resistance penalty, -all%"), "hell_minus_res", 0, 250, 100, this)
               << new SliderWidgetMinMax(tr("Increase Nightmare area levels, +levels"), "levelIncreaseNightmare", 0, 20, 0, this)
               << new SliderWidgetMinMax(tr("Increase Hell area levels, +levels"), "levelIncreaseHell", 0, 20, 0, this));

    closeLayout();
}

QString ConfigPageChallenge::pageHelp() const
{
    return tr("First, you can disable potion drops entirely (that's not the same as drop filter).\n"
              "Second, you can change resistance penalty for each difficulty from default 0/40/100.\n"
              "And the last, you can adjust level area on all maps; \n"
              "note that max value of 85 is still used (so set to 20 to basically make all Hell areas lvl 85).  ");
}

void ConfigPageChallenge::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    {
        TableView view(output.tableSet.tables["treasureclassex"]);

        QSet<QString> disabledIds;
        for (auto& item : m_items) {
            const bool disable = getWidgetValue("nodrop_" + item.settingKey);
            if (disable)
                disabledIds += item.internalIds;
        }
        if (!disabledIds.empty()) {
            view.markModified();
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
        auto      checkPenalty = [this, &output, &view](const QString& key, const QString& name) {
            if (isAllDefault({ key }))
                return;
            const int penalty = getWidgetValue(key);
            view.markModified();
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

    const int levelIncreaseNightmare = getWidgetValue("levelIncreaseNightmare");
    const int levelIncreaseHell      = getWidgetValue("levelIncreaseHell");

    if (levelIncreaseNightmare || levelIncreaseHell) {
        TableView view(output.tableSet.tables["levels"], true);

        const QString nighLevelKey = view.hasColumn("MonLvlEx(N)") ? "MonLvlEx(N)" : "MonLvl2Ex";
        const QString hellLevelKey = view.hasColumn("MonLvlEx(H)") ? "MonLvlEx(H)" : "MonLvl3Ex";

        auto adjustLevel = [&nighLevelKey, &hellLevelKey](TableView::RowView& row, const QString& key, const int levelIncrease) {
            QString& lev   = row[key];
            int      level = lev.toInt();
            if (!level || level > 85)
                return;
            lev = QString("%1").arg(std::min(85, level + levelIncrease));
        };

        for (auto& row : view) {
            adjustLevel(row, nighLevelKey, levelIncreaseNightmare);
            adjustLevel(row, hellLevelKey, levelIncreaseHell);
        }
    }
}

}
