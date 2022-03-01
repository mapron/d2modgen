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

PropertyTreeScalarMap ModulePerfectRoll::defaultValues() const
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

void ModulePerfectRoll::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.getInt("craft")) {
        TableView view(output.tableSet.tables["cubemain"], true);
        for (auto& row : view) {
            const bool isCrafted = row["output"] == "\"usetype,crf\"";
            if (!isCrafted)
                continue;

            for (int i = 1; i <= 5; ++i) {
                QString& mod = row[QString("mod %1").arg(i)];
                if (isMinMaxRange(mod)) {
                    QString& modMin = row[QString("mod %1 min").arg(i)];
                    QString& modMax = row[QString("mod %1 max").arg(i)];
                    modMin          = modMax;
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
                    if (isMinMaxRange(row[col.code]))
                        min = row[col.max];
                }
            }
        }
    };

    if (input.getInt("uniques")) {
        auto&     table = output.tableSet.tables["uniqueitems"];
        TableView view(table);
        updateMinParam(view, Tables::s_descUniques);
    }
    if (input.getInt("runeWords")) {
        TableView view(output.tableSet.tables["runes"]);
        updateMinParam(view, Tables::s_descRunewords);
    }
    if (input.getInt("setItems")) {
        TableView view(output.tableSet.tables["setitems"]);
        updateMinParam(view, Tables::s_descSetItems);
    }
    if (input.getInt("affixes")) {
        for (const char* table : { "magicprefix", "magicsuffix", "automagic" }) {
            if (!output.tableSet.tables.contains(table))
                continue;
            TableView view(output.tableSet.tables[table]);
            updateMinParam(view, Tables::s_descAffix);
        }
    }
    if (input.getInt("armor")) {
        TableView view(output.tableSet.tables["armor"]);
        for (auto& row : view) {
            row["minac"] = row["maxac"];
        }
    }
}

}
