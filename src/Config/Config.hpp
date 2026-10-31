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
            add("moduleChallenge", m_moduleChallenge, false).addPreset(0, true);
            add("moduleCharacter", m_moduleCharacter, false);
            add("moduleCube", m_moduleCube, false);
            add("moduleDropFiltering", m_moduleDropFiltering, false);
            add("moduleGambling", m_moduleGambling, false);
            add("moduleItemDrops", m_moduleItemDrops, false);
            add("moduleItemRandomizer", m_moduleItemRandomizer, false);
            add("moduleMonDensity", m_moduleMonDensity, false);
            add("moduleMonRandomizer", m_moduleMonRandomizer, false);
            add("moduleMonStats", m_moduleMonStats, false);
            add("modulePerfectRoll", m_modulePerfectRoll, false);
            add("moduleQol", m_moduleQol, false);
            add("moduleRequirements", m_moduleRequirements, false);
            add("moduleRuneDrops", m_moduleRuneDrops, false);
            add("moduleSkillRandomizer", m_moduleSkillRandomizer, false);
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
            add("d2rLocaleId", m_d2rLocaleId, 0).setRange(0, 9000);
            add("inputMode", m_inputMode, 0).setRange(0, 9000);
            add("inputPath", m_inputPath, "");
            add("modname", m_modname, "rando");
            add("outputMode", m_outputMode, 0).setRange(0, 9000);
            add("outputPath", m_outputPath, "");
            add("refreshSeed", m_refreshSeed, true);
            add("seed", m_seed, 0).setRange(0, 4294967295);
            add("versionD2R", m_versionD2R, 1).setRange(0, 9000);
            add("versionMajor", m_versionMajor, 1).setRange(0, 9000);
        }
    };
    struct ModuleChallenge : public CommonStruct {
        int64_t m_hell_minus_res{ 100 };

        int64_t m_levelIncreaseHell{ 0 };

        int64_t m_levelIncreaseNightmare{ 0 };

        bool m_levelIncreaseUltra{ false };

        int64_t m_nightmare_minus_res{ 40 };

        bool m_nodrop_hpsa{ false };

        bool m_nodrop_mpsa{ false };

        bool m_nodrop_rvs{ false };

        int64_t m_normal_minus_res{ 0 };

        ModuleChallenge()
        {
            add("hell_minus_res", m_hell_minus_res, 100).setRange(0, 9000);
            add("levelIncreaseHell", m_levelIncreaseHell, 0).setRange(0, 9000);
            add("levelIncreaseNightmare", m_levelIncreaseNightmare, 0).setRange(0, 9000);
            add("levelIncreaseUltra", m_levelIncreaseUltra, false);
            add("nightmare_minus_res", m_nightmare_minus_res, 40).setRange(0, 9000);
            add("nodrop_hpsa", m_nodrop_hpsa, false);
            add("nodrop_mpsa", m_nodrop_mpsa, false);
            add("nodrop_rvs", m_nodrop_rvs, false);
            add("normal_minus_res", m_normal_minus_res, 0).setRange(0, 9000);
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
            add("addCube", m_addCube, false);
            add("addTeleport", m_addTeleport, false);
            add("idTome", m_idTome, false);
            add("manaPots", m_manaPots, false);
            add("mercDam", m_mercDam, 100).setRange(0, 9000);
            add("mercHP", m_mercHP, 100).setRange(0, 9000);
            add("skillPerLevel", m_skillPerLevel, 1).setRange(0, 9000);
            add("skillPointLimit", m_skillPointLimit, 20).setRange(0, 9000);
            add("statPerLevel", m_statPerLevel, 5).setRange(0, 9000);
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
            add("cheatCharms", m_cheatCharms, false);
            add("craftHighIlvl", m_craftHighIlvl, false);
            add("craftNoRunes", m_craftNoRunes, false);
            add("craftNoStrict", m_craftNoStrict, false);
            add("noGemUpgrade", m_noGemUpgrade, false);
            add("quickPortals", m_quickPortals, true);
            add("quickQuests", m_quickQuests, true);
            add("socketing", m_socketing, false);
            add("upgrading", m_upgrading, false);
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
            add("compact_pots", m_compact_pots, false);
            add("compact_scrolls", m_compact_scrolls, false);
            add("hide_ammo", m_hide_ammo, false);
            add("hide_hps", m_hide_hps, false);
            add("hide_hpsa", m_hide_hpsa, false);
            add("hide_isc", m_hide_isc, false);
            add("hide_junks", m_hide_junks, false);
            add("hide_lowq", m_hide_lowq, false);
            add("hide_mps", m_hide_mps, false);
            add("hide_mpsa", m_hide_mpsa, false);
            add("hide_rvl", m_hide_rvl, false);
            add("hide_rvs", m_hide_rvs, false);
            add("hide_stam", m_hide_stam, false);
            add("hide_tsc", m_hide_tsc, false);
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
            add("charmGamble", m_charmGamble, false);
            add("ratioElite", m_ratioElite, 1).setRange(0, 9000);
            add("ratioExc", m_ratioExc, 1).setRange(0, 9000);
            add("ratioRare", m_ratioRare, 1).setRange(0, 9000);
            add("ratioSet", m_ratioSet, 1).setRange(0, 9000);
            add("ratioUnique", m_ratioUnique, 1).setRange(0, 9000);
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
            add("boss_quest_drops", m_boss_quest_drops, true);
            add("chance_highlevel", m_chance_highlevel, 2).setRange(0, 9000);
            add("chance_rare", m_chance_rare, 3).setRange(0, 9000);
            add("chance_set", m_chance_set, 5).setRange(0, 9000);
            add("chance_uni", m_chance_uni, 7).setRange(0, 9000);
            add("drop_percent", m_drop_percent, 100).setRange(0, 9000);
            add("equal_uniques", m_equal_uniques, true);
            add("equip_percent", m_equip_percent, 100).setRange(0, 9000);
            add("gold_percent", m_gold_percent, 100).setRange(0, 9000);
            add("good_percent", m_good_percent, 100).setRange(0, 9000);
            add("high_elite_drops", m_high_elite_drops, true);
            add("junk_percent", m_junk_percent, 10).setRange(0, 9000);
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
            add("affixRandom", m_affixRandom, true);
            add("casterTypePercent", m_casterTypePercent, 25).setRange(0, 9000);
            add("frequencyAuras", m_frequencyAuras, 100).setRange(0, 9000);
            add("frequencyCharAttributes", m_frequencyCharAttributes, 100).setRange(0, 9000);
            add("frequencyProcs", m_frequencyProcs, 100).setRange(0, 9000);
            add("frequencyResistances", m_frequencyResistances, 100).setRange(0, 9000);
            add("frequencySkills", m_frequencySkills, 100).setRange(0, 9000);
            add("gemsRandom", m_gemsRandom, false);
            add("itemFitPercent", m_itemFitPercent, 100).setRange(0, 9000);
            add("keepOriginalPercent", m_keepOriginalPercent, 50).setRange(0, 9000);
            add("levelMinus", m_levelMinus, 20).setRange(0, 9000);
            add("levelPlus", m_levelPlus, 10).setRange(0, 9000);
            add("relativeCountMax", m_relativeCountMax, 200).setRange(0, 9000);
            add("relativeCountMin", m_relativeCountMin, 100).setRange(0, 9000);
            add("removeKnock", m_removeKnock, true);
            add("repeat_sets", m_repeat_sets, 5).setRange(0, 9000);
            add("repeat_uniques", m_repeat_uniques, 5).setRange(0, 9000);
            add("replaceCharges", m_replaceCharges, true);
            add("replaceSkills", m_replaceSkills, false);
            add("setBonusMode", m_setBonusMode, 1).setRange(0, 9000);
            add("setRelativeCountMax", m_setRelativeCountMax, 200).setRange(0, 9000);
            add("setRelativeCountMin", m_setRelativeCountMin, 100).setRange(0, 9000);
            add("updateExisting", m_updateExisting, true);
        }
    };
    struct ModuleMonDensity : public CommonStruct {
        int64_t m_density{ 1 };

        bool m_hellPacks{ false };

        int64_t m_mon_groups{ 0 };

        int64_t m_packs{ 1 };

        ModuleMonDensity()
        {
            add("density", m_density, 1).setRange(0, 9000);
            add("hellPacks", m_hellPacks, false);
            add("mon_groups", m_mon_groups, 0).setRange(0, 9000);
            add("packs", m_packs, 1).setRange(0, 9000);
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
            add("hellResistances", m_hellResistances, false);
            add("maxTypes", m_maxTypes, 25).setRange(0, 9000);
            add("randomizeResistances", m_randomizeResistances, false);
            add("randomizeSpawns", m_randomizeSpawns, true);
            add("spawnedCount", m_spawnedCount, 5).setRange(0, 9000);
        }
    };
    struct ModuleMonStats : public CommonStruct {
        int64_t m_max_resist{ 250 };

        int64_t m_mon_ar{ 100 };

        int64_t m_mon_dam{ 100 };

        int64_t m_mon_def{ 100 };

        int64_t m_mon_hp{ 100 };

        int64_t m_mon_xp{ 100 };

        int64_t m_rel_resist{ 100 };

        bool m_resist_apply_elem{ true };

        bool m_resist_apply_magic{ false };

        bool m_resist_apply_physical{ false };

        bool m_resist_apply_poison{ true };

        ModuleMonStats()
        {
            add("max_resist", m_max_resist, 250).setRange(0, 9000);
            add("mon_ar", m_mon_ar, 100).setRange(0, 9000);
            add("mon_dam", m_mon_dam, 100).setRange(0, 9000);
            add("mon_def", m_mon_def, 100).setRange(0, 9000);
            add("mon_hp", m_mon_hp, 100).setRange(0, 9000);
            add("mon_xp", m_mon_xp, 100).setRange(0, 9000);
            add("rel_resist", m_rel_resist, 100).setRange(0, 9000);
            add("resist_apply_elem", m_resist_apply_elem, true);
            add("resist_apply_magic", m_resist_apply_magic, false);
            add("resist_apply_physical", m_resist_apply_physical, false);
            add("resist_apply_poison", m_resist_apply_poison, true);
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
            add("affix_power_l1", m_affix_power_l1, 100).setRange(0, 9000);
            add("affix_power_l50", m_affix_power_l50, 100).setRange(0, 9000);
            add("affixes", m_affixes, true);
            add("armor", m_armor, true);
            add("craft", m_craft, true);
            add("crafted", m_crafted, true);
            add("crafted_power", m_crafted_power, 100).setRange(0, 9000);
            add("gem_power", m_gem_power, 100).setRange(0, 9000);
            add("runeWords", m_runeWords, true);
            add("rw_power", m_rw_power, 100).setRange(0, 9000);
            add("setItems", m_setItems, true);
            add("set_power_l1", m_set_power_l1, 100).setRange(0, 9000);
            add("set_power_l50", m_set_power_l50, 100).setRange(0, 9000);
            add("uniques", m_uniques, true);
            add("uniques_power_l1", m_uniques_power_l1, 100).setRange(0, 9000);
            add("uniques_power_l50", m_uniques_power_l50, 100).setRange(0, 9000);
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
            add("disableExpPenalty", m_disableExpPenalty, false);
            add("keySize", m_keySize, false);
            add("quiverSize", m_quiverSize, false);
            add("reduceCost", m_reduceCost, 100).setRange(0, 9000);
            add("showItemLevel", m_showItemLevel, false);
            add("tomeSize", m_tomeSize, false);
            add("uniqueCharmLimit", m_uniqueCharmLimit, false);
            add("weakenTownSkills", m_weakenTownSkills, false);
            add("weakenTownSkillsExt", m_weakenTownSkillsExt, false);
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
            add("affixItemLevel", m_affixItemLevel, 100).setRange(0, 9000);
            add("affixReqLevel", m_affixReqLevel, 100).setRange(0, 9000);
            add("equipItemLevel", m_equipItemLevel, 100).setRange(0, 9000);
            add("equipReqLevel", m_equipReqLevel, 100).setRange(0, 9000);
            add("reqDex", m_reqDex, 100).setRange(0, 9000);
            add("reqStr", m_reqStr, 100).setRange(0, 9000);
            add("skillsReqLevel", m_skillsReqLevel, 100).setRange(0, 9000);
            add("uniquesItemLevel", m_uniquesItemLevel, 100).setRange(0, 9000);
            add("uniquesReqLevel", m_uniquesReqLevel, 100).setRange(0, 9000);
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
            add("countess_rune_higher", m_countess_rune_higher, false);
            add("countess_rune_more", m_countess_rune_more, false);
            add("highrune_switch", m_highrune_switch, false);
            add("rune_factor", m_rune_factor, 1).setRange(0, 9000);
            add("wraith_runes", m_wraith_runes, false);
            add("zod_factor", m_zod_factor, 1).setRange(0, 9000);
        }
    };
    struct ModuleSkillRandomizer : public CommonStruct {
        bool m_ensureDifferent{ true };

        bool m_skillDamage{ true };

        bool m_skillTree{ true };

        ModuleSkillRandomizer()
        {
            add("ensureDifferent", m_ensureDifferent, true);
            add("skillDamage", m_skillDamage, true);
            add("skillTree", m_skillTree, true);
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
