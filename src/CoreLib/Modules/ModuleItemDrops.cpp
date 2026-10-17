/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemDrops.hpp"
#include "AttributeHelper.hpp"

namespace D2ModGen {

namespace {

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

const std::map<std::string, std::string> s_bossTC{
    { "Andariel", "Andarielq" },
    { "Andariel (N)", "Andarielq (N)" },
    { "Andariel (H)", "Andarielq (H)" },

    { "Duriel - Base", "Durielq - Base" },
    { "Duriel (N) - Base", "Durielq (N) - Base" },
    { "Duriel (H) - Base", "Durielq (H) - Base" },

    { "Mephisto", "Mephistoq" },
    { "Mephisto (N)", "Mephistoq (N)" },
    { "Mephisto (H)", "Mephistoq (H)" },

    { "Diablo", "Diabloq" },
    { "Diablo (N)", "Diabloq (N)" },
    { "Diablo (H)", "Diabloq (H)" },

    { "Baal", "Baalq" },
    { "Baal (N)", "Baalq (N)" },
    { "Baal (H)", "Baalq (H)" },

};

}

ModuleItemDrops::ModuleItemDrops()
    : PluginModule(Mernel::PropertyTreeMap{
          { "chance_highlevel", Mernel::PropertyTreeScalar{ 1 } },
          { "chance_rare", Mernel::PropertyTreeScalar{ 1 } },
          { "chance_set", Mernel::PropertyTreeScalar{ 1 } },
          { "chance_uni", Mernel::PropertyTreeScalar{ 1 } },
          { "equal_uniques", Mernel::PropertyTreeScalar{ false } },
          { "boss_quest_drops", Mernel::PropertyTreeScalar{ false } },
          { "equip_percent", Mernel::PropertyTreeScalar{ 100 } },
          { "gold_percent", Mernel::PropertyTreeScalar{ 100 } },
          { "good_percent", Mernel::PropertyTreeScalar{ 100 } },
          { "high_elite_drops", Mernel::PropertyTreeScalar{ false } },
          { "junk_percent", Mernel::PropertyTreeScalar{ 100 } },
          { "drop_percent", Mernel::PropertyTreeScalar{ 100 } },
      })
{
    setPresets({
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "chance_uni", Mernel::PropertyTreeScalar{ 50 } },
            { "chance_set", Mernel::PropertyTreeScalar{ 30 } },
            { "chance_highlevel", Mernel::PropertyTreeScalar{ 15 } },
            { "high_elite_drops", Mernel::PropertyTreeScalar{ true } },
            { "boss_quest_drops", Mernel::PropertyTreeScalar{ true } },
            { "equal_uniques", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "chance_uni", Mernel::PropertyTreeScalar{ 15 } },
            { "chance_set", Mernel::PropertyTreeScalar{ 10 } },
            { "high_elite_drops", Mernel::PropertyTreeScalar{ true } },
            { "boss_quest_drops", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "chance_uni", Mernel::PropertyTreeScalar{ 5 } },
            { "chance_set", Mernel::PropertyTreeScalar{ 5 } },
            { "drop_percent", Mernel::PropertyTreeScalar{ 200 } },
        } },
    });
}

void ModuleItemDrops::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    static const std::set<int> s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
    static const StringSet     s_modifyNames{
        "Cow",
        "Cow (N)",
        "Cow (H)",
    };
    {
        const int  factorUnique    = input.getInt("chance_uni");
        const int  factorSet       = input.getInt("chance_set");
        const int  factorRare      = input.getInt("chance_rare");
        const int  factorHighlevel = input.getInt("chance_highlevel");
        const int  percentDrop     = input.getInt("drop_percent");
        const int  percentGoodTC   = input.getInt("good_percent");
        const int  percentGold     = input.getInt("gold_percent");
        const int  percentEquip    = input.getInt("equip_percent");
        const int  percentJunk     = input.getInt("junk_percent");
        const bool highDropsCount  = input.getInt("high_elite_drops");
        const bool alwaysQuest     = input.getInt("boss_quest_drops");

        auto factorAdjust = [](std::optional<int>& value, double factor, int maxFact) {
            maxFact                     = std::max(maxFact, value.value_or(0));
            const double prev           = value.value_or(0);
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value          = (std::min(next, maxFact));
        };
        auto adjustPick = [](int value, int num, int denum) -> int {
            if (!value)
                return 0;
            if (num == denum)
                return value;
            value = value * num / denum;
            return std::max(1, value);
        };
        output.treasureclassex.makeIndex(&DataRow_treasureclassex::name);

        for (auto& row : output.treasureclassex.rows) {
            const auto                     treasureGroup = row.group.value_or(0);
            std::string&                   className     = row.name;
            const DataRow_treasureclassex* questRow      = alwaysQuest && s_bossTC.contains(className) ? output.treasureclassex.index.at(s_bossTC.at(className)) : nullptr;
            if (questRow) {
                row.k_unique = questRow->k_unique;
                row.k_set    = questRow->k_set;
                row.k_rare   = questRow->k_rare;
                row.items    = questRow->items;
                row.noDrop   = questRow->noDrop;
            }

            auto&      uniqueRatio        = row.k_unique;
            auto&      setRatio           = row.k_set;
            auto&      rareRatio          = row.k_rare;
            const bool allowFillNewValues = s_modifyGroups.contains(treasureGroup) || s_modifyNames.contains(className);
            const bool allowModifyValues  = uniqueRatio.has_value() && uniqueRatio.value() != 1024 && setRatio.has_value();
            if (allowFillNewValues || allowModifyValues) {
                // these limits are empyrical - to prevent 100% drop chance on 1000% MF.
                factorAdjust(uniqueRatio, factorUnique, 1010);
                factorAdjust(setRatio, factorSet, 1024);
                factorAdjust(rareRatio, factorRare, 1024);
            }

            row.noDrop = adjustPick(row.noDrop.value_or(0), 100, percentDrop);

            if (highDropsCount) {
                if (treasureGroup == 15) { // Uniques, insead of 1 item + 2*2 potion, make 4 items + 1*2 potion
                    row.items[0].prob = 4;
                    row.items[1].prob = 1;
                    row.picks         = -5;
                }
                if (treasureGroup == 13) { // Champions, insead of 1 item + 1-2*2 potion, make 2 items + 1*2 potion
                    row.items[0].prob = 2;
                    row.items[1].prob = 1;
                    row.picks         = -3;
                }
            }

            const auto rowTC = getTC(className);
            if (rowTC == TCType::Other) {
                for (auto& item : row.items) {
                    int        percent   = 100;
                    const auto subItemTC = getTC(item.tc);

                    if (item.tc.ends_with(" Good"))
                        percent = percentGoodTC;
                    else if (item.tc.starts_with("gld") || item.tc.starts_with("\"gld"))
                        percent = percentGold;
                    else if (subItemTC == TCType::Junk)
                        percent = percentJunk;
                    else if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        percent = percentEquip;

                    item.prob = adjustPick(item.prob.value_or(0), percent, 100);
                }
            }
            if (rowTC == TCType::Equip || rowTC == TCType::Bow) {
                for (auto& item : row.items) {
                    const auto subItemTC = getTC(item.tc);
                    if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        item.prob = adjustPick(item.prob.value_or(0), 1, factorHighlevel);
                }
            }
        }
    }

    if (input.getInt("equal_uniques")) {
        for (auto& row : output.uniqueitems.rows) {
            auto& rarity = row.rarity;
            if (rarity.has_value())
                rarity = 1;
        }
    }
}

}
