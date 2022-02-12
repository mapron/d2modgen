/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonStats.hpp"

#include <QLabel>

namespace D2ModGen {

ConfigPageMonStats::ConfigPageMonStats(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidget(tr("Monster Attack Rating, multiply by"), "mon_ar", 10, 10, this)
               << new SliderWidget(tr("Monster Defense, multiply by"), "mon_def", 10, 10, this)
               << new SliderWidget(tr("Monster HP, multiply by"), "mon_hp", 10, 10, this)
               << new SliderWidget(tr("Monster Damage, multiply by"), "mon_dam", 10, 10, this)
               << new SliderWidget(tr("Monster EXP gain, multiply by"), "mon_xp", 10, 10, this)
               << addHelp(new SliderWidgetMinMax(tr("Maximum resistance allowed for base monsters, percent:"), "max_resist", 90, 250, 250, this),
                          tr("If you make this below 100, then regular monsters will have NO IMMUNITIES at all!\n"
                             "Use with caution. You can set this to 110-115, to make cold immunes breakable.\n"
                             "Also this do not prevent Enchanted monsters to have immunities."))
               << addHelp(new SliderWidget(tr("Change monster resistances, by multiplying damage from magic they take"), "rel_resist", 10, 10, this),
                          tr("DO NOT affect IMMUNE monsters and monsters with 0 resistance!\n"
                             "It affects resistance perent indirectly, through this calculation:\n"
                             "Lets say you have 200% selected for multiply, and monster has 80% basic Fire resist.\n"
                             "That means, he has 20% damage taken from fire, and new value will be 40% damage, or 60% resist.\n"
                             "Damage is capped at 100%, so you won't get negative reistance.\n"
                             "Also monster can't become immune to element, maximum resistance is 99%")));
    addWidget(new QLabel(tr("Apply resistance options to following:"), this));
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Elemental (Fire/Cold/Lightning)", "resist_apply_elem", true, this)
               << new CheckboxWidget("Poison", "resist_apply_poison", true, this)
               << new CheckboxWidget("Magic (Non-elemental)", "resist_apply_magic", false, this)
               << new CheckboxWidget("Physical", "resist_apply_physical", false, this));
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

    {
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
        proceedMonParam("mon_xp", { "XP", "XP(N)", "XP(H)", "L-XP", "L-XP(N)", "L-XP(H)" });
    }

    {
        QStringList resColumns;
        if (getWidgetValue("resist_apply_elem"))
            resColumns << QStringList{ "ResFi", "ResLi", "ResCo", "ResFi(N)", "ResLi(N)", "ResCo(N)", "ResFi(H)", "ResLi(H)", "ResCo(H)" };

        if (getWidgetValue("resist_apply_poison"))
            resColumns << QStringList{ "ResPo", "ResPo(N)", "ResPo(H)" };

        if (getWidgetValue("resist_apply_magic"))
            resColumns << QStringList{ "ResMa", "ResMa(N)", "ResMa(H)" };

        if (getWidgetValue("resist_apply_physical"))
            resColumns << QStringList{ "ResDm", "ResDm(N)", "ResDm(H)" };

        TableView view(output.tableSet.tables["monstats"], true);
        const int maxResistCap = getWidgetValue("max_resist");
        const int relResist    = getWidgetValue("rel_resist");
        for (auto& col : resColumns) {
            if (!view.hasColumn(col))
                continue;

            for (auto& row : view) {
                const bool isRegular = row["isSpawn"] == "1" && row["killable"] == "1" && row["npc"] == "" && row["boss"] == "";
                if (!isRegular)
                    continue;
                QString& res    = row[col];
                int      resist = res.toInt();
                resist          = std::min(maxResistCap, resist);
                if (resist < 100 && relResist != 100) {
                    const int damage    = 100 - resist;
                    const int newDamage = damage * relResist / 100;
                    resist              = std::clamp(100 - newDamage, 0, std::min(99, maxResistCap));
                }
                res = resist == 0 ? "" : QString("%1").arg(resist);
            }
        }
    }
}

}
