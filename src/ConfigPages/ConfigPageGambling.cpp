/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageGambling.hpp"

namespace D2ModGen {

ConfigPageGambling::ConfigPageGambling(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Allow gambling for charms and jewels (Legacy only)\nNote: crashes D2R - so enabled only for D2."), "charmGamble", false, this)
               << new SliderWidgetMinMax(tr("Increase Unique chance, times<br>Note: you still can get a lot of failed uniques for no known reason."), "ratioUnique", 1, 500, 1, this)
               << new SliderWidgetMinMax(tr("Increase Set chance, times"), "ratioSet", 1, 250, 1, this)
               << new SliderWidgetMinMax(tr("Increase Rare chance, times"), "ratioRare", 1, 4, 1, this)
               << new SliderWidgetMinMax(tr("Increase Exceptional upgrade chance, times"), "ratioExc", 1, 4, 1, this)
               << new SliderWidgetMinMax(tr("Increase Elite upgrade chance, times<br>Note: seems like Exc. check is done beforehead, so if it's high enough, elite won't generate."), "ratioElite", 1, 10, 1, this));
    closeLayout();
}

QString ConfigPageGambling::pageHelp() const
{
    return tr("First option enables Charms in Gambling window, but only for D2 Legacy.\n"
              "Next sliders allow you increase basic chance of getting Unique/Rare/Set/High-quality item.  ");
}

KeySet ConfigPageGambling::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto&      tableSet = output.tableSet;
    KeySet     result;
    const bool charmGamble = getWidgetValue("charmGamble") && env.isLegacy; // disabled for D2R now; it crashes the game.
    if (charmGamble) {
        result << "gamble";
        auto& table = tableSet.tables["gamble"];
        table.rows << TableRow({ "Charm Small", "cm1" });
        table.rows << TableRow({ "Charm Medium", "cm2" });
        table.rows << TableRow({ "Charm Large", "cm3" });
        table.rows << TableRow({ "Jewel", "jew" });
    }
    if (isAllDefault({ "ratioUnique", "ratioSet", "ratioRare", "ratioExc", "ratioElite" }))
        return result;

    static const QVector<QPair<QString, QString>> s_columns{
        { "GambleRare", "ratioRare" },
        { "GambleSet", "ratioSet" },
        { "GambleUnique", "ratioUnique" },
        { "GambleUber", "ratioExc" },
        { "GambleUltra", "ratioElite" },
    };

    result << "difficultylevels";
    {
        TableView view(tableSet.tables["difficultylevels"]);
        for (auto& row : view) {
            for (auto& p : s_columns) {
                QString&  col      = row[p.first];
                const int multiply = getWidgetValue(p.second);
                col                = QString("%1").arg(col.toInt() * multiply);
            }
        }
    }

    return result;
}

}
