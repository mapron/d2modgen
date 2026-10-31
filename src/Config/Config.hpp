#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "StructBaseCpp.hpp"

namespace D2ModGen {

struct Config : public CommonStruct {
    struct EnabledModules : public CommonStruct {
        bool m_moduleChallenge{ false };

        bool m_moduleCharacter{ false };

        bool m_moduleCube{ false };

        bool m_moduleDropFiltering{ false };

        bool m_moduleGambling{ false };

        bool m_moduleItemDrops{ false };

        bool m_moduleItemRandomizer{ false };

        bool m_moduleMonDensity{ false };

        bool m_moduleMonRandomizer{ false };

        bool m_moduleMonStats{ false };

        bool m_modulePerfectRoll{ false };

        bool m_moduleQol{ false };

        bool m_moduleRequirements{ false };

        bool m_moduleRuneDrops{ false };

        bool m_moduleSkillRandomizer{ false };

        EnabledModules()
        {
            add("moduleChallenge", m_moduleChallenge, false).setParent(this).addPreset(0, true);
            add("moduleCharacter", m_moduleCharacter, false).setParent(this);
            add("moduleCube", m_moduleCube, false).setParent(this);
            add("moduleDropFiltering", m_moduleDropFiltering, false).setParent(this);
            add("moduleGambling", m_moduleGambling, false).setParent(this);
            add("moduleItemDrops", m_moduleItemDrops, false).setParent(this);
            add("moduleItemRandomizer", m_moduleItemRandomizer, false).setParent(this);
            add("moduleMonDensity", m_moduleMonDensity, false).setParent(this);
            add("moduleMonRandomizer", m_moduleMonRandomizer, false).setParent(this);
            add("moduleMonStats", m_moduleMonStats, false).setParent(this);
            add("modulePerfectRoll", m_modulePerfectRoll, false).setParent(this);
            add("moduleQol", m_moduleQol, false).setParent(this);
            add("moduleRequirements", m_moduleRequirements, false).setParent(this);
            add("moduleRuneDrops", m_moduleRuneDrops, false).setParent(this);
            add("moduleSkillRandomizer", m_moduleSkillRandomizer, false).setParent(this);
        }
    };
    struct ModuleMain : public CommonStruct {
        int64_t m_d2rLocaleId{ 0 };

        int64_t m_inputMode{ 0 };

        std::string m_inputPath{ "" };

        std::string m_modname{ "rando" };

        int64_t m_outputMode{ 0 };

        std::string m_outputPath{ "" };

        bool m_refreshSeed{ true };

        int64_t m_seed{ 0 };

        int64_t m_versionD2R{ 1 };

        int64_t m_versionMajor{ 1 };

