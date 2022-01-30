/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonStats.hpp"

namespace D2ModGen {

ConfigPageMonStats::ConfigPageMonStats(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidget(tr("Monster Attack Rating, multiply by"), "mon_ar", 10, 10, this)
               << new SliderWidget(tr("Monster Defense, multiply by"), "mon_def", 10, 10, this)
               << new SliderWidget(tr("Monster HP, multiply by"), "mon_hp", 10, 10, this)
               << new SliderWidget(tr("Monster Damage, multiply by"), "mon_dam", 10, 10, this)
               << new SliderWidget(tr("Monster EXP gain, multiply by"), "mon_xp", 10, 10, this));
    closeLayout();
}

QString ConfigPageMonStats::pageHelp() const
{
    return tr("Sliders allow you to change monster stats on all difficulties.\n"
              "You can either lower or raise stats up 10x or 10x times.\n"
              "That is orthogonal to /playersX setting - it will multiply stats independent.");
}

void ConfigPageMonStats::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    TableView view(output.tableSet.tables["monlvl"], true);
    auto      proceedMonParam = [&view, this](QString key, QStringList cols) {
        if (isWidgetValueDefault(key))
            return;
        const int percent = getWidgetValue(key);
        for (auto& row : view) {
            if (row["Level"] == "0")
                continue;
            for (auto& col : cols) {
                if (!row.hasColumn(col))
                    continue;

                QString& value = row[col];
                value          = QString("%1").arg(value.toInt() * percent / 100);
            }
        }
    };

    proceedMonParam("mon_ar", { "TH", "TH(N)", "TH(H)", "L-TH", "L-TH(N)", "L-TH(H)" });
    proceedMonParam("mon_def", { "AC", "AC(N)", "AC(H)", "L-AC", "L-AC(N)", "L-AC(H)" });
    proceedMonParam("mon_hp", { "HP", "HP(N)", "HP(H)", "L-HP", "L-HP(N)", "L-HP(H)" });
    proceedMonParam("mon_dam", { "DM", "DM(N)", "DM(H)", "L-DM", "L-DM(N)", "L-DM(H)" });
}

}
