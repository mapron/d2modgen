#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "StructBaseCpp.hpp"

namespace D2ModGen {

struct Config : public CommonStruct {
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
                addScalar("enabled", m_enabled).setZero(false).setRecc(false);
                addScalar("countessSpecialRuneLevel", m_countessSpecialRuneLevel).setRange(0, 15).setZero(0).setRecc(3).addPreset(0, 5);
                addScalar("countessSpecialRuneMore", m_countessSpecialRuneMore).setZero(false);
                addScalar("wraithOnlyRunes", m_wraithOnlyRunes).setZero(false);
                addScalar("runeInGoodTC", m_runeInGoodTC).setRange(20, 1000).setZero(100).setPercent().setLogScale();
                addScalar("zodFactor", m_zodFactor).setRange(1, 1000).setZero(1);
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

            int64_t m_junkPercent{ 10 };

            int64_t m_championItemCount{ 1 };

            int64_t m_uniqueItemCount{ 1 };

            int64_t m_superItemCount{ 2 };

            bool m_removeChampGold{ false };

            bool m_removeElitePotions{ false };

            bool m_sameRarityForSetUniqueItems{ true };

            Items()
            {
                addScalar("enabled", m_enabled).setZero(false).setRecc(false);
                addScalar("forceQuestDrops", m_forceQuestDrops).setZero(false).setRecc(true);
                addScalar("chanceForHighLevel", m_chanceForHighLevel).setRange(50, 2000).setZero(100).setRecc(200).setPercent().setLogScale();
                addScalar("noneliteFactorIncrease", m_noneliteFactorIncrease).setRange(20, 5000).setZero(100).setRecc(500).setPercent().setLogScale();
                addScalar("rareFactorIncrease", m_rareFactorIncrease).setRange(20, 2000).setZero(100).setRecc(300).setPercent().setLogScale();
                addScalar("setFactorIncrease", m_setFactorIncrease).setRange(20, 5000).setZero(100).setRecc(500).setPercent().setLogScale();
                addScalar("uniqueFactorIncrease", m_uniqueFactorIncrease).setRange(20, 5000).setZero(100).setRecc(700).setPercent().setLogScale();
                addScalar("inverseNodropPercent", m_inverseNodropPercent).setRange(25, 9100).setZero(100).setPercent().setLogScale();
                addScalar("equipPercent", m_equipPercent).setRange(20, 1000).setZero(100).setPercent().setLogScale();
                addScalar("goldPercent", m_goldPercent).setRange(5, 1000).setZero(100).setPercent().setLogScale();
                addScalar("goodPercent", m_goodPercent).setRange(25, 1000).setZero(100).setPercent().setLogScale();
                addScalar("junkPercent", m_junkPercent).setRange(5, 500).setZero(10).setPercent().setLogScale();
                addScalar("championItemCount", m_championItemCount).setRange(0, 6).setZero(1);
                addScalar("uniqueItemCount", m_uniqueItemCount).setRange(0, 6).setZero(1);
                addScalar("superItemCount", m_superItemCount).setRange(0, 6).setZero(2);
                addScalar("removeChampGold", m_removeChampGold).setZero(false).setRecc(true);
                addScalar("removeElitePotions", m_removeElitePotions).setZero(false).setRecc(true);
                addScalar("sameRarityForSetUniqueItems", m_sameRarityForSetUniqueItems).setZero(true);
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
    struct ModuleMain : public CommonStruct {
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

        ModuleMain()
        {
            addScalar("d2rLocaleId", m_d2rLocaleId).setRange(0, 14).setZero(0);
            addScalar("inputMode", m_inputMode).setRange(0, 2).setZero(0);
            addScalar("inputPath", m_inputPath);
            addScalar("modname", m_modname).setZero("rando");
            addScalar("outputMode", m_outputMode).setRange(0, 3).setZero(0);
            addScalar("outputPath", m_outputPath);
            addScalar("refreshSeed", m_refreshSeed).setZero(true);
            addScalar("seed", m_seed).setRange(0, 4294967295).setZero(0);
            addScalar("versionD2R", m_versionD2R).setRange(0, 1).setZero(1);
            addScalar("versionMajor", m_versionMajor).setRange(0, 1).setZero(1);
        }
    };
    struct ModuleChallenge : public CommonStruct {
        bool m_enabled{ false };

        int64_t m_levelIncreaseHell{ 0 };

        int64_t m_levelIncreaseNightmare{ 0 };

        bool m_levelIncreaseUltra{ false };

        int64_t m_normal_minus_res{ 0 };

        int64_t m_nightmare_minus_res{ 40 };

        int64_t m_hell_minus_res{ 100 };

        bool m_nodrop_hpsa{ false };

        bool m_nodrop_mpsa{ false };

        bool m_nodrop_rvs{ false };

        ModuleChallenge()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(false);
            addScalar("levelIncreaseHell", m_levelIncreaseHell).setRange(0, 30).setZero(0);
            addScalar("levelIncreaseNightmare", m_levelIncreaseNightmare).setRange(0, 20).setZero(0);
            addScalar("levelIncreaseUltra", m_levelIncreaseUltra).setZero(false);
            addScalar("normal_minus_res", m_normal_minus_res).setRange(0, 250).setZero(0);
            addScalar("nightmare_minus_res", m_nightmare_minus_res).setRange(0, 250).setZero(40);
            addScalar("hell_minus_res", m_hell_minus_res).setRange(0, 250).setZero(100);
            addScalar("nodrop_hpsa", m_nodrop_hpsa).setZero(false);
            addScalar("nodrop_mpsa", m_nodrop_mpsa).setZero(false);
            addScalar("nodrop_rvs", m_nodrop_rvs).setZero(false);
        }
    };
    struct ModuleCharacter : public CommonStruct {
        bool m_enabled{ false };

