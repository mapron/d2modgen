/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonDensity.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleMonDensity>();
}

PropertyTreeScalarMap ModuleMonDensity::defaultValues() const
{
    return {
        { "density", 1 },
        { "packs", 1 },
        { "mon_groups", 0 },
        { "hellPacks", false },
    };
}

IModule::UiControlHintMap ModuleMonDensity::uiHints() const
{
    return {
        { "density", UiControlHintSliderMinMax(1, 20) },
        { "packs", UiControlHintSliderMinMax(1, 20) },
        { "mon_groups", UiControlHintSliderMinMax(0, 20) },
    };
}

void ModuleMonDensity::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    {
        const int groupIncrease = input.getInt("mon_groups");
        if (groupIncrease > 0) {
            TableView view(output.tableSet.tables["monstats"], true);
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
    if (!input.isAllDefault({ "density", "packs", "hellPacks" })) {
        TableView view(output.tableSet.tables["levels"], true);
        auto      isEmptyCell = [](const QString& value) {
            return value.isEmpty() || value == "0";
        };
        auto allCellsNonEmpty = [&isEmptyCell](const QStringList& values) -> bool {
            for (const QString& value : values)
                if (isEmptyCell(value))
                    return false;
            return true;
        };
        const int density       = input.getInt("density");
        auto      densityAdjust = [density](QString& value) {
            const int prev = value.toInt();
            const int next = prev * density;
            value          = QString("%1").arg(std::clamp(next, 100, 9900));
        };
        const int packs       = input.getInt("packs");
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
                if (input.getInt("hellPacks")) {
                    normMin = nighMin = hellMin;
                    normMax = nighMax = hellMax;
                }
            }

            if (allCellsNonEmpty({ normMin, normMax, nighMin, nighMax, hellMin, hellMax })) {
                if (!input.isDefault("packs")) {
                    packsAdjust(normMin);
                    packsAdjust(normMax);
                    packsAdjust(nighMin);
                    packsAdjust(nighMax);
                    packsAdjust(hellMin);
                    packsAdjust(hellMax);
                }
            }
            if (!input.isDefault("density")) {
                QString& normDen = row["MonDen"];
                QString& nighDen = row["MonDen(N)"];
                QString& hellDen = row["MonDen(H)"];
                densityAdjust(normDen);
                densityAdjust(nighDen);
                densityAdjust(hellDen);
            }
        }
    }
}

}
