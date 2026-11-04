#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "StructBaseCpp.hpp"

namespace D2ModGen {

[[maybe_unused]] static inline bool s_presetNamesSetter_unused = CommonStruct::setPresetNames({ "crazy", "strong", "weak", "pain" });

struct Config : public CommonStruct {
    struct Main : public CommonStruct {
        int64_t m_d2rLocaleId{ 0 };

        int64_t m_inputMode{ 0 };

        std::string m_inputPath{};

        std::string m_modname{ "rando" };

        int64_t m_outputMode{ 0 };

        std::string m_outputPath{};

        bool m_refreshSeed{ true };

        int64_t m_seed{ 0 };

        int64_t m_versionD2R{ 1 };

        int64_t m_versionMajor{ 1 };

        bool m_dumpIn{ false };

        bool m_dumpOut{ false };

        bool m_dumpDiff{ false };

        Main()
        {
            addScalar("d2rLocaleId", m_d2rLocaleId).setRange(0, 14).setZero(0);
            addScalar("inputMode", m_inputMode).setRange(0, 2).setZero(0);
            addScalar("inputPath", m_inputPath);
            addScalar("modname", m_modname).setZero("rando");
            addScalar("outputMode", m_outputMode).setRange(0, 3).setZero(0);
            addScalar("outputPath", m_outputPath);
            addScalar("refreshSeed", m_refreshSeed).setZero(true).setRecc(true);
            addScalar("seed", m_seed).setRange(0, 4294967295).setZero(0);
            addScalar("versionD2R", m_versionD2R).setRange(0, 1).setZero(1);
            addScalar("versionMajor", m_versionMajor).setRange(0, 1).setZero(1);
            addScalar("dumpIn", m_dumpIn).setZero(false);
            addScalar("dumpOut", m_dumpOut).setZero(false);
            addScalar("dumpDiff", m_dumpDiff).setZero(false);
        }
    };
    struct TreasureClasses : public CommonStruct {
        struct Runes : public CommonStruct {
            bool m_enabled{ false };

            int64_t m_countessSpecialRuneLevel{ 0 };

            bool m_countessSpecialRuneMore{ false };

            bool m_wraithOnlyRunes{ false };

            int64_t m_runeInGoodTC{ 100 };

            int64_t m_zodFactor{ 1 };

            Runes()
            {
                addScalar("enabled", m_enabled).setZero(false).setRecc(true);
                addScalar("countessSpecialRuneLevel", m_countessSpecialRuneLevel).setRange(0, 15).setZero(0).setRecc(3).addPreset(0, 2000).addPreset(1, 600).addPreset(2, 200);
                addScalar("countessSpecialRuneMore", m_countessSpecialRuneMore).setZero(false).setRecc(false).addPreset(0, true).addPreset(1, true);
                addScalar("wraithOnlyRunes", m_wraithOnlyRunes).setZero(false).setRecc(false).addPreset(0, true);
                addScalar("runeInGoodTC", m_runeInGoodTC).setRange(20, 1000).setZero(100).setPercent().setLogScale().addPreset(0, 1000).addPreset(1, 300).addPreset(2, 100);
                addScalar("zodFactor", m_zodFactor).setRange(1, 1000).setZero(1).setRecc(20).addPreset(0, 1000).addPreset(1, 100).addPreset(2, 10);
            }
        };
        struct Items : public CommonStruct {
            bool m_enabled{ false };

            bool m_forceQuestDrops{ false };

            int64_t m_chanceForHighLevel{ 100 };

            int64_t m_noneliteFactorIncrease{ 100 };

            int64_t m_rareFactorIncrease{ 100 };

            int64_t m_setFactorIncrease{ 100 };

            int64_t m_uniqueFactorIncrease{ 100 };

            int64_t m_inverseNodropPercent{ 100 };

            int64_t m_equipPercent{ 100 };

            int64_t m_goldPercent{ 100 };

            int64_t m_goodPercent{ 100 };

            int64_t m_junkPercent{ 100 };