        bool m_addCube{ false };

        bool m_addTeleport{ false };

        bool m_idTome{ false };

        bool m_manaPots{ false };

        int64_t m_mercDam{ 100 };

        int64_t m_mercHP{ 100 };

        int64_t m_skillPerLevel{ 1 };

        int64_t m_skillPointLimit{ 20 };

        int64_t m_statPerLevel{ 5 };

        ModuleCharacter()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(false);
            addScalar("addCube", m_addCube).setZero(false);
            addScalar("addTeleport", m_addTeleport).setZero(false);
            addScalar("idTome", m_idTome).setZero(false);
            addScalar("manaPots", m_manaPots).setZero(false);
            addScalar("mercDam", m_mercDam).setRange(20, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mercHP", m_mercHP).setRange(20, 1000).setZero(100).setPercent().setLogScale();
            addScalar("skillPerLevel", m_skillPerLevel).setRange(1, 20).setZero(1);
            addScalar("skillPointLimit", m_skillPointLimit).setRange(1, 60).setZero(20);
            addScalar("statPerLevel", m_statPerLevel).setRange(1, 25).setZero(5);
        }
    };
    struct ModuleCube : public CommonStruct {
        bool m_cheatCharms{ false };

        bool m_craftHighIlvl{ false };

        bool m_craftNoRunes{ false };

        bool m_craftNoStrict{ false };

        bool m_noGemUpgrade{ false };

        bool m_quickPortals{ true };

        bool m_quickQuests{ true };

        bool m_socketing{ false };

        bool m_upgrading{ false };

        ModuleCube()
        {
            addScalar("cheatCharms", m_cheatCharms).setZero(false);
            addScalar("craftHighIlvl", m_craftHighIlvl).setZero(false);
            addScalar("craftNoRunes", m_craftNoRunes).setZero(false);
            addScalar("craftNoStrict", m_craftNoStrict).setZero(false);
            addScalar("noGemUpgrade", m_noGemUpgrade).setZero(false);
            addScalar("quickPortals", m_quickPortals).setZero(true);
            addScalar("quickQuests", m_quickQuests).setZero(true);
            addScalar("socketing", m_socketing).setZero(false);
            addScalar("upgrading", m_upgrading).setZero(false);
        }
    };
    struct ModuleDropFiltering : public CommonStruct {
        bool m_enabled{ false };

