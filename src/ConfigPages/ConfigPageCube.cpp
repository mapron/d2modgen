/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCube.hpp"

#include "AttributeHelper.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {
const QMap<QString, QString> s_craftedTypeReplace{
    { "\"fhl,mag,upg\"", "\"helm,mag\"" },
    { "\"mbt,mag,upg\"", "\"boot,mag\"" },
    { "\"mgl,mag,upg\"", "\"glov,mag\"" },
    { "\"tbl,mag,upg\"", "\"belt,mag\"" },
    { "\"gts,mag,upg\"", "\"shld,mag\"" },
    { "\"fld,mag,upg\"", "\"tors,mag\"" },
    { "\"blun,mag\"", "\"weap,mag\"" },

    { "\"hlm,mag,upg\"", "\"helm,mag\"" },
    { "\"tbt,mag,upg\"", "\"boot,mag\"" },
    { "\"vgl,mag,upg\"", "\"glov,mag\"" },
    { "\"mbl,mag,upg\"", "\"belt,mag\"" },
    { "\"spk,mag,upg\"", "\"shld,mag\"" },
    { "\"plt,mag,upg\"", "\"tors,mag\"" },
    { "\"axe,mag\"", "\"weap,mag\"" },

    { "\"msk,mag,upg\"", "\"helm,mag\"" },
    { "\"lbt,mag,upg\"", "\"boot,mag\"" },
    { "\"lgl,mag,upg\"", "\"glov,mag\"" },
    { "\"vbl,mag,upg\"", "\"belt,mag\"" },
    { "\"sml,mag,upg\"", "\"shld,mag\"" },
    { "\"ltp,mag,upg\"", "\"tors,mag\"" },
    { "\"rod,mag\"", "\"weap,mag\"" },

    { "\"crn,mag,upg\"", "\"helm,mag\"" },
    { "\"hbt,mag,upg\"", "\"boot,mag\"" },
    { "\"hgl,mag,upg\"", "\"glov,mag\"" },
    { "\"lbl,mag,upg\"", "\"belt,mag\"" },
    { "\"kit,mag,upg\"", "\"shld,mag\"" },
    { "\"brs,mag,upg\"", "\"tors,mag\"" },
    { "\"spea,mag\"", "\"miss,mag\"" },
};

const QMap<QString, QString> s_craftedGemReplace{
    { "gpb", "gems" },
    { "gpr", "gemr" },
    { "gpv", "gema" },
    { "gpg", "geme" },
    { "gpy", "gemt" },
    { "skz", "gemz" },
};

}

ConfigPageCube::ConfigPageCube(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Remove gem component from Rune upgrade recipes (Rune x3,x2 => next Rune)"), "noGemUpgrade", false, this)
               << new CheckboxWidget(tr("Add quick portal access recipes:") + "\n"
                                                                              "1. TP book + Id scroll = Cow Portal\n"
                                                                              "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                                                              "3. TP book + Id scroll x3 = Uber Tristram Portal",
                                     "quickPortals",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add quick quest recipes:") + " "
                                                                      "1. key + Id scroll = Horadric Staff "
                                                                      "2. key + TP scroll = Khalim Will",
                                     "quickQuests",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add socketing recipes:") + "\n"
                                                                    "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                                                    "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                                                    "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                                                    "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                                                    "5. Unique/Set/Rare item + TP scroll x1 + Id scroll x1 = Add 1 socket\n"
                                                                    "6. Socketed item + TP scroll x1 = Clear sockets",
                                     "socketing",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add upgrade recipes:") + "\n"
                                                                  "1. Normal item (normal,magic,rare,unique,set) + Antidote = Exceptional item\n"
                                                                  "2. Exceptional item (normal,magic,rare,unique,set) + Antidote = Elite item\n"
                                                                  "3. Any item + Stamina x2 = Add Ethereal\n"
                                                                  "4. Magic (magic,rare,unique,set) item  + Stamina = Normal item of same type",
                                     "upgrading",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Add Rune downgrade:") + " Rune + key + key = Lower rune",
                                     "runeDowngrade",
                                     false,
                                     this));
    addWidget(new QLabel(tr("Simplyfy crafting:"), this));
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Remove strict item types for Crafted (any helm, any gloves etc)"), "craftNoStrict", false, this)
               << new CheckboxWidget(tr("Remove Rune/Jewel requirement for Crafted\n"
                                        "(So any recipe is 'item + gem')"),
                                     "craftNoRunes",
                                     false,
                                     this)
               << new CheckboxWidget(tr("Make Item Level always 99"), "craftHighIlvl", false, this)
               << new SliderWidgetMinMax(tr("Multiply crafted item bonuses by"), "craftMultBonus", 1, 5, 1, true, this));
    closeLayout();
}

