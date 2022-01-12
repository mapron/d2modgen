/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageQol.hpp"

ConfigPageQol::ConfigPageQol(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Increase tome sizes 20 -> 60", "tomeSize", false, this)
               << new CheckboxWidget("Increase key chain 12 -> 50", "keySize", false, this)
               << new CheckboxWidget("Increase quivers size (250,350) -> 511", "quiverSize", false, this)
               << new CheckboxWidget("Remove limit on Unique charms", "uniqueCharmLimit", false, this)
               << new CheckboxWidget("Allow using Teleport, BC, BO in town", "weakenTownSkills", false, this));
    closeLayout();
}

KeySet ConfigPageQol::generate(TableSet& tableSet, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return {};
    KeySet result;

    const bool tomeSize         = getWidgetValue("tomeSize");
    const bool keySize          = getWidgetValue("keySize");
    const bool quiverSize       = getWidgetValue("quiverSize");
    const bool uniqueCharmLimit = getWidgetValue("uniqueCharmLimit");
    const bool weakenTownSkills = getWidgetValue("weakenTownSkills");
    if (tomeSize || keySize || quiverSize) {
        result << "misc";
        Table&    table = tableSet.tables["misc"];
        TableView tableView(table);
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
        result << "uniqueitems";
        Table&    table = tableSet.tables["uniqueitems"];
        TableView tableView(table);
        for (auto& row : tableView) {
            row["carry1"] = "";
        }
    }
    if (weakenTownSkills) {
        static const QSet<QString> s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        result << "skills";
        Table&    table = tableSet.tables["skills"];
        TableView tableView(table);
        for (auto& row : tableView) {
            if (s_exceptions.contains(row["skill"]))
                row["InTown"] = "1";
        }
    }
    return result;
}
