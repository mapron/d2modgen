/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCube.hpp"

ConfigPageCube::ConfigPageCube(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Remove gem component from Rune upgrade recipes (Rune x3,x2 => next Rune)", "noGemUpgrade", false, this)
               << new CheckboxWidget("Add quick portal access recipes:\n"
                                     "1. TP book + Id scroll = Cow Portal\n"
                                     "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                     "3. TP book + Id scroll x3 = Uber Tristram Portal\n",
                                     "quickPortals",
                                     false,
                                     this)
               << new CheckboxWidget("Add quick quest recipes:\n"
                                     "1. key + Id scroll = Horadric Staff\n"
                                     "2. key + TP scroll = Khalim Will\n",
                                     "quickQuests",
                                     false,
                                     this)
               << new CheckboxWidget("Add socketing recipes:\n"
                                     "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                     "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                     "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                     "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                     "5. Unique item + TP scroll x1 + Id scroll x1 = Add 1 socket\n"
                                     "6. Socketed item + TP scroll x1 = Clear sockets\n",
                                     "socketing",
                                     false,
                                     this));
    closeLayout();
}

KeySet ConfigPageCube::generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return {};
    KeySet result;
    result << "cubemain";

    const bool noGemUpgrade = getWidgetValue("noGemUpgrade");
    const bool quickPortals = getWidgetValue("quickPortals");
    const bool quickQuests  = getWidgetValue("quickQuests");
    const bool newSocketing = getWidgetValue("socketing");

    auto&     tableSet = output.tableSet;
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
            { "version", "100" },
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
    if (quickQuests) {
        using StringMap = QMap<QString, QString>;
        const StringMap base{
            { "description", "Quick Quest" },
            { "enabled", "1" },
            { "version", "0" },
            { "op", "28" },
            { "*eol", "0" },
            { "input 1", "key" },
            { "numinputs", "2" },

        };
        StringMap staff  = base;
        staff["input 2"] = "isc";
        staff["output"]  = "hst";

        StringMap khalim  = base;
        khalim["input 2"] = "tsc";
        khalim["output"]  = "qf2";

        view.appendRow(staff);
        view.appendRow(khalim);
    }
    if (newSocketing) {
        using StringMap = QMap<QString, QString>;
        const StringMap base{
            { "description", "Quick Sockets" },
            { "enabled", "1" },
            { "version", "100" },
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

        StringMap socketClear    = base;
        socketClear["numinputs"] = "2";
        socketClear["input 1"]   = "\"any,sock\"";
        socketClear["input 2"]   = "tsc";
        socketClear["output"]    = "\"useitem,uns\"";

        view.appendRow(socket3);
        view.appendRow(socket4);
        view.appendRow(socket5);
        view.appendRow(socket6);
        view.appendRow(socket1);
        view.appendRow(socketClear);
    }

    return result;
}
