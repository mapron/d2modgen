/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleItemDrops.hpp"
#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleItemDrops>();

enum class TCType
{
    Equip,
    Bow,
    Junk,
    Other,
};

// "Act [12345]( \\([NH]\\))? Equip [ABC]"
// "Act [12345]( \\([NH]\\))? Bow [ABC]"
// "Act [12345]( \\([NH]\\))? Junk"

TCType getTC(const std::string& tc)
{
    std::string_view sv(tc);
    if (!sv.starts_with("Act "))
        return TCType::Other;
    sv = sv.substr(5);
    if (sv.starts_with(" (N)") || sv.starts_with(" (H)"))
        sv = sv.substr(4);

    if (sv.starts_with(" Equip "))
        return TCType::Equip;
    if (sv.starts_with(" Bow "))
        return TCType::Bow;
    if (sv.starts_with(" Junk"))
        return TCType::Junk;
    return TCType::Other;
}

}

IModule::PresetList ModuleItemDrops::presets() const
{
    return {
        PropertyTreeScalarMap{
            { "chance_uni", 50 },
            { "chance_set", 30 },
            { "chance_highlevel", 15 },
            { "high_elite_drops", true },
            { "equal_uniques", true },
        },
        PropertyTreeScalarMap{
            { "chance_uni", 15 },
            { "chance_set", 10 },
            { "high_elite_drops", true },
        },
        PropertyTreeScalarMap{
            { "chance_uni", 5 },
            { "chance_set", 5 },
            { "nodrop_percent", 50 },
            { "gold_percent", 50 },
            { "junk_percent", 10 },
        },
    };
}

PropertyTreeScalarMap ModuleItemDrops::defaultValues() const
{
    return {
        { "chance_uni", 1 },
        { "chance_set", 1 },
        { "chance_rare", 1 },
        { "chance_highlevel", 1 },
        { "high_elite_drops", false },
        { "nodrop_percent", 100 },
        { "good_percent", 100 },
        { "equip_percent", 100 },
        { "gold_percent", 100 },
        { "junk_percent", 100 },
        { "equal_uniques", false },
    };
}

IModule::UiControlHintMap ModuleItemDrops::uiHints() const
{
    return {
        { "chance_uni", UiControlHintSliderMinMax(1, 50) },
        { "chance_set", UiControlHintSliderMinMax(1, 30) },
        { "chance_rare", UiControlHintSliderMinMax(1, 15) },
        { "chance_highlevel", UiControlHintSliderMinMax(1, 15) },
        { "nodrop_percent", UiControlHintSlider(20, 5, true) },
        { "good_percent", UiControlHintSlider(2, 10, true) },
        { "equip_percent", UiControlHintSlider(5, 10, true) },
        { "gold_percent", UiControlHintSlider(20, 10, true) },
        { "junk_percent", UiControlHintSlider(20, 10, true) },
    };
}

void ModuleItemDrops::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    static const std::set<int> s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
    static const StringSet     s_modifyNames{
        "Cow",
        "Cow (N)",
        "Cow (H)",
    };
    auto& tableSet = output.tableSet;
    {
        TableView view(tableSet.tables[TableId::treasureclassex], true);

        const int  factorUnique    = input.getInt("chance_uni");
        const int  factorSet       = input.getInt("chance_set");
        const int  factorRare      = input.getInt("chance_rare");
        const int  factorHighlevel = input.getInt("chance_highlevel");
        const int  percentNoDrop   = input.getInt("nodrop_percent");
        const int  percentGoodTC   = input.getInt("good_percent");
        const int  percentGold     = input.getInt("gold_percent");
        const int  percentEquip    = input.getInt("equip_percent");
        const int  percentJunk     = input.getInt("junk_percent");
        const bool highDropsCount  = input.getInt("high_elite_drops");

        auto factorAdjust = [](TableCell& value, double factor, int maxFact) {
            const double prev           = value.toInt();
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value.setInt(std::min(next, maxFact));
        };
        auto adjustPick = [](int value, int num, int denum) -> int {
            if (!value)
                return 0;
            if (num == denum)
                return value;
            value = value * num / denum;
            return std::max(1, value);
        };

        for (auto& row : view) {
            const auto   treasureGroup      = row["group"].toInt();
            std::string& className          = row["Treasure Class"].str;
            auto&        uniqueRatio        = row["Unique"];
            auto&        setRatio           = row["Set"];
            auto&        rareRatio          = row["Rare"];
            const bool   allowFillNewValues = s_modifyGroups.contains(treasureGroup) || s_modifyNames.contains(className);
            const bool   allowModifyValues  = !uniqueRatio.isEmpty() && uniqueRatio != "1024" && !setRatio.isEmpty();
            if (allowFillNewValues || allowModifyValues) {
                // these limits are empyrical - to prevent 100% drop chance on 1000% MF.
                factorAdjust(uniqueRatio, factorUnique, 1010);
                factorAdjust(setRatio, factorSet, 990);
                factorAdjust(rareRatio, factorRare, 960);
            }
            DropSet dropSet;
            dropSet.readRow(row);

            dropSet.m_noDrop = adjustPick(dropSet.m_noDrop, percentNoDrop, 100);

            if (highDropsCount) {
                if (treasureGroup == 15) { // Uniques, insead of 1 item + 2*2 potion, make 4 items + 1*2 potion
                    dropSet.m_items[0].prob = 4;
                    dropSet.m_items[1].prob = 1;
                    row["Picks"].str        = "-5";
                }
                if (treasureGroup == 13) { // Champions, insead of 1 item + 1-2*2 potion, make 2 items + 1*2 potion
                    dropSet.m_items[0].prob = 2;
                    dropSet.m_items[1].prob = 1;
                    row["Picks"].str        = "-3";
                }
            }

            const auto rowTC = getTC(className);
            if (rowTC == TCType::Other) {
                for (auto& item : dropSet.m_items) {
                    int        percent   = 100;
                    const auto subItemTC = getTC(item.tc.str);

                    if (item.tc.endsWith(" Good"))
                        percent = percentGoodTC;
                    else if (item.tc.startsWith("gld") || item.tc.startsWith("\"gld"))
                        percent = percentGold;
                    else if (subItemTC == TCType::Junk)
                        percent = percentJunk;
                    else if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        percent = percentEquip;

                    item.prob = adjustPick(item.prob, percent, 100);
                }
            }
            if (rowTC == TCType::Equip || rowTC == TCType::Bow) {
                for (auto& item : dropSet.m_items) {
                    const auto subItemTC = getTC(item.tc.str);
                    if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        item.prob = adjustPick(item.prob, 1, factorHighlevel);
                }
            }

            dropSet.writeRow(row);
        }
    }

    const bool equalRarity = input.getInt("equal_uniques");
    if (equalRarity) {
        TableView view(tableSet.tables[TableId::uniqueitems], true);
        for (auto& row : view) {
            auto& rarity = row["rarity"];
            if (!rarity.isEmpty())
                rarity.str = "1";
        }
    }
}

}