            int64_t m_championItemCount{ 1 };

            int64_t m_uniqueItemCount{ 1 };

            int64_t m_superItemCount{ 2 };

            bool m_removeChampGold{ false };

            bool m_removeElitePotions{ false };

            Items()
            {
                addScalar("enabled", m_enabled).setZero(false).setRecc(true);
                addScalar("forceQuestDrops", m_forceQuestDrops).setZero(false).setRecc(true);
                addScalar("chanceForHighLevel", m_chanceForHighLevel).setRange(50, 2000).setZero(100).setRecc(200).setPercent().setLogScale();
                addScalar("noneliteFactorIncrease", m_noneliteFactorIncrease).setRange(20, 5000).setZero(100).setRecc(200).setPercent().setLogScale().addPreset(0, 1000).addPreset(1, 300).addPreset(2, 150);
                addScalar("rareFactorIncrease", m_rareFactorIncrease).setRange(20, 2000).setZero(100).setRecc(200).setPercent().setLogScale().addPreset(0, 2000).addPreset(1, 200).addPreset(2, 100);
                addScalar("setFactorIncrease", m_setFactorIncrease).setRange(20, 5000).setZero(100).setRecc(400).setPercent().setLogScale().addPreset(0, 5000).addPreset(1, 600).addPreset(2, 200);
                addScalar("uniqueFactorIncrease", m_uniqueFactorIncrease).setRange(20, 5000).setZero(100).setRecc(500).setPercent().setLogScale().addPreset(0, 5000).addPreset(1, 700).addPreset(2, 250);
                addScalar("inverseNodropPercent", m_inverseNodropPercent).setRange(25, 9100).setZero(100).setRecc(200).setPercent().setLogScale().addPreset(0, 9000).addPreset(1, 300);
                addScalar("equipPercent", m_equipPercent).setRange(20, 1000).setZero(100).setPercent().setLogScale();
                addScalar("goldPercent", m_goldPercent).setRange(5, 1000).setZero(100).setPercent().setLogScale();
                addScalar("goodPercent", m_goodPercent).setRange(25, 1000).setZero(100).setPercent().setLogScale();
                addScalar("junkPercent", m_junkPercent).setRange(5, 500).setZero(100).setRecc(10).setPercent().setLogScale();
                addScalar("championItemCount", m_championItemCount).setRange(0, 6).setZero(1).setRecc(2).addPreset(0, 6).addPreset(1, 3);
                addScalar("uniqueItemCount", m_uniqueItemCount).setRange(0, 6).setZero(1).setRecc(2).addPreset(0, 6).addPreset(1, 3);
                addScalar("superItemCount", m_superItemCount).setRange(0, 6).setZero(2).setRecc(3).addPreset(0, 6).addPreset(1, 4);
                addScalar("removeChampGold", m_removeChampGold).setZero(false).setRecc(true);
                addScalar("removeElitePotions", m_removeElitePotions).setZero(false).setRecc(false).addPreset(3, true);
            }
        };
        Runes m_runes{};

        Items m_items{};

        TreasureClasses()
        {
            addStruct("runes", m_runes);
            addStruct("items", m_items);
        }
    };
    struct ItemBalance : public CommonStruct {
        struct CommonSettings : public CommonStruct {
            bool m_doOverride{ false };

            int64_t m_rollMode{ 0 };

            int64_t m_itemLevel{ 100 };

            int64_t m_reqLevel{ 100 };

            int64_t m_randoKeepOriginalPercent{ 100 };

            int64_t m_randoGenerateMin{ 0 };

            int64_t m_randoGenerateMax{ 0 };

            int64_t m_powerScaleLevel1{ 100 };

            int64_t m_powerScaleLevel50{ 100 };

