/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonDensity.hpp"
#include "TableUtils.hpp"

#include <algorithm>

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

void ModuleMonDensity::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    {
        const int groupIncrease = input.getInt("mon_groups");
        if (groupIncrease > 0) {
            TableView view(output.tableSet.tables[TableId::monstats], true);
            for (auto& row : view) {
                if (row["Level"].isEmpty())
                    continue;
                TableCell& valueMin = row["MinGrp"];
                TableCell& valueMax = row["MaxGrp"];
                if (valueMin.isEmpty() || valueMax.isEmpty())
                    continue;
                if (valueMin.str == valueMax.str && valueMax == "1")
                    continue;
                for (TableCell* value : { &valueMin, &valueMax }) {
                    const int prev = value->toInt();
                    const int next = prev + groupIncrease;
                    value->setInt(std::min(next, 99));
                }
            }
        }
    }
    if (!input.isAllDefault({ "density", "packs", "hellPacks" })) {
        TableView view(output.tableSet.tables[TableId::levels], true);
        auto      isEmptyCell = [](const TableCell& value) {
            return value.isEmpty() || value == "0";
        };
        auto allCellsNonEmpty = [&isEmptyCell](const std::vector<TableCell>& values) -> bool {
            for (const TableCell& value : values)
                if (isEmptyCell(value))
                    return false;
            return true;
        };
        const int density       = input.getInt("density");
        auto      densityAdjust = [density](TableCell& value) {
            const int prev = value.toInt();
            const int next = prev * density;
            value.setInt(std::clamp(next, 100, 9900));
        };
        const int packs       = input.getInt("packs");
        auto      packsAdjust = [packs](TableCell& value) {
            const int prev = value.toInt();
            const int next = prev * packs;
            value.setInt(std::min(next, 255));
        };
        for (auto& row : view) {
            auto& normMin = row["MonUMin"];
            auto& normMax = row["MonUMax"];
            auto& nighMin = row["MonUMin(N)"];
            auto& nighMax = row["MonUMax(N)"];
            auto& hellMin = row["MonUMin(H)"];
            auto& hellMax = row["MonUMax(H)"];

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
                TableCell& normDen = row["MonDen"];
                TableCell& nighDen = row["MonDen(N)"];
                TableCell& hellDen = row["MonDen(H)"];
                densityAdjust(normDen);
                densityAdjust(nighDen);
                densityAdjust(hellDen);
            }
        }
    }
}

}
