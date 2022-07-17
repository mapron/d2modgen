/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModulePerfectRoll.hpp"

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModulePerfectRoll>();
}

void ModulePerfectRoll::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.getInt("craft")) {
        TableView view(output.tableSet.tables[TableId::cubemain], true);
        for (auto& row : view) {
            const bool isCrafted = row["output"] == "\"usetype,crf\"";
            if (!isCrafted)
                continue;

            for (int i = 1; i <= 5; ++i) {
                auto& mod = row[argCompat("mod %1", i)];
                if (isMinMaxRange(mod.str)) {
                    auto& modMin = row[argCompat("mod %1 min", i)];
                    auto& modMax = row[argCompat("mod %1 max", i)];
                    modMin       = modMax;
                }
            }
        }
    }

    auto makePerfectMin = [](TableView& view, const ColumnsDescList& columnsList, bool perfectMin) {
        if (!perfectMin)
            return;
        view.markModified();
        for (auto& row : view) {
            for (const auto& cols : columnsList) {
                for (const auto& col : cols.m_cols) {
                    auto& min = row[col.min];
                    if (min.isEmpty())
                        continue;
                    if (isMinMaxRange(row[col.code].str))
                        min = row[col.max];
                }
            }
        }
    };
    auto commonLvlReq = [](const TableView::RowView& row) { return row["lvl"].toInt(); };
    auto makeEmpower  = [](TableView& view, const ColumnsDescList& columnsList, int level1, int level50, auto levelCallback) {
        if (level1 == 100 && level50 == 100)
            return;

        const int diff = level50 - level1;

        view.markModified();
        for (auto& row : view) {
            int level = levelCallback(row);
            if (!level)
                continue;

            const int levelDiff = diff * level / 50;
            const int powerMult = level1 + levelDiff;
            if (powerMult == 100)
                continue;

            for (const auto& cols : columnsList) {
                for (const auto& col : cols.m_cols) {
                    auto&       min  = row[col.min];
                    auto&       max  = row[col.max];
                    const auto& code = row[col.code].str;
                    if (min.isEmpty() && max.isEmpty())
                        continue;
                    min.str = powerMultiply(code, min.str, powerMult);
                    max.str = powerMultiply(code, max.str, powerMult);
                }
            }
        }
    };

    {
        TableView view(output.tableSet.tables[TableId::uniqueitems]);
        makePerfectMin(view, Tables::s_descUniques, input.getInt("uniques"));
        makeEmpower(view, Tables::s_descUniques, input.getInt("uniques_power_l1"), input.getInt("uniques_power_l50"), commonLvlReq);
    }
    {
        TableView view(output.tableSet.tables[TableId::runes]);
        const int powerLevel = input.getInt("rw_power");
        makePerfectMin(view, Tables::s_descRunewords, input.getInt("runeWords"));
        makeEmpower(view, Tables::s_descRunewords, powerLevel, powerLevel, [](const TableView::RowView& row) { return 1; });
    }
    {
        TableView view(output.tableSet.tables[TableId::setitems]);
        makePerfectMin(view, Tables::s_descSetItems, input.getInt("setItems"));
        makeEmpower(view, Tables::s_descSetItems, input.getInt("set_power_l1"), input.getInt("set_power_l50"), commonLvlReq);
    }
    {
        for (const auto table : { TableId::magicprefix, TableId::magicsuffix, TableId::automagic }) {
            if (!output.tableSet.tables.contains(table))
                continue;
            TableView view(output.tableSet.tables[table]);
            makePerfectMin(view, Tables::s_descAffix, input.getInt("affixes"));
            makeEmpower(view, Tables::s_descAffix, input.getInt("affix_power_l1"), input.getInt("affix_power_l50"), [](const TableView::RowView& row) {
                return row["spawnable"] == "1" ? row["level"].toInt() : 0;
            });
        }
    }
    {
        TableView view(output.tableSet.tables[TableId::cubemain]);
        const int powerLevel = input.getInt("crafted_power");
        makePerfectMin(view, Tables::s_descCube, input.getInt("crafted"));
        makeEmpower(view, Tables::s_descCube, powerLevel, powerLevel, [](const TableView::RowView& row) { return 1; });
    }
    {
        TableView view(output.tableSet.tables[TableId::gems]);
        const int powerLevel = input.getInt("gem_power");
        makeEmpower(view, Tables::s_descGems, powerLevel, powerLevel, [](const TableView::RowView& row) { return 1; });
    }
    if (input.getInt("armor")) {
        TableView view(output.tableSet.tables[TableId::armor]);
        for (auto& row : view) {
            row["minac"] = row["maxac"];
        }
    }
}

}
