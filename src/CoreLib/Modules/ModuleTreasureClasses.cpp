/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleTreasureClasses.hpp"

#include <cmath>

namespace D2ModGen {

namespace {
int getRuneTc(const std::string& tcName)
{
    if (!tcName.starts_with("Runes "))
        return -1;
    return std::atoi(tcName.substr(6).c_str());
}

enum class TCType
{
    Equip,
    Bow,
    Junk,
    Gold,
    Good,
    Other,
};

enum class MobType
{
    Skip,
    Normal,
    Champion,
    Unique,
    SuperUnique,
    SuperUniqueSpecial,
    ActBoss,
};

enum
{
    BaseEquip = 1,
    BaseMelee = 2,
    BaseBow   = 3,
    BaseMagic = 4,
    BaseGood  = 5,

    NormalChest  = 6,
    NormalH2H    = 7,
    NormalCast   = 8,
    NormalMiss   = 9,
    NormalWraith = 10,

    ChampionItem  = 11,
    ChampionPot   = 12,
    GroupChampion = 13,
    UniqueItem    = 14,
    GroupUnique   = 15,

    NormalQuill = 16,
    NormalSwarm = 17,

    GroupSuper = 18,
};

static const StringSet s_normalMobNames{
    "Cow",
    "Flying Scimitar",
    "Trapped Soul",
};

static const StringSet s_superUniqueNames{
    "Summoner",
    "Council",
    "Griswold",
    "Haphesto",
    "Nihlathak",
    "Blood Raven",
    "Smith",
    "Izual",
    "Cow King",
    "Countess",
};

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

std::string removeDifficultyName(std::string tc)
{
    if (tc.ends_with(" (N)") || tc.ends_with(" (H)"))
        return tc.substr(0, tc.size() - 4);
    return tc;
}

MobType getMobType(int group, const std::string& className)
{
    if (group >= NormalChest && group <= NormalWraith)
        return MobType::Normal;
    if (group >= NormalQuill && group <= NormalSwarm)
        return MobType::Normal;
    if (s_bossTC.contains(className))
        return MobType::ActBoss;

    auto timmed = removeDifficultyName(className);
    if (s_normalMobNames.contains(timmed))
        return MobType::Normal;
    if (s_superUniqueNames.contains(timmed))
        return MobType::SuperUniqueSpecial;

    if (group == GroupUnique) {
        assert(className.contains(" Unique "));
        return MobType::Unique;
    }
    if (group == GroupChampion) {
        assert(className.contains(" Champ "));
        return MobType::Champion;
    }
    if (group == GroupSuper) {
        assert(className.contains(" Super "));
        return MobType::SuperUnique;
    }
    if (className.contains("Map Champ "))
        return MobType::Champion;
    if (className.contains("Map Unique "))
        return MobType::Unique;
    return MobType::Skip;
}

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

    if (sv.ends_with(" Good") || sv.starts_with("Map Good "))
        return TCType::Good;
    if (sv.starts_with("gld") || sv.starts_with("\"gld"))
        return TCType::Gold;

    return TCType::Other;
}

}

struct ActAndDiff {
    int m_difficulty = 0;
    int m_act        = 0;

    int getTotalAct() const { return m_difficulty * 5 + m_act; }

    void detect(const std::string& tc)
    {
        std::vector<std::string> acts{ "Act 1", "Act 2", "Act 3", "Act 4", "Act 5" };
        for (int i = 0; auto& act : acts) {
            if (tc.find(act) != std::string::npos) {
                m_act = i;
            }
            i++;
        }
        if (tc.find("(H)") != std::string::npos)
            m_difficulty = 2;
        if (tc.find("(N)") != std::string::npos)
            m_difficulty = 1;
    }
};

void ModuleTreasureClasses::generate()
{
    {
        const auto& module = config.m_treasureClasses.m_runes;
        if (module.m_enabled)
            generateRunes(output, module);
    }
    {
        const auto& module = config.m_treasureClasses.m_items;
        if (module.m_enabled)
            generateItems(output, module);
    }
}

