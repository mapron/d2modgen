/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemDrops.hpp"
#include "MernelPlatform/StringUtils.hpp"

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

void ModuleItemDrops::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleItemDrops)
        return;
    const auto& module = config.m_moduleItemDrops;

    static const std::set<int> s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
    static const StringSet     s_modifyNames{
        "Cow",
        "Cow (N)",
        "Cow (H)",
    };
    {
        const int  factorUnique    = module.m_chance_uni;
        const int  factorSet       = module.m_chance_set;
        const int  factorRare      = module.m_chance_rare;
        const int  factorHighlevel = module.m_chance_highlevel;
        const int  percentDrop     = module.m_drop_percent;
        const int  percentGoodTC   = module.m_good_percent;
        const int  percentGold     = module.m_gold_percent;
        const int  percentEquip    = module.m_equip_percent;
        const int  percentJunk     = module.m_junk_percent;
        const bool highDropsCount  = module.m_high_elite_drops;
        const bool alwaysQuest     = module.m_boss_quest_drops;

        auto factorAdjust = [](std::optional<int>& value, double factor, int maxFact) {
            maxFact                     = std::max(maxFact, value.value_or(0));
            const double prev           = value.value_or(0);
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value          = (std::min(next, maxFact));
        };
        auto adjustPick = [](std::optional<int>& value, int num, int denum) {
            if (!value.has_value() || value.value() == 0)
                return;
            if (num == denum)
                return;
            value = value.value() * num / denum;
            value = std::max(1, value.value());
        };
        output.treasureclassex.makeIndex(&DataRow_treasureclassex::name);

        for (auto& row : output.treasureclassex.rows) {
            const auto                     treasureGroup = row.group.value_or(0);
            std::string&                   className     = row.name;
            const DataRow_treasureclassex* questRow      = nullptr;
            if (alwaysQuest && s_bossTC.contains(className))
                questRow = output.treasureclassex.index.at(Mernel::strToLower(s_bossTC.at(className)));
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

            adjustPick(row.noDrop, 100, percentDrop);

            if (highDropsCount) {
                /// @todo: remove hardcode group ids.
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
                if (treasureGroup == 18 || treasureGroup == 19) { // SuperUniques, make 6 items
                    row.items[0].prob = 6;
                    row.items[1].prob = 0;
                    row.picks         = -6;
                }
                if (treasureGroup == 11) { // Citems, remove gold
                    row.items.erase(row.items.cbegin());
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

                    adjustPick(item.prob, percent, 100);
                }
            }
            if (rowTC == TCType::Equip || rowTC == TCType::Bow) {
                for (auto& item : row.items) {
                    const auto subItemTC = getTC(item.tc);
                    if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        adjustPick(item.prob, 1, factorHighlevel);
                }
            }
        }
    }

    if (module.m_equal_uniques) {
        for (auto& row : output.uniqueitems.rows) {
            auto& rarity = row.rarity;
            if (rarity.has_value())
                rarity = 1;
        }
        for (auto& row : output.setitems.rows) {
            auto& rarity = row.rarity;
            if (rarity.has_value())
                rarity = 1;
        }
    }
}

}
