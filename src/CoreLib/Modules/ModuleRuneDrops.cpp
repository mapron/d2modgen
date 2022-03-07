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

PropertyTreeScalarMap ModuleRuneDrops::defaultValuesScalar() const
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

void ModuleRuneDrops::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto& tableSet = output.tableSet;
    {
        TableView view(tableSet.tables[TableId::treasureclassex], true);

        const int  factorRune      = input.getInt("rune_factor");
        const int  factorZod       = input.getInt("zod_factor");
        const bool countessMore    = input.getInt("countess_rune_more");
        const int  countessUpg     = input.getInt("countess_rune_higher") ? 5 : 0;
        const bool wraithMore      = input.getInt("wraith_runes");
        const bool switchHighRunes = input.getInt("highrune_switch");

        std::map<std::string, std::string> highRuneReplacement{
            { "r33", "r31" },
            { "r32", "r30" },
            { "r31", "r33" },
            { "r30", "r32" },
        };

        std::map<std::string, double> runesReplaceMult;
        if (factorZod > 1) {
            const double factor   = factorZod;
            const double iterMult = std::pow(factor, 0.1);
            double       mult     = 1. / factor;

            for (int i = 16; i >= 6; --i) {
                runesReplaceMult[argCompat("Runes %1", i)] = mult;
                mult *= iterMult;
            }
        }

        for (auto& row : view) {
            auto& className = row["Treasure Class"];

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
                    auto& tcName = item.tc;
                    if (!tcName.startsWith("Runes ") || !runesReplaceMult.contains(tcName.str))
                        continue;
                    const int newProb = static_cast<int>(item.prob * runesReplaceMult[tcName.str]);
                    item.prob         = std::max(newProb, 5);
                    break;
                }
            }
            const bool countessRuneTC = className.startsWith("Countess Rune");
            const bool countessItemTC = className.startsWith("Countess Item");
            if (countessUpg > 0 && countessRuneTC) {
                for (auto& item : dropSet.m_items) {
                    auto& tcName = item.tc;
                    if (!tcName.startsWith("Runes "))
                        continue;
                    const int oldTC = TableCell(tcName.str.substr(6)).toInt();
                    const int newTC = oldTC + countessUpg;
                    tcName          = argCompat("Runes %1", newTC);
                    break;
                }
            }
            if (countessMore) {
                if (countessRuneTC) {
                    row["Picks"].str = "5";
                    dropSet.m_noDrop = 1;
                }
                if (countessItemTC)
                    row["Picks"].str = "1";
            }
            if (switchHighRunes && className.startsWith("Runes ")) {
                for (auto& item : dropSet.m_items) {
                    auto& tcName = item.tc.str;
                    auto  it     = highRuneReplacement.find(tcName);
                    if (it != highRuneReplacement.cend())
                        tcName = it->second;
                }
            }
            if (wraithMore && className.contains(") Wraith ")) {
                static const std::map<std::string, std::string> s_wraithReplacement{
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
                    auto& tcName = item.tc;
                    if (!tcName.contains(") Magic "))
                        continue;
                    auto it = s_wraithReplacement.find(tcName.str.substr(0, 9));
                    if (it != s_wraithReplacement.cend())
                        tcName.str = it->second;
                    break;
                }
            }
            dropSet.writeRow(row);
        }
    }
}

}
