/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModulePerfectRoll.hpp"

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModulePerfectRoll>();
}

PropertyTreeScalarMap ModulePerfectRoll::defaultValuesScalar() const
{
    return {
        { "craft", true },
        { "uniques", true },
        { "runeWords", true },
        { "setItems", true },
        { "affixes", true },
        { "armor", true },
    };
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

    auto updateMinParam = [](TableView& view, const ColumnsDescList& columnsList) {
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

    if (input.getInt("uniques")) {
        auto&     table = output.tableSet.tables[TableId::uniqueitems];
        TableView view(table);
        updateMinParam(view, Tables::s_descUniques);
    }
    if (input.getInt("runeWords")) {
        TableView view(output.tableSet.tables[TableId::runes]);
        updateMinParam(view, Tables::s_descRunewords);
    }
    if (input.getInt("setItems")) {
        TableView view(output.tableSet.tables[TableId::setitems]);
        updateMinParam(view, Tables::s_descSetItems);
    }
    if (input.getInt("affixes")) {
        for (const auto table : { TableId::magicprefix, TableId::magicsuffix, TableId::automagic }) {
            if (!output.tableSet.tables.contains(table))
                continue;
            TableView view(output.tableSet.tables[table]);
            updateMinParam(view, Tables::s_descAffix);
        }
    }
    if (input.getInt("armor")) {
        TableView view(output.tableSet.tables[TableId::armor]);
        for (auto& row : view) {
            row["minac"] = row["maxac"];
        }
    }
}

}