void ModuleTreasureClasses::generateItems(DataContext& output, const Config::TreasureClasses::Items& config)
{
    {
        const double factorUnique   = double(config.m_uniqueFactorIncrease) / 100.0;
        const double factorSet      = double(config.m_setFactorIncrease) / 100.0;
        const double factorRare     = double(config.m_rareFactorIncrease) / 100.0;
        const double factorNonElite = double(config.m_noneliteFactorIncrease) / 100.0;

        const int  factorHighlevel    = config.m_chanceForHighLevel;
        const int  percentDrop        = config.m_inverseNodropPercent;
        const int  percentGoodTC      = config.m_goodPercent;
        const int  percentGold        = config.m_goldPercent;
        const int  percentEquip       = config.m_equipPercent;
        const int  percentJunk        = config.m_junkPercent;
        const int  championItemCount  = config.m_championItemCount;
        const int  uniqueItemCount    = config.m_uniqueItemCount;
        const int  superItemCount     = config.m_superItemCount;
        const bool alwaysQuest        = config.m_forceQuestDrops;
        const bool removeGoldChamp    = config.m_removeChampGold;
        const bool removeElitePotions = config.m_removeElitePotions;

        auto factorAdjust = [](std::optional<int>& value, double factor, int maxFact) {
            if (factor == 1.0)
                return;
            const int prevInt = value.value_or(0);
            if (prevInt >= maxFact)
                return;

            const double prev           = prevInt;
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

        auto adjustElite = [removeElitePotions](std::optional<int>& picks, std::vector<DataRow_treasureclassex::Item>& items, int total) {
            int pots = 0;
            if (total <= 4)
                pots = 1;
            if (total <= 2)
                pots = 2;
            if (removeElitePotions)
                pots = 0;

            if (pots == 0)
                items.pop_back();

            if (items[0].prob.value() == total)
                return;

            items[0].prob = total;
            if (pots > 0)
                items[1].prob = pots;
            picks = -(total + pots);
        };

        output.treasureclassex.makeIndex(&DataRow_treasureclassex::name);

        for (auto& row : output.treasureclassex.rows) {
            const auto   treasureGroup = row.group.value_or(0);
            std::string& className     = row.name;

            const auto mobType = getMobType(treasureGroup, className);
            if (alwaysQuest && mobType == MobType::ActBoss) {
                auto* questRow = output.treasureclassex.index.at(Mernel::strToLower(s_bossTC.at(className)));
                if (questRow) {
                    row.k_unique = questRow->k_unique;
                    row.k_set    = questRow->k_set;
                    row.k_rare   = questRow->k_rare;
                    row.items    = questRow->items;
                    row.noDrop   = questRow->noDrop;
                }
            }
            const double nonEliteAdjust = mobType == MobType::Normal ? factorNonElite : 1.0;

            if (row.k_unique.has_value() || mobType == MobType::Normal) {
                // these limits are empyrical - to prevent 100% drop chance on 1000% MF.
                factorAdjust(row.k_unique, factorUnique * nonEliteAdjust, 1020);
                factorAdjust(row.k_set, factorSet * nonEliteAdjust, 1020);
                factorAdjust(row.k_rare, factorRare * nonEliteAdjust, 1024);
            }

            if (mobType == MobType::ActBoss || mobType == MobType::SuperUniqueSpecial || mobType == MobType::Normal)
                adjustPick(row.noDrop, 100, percentDrop);

            if (mobType == MobType::Unique) {
                adjustElite(row.picks, row.items, uniqueItemCount);
            }
            if (mobType == MobType::Champion) {
                adjustElite(row.picks, row.items, championItemCount);
            }
            if (mobType == MobType::SuperUnique) {
                adjustElite(row.picks, row.items, superItemCount);
            }
        }

        for (auto& row : output.treasureclassex.rows) {
            std::string& className = row.name;
            const auto   rowTC     = getTC(className);

            if (row.group.value_or(0) == ChampionItem && removeGoldChamp) { // Citems, remove gold
                row.items.erase(row.items.cbegin());
            }

            if (rowTC == TCType::Other) {
                for (auto& item : row.items) {
                    int        percent   = 100;
                    const auto subItemTC = getTC(item.tc);

                    if (subItemTC == TCType::Good)
                        percent = percentGoodTC;
                    else if (subItemTC == TCType::Gold)
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
                        adjustPick(item.prob, 100, factorHighlevel);
                }
            }
        }
    }
}

void ModuleTreasureClasses::generateRunes(DataContext& output, const Config::TreasureClasses::Runes& config)
{
    const int  factorRunePercent = config.m_runeInGoodTC;
    const int  factorZod         = config.m_zodFactor;
    const bool countessMore      = config.m_countessSpecialRuneMore;
    const int  countessUpg       = config.m_countessSpecialRuneLevel;
    const bool wraithOnlyRunes   = config.m_wraithOnlyRunes;

    auto applyScale = [](std::optional<int>& value, int64_t mult, int64_t denom, int64_t min = 1) {
        int64_t v = value.value_or(0);
        v         = v * mult / denom;
        v         = std::max(min, v);
        value     = static_cast<int>(v);
    };

    std::map<int, double> runesReplaceMult;
    if (factorZod > 1) {
        const double factor   = factorZod;
        const double iterMult = std::pow(factor, 0.1);
        double       mult     = 1. / factor;

        for (int i = 16; i >= 6; --i) {
            runesReplaceMult[i] = mult;
            mult *= iterMult;
        }
    }

    for (auto& row : output.treasureclassex.rows) {
        auto& className = row.name;

        if (factorRunePercent != 100 && className.ends_with(" Good")) {
            for (auto& item : row.items) {
                if (!item.tc.starts_with("Runes "))
                    continue;
                applyScale(item.prob, factorRunePercent, 100);
                break;
            }
        }
        if (factorZod > 1 && className.starts_with("Runes ")) {
            for (auto& item : row.items) {
                const int oldTC  = getRuneTc(item.tc);
                double    factor = runesReplaceMult[oldTC];
                if (oldTC < 0 || !factor)
                    continue;
                applyScale(item.prob, factor * 1000, 1000, 5);
                break;
            }
        }
        const bool countessRuneTC = className.starts_with("Countess Rune");
        const bool countessItemTC = className.starts_with("Countess Item");
        if (countessUpg > 0 && countessRuneTC) {
            for (auto& item : row.items) {
                const int oldTC = getRuneTc(item.tc);
                if (oldTC < 0)
                    continue;
                const int newTC = oldTC + countessUpg;
                item.tc         = "Runes " + std::to_string(newTC);
                break;
            }
        }
        if (countessMore) {
            if (countessRuneTC) {
                row.picks  = 5;
                row.noDrop = 1;
            }
            if (countessItemTC)
                row.picks = 1;
        }

        if (wraithOnlyRunes && className.find(") Wraith ") != std::string::npos) {
            ActAndDiff act;
            act.detect(className);
            int runesTc = 3 + act.getTotalAct();
            row.items.resize(1);
            row.items[0].tc = "Runes " + std::to_string(runesTc);
        }
    }
}

}