        bool m_compact_pots{ false };

        bool m_compact_scrolls{ false };

        bool m_hide_ammo{ false };

        bool m_hide_hps{ false };

        bool m_hide_hpsa{ false };

        bool m_hide_isc{ false };

        bool m_hide_junks{ false };

        bool m_hide_lowq{ false };

        bool m_hide_mps{ false };

        bool m_hide_mpsa{ false };

        bool m_hide_rvl{ false };

        bool m_hide_rvs{ false };

        bool m_hide_stam{ false };

        bool m_hide_tsc{ false };

        ModuleDropFiltering()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(false);
            addScalar("compact_pots", m_compact_pots).setZero(false);
            addScalar("compact_scrolls", m_compact_scrolls).setZero(false);
            addScalar("hide_ammo", m_hide_ammo).setZero(false);
            addScalar("hide_hps", m_hide_hps).setZero(false);
            addScalar("hide_hpsa", m_hide_hpsa).setZero(false);
            addScalar("hide_isc", m_hide_isc).setZero(false);
            addScalar("hide_junks", m_hide_junks).setZero(false);
            addScalar("hide_lowq", m_hide_lowq).setZero(false);
            addScalar("hide_mps", m_hide_mps).setZero(false);
            addScalar("hide_mpsa", m_hide_mpsa).setZero(false);
            addScalar("hide_rvl", m_hide_rvl).setZero(false);
            addScalar("hide_rvs", m_hide_rvs).setZero(false);
            addScalar("hide_stam", m_hide_stam).setZero(false);
            addScalar("hide_tsc", m_hide_tsc).setZero(false);
        }
    };
    struct ModuleGambling : public CommonStruct {
        bool m_charmGamble{ false };

        int64_t m_ratioElite{ 1 };

        int64_t m_ratioExc{ 1 };

        int64_t m_ratioRare{ 1 };

        int64_t m_ratioSet{ 1 };

        int64_t m_ratioUnique{ 1 };

        ModuleGambling()
        {
            addScalar("charmGamble", m_charmGamble).setZero(false);
            addScalar("ratioElite", m_ratioElite).setRange(1, 10).setZero(1);
            addScalar("ratioExc", m_ratioExc).setRange(1, 4).setZero(1);
            addScalar("ratioRare", m_ratioRare).setRange(1, 4).setZero(1);
            addScalar("ratioSet", m_ratioSet).setRange(1, 250).setZero(1);
            addScalar("ratioUnique", m_ratioUnique).setRange(1, 500).setZero(1);
        }
    };
    struct ModuleItemRandomizer : public CommonStruct {
        bool m_enabled{ false };

        bool m_affixRandom{ true };

        int64_t m_casterTypePercent{ 25 };

        int64_t m_frequencyAuras{ 100 };

        int64_t m_frequencyCharAttributes{ 100 };

        int64_t m_frequencyProcs{ 100 };

        int64_t m_frequencyResistances{ 100 };

        int64_t m_frequencySkills{ 100 };

        bool m_gemsRandom{ false };

        int64_t m_itemFitPercent{ 100 };

        int64_t m_keepOriginalPercent{ 50 };

        int64_t m_levelMinus{ 20 };

        int64_t m_levelPlus{ 10 };

        int64_t m_relativeCountMax{ 200 };

        int64_t m_relativeCountMin{ 100 };

        bool m_removeKnock{ true };

        int64_t m_repeat_sets{ 5 };

        int64_t m_repeat_uniques{ 5 };

        bool m_replaceCharges{ true };

        bool m_replaceSkills{ false };

        int64_t m_setBonusMode{ 1 };

        int64_t m_setRelativeCountMax{ 200 };

        int64_t m_setRelativeCountMin{ 100 };

        bool m_updateExisting{ true };

        ModuleItemRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(true);
            addScalar("affixRandom", m_affixRandom).setZero(true);
            addScalar("casterTypePercent", m_casterTypePercent).setRange(0, 100).setZero(25).setPercent();
            addScalar("frequencyAuras", m_frequencyAuras).setRange(5, 1000).setZero(100).setPercent().setLogScale();
            addScalar("frequencyCharAttributes", m_frequencyCharAttributes).setRange(5, 1000).setZero(100).setPercent().setLogScale();
            addScalar("frequencyProcs", m_frequencyProcs).setRange(5, 1000).setZero(100).setPercent().setLogScale();
            addScalar("frequencyResistances", m_frequencyResistances).setRange(5, 1000).setZero(100).setPercent().setLogScale();
            addScalar("frequencySkills", m_frequencySkills).setRange(5, 1000).setZero(100).setPercent().setLogScale();
            addScalar("gemsRandom", m_gemsRandom).setZero(false);
            addScalar("itemFitPercent", m_itemFitPercent).setRange(0, 100).setZero(100).setPercent();
            addScalar("keepOriginalPercent", m_keepOriginalPercent).setRange(0, 100).setZero(50).setPercent();
            addScalar("levelMinus", m_levelMinus).setRange(0, 100).setZero(20);
            addScalar("levelPlus", m_levelPlus).setRange(0, 100).setZero(10);
            addScalar("relativeCountMax", m_relativeCountMax).setRange(20, 1000).setZero(200).setPercent().setLogScale();
            addScalar("relativeCountMin", m_relativeCountMin).setRange(20, 1000).setZero(100).setPercent().setLogScale();
            addScalar("removeKnock", m_removeKnock).setZero(true);
            addScalar("repeat_sets", m_repeat_sets).setRange(1, 5).setZero(5);
            addScalar("repeat_uniques", m_repeat_uniques).setRange(1, 5).setZero(5);
            addScalar("replaceCharges", m_replaceCharges).setZero(true);
            addScalar("replaceSkills", m_replaceSkills).setZero(false);
            addScalar("setBonusMode", m_setBonusMode).setRange(0, 2).setZero(1);
            addScalar("setRelativeCountMax", m_setRelativeCountMax).setRange(20, 1000).setZero(200).setPercent().setLogScale();
            addScalar("setRelativeCountMin", m_setRelativeCountMin).setRange(20, 1000).setZero(100).setPercent().setLogScale();
            addScalar("updateExisting", m_updateExisting).setZero(true);
        }
    };
    struct ModuleMonDensity : public CommonStruct {
        int64_t m_density{ 1 };

        bool m_hellPacks{ false };

        int64_t m_mon_groups{ 0 };

        int64_t m_packs{ 1 };

        ModuleMonDensity()
        {
            addScalar("density", m_density).setRange(1, 20).setZero(1);
            addScalar("hellPacks", m_hellPacks).setZero(false);
            addScalar("mon_groups", m_mon_groups).setRange(0, 20).setZero(0);
            addScalar("packs", m_packs).setRange(1, 20).setZero(1);
        }
    };
    struct ModuleMonRandomizer : public CommonStruct {
        bool m_enabled{ false };

        bool m_hellResistances{ false };

        int64_t m_maxTypes{ 25 };

        bool m_randomizeResistances{ false };

        bool m_randomizeSpawns{ true };

        int64_t m_spawnedCount{ 5 };

        ModuleMonRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(false);
            addScalar("hellResistances", m_hellResistances).setZero(false);
            addScalar("maxTypes", m_maxTypes).setRange(3, 25).setZero(25);
            addScalar("randomizeResistances", m_randomizeResistances).setZero(false);
            addScalar("randomizeSpawns", m_randomizeSpawns).setZero(true);
            addScalar("spawnedCount", m_spawnedCount).setRange(2, 10).setZero(5);
        }
    };
    struct ModuleMonStats : public CommonStruct {
        int64_t m_mon_ar{ 100 };

        int64_t m_mon_dam{ 100 };

        int64_t m_mon_def{ 100 };

        int64_t m_mon_hp{ 100 };

        int64_t m_mon_xp{ 100 };

        int64_t m_max_resist{ 250 };

        int64_t m_rel_resist{ 100 };

        bool m_resist_apply_elem{ true };

        bool m_resist_apply_magic{ false };

        bool m_resist_apply_physical{ false };

        bool m_resist_apply_poison{ true };

        ModuleMonStats()
        {
            addScalar("mon_ar", m_mon_ar).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mon_dam", m_mon_dam).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mon_def", m_mon_def).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mon_hp", m_mon_hp).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("mon_xp", m_mon_xp).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("max_resist", m_max_resist).setRange(90, 250).setZero(250);
            addScalar("rel_resist", m_rel_resist).setRange(10, 1000).setZero(100).setPercent().setLogScale();
            addScalar("resist_apply_elem", m_resist_apply_elem).setZero(true);
            addScalar("resist_apply_magic", m_resist_apply_magic).setZero(false);
            addScalar("resist_apply_physical", m_resist_apply_physical).setZero(false);
            addScalar("resist_apply_poison", m_resist_apply_poison).setZero(true);
        }
    };
    struct ModulePerfectRoll : public CommonStruct {
        int64_t m_affix_power_l1{ 100 };

        int64_t m_affix_power_l50{ 100 };

        bool m_affixes{ true };

        bool m_armor{ true };

        bool m_craft{ true };

        bool m_crafted{ true };

        bool m_runeWords{ true };

        bool m_setItems{ true };

        int64_t m_crafted_power{ 100 };

        int64_t m_gem_power{ 100 };

        int64_t m_rw_power{ 100 };

        int64_t m_set_power_l1{ 100 };

        int64_t m_set_power_l50{ 100 };

        bool m_uniques{ true };

        int64_t m_uniques_power_l1{ 100 };

        int64_t m_uniques_power_l50{ 100 };

        ModulePerfectRoll()
        {
            addScalar("affix_power_l1", m_affix_power_l1).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("affix_power_l50", m_affix_power_l50).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("affixes", m_affixes).setZero(true);
            addScalar("armor", m_armor).setZero(true);
            addScalar("craft", m_craft).setZero(true);
            addScalar("crafted", m_crafted).setZero(true);
            addScalar("runeWords", m_runeWords).setZero(true);
            addScalar("setItems", m_setItems).setZero(true);
            addScalar("crafted_power", m_crafted_power).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("gem_power", m_gem_power).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("rw_power", m_rw_power).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("set_power_l1", m_set_power_l1).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("set_power_l50", m_set_power_l50).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("uniques", m_uniques).setZero(true);
            addScalar("uniques_power_l1", m_uniques_power_l1).setRange(50, 500).setZero(100).setPercent().setLogScale();
            addScalar("uniques_power_l50", m_uniques_power_l50).setRange(50, 500).setZero(100).setPercent().setLogScale();
        }
    };
    struct ModuleQol : public CommonStruct {
        bool m_disableExpPenalty{ false };

        bool m_keySize{ false };

        bool m_quiverSize{ false };

        int64_t m_reduceCost{ 100 };

        bool m_showItemLevel{ false };

        int64_t m_tomeSize{ 20 };

        bool m_uniqueCharmLimit{ false };

        bool m_weakenTownSkills{ false };

        bool m_weakenTownSkillsExt{ false };

        ModuleQol()
        {
            addScalar("disableExpPenalty", m_disableExpPenalty).setZero(false).setRecc(true);
            addScalar("keySize", m_keySize).setZero(false).setRecc(true);
            addScalar("quiverSize", m_quiverSize).setZero(false);
            addScalar("reduceCost", m_reduceCost).setRange(5, 100).setZero(100).setPercent();
            addScalar("showItemLevel", m_showItemLevel).setZero(false).setRecc(false);
            addScalar("tomeSize", m_tomeSize).setRange(10, 100).setZero(20).setRecc(false).setPercent();
            addScalar("uniqueCharmLimit", m_uniqueCharmLimit).setZero(false).setRecc(false);
            addScalar("weakenTownSkills", m_weakenTownSkills).setZero(false).setRecc(true);
            addScalar("weakenTownSkillsExt", m_weakenTownSkillsExt).setZero(false).setRecc(false);
        }
    };
    struct ModuleRequirements : public CommonStruct {
        int64_t m_affixItemLevel{ 100 };

        int64_t m_affixReqLevel{ 100 };

        int64_t m_equipItemLevel{ 100 };

        int64_t m_equipReqLevel{ 100 };

        int64_t m_reqDex{ 100 };

        int64_t m_reqStr{ 100 };

        int64_t m_skillsReqLevel{ 100 };

        int64_t m_uniquesItemLevel{ 100 };

        int64_t m_uniquesReqLevel{ 100 };

        ModuleRequirements()
        {
            addScalar("affixItemLevel", m_affixItemLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("affixReqLevel", m_affixReqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("equipItemLevel", m_equipItemLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("equipReqLevel", m_equipReqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("reqDex", m_reqDex).setRange(5, 150).setZero(100).setRecc(80).setPercent();
            addScalar("reqStr", m_reqStr).setRange(5, 150).setZero(100).setRecc(80).setPercent();
            addScalar("skillsReqLevel", m_skillsReqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("uniquesItemLevel", m_uniquesItemLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
            addScalar("uniquesReqLevel", m_uniquesReqLevel).setRange(5, 150).setZero(100).setRecc(100).setPercent();
        }
    };
    struct ModuleSkillRandomizer : public CommonStruct {
        bool m_enabled{ false };

        bool m_ensureDifferent{ true };

        bool m_skillDamage{ true };

        bool m_skillTree{ true };

        ModuleSkillRandomizer()
        {
            addScalar("enabled", m_enabled).setZero(false).setRecc(false);
            addScalar("ensureDifferent", m_ensureDifferent).setZero(true);
            addScalar("skillDamage", m_skillDamage).setZero(true);
            addScalar("skillTree", m_skillTree).setZero(true);
        }
    };
    TreasureClasses m_treasureClasses{};

    ModuleMain m_moduleMain{};

    ModuleChallenge m_moduleChallenge{};

    ModuleCharacter m_moduleCharacter{};

    ModuleCube m_moduleCube{};

    ModuleDropFiltering m_moduleDropFiltering{};

    ModuleGambling m_moduleGambling{};

    ModuleItemRandomizer m_moduleItemRandomizer{};

    ModuleMonDensity m_moduleMonDensity{};

    ModuleMonRandomizer m_moduleMonRandomizer{};

    ModuleMonStats m_moduleMonStats{};

    ModulePerfectRoll m_modulePerfectRoll{};

    ModuleQol m_moduleQol{};

    ModuleRequirements m_moduleRequirements{};

    ModuleSkillRandomizer m_moduleSkillRandomizer{};

    Config()
    {
        addStruct("treasureClasses", m_treasureClasses);
        addStruct("moduleMain", m_moduleMain);
        addStruct("moduleChallenge", m_moduleChallenge);
        addStruct("moduleCharacter", m_moduleCharacter);
        addStruct("moduleCube", m_moduleCube);
        addStruct("moduleDropFiltering", m_moduleDropFiltering);
        addStruct("moduleGambling", m_moduleGambling);
        addStruct("moduleItemRandomizer", m_moduleItemRandomizer);
        addStruct("moduleMonDensity", m_moduleMonDensity);
        addStruct("moduleMonRandomizer", m_moduleMonRandomizer);
        addStruct("moduleMonStats", m_moduleMonStats);
        addStruct("modulePerfectRoll", m_modulePerfectRoll);
        addStruct("moduleQol", m_moduleQol);
        addStruct("moduleRequirements", m_moduleRequirements);
        addStruct("moduleSkillRandomizer", m_moduleSkillRandomizer);
    }
};

static inline std::vector<std::string> s_presetNames{ "crazy" };
}
