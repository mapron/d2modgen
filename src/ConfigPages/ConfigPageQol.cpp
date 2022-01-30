/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageQol.hpp"

namespace D2ModGen {

ConfigPageQol::ConfigPageQol(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Increase tome sizes 20 -> 60"), "tomeSize", false, this)
               << new CheckboxWidget(tr("Increase key chain 12 -> 50"), "keySize", false, this)
               << new CheckboxWidget(tr("Increase quivers size (250,350) -> 511"), "quiverSize", false, this)
               << new CheckboxWidget(tr("Remove limit on Unique charms"), "uniqueCharmLimit", false, this)
               << new CheckboxWidget(tr("Allow using Teleport, BC, BO in town"), "weakenTownSkills", false, this)
               << new SliderWidgetMinMax(tr("Reduce costs of skills and stats on items (affects repair cost mostly)"),
                                         "reduceCost",
                                         5,
                                         100,
                                         100,
                                         this));
    closeLayout();
}

QString ConfigPageQol::pageHelp() const
{
    return tr("Tiny quality things to make you life easier: \n"
              "1. Insreasing stacks for tomes/quivers/keys;\n"
              "2. Remove Unique charm limit (useful with randomizer);\n"
              "3. Allow usage of some skills in the town;\n"
              "4. Reduce item costs. ");
}

void ConfigPageQol::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    const bool tomeSize         = getWidgetValue("tomeSize");
    const bool keySize          = getWidgetValue("keySize");
    const bool quiverSize       = getWidgetValue("quiverSize");
    const bool uniqueCharmLimit = getWidgetValue("uniqueCharmLimit");
    const bool weakenTownSkills = getWidgetValue("weakenTownSkills");
    const int  reduceCost       = getWidgetValue("reduceCost");
    if (tomeSize || keySize || quiverSize) {
        Table&    table = output.tableSet.tables["misc"];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            auto& code     = row["code"];
            auto& maxstack = row["maxstack"];
            if (tomeSize && (code == "ibk" || code == "tbk"))
                maxstack = "60";
            if (keySize && code == "key")
                maxstack = "50";
            if (quiverSize && (code == "aqv" || code == "cqv"))
                maxstack = "511";
        }
    }
    if (uniqueCharmLimit) {
        Table&    table = output.tableSet.tables["uniqueitems"];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            row["carry1"] = "";
        }
    }
    if (weakenTownSkills) {
        static const QSet<QString> s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        Table&    table = output.tableSet.tables["skills"];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            if (s_exceptions.contains(row["skill"]))
                row["InTown"] = "1";
        }
    }
    if (reduceCost != 100) {
        {
            Table&    table = output.tableSet.tables["skills"];
            TableView tableView(table, true);
            for (auto& row : tableView) {
                QString& mult = row["cost mult"];
                QString& add  = row["cost add"];
                if (mult.isEmpty())
                    continue;
                mult = QString("%1").arg(mult.toInt() * reduceCost / 100);
                add  = QString("%1").arg(add.toInt() * reduceCost / 100);
            }
        }
        {
            Table&    table = output.tableSet.tables["itemstatcost"];
            TableView tableView(table, true);
            for (auto& row : tableView) {
                QString& mult = row["Multiply"];
                QString& add  = row["Add"];
                if (mult.isEmpty())
                    continue;
                mult = QString("%1").arg(mult.toInt() * reduceCost / 100);
                add  = QString("%1").arg(add.toInt() * reduceCost / 100);
            }
        }
    }
}

}
