/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCube.hpp"

CubePage::CubePage(QWidget* parent)
    : AbstractPage(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Remove gems from Rune upgrades", "noGemUpgrade", false, this)
               << new CheckboxWidget("Add quick portal access recipes:\n"
                                     "1. TP book + Id scroll = Cow Portal\n"
                                     "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                     "3. TP book + Id scroll x3 = Uber Tristram Portal\n",
                                     "quickPortals",
                                     false,
                                     this)
               << new CheckboxWidget("Add socketing recipes:\n"
                                     "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                     "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                     "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                     "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                     "5. Unique item + TP scroll x1 + Id scroll x1 = Add 1 socket\n",
                                     "socketing",
                                     false,
                                     this));
    closeLayout();
}

KeySet CubePage::generate(TableSet& tableSet, QRandomGenerator& rng) const
{
    if (isAllDefault({ "noGemUpgrade", "quickPortals", "socketing" }))
        return {};
    KeySet result;
    result << "cubemain";

    const bool noGemUpgrade = getWidgetValue("noGemUpgrade");
    const bool quickPortals = getWidgetValue("quickPortals");
    const bool newSocketing = getWidgetValue("socketing");

    TableView view(tableSet.tables["cubemain"]);
    for (auto& row : view) {
        QString& description = row["description"];
        QString& numinputs   = row["numinputs"];
        QString& input1      = row["input 1"];
        QString& input2      = row["input 2"];
        QString& input3      = row["input 3"];

        if (noGemUpgrade && description.contains("->") && description.endsWith(" Rune") && !input2.isEmpty()) {
            numinputs = QString("%1").arg(numinputs.toInt() - 1);
            input2    = "";
        }
    }
    if (quickPortals) {
        using StringMap = QMap<QString, QString>;
        const StringMap base{
            { "description", "Quick Portal" },
            { "enabled", "1" },
            { "version", "0" },
            { "op", "28" },
            { "*eol", "0" },
            { "input 1", "tbk" },

        };
        StringMap cows    = base;
        cows["numinputs"] = "2";
        cows["input 2"]   = "isc";
        cows["output"]    = "Cow Portal";

        StringMap uber1    = base;
        uber1["numinputs"] = "3";
        uber1["input 2"]   = "\"isc,qty=2\"";
        uber1["output"]    = "Pandemonium Portal";

        StringMap uber2    = base;
        uber2["numinputs"] = "4";
        uber2["input 2"]   = "\"isc,qty=3\"";
        uber2["output"]    = "Pandemonium Finale Portal";

        view.appendRow(cows);
        view.appendRow(uber1);
        view.appendRow(uber2);
    }
    if (newSocketing) {
        using StringMap = QMap<QString, QString>;
        const StringMap base{
            { "description", "Quick Sockets" },
            { "enabled", "1" },
            { "version", "0" },
            { "*eol", "0" },
            { "input 1", "\"any,nor,nos\"" },

        };
        StringMap socket3    = base;
        socket3["numinputs"] = "3";
        socket3["input 2"]   = "tsc";
        socket3["input 3"]   = "isc";
        socket3["output"]    = "\"useitem,sock=3\"";

        StringMap socket4    = base;
        socket4["numinputs"] = "4";
        socket4["input 2"]   = "\"tsc,qty=2\"";
        socket4["input 3"]   = "isc";
        socket4["output"]    = "\"useitem,sock=4\"";

        StringMap socket5    = base;
        socket5["numinputs"] = "4";
        socket5["input 2"]   = "\"isc,qty=2\"";
        socket5["input 3"]   = "tsc";
        socket5["output"]    = "\"useitem,sock=5\"";

        StringMap socket6    = base;
        socket6["numinputs"] = "5";
        socket6["input 2"]   = "\"isc,qty=2\"";
        socket6["input 3"]   = "\"tsc,qty=2\"";
        socket6["output"]    = "\"useitem,sock=6\"";

        StringMap socket1    = base;
        socket1["numinputs"] = "3";
        socket1["input 1"]   = "\"any,uni,nos\"";
        socket1["input 2"]   = "tsc";
        socket1["input 3"]   = "isc";
        socket1["output"]    = "\"useitem,sock=1\"";

        view.appendRow(socket3);
        view.appendRow(socket4);
        view.appendRow(socket5);
        view.appendRow(socket6);
        view.appendRow(socket1);
    }

    return result;
}
