/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonDensity.hpp"

namespace D2ModGen {

ConfigPageMonDensity::ConfigPageMonDensity(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax(tr("Increase density, times"), "density", 1, 20, 1, this)
               << new SliderWidgetMinMax(tr("Increase Boss packs count, times"), "packs", 1, 20, 1, this)
               << new SliderWidgetMinMax(tr("Increase monster groups population, +count<br>"
                                            "<b>Beware! This setting have the most impact on getting screen laggy!<b><br>"
                                            "<b>Having value above +3 will work well only if previous options are not maxed!</b>"),
                                         "mon_groups",
                                         0,
                                         20,
                                         0,
                                         this)
               << new CheckboxWidget(tr("Use Hell elite pack count on Nightmare and Normal"), "hellPacks", false, this));
    closeLayout();
}

QString ConfigPageMonDensity::pageHelp() const
{
    return tr("Density slider - that will affect increase of density of regular monsters \n"
              "(note that it is higly dependant on map and tiles; max density can be reached on open area tiles.)\n"
              "Boss packs slider icrease number of Champion/Uniques in zones; again, that also not a precise setting.\n"
              "Last Checkbox can be used to quickly setup boss count on Normal and Nightmare exactly equal to Hell count.");
}

KeySet ConfigPageMonDensity::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto&  tableSet = output.tableSet;
    KeySet result;
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
    if (!isAllDefault({ "density", "packs", "hellPacks" })) {
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
        for (auto& row : view) {
            QString& normMin = row["MonUMin"];
            QString& normMax = row["MonUMax"];
            QString& nighMin = row["MonUMin(N)"];
            QString& nighMax = row["MonUMax(N)"];
            QString& hellMin = row["MonUMin(H)"];
            QString& hellMax = row["MonUMax(H)"];

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
        }
    }
    return result;
}

}
