/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleRuneDrops.hpp"
#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleRuneDrops>();
}

IModule::PresetList ModuleRuneDrops::presets() const
{
    return {
        PropertyTreeScalarMap{
            { "zod_factor", 200 },
            { "rune_factor", 10 },
            { "countess_rune_higher", true },
            { "countess_rune_more", true },
            { "wraith_runes", true },
        },
        PropertyTreeScalarMap{
            { "zod_factor", 10 },
            { "rune_factor", 3 },
        },
        PropertyTreeScalarMap{
            { "rune_factor", 3 },
            { "countess_rune_higher", true },
            { "countess_rune_more", true },
            { "wraith_runes", true },
        },
    };
}

PropertyTreeScalarMap ModuleRuneDrops::defaultValues() const
{
    return {
        { "zod_factor", 1 },
        { "rune_factor", 1 },
        { "highrune_switch", false },
        { "countess_rune_higher", false },
        { "countess_rune_more", false },
        { "wraith_runes", false },
    };
}

IModule::UiControlHintMap ModuleRuneDrops::uiHints() const
{
    return {
        { "zod_factor", UiControlHintSliderMinMax(1, 1000) },
        { "rune_factor", UiControlHintSliderMinMax(1, 10) },
    };
}

void ModuleRuneDrops::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto& tableSet = output.tableSet;
    {
        TableView view(tableSet.tables["treasureclassex"], true);

        const int  factorRune      = input.getInt("rune_factor");
        const int  factorZod       = input.getInt("zod_factor");
        const bool countessMore    = input.getInt("countess_rune_more");
        const int  countessUpg     = input.getInt("countess_rune_higher") ? 5 : 0;
        const bool wraithMore      = input.getInt("wraith_runes");
        const bool switchHighRunes = input.getInt("highrune_switch");
        auto       factorAdjust    = [](QString& value, double factor, int maxFact) {
            const double prev           = value.toInt();
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value          = QString("%1").arg(std::min(next, maxFact));
        };

        QMap<QString, QString> highRuneReplacement{
            { "r33", "r31" },
            { "r32", "r30" },
            { "r31", "r33" },
            { "r30", "r32" },
        };

        QMap<QString, double> runesReplaceMult;
        if (factorZod > 1) {
            const double factor   = factorZod;
            const double iterMult = std::pow(factor, 0.1);
            double       mult     = 1. / factor;

            for (int i = 16; i >= 6; --i) {
                runesReplaceMult[QString("Runes %1").arg(i)] = mult;
                mult *= iterMult;
            }
        }

        for (auto& row : view) {
            QString& className = row["Treasure Class"];

            DropSet dropSet;
            dropSet.readRow(row);

            if (factorRune > 1 && className.endsWith(" Good")) {
                for (auto& item : dropSet.m_items) {
                    if (!item.tc.startsWith("Runes "))
                        continue;
                    item.prob *= factorRune;
                    break;
                }
            }
            if (factorZod > 1 && className.startsWith("Runes ")) {
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    if (!tcName.startsWith("Runes ") || !runesReplaceMult.contains(tcName))
                        continue;
                    const int newProb = static_cast<int>(item.prob * runesReplaceMult[tcName]);
                    item.prob         = std::max(newProb, 5);
                    break;
                }
            }
            const bool countessRuneTC = className.startsWith("Countess Rune");
            const bool countessItemTC = className.startsWith("Countess Item");
            if (countessUpg > 0 && countessRuneTC) {
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    if (!tcName.startsWith("Runes "))
                        continue;
                    const int oldTC = tcName.midRef(6).toInt();
                    const int newTC = oldTC + countessUpg;
                    tcName          = QString("Runes %1").arg(newTC);
                    break;
                }
            }
            if (countessMore) {
                if (countessRuneTC) {
                    row["Picks"]     = "5";
                    dropSet.m_noDrop = 1;
                }
                if (countessItemTC)
                    row["Picks"] = "1";
            }
            if (switchHighRunes && className.startsWith("Runes ")) {
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    tcName          = highRuneReplacement.value(tcName, tcName);
                }
            }
            if (wraithMore && className.contains(") Wraith ")) {
                static const QMap<QString, QString> s_wraithReplacement{
                    { "Act 1 (N)", "Runes 8" },
                    { "Act 2 (N)", "Runes 9" },
                    { "Act 3 (N)", "Runes 10" },
                    { "Act 4 (N)", "Runes 11" },
                    { "Act 5 (N)", "Runes 12" },
                    { "Act 1 (H)", "Runes 13" },
                    { "Act 2 (H)", "Runes 14" },
                    { "Act 3 (H)", "Runes 15" },
                    { "Act 4 (H)", "Runes 16" },
                    { "Act 5 (H)", "Runes 17" },
                };
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    if (!tcName.contains(") Magic "))
                        continue;
                    tcName = s_wraithReplacement.value(tcName.mid(0, 9), tcName);
                    break;
                }
            }
            dropSet.writeRow(row);
        }
    }
}

}
