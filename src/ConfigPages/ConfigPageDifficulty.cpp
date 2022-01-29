/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageDifficulty.hpp"

namespace D2ModGen {

ConfigPageDifficulty::ConfigPageDifficulty(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidget("Monster Attack Rating, multiply by", "mon_ar", 10, 10, this)
               << new SliderWidget("Monster Defense, multiply by", "mon_def", 10, 10, this)
               << new SliderWidget("Monster HP, multiply by", "mon_hp", 10, 10, this)
               << new SliderWidget("Monster Damage, multiply by", "mon_dam", 10, 10, this)
               << new SliderWidget("Monster XP, multiply by", "mon_xp", 10, 10, this)
               << new SliderWidgetMinMax("Increase density, times", "density", 1, 20, 1, this)
               << new SliderWidgetMinMax("Increase Boss packs count, times", "packs", 1, 20, 1, this)
               << new SliderWidgetMinMax("Increase monster groups population, +count<br>"
                                         "<b>Beware! This setting have the most impact on getting screen laggy!<b><br>"
                                         "<b>Having value above +3 will work well only if previous options are not maxed!</b>",
                                         "mon_groups",
                                         0,
                                         20,
                                         0,
                                         this)
               << new CheckboxWidget("Use Hell pack count everywhere", "hellPacks", false, this)
               << new SliderWidgetMinMax("Increase area level (Nightmare and Hell), +levels", "levelIncrease", 0, 20, 0, this));
    closeLayout();
}

KeySet ConfigPageDifficulty::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto&  tableSet = output.tableSet;
    KeySet result;
    if (!isAllDefault({ "mon_ar", "mon_def", "mon_hp", "mon_dam", "mon_xp" })) {
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
        proceedMonParam("mon_xp", { "XP", "XP(N)", "XP(H)", "L-XP", "L-XP(N)", "L-XP(H)" });
    }
    {
        const int groupIncrease = getWidgetValue("mon_groups");
        TableView view(tableSet.tables["monstats"]);
        if (groupIncrease > 0) {
            result << "monstats";
            for (auto& row : view) {
                if (row["Level"].isEmpty())
                    continue;
                QString& valueMin = row["MinGrp"];
                QString& valueMax = row["MaxGrp"];
                if (valueMin.isEmpty() || valueMax.isEmpty())
                    continue;
                if (valueMin == valueMax && valueMax == "1")
                    continue;
                for (QString* value : { &valueMin, &valueMax }) {
                    const int prev = value->toInt();
                    const int next = prev + groupIncrease;
                    *value         = QString("%1").arg(std::min(next, 99));
                }
            }
        }
    }
    if (!isAllDefault({ "density", "packs", "hellPacks", "levelIncrease" })) {
        result << "levels";
        TableView view(tableSet.tables["levels"]);
        auto      isEmptyCell = [](const QString& value) {
            return value.isEmpty() || value == "0";
        };
        auto allCellsNonEmpty = [&isEmptyCell](const QStringList& values) -> bool {
            for (const QString& value : values)
                if (isEmptyCell(value))
                    return false;
            return true;
        };
        const int density       = getWidgetValue("density");
        const int levelIncrease = getWidgetValue("levelIncrease");
        auto      densityAdjust = [density](QString& value) {
            const int prev = value.toInt();
            const int next = prev * density;
            value          = QString("%1").arg(std::clamp(next, 100, 9900));
        };
        const int packs       = getWidgetValue("packs");
        auto      packsAdjust = [packs](QString& value) {
            const int prev = value.toInt();
            const int next = prev * packs;
            value          = QString("%1").arg(std::min(next, 255));
        };
        const QString nighLevelKey = view.hasColumn("MonLvlEx(N)") ? "MonLvlEx(N)" : "MonLvl2Ex";
        const QString hellLevelKey = view.hasColumn("MonLvlEx(H)") ? "MonLvlEx(H)" : "MonLvl3Ex";
        for (auto& row : view) {
            QString& normMin   = row["MonUMin"];
            QString& normMax   = row["MonUMax"];
            QString& nighMin   = row["MonUMin(N)"];
            QString& nighMax   = row["MonUMax(N)"];
            QString& hellMin   = row["MonUMin(H)"];
            QString& hellMax   = row["MonUMax(H)"];
            QString& nighLevel = row[nighLevelKey];
            QString& hellLevel = row[hellLevelKey];

            if (allCellsNonEmpty({ hellMin, hellMax })) {
                if (getWidgetValue("hellPacks")) {
                    normMin = nighMin = hellMin;
                    normMax = nighMax = hellMax;
                }
            }

            if (allCellsNonEmpty({ normMin, normMax, nighMin, nighMax, hellMin, hellMax })) {
                if (!isWidgetValueDefault("packs")) {
                    packsAdjust(normMin);
                    packsAdjust(normMax);
                    packsAdjust(nighMin);
                    packsAdjust(nighMax);
                    packsAdjust(hellMin);
                    packsAdjust(hellMax);
                }
            }
            if (!isWidgetValueDefault("density")) {
                QString& normDen = row["MonDen"];
                QString& nighDen = row["MonDen(N)"];
                QString& hellDen = row["MonDen(H)"];
                densityAdjust(normDen);
                densityAdjust(nighDen);
                densityAdjust(hellDen);
            }
            if (levelIncrease) {
                for (QString* lev : { &nighLevel, &hellLevel }) {
                    int level = lev->toInt();
                    if (!level || level > 85)
                        continue;
                    *lev = QString("%1").arg(std::min(85, level + levelIncrease));
                }
            }
        }
    }
    return result;
}

}
