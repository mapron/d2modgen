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
               << new SliderWidget("Monster Attack Rating, multiply by", "mon_ar", 10, 10, this)
               << new SliderWidget("Monster Defense, multiply by", "mon_def", 10, 10, this)
               << new SliderWidget("Monster HP, multiply by", "mon_hp", 10, 10, this)
               << new SliderWidget("Monster Damage, multiply by", "mon_dam", 10, 10, this)
               << new SliderWidget("Monster XP, multiply by", "mon_xp", 10, 10, this));
    closeLayout();
}

KeySet ConfigPageMonStats::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto&  tableSet = output.tableSet;
    KeySet result;
    result << "monlvl";
    TableView view(tableSet.tables["monlvl"]);
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

    return result;
}

}