        ModuleMain()
        {
            add("d2rLocaleId", m_d2rLocaleId, 0).setParent(this).setRange(0, 14);
            add("inputMode", m_inputMode, 0).setParent(this).setRange(0, 2);
            add("inputPath", m_inputPath, "").setParent(this);
            add("modname", m_modname, "rando").setParent(this);
            add("outputMode", m_outputMode, 0).setParent(this).setRange(0, 3);
            add("outputPath", m_outputPath, "").setParent(this);
            add("refreshSeed", m_refreshSeed, true).setParent(this);
            add("seed", m_seed, 0).setParent(this).setRange(0, 4294967295);
            add("versionD2R", m_versionD2R, 1).setParent(this).setRange(0, 1);
            add("versionMajor", m_versionMajor, 1).setParent(this).setRange(0, 1);
        }
    };
    struct ModuleChallenge : public CommonStruct {
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
            add("levelIncreaseHell", m_levelIncreaseHell, 0).setParent(this).setRange(0, 30);
            add("levelIncreaseNightmare", m_levelIncreaseNightmare, 0).setParent(this).setRange(0, 20);
            add("levelIncreaseUltra", m_levelIncreaseUltra, false).setParent(this);
            add("normal_minus_res", m_normal_minus_res, 0).setParent(this).setRange(0, 250);
            add("nightmare_minus_res", m_nightmare_minus_res, 40).setParent(this).setRange(0, 250);
            add("hell_minus_res", m_hell_minus_res, 100).setParent(this).setRange(0, 250);
            add("nodrop_hpsa", m_nodrop_hpsa, false).setParent(this);
            add("nodrop_mpsa", m_nodrop_mpsa, false).setParent(this);
            add("nodrop_rvs", m_nodrop_rvs, false).setParent(this);
        }
    };
    struct ModuleCharacter : public CommonStruct {
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
            add("addCube", m_addCube, false).setParent(this);
            add("addTeleport", m_addTeleport, false).setParent(this);
            add("idTome", m_idTome, false).setParent(this);
            add("manaPots", m_manaPots, false).setParent(this);
            add("mercDam", m_mercDam, 100).setParent(this).setRange(20, 1000).setPercent();
            add("mercHP", m_mercHP, 100).setParent(this).setRange(20, 1000).setPercent();
            add("skillPerLevel", m_skillPerLevel, 1).setParent(this).setRange(1, 20);
            add("skillPointLimit", m_skillPointLimit, 20).setParent(this).setRange(1, 60);
            add("statPerLevel", m_statPerLevel, 5).setParent(this).setRange(1, 25);
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
            add("cheatCharms", m_cheatCharms, false).setParent(this);
            add("craftHighIlvl", m_craftHighIlvl, false).setParent(this);
            add("craftNoRunes", m_craftNoRunes, false).setParent(this);
            add("craftNoStrict", m_craftNoStrict, false).setParent(this);
            add("noGemUpgrade", m_noGemUpgrade, false).setParent(this);
            add("quickPortals", m_quickPortals, true).setParent(this);
            add("quickQuests", m_quickQuests, true).setParent(this);
            add("socketing", m_socketing, false).setParent(this);
            add("upgrading", m_upgrading, false).setParent(this);
        }
    };
    struct ModuleDropFiltering : public CommonStruct {
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
            add("compact_pots", m_compact_pots, false).setParent(this);
            add("compact_scrolls", m_compact_scrolls, false).setParent(this);
            add("hide_ammo", m_hide_ammo, false).setParent(this);
            add("hide_hps", m_hide_hps, false).setParent(this);
            add("hide_hpsa", m_hide_hpsa, false).setParent(this);
            add("hide_isc", m_hide_isc, false).setParent(this);
            add("hide_junks", m_hide_junks, false).setParent(this);
            add("hide_lowq", m_hide_lowq, false).setParent(this);
            add("hide_mps", m_hide_mps, false).setParent(this);
            add("hide_mpsa", m_hide_mpsa, false).setParent(this);
            add("hide_rvl", m_hide_rvl, false).setParent(this);
            add("hide_rvs", m_hide_rvs, false).setParent(this);
            add("hide_stam", m_hide_stam, false).setParent(this);
            add("hide_tsc", m_hide_tsc, false).setParent(this);
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
            add("charmGamble", m_charmGamble, false).setParent(this);
            add("ratioElite", m_ratioElite, 1).setParent(this).setRange(1, 10);
            add("ratioExc", m_ratioExc, 1).setParent(this).setRange(1, 4);
            add("ratioRare", m_ratioRare, 1).setParent(this).setRange(1, 4);
            add("ratioSet", m_ratioSet, 1).setParent(this).setRange(1, 250);
            add("ratioUnique", m_ratioUnique, 1).setParent(this).setRange(1, 500);
        }
    };
    struct ModuleItemDrops : public CommonStruct {
        bool m_boss_quest_drops{ true };

        int64_t m_chance_highlevel{ 2 };

        int64_t m_chance_rare{ 3 };

        int64_t m_chance_set{ 5 };

        int64_t m_chance_uni{ 7 };

        int64_t m_drop_percent{ 100 };

        bool m_equal_uniques{ true };

        int64_t m_equip_percent{ 100 };

        int64_t m_gold_percent{ 100 };

        int64_t m_good_percent{ 100 };

        bool m_high_elite_drops{ true };

        int64_t m_junk_percent{ 10 };

        ModuleItemDrops()
        {
            add("boss_quest_drops", m_boss_quest_drops, true).setParent(this);
            add("chance_highlevel", m_chance_highlevel, 2).setParent(this).setRange(1, 15);
            add("chance_rare", m_chance_rare, 3).setParent(this).setRange(1, 15);
            add("chance_set", m_chance_set, 5).setParent(this).setRange(1, 30);
            add("chance_uni", m_chance_uni, 7).setParent(this).setRange(1, 50);
            add("drop_percent", m_drop_percent, 100).setParent(this).setRange(25, 9100).setPercent();
            add("equal_uniques", m_equal_uniques, true).setParent(this);
            add("equip_percent", m_equip_percent, 100).setParent(this).setRange(20, 1000).setPercent();
            add("gold_percent", m_gold_percent, 100).setParent(this).setRange(5, 1000);
            add("good_percent", m_good_percent, 100).setParent(this).setRange(25, 1000).setPercent();
            add("high_elite_drops", m_high_elite_drops, true).setParent(this);
            add("junk_percent", m_junk_percent, 10).setParent(this).setRange(5, 500).setPercent();
        }
    };
    struct ModuleItemRandomizer : public CommonStruct {
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
            add("affixRandom", m_affixRandom, true).setParent(this);
            add("casterTypePercent", m_casterTypePercent, 25).setParent(this).setRange(0, 100);
            add("frequencyAuras", m_frequencyAuras, 100).setParent(this).setRange(5, 1000).setPercent();
            add("frequencyCharAttributes", m_frequencyCharAttributes, 100).setParent(this).setRange(5, 1000).setPercent();
            add("frequencyProcs", m_frequencyProcs, 100).setParent(this).setRange(5, 1000).setPercent();
            add("frequencyResistances", m_frequencyResistances, 100).setParent(this).setRange(5, 1000).setPercent();
            add("frequencySkills", m_frequencySkills, 100).setParent(this).setRange(5, 1000).setPercent();
            add("gemsRandom", m_gemsRandom, false).setParent(this);
            add("itemFitPercent", m_itemFitPercent, 100).setParent(this).setRange(0, 100);
            add("keepOriginalPercent", m_keepOriginalPercent, 50).setParent(this).setRange(0, 100);
            add("levelMinus", m_levelMinus, 20).setParent(this).setRange(0, 100);
            add("levelPlus", m_levelPlus, 10).setParent(this).setRange(0, 100);
            add("relativeCountMax", m_relativeCountMax, 200).setParent(this).setRange(20, 1000).setPercent();
            add("relativeCountMin", m_relativeCountMin, 100).setParent(this).setRange(20, 1000).setPercent();
            add("removeKnock", m_removeKnock, true).setParent(this);
            add("repeat_sets", m_repeat_sets, 5).setParent(this).setRange(1, 5);
            add("repeat_uniques", m_repeat_uniques, 5).setParent(this).setRange(1, 5);
            add("replaceCharges", m_replaceCharges, true).setParent(this);
            add("replaceSkills", m_replaceSkills, false).setParent(this);
            add("setBonusMode", m_setBonusMode, 1).setParent(this).setRange(0, 2);
            add("setRelativeCountMax", m_setRelativeCountMax, 200).setParent(this).setRange(20, 1000).setPercent();
            add("setRelativeCountMin", m_setRelativeCountMin, 100).setParent(this).setRange(20, 1000).setPercent();
            add("updateExisting", m_updateExisting, true).setParent(this);
        }
    };
    struct ModuleMonDensity : public CommonStruct {
        int64_t m_density{ 1 };

        bool m_hellPacks{ false };

        int64_t m_mon_groups{ 0 };

        int64_t m_packs{ 1 };

        ModuleMonDensity()
        {
            add("density", m_density, 1).setParent(this).setRange(1, 20);
            add("hellPacks", m_hellPacks, false).setParent(this);
            add("mon_groups", m_mon_groups, 0).setParent(this).setRange(0, 20);
            add("packs", m_packs, 1).setParent(this).setRange(1, 20);
        }
    };
    struct ModuleMonRandomizer : public CommonStruct {
        bool m_hellResistances{ false };

        int64_t m_maxTypes{ 25 };

        bool m_randomizeResistances{ false };

        bool m_randomizeSpawns{ true };

        int64_t m_spawnedCount{ 5 };

        ModuleMonRandomizer()
        {
            add("hellResistances", m_hellResistances, false).setParent(this);
            add("maxTypes", m_maxTypes, 25).setParent(this).setRange(3, 25);
            add("randomizeResistances", m_randomizeResistances, false).setParent(this);
            add("randomizeSpawns", m_randomizeSpawns, true).setParent(this);
            add("spawnedCount", m_spawnedCount, 5).setParent(this).setRange(2, 10);
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
            add("mon_ar", m_mon_ar, 100).setParent(this).setRange(10, 1000).setPercent();
            add("mon_dam", m_mon_dam, 100).setParent(this).setRange(10, 1000).setPercent();
            add("mon_def", m_mon_def, 100).setParent(this).setRange(10, 1000).setPercent();
            add("mon_hp", m_mon_hp, 100).setParent(this).setRange(10, 1000).setPercent();
            add("mon_xp", m_mon_xp, 100).setParent(this).setRange(10, 1000).setPercent();
            add("max_resist", m_max_resist, 250).setParent(this).setRange(90, 250);
            add("rel_resist", m_rel_resist, 100).setParent(this).setRange(10, 1000).setPercent();
            add("resist_apply_elem", m_resist_apply_elem, true).setParent(this);
            add("resist_apply_magic", m_resist_apply_magic, false).setParent(this);
            add("resist_apply_physical", m_resist_apply_physical, false).setParent(this);
            add("resist_apply_poison", m_resist_apply_poison, true).setParent(this);
        }
    };
    struct ModulePerfectRoll : public CommonStruct {
        int64_t m_affix_power_l1{ 100 };

        int64_t m_affix_power_l50{ 100 };

        bool m_affixes{ true };

        bool m_armor{ true };

        bool m_craft{ true };

        bool m_crafted{ true };

        int64_t m_crafted_power{ 100 };

        int64_t m_gem_power{ 100 };

        bool m_runeWords{ true };

        int64_t m_rw_power{ 100 };

        bool m_setItems{ true };

        int64_t m_set_power_l1{ 100 };

        int64_t m_set_power_l50{ 100 };

        bool m_uniques{ true };

        int64_t m_uniques_power_l1{ 100 };

        int64_t m_uniques_power_l50{ 100 };

        ModulePerfectRoll()
        {
            add("affix_power_l1", m_affix_power_l1, 100).setParent(this).setRange(50, 500).setPercent();
            add("affix_power_l50", m_affix_power_l50, 100).setParent(this).setRange(50, 500).setPercent();
            add("affixes", m_affixes, true).setParent(this);
            add("armor", m_armor, true).setParent(this);
            add("craft", m_craft, true).setParent(this);
            add("crafted", m_crafted, true).setParent(this);
            add("crafted_power", m_crafted_power, 100).setParent(this).setRange(50, 500).setPercent();
            add("gem_power", m_gem_power, 100).setParent(this).setRange(50, 500).setPercent();
            add("runeWords", m_runeWords, true).setParent(this);
            add("rw_power", m_rw_power, 100).setParent(this).setRange(50, 500).setPercent();
            add("setItems", m_setItems, true).setParent(this);
            add("set_power_l1", m_set_power_l1, 100).setParent(this).setRange(50, 500);
            add("set_power_l50", m_set_power_l50, 100).setParent(this).setRange(50, 500);
            add("uniques", m_uniques, true).setParent(this);
            add("uniques_power_l1", m_uniques_power_l1, 100).setParent(this).setRange(50, 500).setPercent();
            add("uniques_power_l50", m_uniques_power_l50, 100).setParent(this).setRange(50, 500).setPercent();
        }
    };
    struct ModuleQol : public CommonStruct {
        bool m_disableExpPenalty{ false };

        bool m_keySize{ false };

        bool m_quiverSize{ false };

        int64_t m_reduceCost{ 100 };

        bool m_showItemLevel{ false };

        bool m_tomeSize{ false };

        bool m_uniqueCharmLimit{ false };

        bool m_weakenTownSkills{ false };

        bool m_weakenTownSkillsExt{ false };

        ModuleQol()
        {
            add("disableExpPenalty", m_disableExpPenalty, false).setParent(this);
            add("keySize", m_keySize, false).setParent(this);
            add("quiverSize", m_quiverSize, false).setParent(this);
            add("reduceCost", m_reduceCost, 100).setParent(this).setRange(5, 100);
            add("showItemLevel", m_showItemLevel, false).setParent(this);
            add("tomeSize", m_tomeSize, false).setParent(this);
            add("uniqueCharmLimit", m_uniqueCharmLimit, false).setParent(this);
            add("weakenTownSkills", m_weakenTownSkills, false).setParent(this);
            add("weakenTownSkillsExt", m_weakenTownSkillsExt, false).setParent(this);
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
            add("affixItemLevel", m_affixItemLevel, 100).setParent(this).setRange(5, 150);
            add("affixReqLevel", m_affixReqLevel, 100).setParent(this).setRange(5, 150);
            add("equipItemLevel", m_equipItemLevel, 100).setParent(this).setRange(5, 150);
            add("equipReqLevel", m_equipReqLevel, 100).setParent(this).setRange(5, 150);
            add("reqDex", m_reqDex, 100).setParent(this).setRange(5, 150);
            add("reqStr", m_reqStr, 100).setParent(this).setRange(5, 150);
            add("skillsReqLevel", m_skillsReqLevel, 100).setParent(this).setRange(5, 150);
            add("uniquesItemLevel", m_uniquesItemLevel, 100).setParent(this).setRange(5, 150);
            add("uniquesReqLevel", m_uniquesReqLevel, 100).setParent(this).setRange(5, 150);
        }
    };
    struct ModuleRuneDrops : public CommonStruct {
        bool m_countess_rune_higher{ false };

        bool m_countess_rune_more{ false };

        bool m_highrune_switch{ false };

        int64_t m_rune_factor{ 1 };

        bool m_wraith_runes{ false };

        int64_t m_zod_factor{ 1 };

        ModuleRuneDrops()
        {
            add("countess_rune_higher", m_countess_rune_higher, false).setParent(this);
            add("countess_rune_more", m_countess_rune_more, false).setParent(this);
            add("highrune_switch", m_highrune_switch, false).setParent(this);
            add("rune_factor", m_rune_factor, 1).setParent(this).setRange(1, 10);
            add("wraith_runes", m_wraith_runes, false).setParent(this);
            add("zod_factor", m_zod_factor, 1).setParent(this).setRange(1, 1000);
        }
    };
    struct ModuleSkillRandomizer : public CommonStruct {
        bool m_ensureDifferent{ true };

        bool m_skillDamage{ true };

        bool m_skillTree{ true };

        ModuleSkillRandomizer()
        {
            add("ensureDifferent", m_ensureDifferent, true).setParent(this);
            add("skillDamage", m_skillDamage, true).setParent(this);
            add("skillTree", m_skillTree, true).setParent(this);
        }
    };
    EnabledModules m_enabledModules{};

    ModuleMain m_moduleMain{};

    ModuleChallenge m_moduleChallenge{};

    ModuleCharacter m_moduleCharacter{};

    ModuleCube m_moduleCube{};

    ModuleDropFiltering m_moduleDropFiltering{};

    ModuleGambling m_moduleGambling{};

    ModuleItemDrops m_moduleItemDrops{};

    ModuleItemRandomizer m_moduleItemRandomizer{};

    ModuleMonDensity m_moduleMonDensity{};

    ModuleMonRandomizer m_moduleMonRandomizer{};

    ModuleMonStats m_moduleMonStats{};

    ModulePerfectRoll m_modulePerfectRoll{};

    ModuleQol m_moduleQol{};

    ModuleRequirements m_moduleRequirements{};

    ModuleRuneDrops m_moduleRuneDrops{};

    ModuleSkillRandomizer m_moduleSkillRandomizer{};

    Config()
    {
        add("enabledModules", m_enabledModules);
        add("moduleMain", m_moduleMain);
        add("moduleChallenge", m_moduleChallenge);
        add("moduleCharacter", m_moduleCharacter);
        add("moduleCube", m_moduleCube);
        add("moduleDropFiltering", m_moduleDropFiltering);
        add("moduleGambling", m_moduleGambling);
        add("moduleItemDrops", m_moduleItemDrops);
        add("moduleItemRandomizer", m_moduleItemRandomizer);
        add("moduleMonDensity", m_moduleMonDensity);
        add("moduleMonRandomizer", m_moduleMonRandomizer);
        add("moduleMonStats", m_moduleMonStats);
        add("modulePerfectRoll", m_modulePerfectRoll);
        add("moduleQol", m_moduleQol);
        add("moduleRequirements", m_moduleRequirements);
        add("moduleRuneDrops", m_moduleRuneDrops);
        add("moduleSkillRandomizer", m_moduleSkillRandomizer);
    }
};

static inline std::vector<std::string> s_presetNames{ "crazy" };
}