            CommonSettings()
            {
                addScalar("doOverride", m_doOverride).setZero(false).setRecc(false);
                addScalar("rollMode", m_rollMode).setRange(0, 3).setZero(0).setRecc(0).addPreset(0, 3).addPreset(1, 3);
                addScalar("itemLevel", m_itemLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
                addScalar("reqLevel", m_reqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
                addScalar("randoKeepOriginalPercent", m_randoKeepOriginalPercent).setRange(0, 100).setZero(100).setRecc(50).setPercent().addPreset(0, 25).addPreset(3, 0);
                addScalar("randoGenerateMin", m_randoGenerateMin).setRange(0, 1000).setZero(0).setRecc(50).setPercent().setLogScale().addPreset(0, 1000).addPreset(1, 60).addPreset(2, 40);
                addScalar("randoGenerateMax", m_randoGenerateMax).setRange(0, 1000).setZero(0).setRecc(100).setPercent().setLogScale().addPreset(0, 1000).addPreset(1, 200).addPreset(2, 120);
                addScalar("powerScaleLevel1", m_powerScaleLevel1).setRange(50, 500).setZero(100).setPercent().setLogScale();
                addScalar("powerScaleLevel50", m_powerScaleLevel50).setRange(50, 500).setZero(100).setPercent().setLogScale();
            }
        };
        CommonSettings m_defaults{};

        CommonSettings m_uniques{};

        CommonSettings m_setitems{};

        CommonSettings m_sets{};

        CommonSettings m_affixes{};

        CommonSettings m_crafted{};

        CommonSettings m_runewords{};

        CommonSettings m_gems{};

        CommonSettings m_equipment{};

        int64_t m_reqDex{ 100 };

        int64_t m_reqStr{ 100 };

        ItemBalance()
        {
            addStruct("defaults", m_defaults);
            addStruct("uniques", m_uniques);
            addStruct("setitems", m_setitems);
            addStruct("sets", m_sets);
            addStruct("affixes", m_affixes);
            addStruct("crafted", m_crafted);
            addStruct("runewords", m_runewords);
            addStruct("gems", m_gems);
            addStruct("equipment", m_equipment);
            addScalar("reqDex", m_reqDex).setRange(5, 150).setZero(100).setRecc(80).setPercent();
            addScalar("reqStr", m_reqStr).setRange(5, 150).setZero(100).setRecc(80).setPercent();
        }
    };
    struct ItemRandomizer : public CommonStruct {
        bool m_enabled{ false };

        int64_t m_forceCasterType{ 0 };

        int64_t m_itemFit{ 100 };

        int64_t m_levelMinus{ 0 };

        int64_t m_levelPlus{ 0 };

        int64_t m_repeatSets{ 1 };

        int64_t m_repeatUniques{ 1 };

        bool m_affixRandom{ false };

        bool m_gemsRandom{ false };

        bool m_craftedRandom{ false };

        bool m_removeKnock{ false };

        bool m_replaceCharges{ false };

        bool m_replaceSkills{ false };

        bool m_updateExisting{ false };

        int64_t m_setBonusMode{ 0 };

        int64_t m_frequencyAuras{ 100 };

        int64_t m_frequencyCharAttributes{ 100 };

        int64_t m_frequencyProcs{ 100 };

        int64_t m_frequencyResistances{ 100 };

        int64_t m_frequencySkills{ 100 };

        bool m_sameRarityForSetUniqueItems{ false };

        ItemRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(true);
            addScalar("forceCasterType", m_forceCasterType).setRange(0, 100).setZero(0).setRecc(25).setPercent();
            addScalar("itemFit", m_itemFit).setRange(0, 100).setZero(100).setRecc(90).setPercent().addPreset(0, 50).addPreset(1, 75);
            addScalar("levelMinus", m_levelMinus).setRange(0, 100).setZero(0).setRecc(10).addPreset(0, 0).addPreset(1, 5);
            addScalar("levelPlus", m_levelPlus).setRange(0, 100).setZero(0).setRecc(5).addPreset(0, 100).addPreset(1, 20);
            addScalar("repeatSets", m_repeatSets).setRange(1, 5).setZero(1).setRecc(5);
            addScalar("repeatUniques", m_repeatUniques).setRange(1, 5).setZero(1).setRecc(5);
            addScalar("affixRandom", m_affixRandom).setZero(false).setRecc(true).addPreset(3, false);
            addScalar("gemsRandom", m_gemsRandom).setZero(false);
            addScalar("craftedRandom", m_craftedRandom).setZero(false);
            addScalar("removeKnock", m_removeKnock).setZero(false).setRecc(true);
            addScalar("replaceCharges", m_replaceCharges).setZero(false).setRecc(true);
            addScalar("replaceSkills", m_replaceSkills).setZero(false);
            addScalar("updateExisting", m_updateExisting).setZero(false).setRecc(true);
            addScalar("setBonusMode", m_setBonusMode).setRange(0, 2).setZero(0).setRecc(1);
            addScalar("frequencyAuras", m_frequencyAuras).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(0, 1000);
            addScalar("frequencyCharAttributes", m_frequencyCharAttributes).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 50);
            addScalar("frequencyProcs", m_frequencyProcs).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(0, 1000);
            addScalar("frequencyResistances", m_frequencyResistances).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 50);
            addScalar("frequencySkills", m_frequencySkills).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(0, 1000);
            addScalar("sameRarityForSetUniqueItems", m_sameRarityForSetUniqueItems).setZero(false).setRecc(true);
        }
    };
    struct MonsterRandomizer : public CommonStruct {
        bool m_enabled{ false };

        int64_t m_numMonExtra{ 0 };

        int64_t m_spawnKeepOriginal{ 100 };

        int64_t m_guestsMin{ 0 };

        int64_t m_guestsMax{ 0 };

        int64_t m_chimeraAIMin{ 0 };

        int64_t m_chimeraAIMax{ 0 };

        int64_t m_rollResistancesMin{ 0 };

        int64_t m_rollResistancesMax{ 0 };

        int64_t m_rollStatMin{ 0 };

        int64_t m_rollStatMax{ 0 };

        int64_t m_densityMin{ 100 };

        int64_t m_densityMax{ 100 };

        int64_t m_elitePacksMin{ 100 };

        int64_t m_elitePacksMax{ 100 };

        int64_t m_monGroupIncreaseSize{ 0 };

        bool m_hellResistances{ false };

        bool m_hellPacks{ false };

        bool m_maxPacks{ false };

        MonsterRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(true);
            addScalar("numMonExtra", m_numMonExtra).setRange(-2, 10).setZero(0);
            addScalar("spawnKeepOriginal", m_spawnKeepOriginal).setRange(0, 100).setZero(100).setRecc(100).setPercent().addPreset(3, 50);
            addScalar("guestsMin", m_guestsMin).setRange(0, 1000).setZero(0).setRecc(25).setPercent().addPreset(3, 200);
            addScalar("guestsMax", m_guestsMax).setRange(0, 1000).setZero(0).setRecc(50).setPercent().addPreset(3, 200);
            addScalar("chimeraAIMin", m_chimeraAIMin).setRange(0, 100).setZero(0).setRecc(0).setPercent().addPreset(3, 0);
            addScalar("chimeraAIMax", m_chimeraAIMax).setRange(0, 100).setZero(0).setRecc(0).setPercent().addPreset(3, 0);
            addScalar("rollResistancesMin", m_rollResistancesMin).setRange(0, 100).setZero(0).setRecc(0).setPercent().addPreset(3, 100);
            addScalar("rollResistancesMax", m_rollResistancesMax).setRange(0, 100).setZero(0).setRecc(30).setPercent().addPreset(3, 100);
            addScalar("rollStatMin", m_rollStatMin).setRange(0, 100).setZero(0).setRecc(0).setPercent().addPreset(3, 100);
            addScalar("rollStatMax", m_rollStatMax).setRange(0, 100).setZero(0).setRecc(50).setPercent().addPreset(3, 100);
            addScalar("densityMin", m_densityMin).setRange(20, 1000).setZero(100).setRecc(100).setPercent().setLogScale().addPreset(0, 200);
            addScalar("densityMax", m_densityMax).setRange(20, 1000).setZero(100).setRecc(120).setPercent().setLogScale().addPreset(0, 300);
            addScalar("elitePacksMin", m_elitePacksMin).setRange(20, 1000).setZero(100).setRecc(50).setPercent().setLogScale().addPreset(0, 1000);
            addScalar("elitePacksMax", m_elitePacksMax).setRange(20, 1000).setZero(100).setRecc(100).setPercent().setLogScale().addPreset(0, 1000);
            addScalar("monGroupIncreaseSize", m_monGroupIncreaseSize).setRange(0, 20).setZero(0);
            addScalar("hellResistances", m_hellResistances).setZero(false).setRecc(false).addPreset(3, true);
            addScalar("hellPacks", m_hellPacks).setZero(false).setRecc(false).addPreset(3, true);
            addScalar("maxPacks", m_maxPacks).setZero(false).setRecc(true);
        }
    };
    struct SkillRandomizer : public CommonStruct {
        bool m_enabled{ false };

        bool m_ensureDifferent{ false };

        bool m_skillDamage{ false };

        bool m_skillTree{ false };

        SkillRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).addPreset(3, true);
            addScalar("ensureDifferent", m_ensureDifferent).setZero(false).setRecc(true);
            addScalar("skillDamage", m_skillDamage).setZero(false).setRecc(true);
            addScalar("skillTree", m_skillTree).setZero(false).setRecc(true);
        }
    };
    struct Challenge : public CommonStruct {
        int64_t m_levelIncreaseHell{ 0 };

        int64_t m_levelIncreaseNightmare{ 0 };

        bool m_levelIncreaseOverBound{ false };

        int64_t m_normalResPenalty{ 0 };

        int64_t m_nightmareResPenalty{ 40 };

        int64_t m_hellResPenalty{ 100 };

        bool m_disableDropsHealthPot{ false };

        bool m_disableDropsManaPot{ false };

        bool m_disableDropsRejuvPot{ false };

        Challenge()
        {
            addScalar("levelIncreaseHell", m_levelIncreaseHell).setRange(0, 30).setZero(0).setRecc(10);
            addScalar("levelIncreaseNightmare", m_levelIncreaseNightmare).setRange(0, 20).setZero(0).setRecc(5);
            addScalar("levelIncreaseOverBound", m_levelIncreaseOverBound).setZero(false);
            addScalar("normalResPenalty", m_normalResPenalty).setRange(0, 250).setZero(0).setRecc(10).addPreset(3, 30);
            addScalar("nightmareResPenalty", m_nightmareResPenalty).setRange(0, 250).setZero(40).setRecc(50).addPreset(3, 100);
            addScalar("hellResPenalty", m_hellResPenalty).setRange(0, 250).setZero(100).setRecc(120).addPreset(3, 250);
            addScalar("disableDropsHealthPot", m_disableDropsHealthPot).setZero(false);
            addScalar("disableDropsManaPot", m_disableDropsManaPot).setZero(false);
            addScalar("disableDropsRejuvPot", m_disableDropsRejuvPot).setZero(false).addPreset(3, true);
        }
    };
    struct Character : public CommonStruct {
        bool m_addCube{ false };

        bool m_addTeleport{ false };

        bool m_idTome{ false };

        bool m_tpTome{ false };

        bool m_manaPots{ false };

        int64_t m_mercDam{ 100 };

        int64_t m_mercHP{ 100 };

        int64_t m_skillPointLimit{ 20 };

        int64_t m_skillsReqLevel{ 100 };

        int64_t m_extraSkillPerLevel{ 0 };

        int64_t m_extraStatPerLevel{ 0 };

        int64_t m_extraHpPerLevel{ 0 };

        int64_t m_extraManaPerLevel{ 0 };

        int64_t m_hpPotEfficacy{ 100 };

        int64_t m_mpPotEfficacy{ 100 };

        Character()
        {
            addScalar("addCube", m_addCube).setZero(false).setRecc(true);
            addScalar("addTeleport", m_addTeleport).setZero(false).setRecc(true);
            addScalar("idTome", m_idTome).setZero(false).setRecc(true);
            addScalar("tpTome", m_tpTome).setZero(false).setRecc(true);
            addScalar("manaPots", m_manaPots).setZero(false).setRecc(false);
            addScalar("mercDam", m_mercDam).setRange(20, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mercHP", m_mercHP).setRange(20, 1000).setZero(100).setRecc(300).setPercent().setLogScale().addPreset(3, 20);
            addScalar("skillPointLimit", m_skillPointLimit).setRange(1, 60).setZero(20);
            addScalar("skillsReqLevel", m_skillsReqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("extraSkillPerLevel", m_extraSkillPerLevel).setRange(-1, 20).setZero(0).addPreset(3, true);
            addScalar("extraStatPerLevel", m_extraStatPerLevel).setRange(-5, 50).setZero(0).addPreset(3, -2);
            addScalar("extraHpPerLevel", m_extraHpPerLevel).setRange(-3, 30).setZero(0).addPreset(3, -1);
            addScalar("extraManaPerLevel", m_extraManaPerLevel).setRange(-3, 30).setZero(0).addPreset(3, -1);
            addScalar("hpPotEfficacy", m_hpPotEfficacy).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 50);
            addScalar("mpPotEfficacy", m_mpPotEfficacy).setRange(5, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 50);
        }
    };
    struct HoradricCube : public CommonStruct {
        bool m_cheatCharms{ false };

        bool m_craftHighIlvl{ false };

        bool m_craftNoRunes{ false };

        bool m_craftNoStrict{ false };

        bool m_noGemUpgrade{ false };

        bool m_quickPortals{ false };

        bool m_quickQuests{ false };

        bool m_socketing{ false };

        bool m_upgrading{ false };

        HoradricCube()
        {
            addScalar("cheatCharms", m_cheatCharms).setZero(false).setRecc(true);
            addScalar("craftHighIlvl", m_craftHighIlvl).setZero(false);
            addScalar("craftNoRunes", m_craftNoRunes).setZero(false);
            addScalar("craftNoStrict", m_craftNoStrict).setZero(false);
            addScalar("noGemUpgrade", m_noGemUpgrade).setZero(false);
            addScalar("quickPortals", m_quickPortals).setZero(false).setRecc(true);
            addScalar("quickQuests", m_quickQuests).setZero(false).setRecc(true);
            addScalar("socketing", m_socketing).setZero(false).setRecc(true);
            addScalar("upgrading", m_upgrading).setZero(false).setRecc(true);
        }
    };
    struct Gambling : public CommonStruct {
        bool m_charmGamble{ false };

        int64_t m_ratioElite{ 1 };

        int64_t m_ratioExc{ 1 };

        int64_t m_ratioRare{ 1 };

        int64_t m_ratioSet{ 1 };

        int64_t m_ratioUnique{ 1 };

        Gambling()
        {
            addScalar("charmGamble", m_charmGamble).setZero(false);
            addScalar("ratioElite", m_ratioElite).setRange(1, 10).setZero(1);
            addScalar("ratioExc", m_ratioExc).setRange(1, 4).setZero(1);
            addScalar("ratioRare", m_ratioRare).setRange(1, 4).setZero(1);
            addScalar("ratioSet", m_ratioSet).setRange(1, 250).setZero(1);
            addScalar("ratioUnique", m_ratioUnique).setRange(1, 500).setZero(1);
        }
    };
    struct MonsterStats : public CommonStruct {
        int64_t m_monsterAttack{ 100 };

        int64_t m_monsterDamage{ 100 };

        int64_t m_monsterDefense{ 100 };

        int64_t m_monsterHealth{ 100 };

        int64_t m_monsterExperience{ 100 };

        int64_t m_immunityLimit{ 250 };

        int64_t m_relativeResistDmg{ 100 };

        bool m_applyFire{ false };

        bool m_applyCold{ false };

        bool m_applyLight{ false };

        bool m_applyPois{ false };

        bool m_applyMag{ false };

        bool m_applyPhys{ false };

        MonsterStats()
        {
            addScalar("monsterAttack", m_monsterAttack).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("monsterDamage", m_monsterDamage).setRange(10, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 150);
            addScalar("monsterDefense", m_monsterDefense).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("monsterHealth", m_monsterHealth).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("monsterExperience", m_monsterExperience).setRange(10, 1000).setZero(100).setPercent().setLogScale().addPreset(3, 75);
            addScalar("immunityLimit", m_immunityLimit).setRange(90, 250).setZero(250);
            addScalar("relativeResistDmg", m_relativeResistDmg).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("applyFire", m_applyFire).setZero(false).setRecc(true);
            addScalar("applyCold", m_applyCold).setZero(false).setRecc(true);
            addScalar("applyLight", m_applyLight).setZero(false).setRecc(true);
            addScalar("applyPois", m_applyPois).setZero(false).setRecc(true);
            addScalar("applyMag", m_applyMag).setZero(false);
            addScalar("applyPhys", m_applyPhys).setZero(false);
        }
    };
    struct QualityOfLife : public CommonStruct {
        bool m_disableExpPenalty{ false };

        bool m_showItemLevel{ false };

        bool m_uniqueCharmLimit{ false };

        bool m_weakenTownSkills{ false };

        bool m_weakenTownSkillsExt{ false };

        int64_t m_tomeSize{ 20 };

        int64_t m_quiverSize{ 150 };

        int64_t m_keySize{ 12 };

        int64_t m_reduceCost{ 100 };

        bool m_compactPots{ false };

        bool m_compactScrolls{ false };

        QualityOfLife()
        {
            addScalar("disableExpPenalty", m_disableExpPenalty).setZero(false).setRecc(true);
            addScalar("showItemLevel", m_showItemLevel).setZero(false).setRecc(false);
            addScalar("uniqueCharmLimit", m_uniqueCharmLimit).setZero(false).setRecc(false);
            addScalar("weakenTownSkills", m_weakenTownSkills).setZero(false).setRecc(true);
            addScalar("weakenTownSkillsExt", m_weakenTownSkillsExt).setZero(false).setRecc(false);
            addScalar("tomeSize", m_tomeSize).setRange(2, 511).setZero(20).setRecc(60);
            addScalar("quiverSize", m_quiverSize).setRange(2, 511).setZero(150).setRecc(500);
            addScalar("keySize", m_keySize).setRange(2, 511).setZero(12).setRecc(50);
            addScalar("reduceCost", m_reduceCost).setRange(5, 100).setZero(100).setPercent();
            addScalar("compactPots", m_compactPots).setZero(false).setRecc(true);
            addScalar("compactScrolls", m_compactScrolls).setZero(false).setRecc(true);
        }
    };
    Main m_main{};

    TreasureClasses m_treasureClasses{};

    ItemBalance m_itemBalance{};

    ItemRandomizer m_itemRandomizer{};

    MonsterRandomizer m_monsterRandomizer{};

    SkillRandomizer m_skillRandomizer{};

    Challenge m_challenge{};

    Character m_character{};

    HoradricCube m_horadricCube{};

    Gambling m_gambling{};

    MonsterStats m_monsterStats{};

    QualityOfLife m_qualityOfLife{};

    Config()
    {
        addStruct("main", m_main);
        addStruct("treasureClasses", m_treasureClasses);
        addStruct("itemBalance", m_itemBalance);
        addStruct("itemRandomizer", m_itemRandomizer);
        addStruct("monsterRandomizer", m_monsterRandomizer);
        addStruct("skillRandomizer", m_skillRandomizer);
        addStruct("challenge", m_challenge);
        addStruct("character", m_character);
        addStruct("horadricCube", m_horadricCube);
        addStruct("gambling", m_gambling);
        addStruct("monsterStats", m_monsterStats);
        addStruct("qualityOfLife", m_qualityOfLife);
    }
};
}
