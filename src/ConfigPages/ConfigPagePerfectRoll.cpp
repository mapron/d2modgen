/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPagePerfectRoll.hpp"

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

ConfigPagePerfectRoll::ConfigPagePerfectRoll(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Perfect rolls for crafting in the Cube"), "craft", true, this)
               << new CheckboxWidget(tr("Perfect Uniques"), "uniques", true, this)
               << new CheckboxWidget(tr("Perfect Rune Word rolls"), "runeWords", true, this)
               << new CheckboxWidget(tr("Perfect Set items"), "setItems", true, this)
               << new CheckboxWidget(tr("Perfect Magic/Rare affixes"), "affixes", true, this)
               << new CheckboxWidget(tr("Always max defense Armor"), "armor", true, this));
    closeLayout();
}

QString ConfigPagePerfectRoll::pageHelp() const
{
    return tr("Remove random effect when rolling between min and max value for something.");
}

void ConfigPagePerfectRoll::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (getWidgetValue("craft")) {
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

    if (getWidgetValue("uniques")) {
        auto&     table = output.tableSet.tables["uniqueitems"];
        TableView view(table);
        updateMinParam(view, Tables::s_descUniques);
    }
    if (getWidgetValue("runeWords")) {
        TableView view(output.tableSet.tables["runes"]);
        updateMinParam(view, Tables::s_descRunewords);
    }
    if (getWidgetValue("setItems")) {
        TableView view(output.tableSet.tables["setitems"]);
        updateMinParam(view, Tables::s_descSetItems);
    }
    if (getWidgetValue("affixes")) {
        for (const char* table : { "magicprefix", "magicsuffix", "automagic" }) {
            if (!output.tableSet.tables.contains(table))
                continue;
            TableView view(output.tableSet.tables[table]);
            updateMinParam(view, Tables::s_descAffix);
        }
    }
    if (getWidgetValue("armor")) {
        TableView view(output.tableSet.tables["armor"]);
        for (auto& row : view) {
            row["minac"] = row["maxac"];
        }
    }
}

}