QString ConfigPageCube::pageHelp() const
{
    return tr("Create some recipes to cheese/cut corners around the game. \n"
              "Input and results are self-explanatory.  ");
}

void ConfigPageCube::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    const bool noGemUpgrade   = getWidgetValue("noGemUpgrade");
    const bool quickPortals   = getWidgetValue("quickPortals");
    const bool quickQuests    = getWidgetValue("quickQuests");
    const bool newSocketing   = getWidgetValue("socketing");
    const bool upgrading      = getWidgetValue("upgrading");
    const bool craftNoStrict  = getWidgetValue("craftNoStrict");
    const bool craftNoRunes   = getWidgetValue("craftNoRunes");
    const bool craftHighIlvl  = getWidgetValue("craftHighIlvl");
    const int  craftMultBonus = getWidgetValue("craftMultBonus");
    const bool runeDowngrade  = getWidgetValue("runeDowngrade");

    auto&     tableSet = output.tableSet;
    TableView view(tableSet.tables["cubemain"], true);
    for (auto& row : view) {
        QString&   description = row["description"];
        QString&   numinputs   = row["numinputs"];
        QString&   input1      = row["input 1"];
        QString&   input2      = row["input 2"];
        QString&   input3      = row["input 3"];
        QString&   input4      = row["input 4"];
        const bool isCrafted   = row["output"] == "\"usetype,crf\"";

        if (noGemUpgrade && description.contains("->") && description.endsWith(" Rune") && !input2.isEmpty()) {
            numinputs = QString("%1").arg(numinputs.toInt() - 1);
            input2    = "";
        }
        if (craftHighIlvl && (isCrafted || !row["ilvl"].isEmpty() || !row["plvl"].isEmpty() || !row["lvl"].isEmpty())) {
            row["ilvl"] = "";
            row["plvl"] = "";
            row["lvl"]  = "99";
        }
        if (isCrafted) {
            if (craftNoStrict)
                input1 = s_craftedTypeReplace.value(input1, input1);

            if (craftNoRunes) {
                input2 = input4;
                input3 = input4 = "";
                numinputs       = "2";
                input2          = s_craftedGemReplace.value(input2, input2);
            }
            if (craftMultBonus > 1) {
                for (int i = 1; i <= 5; ++i) {
                    QString& mod = row[QString("mod %1").arg(i)];
                    if (isMinMaxRange(mod)) {
                        QString& modMin = row[QString("mod %1 min").arg(i)];
                        QString& modMax = row[QString("mod %1 max").arg(i)];
                        modMin          = QString("%1").arg(modMin.toInt() * craftMultBonus);
                        modMax          = QString("%1").arg(modMax.toInt() * craftMultBonus);
                    }
                }
            }
        }
    }
    using StringMap = TableView::RowValues;
    if (quickPortals) {
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
        socket4["input 2"]   = "tsc";
        socket4["input 3"]   = "\"isc,qty=2\"";
        socket4["output"]    = "\"useitem,sock=4\"";

        StringMap socket5    = base;
        socket5["numinputs"] = "4";
        socket5["input 2"]   = "\"tsc,qty=2\"";
        socket5["input 3"]   = "isc";
        socket5["output"]    = "\"useitem,sock=5\"";

        StringMap socket6    = base;
        socket6["numinputs"] = "5";
        socket6["input 2"]   = "\"tsc,qty=2\"";
        socket6["input 3"]   = "\"isc,qty=2\"";
        socket6["output"]    = "\"useitem,sock=6\"";

        StringMap socket1    = base;
        socket1["numinputs"] = "3";
        socket1["input 1"]   = "\"any,uni,nos\"";
        socket1["input 2"]   = "tsc";
        socket1["input 3"]   = "isc";
        socket1["output"]    = "\"useitem,sock=1\"";

        StringMap socket1a  = socket1;
        socket1a["input 1"] = "\"any,rar,nos\"";

        StringMap socket1b  = socket1;
        socket1b["input 1"] = "\"any,set,nos\"";

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
        view.appendRow(socket1a);
        view.appendRow(socket1b);
        view.appendRow(socketClear);
    }

    if (upgrading) {
        StringMap base{
            { "description", "Quick Upgrading" },
            { "enabled", "1" },
            { "version", "100" },
            { "*eol", "0" },
            { "numinputs", "2" },
            { "ilvl", "100" },
            { "input 2", "yps" },

        };
        if (craftHighIlvl) {
            base["ilvl"] = "";
            base["plvl"] = "";
            base["lvl"]  = "99";
        }
        StringMap armo1  = base;
        armo1["input 1"] = "\"armo,bas\"";
        armo1["output"]  = "\"useitem,mod,exc\"";

        StringMap weap1  = armo1;
        weap1["input 1"] = "\"weap,bas\"";

        StringMap armo2  = base;
        armo2["input 1"] = "\"armo,exc\"";
        armo2["output"]  = "\"useitem,mod,eli\"";

        StringMap weap2  = armo2;
        weap2["input 1"] = "\"weap,exc\"";

        StringMap armoEth      = base;
        armoEth["numinputs"]   = "3";
        armoEth["input 1"]     = "\"armo,noe\"";
        armoEth["input 2"]     = "\"vps,qty=2\"";
        armoEth["output"]      = "\"useitem\"";
        armoEth["mod 1"]       = "ethereal";
        armoEth["mod 1 param"] = "1";
        armoEth["mod 1 min"]   = "1";
        armoEth["mod 1 max"]   = "1";

        armoEth["mod 2"]     = "ac%";
        armoEth["mod 2 min"] = "50";
        armoEth["mod 2 max"] = "50";

        StringMap weapEth  = armoEth;
        weapEth["input 1"] = "\"weap,noe\"";
        weapEth["mod 2"]   = "dmg %";

        StringMap armoNor  = base;
        armoNor["input 1"] = "armo";
        armoNor["input 2"] = "vps";
        armoNor["output"]  = "\"usetype,nor\"";

        StringMap weapNor  = armoNor;
        weapNor["input 1"] = "weap";

        view.appendRow(armo1);
        view.appendRow(weap1);
        view.appendRow(armo2);
        view.appendRow(weap2);
        view.appendRow(armoEth);
        view.appendRow(weapEth);
        view.appendRow(armoNor);
        view.appendRow(weapNor);
    }

    if (runeDowngrade) {
        for (int i = 2; i <= 33; ++i) {
            StringMap rune{
                { "description", "Rune downgrade" },
                { "enabled", "1" },
                { "version", "100" },
                { "*eol", "0" },
                { "numinputs", "3" },
                { "input 1", QString("r%1").arg(i, 2, 10, QLatin1Char('0')) },
                { "input 2", "\"key,qty=2\"" },
                { "output", QString("r%1").arg(i - 1, 2, 10, QLatin1Char('0')) },
            };
            view.appendRow(rune);
        }
    }
}

}
