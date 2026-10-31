#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "Config.hpp"
#include "StructBaseQt.hpp"

namespace D2ModGen {

class Config_EnabledModules_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* moduleChallenge READ get_moduleChallenge CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleCharacter READ get_moduleCharacter CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleCube READ get_moduleCube CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleDropFiltering READ get_moduleDropFiltering CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleGambling READ get_moduleGambling CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleItemDrops READ get_moduleItemDrops CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleItemRandomizer READ get_moduleItemRandomizer CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleMonDensity READ get_moduleMonDensity CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleMonRandomizer READ get_moduleMonRandomizer CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleMonStats READ get_moduleMonStats CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* modulePerfectRoll READ get_modulePerfectRoll CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleQol READ get_moduleQol CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleRequirements READ get_moduleRequirements CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleRuneDrops READ get_moduleRuneDrops CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* moduleSkillRandomizer READ get_moduleSkillRandomizer CONSTANT)

public:
    explicit Config_EnabledModules_Qt(Config::EnabledModules& data)
        : m_data(data)
        , m_moduleChallenge(data.getPropertyBool(0))
        , m_moduleCharacter(data.getPropertyBool(1))
        , m_moduleCube(data.getPropertyBool(2))
        , m_moduleDropFiltering(data.getPropertyBool(3))
        , m_moduleGambling(data.getPropertyBool(4))
        , m_moduleItemDrops(data.getPropertyBool(5))
        , m_moduleItemRandomizer(data.getPropertyBool(6))
        , m_moduleMonDensity(data.getPropertyBool(7))
        , m_moduleMonRandomizer(data.getPropertyBool(8))
        , m_moduleMonStats(data.getPropertyBool(9))
        , m_modulePerfectRoll(data.getPropertyBool(10))
        , m_moduleQol(data.getPropertyBool(11))
        , m_moduleRequirements(data.getPropertyBool(12))
        , m_moduleRuneDrops(data.getPropertyBool(13))
        , m_moduleSkillRandomizer(data.getPropertyBool(14))
    {
    }

    PropertyBool_Qt* get_moduleChallenge() { return &m_moduleChallenge; }

    PropertyBool_Qt* get_moduleCharacter() { return &m_moduleCharacter; }

    PropertyBool_Qt* get_moduleCube() { return &m_moduleCube; }

    PropertyBool_Qt* get_moduleDropFiltering() { return &m_moduleDropFiltering; }

    PropertyBool_Qt* get_moduleGambling() { return &m_moduleGambling; }

    PropertyBool_Qt* get_moduleItemDrops() { return &m_moduleItemDrops; }

    PropertyBool_Qt* get_moduleItemRandomizer() { return &m_moduleItemRandomizer; }

    PropertyBool_Qt* get_moduleMonDensity() { return &m_moduleMonDensity; }

    PropertyBool_Qt* get_moduleMonRandomizer() { return &m_moduleMonRandomizer; }

    PropertyBool_Qt* get_moduleMonStats() { return &m_moduleMonStats; }

    PropertyBool_Qt* get_modulePerfectRoll() { return &m_modulePerfectRoll; }

    PropertyBool_Qt* get_moduleQol() { return &m_moduleQol; }

    PropertyBool_Qt* get_moduleRequirements() { return &m_moduleRequirements; }

    PropertyBool_Qt* get_moduleRuneDrops() { return &m_moduleRuneDrops; }

    PropertyBool_Qt* get_moduleSkillRandomizer() { return &m_moduleSkillRandomizer; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::EnabledModules& m_data;

    PropertyBool_Qt m_moduleChallenge;

    PropertyBool_Qt m_moduleCharacter;

    PropertyBool_Qt m_moduleCube;

    PropertyBool_Qt m_moduleDropFiltering;

    PropertyBool_Qt m_moduleGambling;

    PropertyBool_Qt m_moduleItemDrops;

    PropertyBool_Qt m_moduleItemRandomizer;

    PropertyBool_Qt m_moduleMonDensity;

    PropertyBool_Qt m_moduleMonRandomizer;

    PropertyBool_Qt m_moduleMonStats;

    PropertyBool_Qt m_modulePerfectRoll;

    PropertyBool_Qt m_moduleQol;

    PropertyBool_Qt m_moduleRequirements;

    PropertyBool_Qt m_moduleRuneDrops;

    PropertyBool_Qt m_moduleSkillRandomizer;
};

class Config_ModuleMain_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* d2rLocaleId READ get_d2rLocaleId CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* inputMode READ get_inputMode CONSTANT)
    Q_PROPERTY(PropertyString_Qt* inputPath READ get_inputPath CONSTANT)
    Q_PROPERTY(PropertyString_Qt* modname READ get_modname CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* outputMode READ get_outputMode CONSTANT)
    Q_PROPERTY(PropertyString_Qt* outputPath READ get_outputPath CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* refreshSeed READ get_refreshSeed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* seed READ get_seed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* versionD2R READ get_versionD2R CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* versionMajor READ get_versionMajor CONSTANT)

public:
    explicit Config_ModuleMain_Qt(Config::ModuleMain& data)
        : m_data(data)
        , m_d2rLocaleId(data.getPropertyInt(0))
        , m_inputMode(data.getPropertyInt(1))
        , m_inputPath(data.getPropertyString(0))
        , m_modname(data.getPropertyString(1))
        , m_outputMode(data.getPropertyInt(2))
        , m_outputPath(data.getPropertyString(2))
        , m_refreshSeed(data.getPropertyBool(0))
        , m_seed(data.getPropertyInt(3))
        , m_versionD2R(data.getPropertyInt(4))
        , m_versionMajor(data.getPropertyInt(5))
    {
    }

    PropertyInt_Qt* get_d2rLocaleId() { return &m_d2rLocaleId; }

    PropertyInt_Qt* get_inputMode() { return &m_inputMode; }

    PropertyString_Qt* get_inputPath() { return &m_inputPath; }

    PropertyString_Qt* get_modname() { return &m_modname; }

    PropertyInt_Qt* get_outputMode() { return &m_outputMode; }

    PropertyString_Qt* get_outputPath() { return &m_outputPath; }

    PropertyBool_Qt* get_refreshSeed() { return &m_refreshSeed; }

    PropertyInt_Qt* get_seed() { return &m_seed; }

    PropertyInt_Qt* get_versionD2R() { return &m_versionD2R; }

    PropertyInt_Qt* get_versionMajor() { return &m_versionMajor; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleMain& m_data;

    PropertyInt_Qt m_d2rLocaleId;

    PropertyInt_Qt m_inputMode;

    PropertyString_Qt m_inputPath;

    PropertyString_Qt m_modname;

    PropertyInt_Qt m_outputMode;

    PropertyString_Qt m_outputPath;

    PropertyBool_Qt m_refreshSeed;

    PropertyInt_Qt m_seed;

    PropertyInt_Qt m_versionD2R;

    PropertyInt_Qt m_versionMajor;
};

class Config_ModuleChallenge_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* levelIncreaseHell READ get_levelIncreaseHell CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelIncreaseNightmare READ get_levelIncreaseNightmare CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* levelIncreaseUltra READ get_levelIncreaseUltra CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* normal_minus_res READ get_normal_minus_res CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* nightmare_minus_res READ get_nightmare_minus_res CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* hell_minus_res READ get_hell_minus_res CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* nodrop_hpsa READ get_nodrop_hpsa CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* nodrop_mpsa READ get_nodrop_mpsa CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* nodrop_rvs READ get_nodrop_rvs CONSTANT)

public:
    explicit Config_ModuleChallenge_Qt(Config::ModuleChallenge& data)
        : m_data(data)
        , m_levelIncreaseHell(data.getPropertyInt(0))
        , m_levelIncreaseNightmare(data.getPropertyInt(1))
        , m_levelIncreaseUltra(data.getPropertyBool(0))
        , m_normal_minus_res(data.getPropertyInt(2))
        , m_nightmare_minus_res(data.getPropertyInt(3))
        , m_hell_minus_res(data.getPropertyInt(4))
        , m_nodrop_hpsa(data.getPropertyBool(1))
        , m_nodrop_mpsa(data.getPropertyBool(2))
        , m_nodrop_rvs(data.getPropertyBool(3))
    {
    }

    PropertyInt_Qt* get_levelIncreaseHell() { return &m_levelIncreaseHell; }

    PropertyInt_Qt* get_levelIncreaseNightmare() { return &m_levelIncreaseNightmare; }

    PropertyBool_Qt* get_levelIncreaseUltra() { return &m_levelIncreaseUltra; }

    PropertyInt_Qt* get_normal_minus_res() { return &m_normal_minus_res; }

    PropertyInt_Qt* get_nightmare_minus_res() { return &m_nightmare_minus_res; }

    PropertyInt_Qt* get_hell_minus_res() { return &m_hell_minus_res; }

    PropertyBool_Qt* get_nodrop_hpsa() { return &m_nodrop_hpsa; }

    PropertyBool_Qt* get_nodrop_mpsa() { return &m_nodrop_mpsa; }

    PropertyBool_Qt* get_nodrop_rvs() { return &m_nodrop_rvs; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleChallenge& m_data;

    PropertyInt_Qt m_levelIncreaseHell;

    PropertyInt_Qt m_levelIncreaseNightmare;

    PropertyBool_Qt m_levelIncreaseUltra;

    PropertyInt_Qt m_normal_minus_res;

    PropertyInt_Qt m_nightmare_minus_res;

    PropertyInt_Qt m_hell_minus_res;

    PropertyBool_Qt m_nodrop_hpsa;

    PropertyBool_Qt m_nodrop_mpsa;

    PropertyBool_Qt m_nodrop_rvs;
};

class Config_ModuleCharacter_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* addCube READ get_addCube CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* addTeleport READ get_addTeleport CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* idTome READ get_idTome CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* manaPots READ get_manaPots CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mercDam READ get_mercDam CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mercHP READ get_mercHP CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* skillPerLevel READ get_skillPerLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* skillPointLimit READ get_skillPointLimit CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* statPerLevel READ get_statPerLevel CONSTANT)

public:
    explicit Config_ModuleCharacter_Qt(Config::ModuleCharacter& data)
        : m_data(data)
        , m_addCube(data.getPropertyBool(0))
        , m_addTeleport(data.getPropertyBool(1))
        , m_idTome(data.getPropertyBool(2))
        , m_manaPots(data.getPropertyBool(3))
        , m_mercDam(data.getPropertyInt(0))
        , m_mercHP(data.getPropertyInt(1))
        , m_skillPerLevel(data.getPropertyInt(2))
        , m_skillPointLimit(data.getPropertyInt(3))
        , m_statPerLevel(data.getPropertyInt(4))
    {
    }

    PropertyBool_Qt* get_addCube() { return &m_addCube; }

    PropertyBool_Qt* get_addTeleport() { return &m_addTeleport; }

    PropertyBool_Qt* get_idTome() { return &m_idTome; }

    PropertyBool_Qt* get_manaPots() { return &m_manaPots; }

    PropertyInt_Qt* get_mercDam() { return &m_mercDam; }

    PropertyInt_Qt* get_mercHP() { return &m_mercHP; }

    PropertyInt_Qt* get_skillPerLevel() { return &m_skillPerLevel; }

    PropertyInt_Qt* get_skillPointLimit() { return &m_skillPointLimit; }

    PropertyInt_Qt* get_statPerLevel() { return &m_statPerLevel; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleCharacter& m_data;

    PropertyBool_Qt m_addCube;

    PropertyBool_Qt m_addTeleport;

    PropertyBool_Qt m_idTome;

    PropertyBool_Qt m_manaPots;

    PropertyInt_Qt m_mercDam;

    PropertyInt_Qt m_mercHP;

    PropertyInt_Qt m_skillPerLevel;

    PropertyInt_Qt m_skillPointLimit;

    PropertyInt_Qt m_statPerLevel;
};

class Config_ModuleCube_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* cheatCharms READ get_cheatCharms CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* craftHighIlvl READ get_craftHighIlvl CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* craftNoRunes READ get_craftNoRunes CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* craftNoStrict READ get_craftNoStrict CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* noGemUpgrade READ get_noGemUpgrade CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* quickPortals READ get_quickPortals CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* quickQuests READ get_quickQuests CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* socketing READ get_socketing CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* upgrading READ get_upgrading CONSTANT)

public:
    explicit Config_ModuleCube_Qt(Config::ModuleCube& data)
        : m_data(data)
        , m_cheatCharms(data.getPropertyBool(0))
        , m_craftHighIlvl(data.getPropertyBool(1))
        , m_craftNoRunes(data.getPropertyBool(2))
        , m_craftNoStrict(data.getPropertyBool(3))
        , m_noGemUpgrade(data.getPropertyBool(4))
        , m_quickPortals(data.getPropertyBool(5))
        , m_quickQuests(data.getPropertyBool(6))
        , m_socketing(data.getPropertyBool(7))
        , m_upgrading(data.getPropertyBool(8))
    {
    }

    PropertyBool_Qt* get_cheatCharms() { return &m_cheatCharms; }

    PropertyBool_Qt* get_craftHighIlvl() { return &m_craftHighIlvl; }

    PropertyBool_Qt* get_craftNoRunes() { return &m_craftNoRunes; }

    PropertyBool_Qt* get_craftNoStrict() { return &m_craftNoStrict; }

    PropertyBool_Qt* get_noGemUpgrade() { return &m_noGemUpgrade; }

    PropertyBool_Qt* get_quickPortals() { return &m_quickPortals; }

    PropertyBool_Qt* get_quickQuests() { return &m_quickQuests; }

    PropertyBool_Qt* get_socketing() { return &m_socketing; }

    PropertyBool_Qt* get_upgrading() { return &m_upgrading; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleCube& m_data;

    PropertyBool_Qt m_cheatCharms;

    PropertyBool_Qt m_craftHighIlvl;

    PropertyBool_Qt m_craftNoRunes;

    PropertyBool_Qt m_craftNoStrict;

    PropertyBool_Qt m_noGemUpgrade;

    PropertyBool_Qt m_quickPortals;

    PropertyBool_Qt m_quickQuests;

    PropertyBool_Qt m_socketing;

    PropertyBool_Qt m_upgrading;
};

class Config_ModuleDropFiltering_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* compact_pots READ get_compact_pots CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* compact_scrolls READ get_compact_scrolls CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_ammo READ get_hide_ammo CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_hps READ get_hide_hps CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_hpsa READ get_hide_hpsa CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_isc READ get_hide_isc CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_junks READ get_hide_junks CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_lowq READ get_hide_lowq CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_mps READ get_hide_mps CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_mpsa READ get_hide_mpsa CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_rvl READ get_hide_rvl CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_rvs READ get_hide_rvs CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_stam READ get_hide_stam CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hide_tsc READ get_hide_tsc CONSTANT)

public:
    explicit Config_ModuleDropFiltering_Qt(Config::ModuleDropFiltering& data)
        : m_data(data)
        , m_compact_pots(data.getPropertyBool(0))
        , m_compact_scrolls(data.getPropertyBool(1))
        , m_hide_ammo(data.getPropertyBool(2))
        , m_hide_hps(data.getPropertyBool(3))
        , m_hide_hpsa(data.getPropertyBool(4))
        , m_hide_isc(data.getPropertyBool(5))
        , m_hide_junks(data.getPropertyBool(6))
        , m_hide_lowq(data.getPropertyBool(7))
        , m_hide_mps(data.getPropertyBool(8))
        , m_hide_mpsa(data.getPropertyBool(9))
        , m_hide_rvl(data.getPropertyBool(10))
        , m_hide_rvs(data.getPropertyBool(11))
        , m_hide_stam(data.getPropertyBool(12))
        , m_hide_tsc(data.getPropertyBool(13))
    {
    }

    PropertyBool_Qt* get_compact_pots() { return &m_compact_pots; }

    PropertyBool_Qt* get_compact_scrolls() { return &m_compact_scrolls; }

    PropertyBool_Qt* get_hide_ammo() { return &m_hide_ammo; }

    PropertyBool_Qt* get_hide_hps() { return &m_hide_hps; }

    PropertyBool_Qt* get_hide_hpsa() { return &m_hide_hpsa; }

    PropertyBool_Qt* get_hide_isc() { return &m_hide_isc; }

    PropertyBool_Qt* get_hide_junks() { return &m_hide_junks; }

    PropertyBool_Qt* get_hide_lowq() { return &m_hide_lowq; }

    PropertyBool_Qt* get_hide_mps() { return &m_hide_mps; }

    PropertyBool_Qt* get_hide_mpsa() { return &m_hide_mpsa; }

    PropertyBool_Qt* get_hide_rvl() { return &m_hide_rvl; }

    PropertyBool_Qt* get_hide_rvs() { return &m_hide_rvs; }

    PropertyBool_Qt* get_hide_stam() { return &m_hide_stam; }

    PropertyBool_Qt* get_hide_tsc() { return &m_hide_tsc; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleDropFiltering& m_data;

    PropertyBool_Qt m_compact_pots;

    PropertyBool_Qt m_compact_scrolls;

    PropertyBool_Qt m_hide_ammo;

    PropertyBool_Qt m_hide_hps;

    PropertyBool_Qt m_hide_hpsa;

    PropertyBool_Qt m_hide_isc;

    PropertyBool_Qt m_hide_junks;

    PropertyBool_Qt m_hide_lowq;

    PropertyBool_Qt m_hide_mps;

    PropertyBool_Qt m_hide_mpsa;

    PropertyBool_Qt m_hide_rvl;

    PropertyBool_Qt m_hide_rvs;

    PropertyBool_Qt m_hide_stam;

    PropertyBool_Qt m_hide_tsc;
};

class Config_ModuleGambling_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* charmGamble READ get_charmGamble CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioElite READ get_ratioElite CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioExc READ get_ratioExc CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioRare READ get_ratioRare CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioSet READ get_ratioSet CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioUnique READ get_ratioUnique CONSTANT)

public:
    explicit Config_ModuleGambling_Qt(Config::ModuleGambling& data)
        : m_data(data)
        , m_charmGamble(data.getPropertyBool(0))
        , m_ratioElite(data.getPropertyInt(0))
        , m_ratioExc(data.getPropertyInt(1))
        , m_ratioRare(data.getPropertyInt(2))
        , m_ratioSet(data.getPropertyInt(3))
        , m_ratioUnique(data.getPropertyInt(4))
    {
    }

    PropertyBool_Qt* get_charmGamble() { return &m_charmGamble; }

    PropertyInt_Qt* get_ratioElite() { return &m_ratioElite; }

    PropertyInt_Qt* get_ratioExc() { return &m_ratioExc; }

    PropertyInt_Qt* get_ratioRare() { return &m_ratioRare; }

    PropertyInt_Qt* get_ratioSet() { return &m_ratioSet; }

    PropertyInt_Qt* get_ratioUnique() { return &m_ratioUnique; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleGambling& m_data;

    PropertyBool_Qt m_charmGamble;

    PropertyInt_Qt m_ratioElite;

    PropertyInt_Qt m_ratioExc;

    PropertyInt_Qt m_ratioRare;

    PropertyInt_Qt m_ratioSet;

    PropertyInt_Qt m_ratioUnique;
};

class Config_ModuleItemDrops_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* boss_quest_drops READ get_boss_quest_drops CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chance_highlevel READ get_chance_highlevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chance_rare READ get_chance_rare CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chance_set READ get_chance_set CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chance_uni READ get_chance_uni CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* drop_percent READ get_drop_percent CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* equal_uniques READ get_equal_uniques CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* equip_percent READ get_equip_percent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* gold_percent READ get_gold_percent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* good_percent READ get_good_percent CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* high_elite_drops READ get_high_elite_drops CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* junk_percent READ get_junk_percent CONSTANT)

public:
    explicit Config_ModuleItemDrops_Qt(Config::ModuleItemDrops& data)
        : m_data(data)
        , m_boss_quest_drops(data.getPropertyBool(0))
        , m_chance_highlevel(data.getPropertyInt(0))
        , m_chance_rare(data.getPropertyInt(1))
        , m_chance_set(data.getPropertyInt(2))
        , m_chance_uni(data.getPropertyInt(3))
        , m_drop_percent(data.getPropertyInt(4))
        , m_equal_uniques(data.getPropertyBool(1))
        , m_equip_percent(data.getPropertyInt(5))
        , m_gold_percent(data.getPropertyInt(6))
        , m_good_percent(data.getPropertyInt(7))
        , m_high_elite_drops(data.getPropertyBool(2))
        , m_junk_percent(data.getPropertyInt(8))
    {
    }

    PropertyBool_Qt* get_boss_quest_drops() { return &m_boss_quest_drops; }

    PropertyInt_Qt* get_chance_highlevel() { return &m_chance_highlevel; }

    PropertyInt_Qt* get_chance_rare() { return &m_chance_rare; }

    PropertyInt_Qt* get_chance_set() { return &m_chance_set; }

    PropertyInt_Qt* get_chance_uni() { return &m_chance_uni; }

    PropertyInt_Qt* get_drop_percent() { return &m_drop_percent; }

    PropertyBool_Qt* get_equal_uniques() { return &m_equal_uniques; }

    PropertyInt_Qt* get_equip_percent() { return &m_equip_percent; }

    PropertyInt_Qt* get_gold_percent() { return &m_gold_percent; }

    PropertyInt_Qt* get_good_percent() { return &m_good_percent; }

    PropertyBool_Qt* get_high_elite_drops() { return &m_high_elite_drops; }

    PropertyInt_Qt* get_junk_percent() { return &m_junk_percent; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleItemDrops& m_data;

    PropertyBool_Qt m_boss_quest_drops;

    PropertyInt_Qt m_chance_highlevel;

    PropertyInt_Qt m_chance_rare;

    PropertyInt_Qt m_chance_set;

    PropertyInt_Qt m_chance_uni;

    PropertyInt_Qt m_drop_percent;

    PropertyBool_Qt m_equal_uniques;

    PropertyInt_Qt m_equip_percent;

    PropertyInt_Qt m_gold_percent;

    PropertyInt_Qt m_good_percent;

    PropertyBool_Qt m_high_elite_drops;

    PropertyInt_Qt m_junk_percent;
};

class Config_ModuleItemRandomizer_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* affixRandom READ get_affixRandom CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* casterTypePercent READ get_casterTypePercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyAuras READ get_frequencyAuras CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyCharAttributes READ get_frequencyCharAttributes CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyProcs READ get_frequencyProcs CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyResistances READ get_frequencyResistances CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencySkills READ get_frequencySkills CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* gemsRandom READ get_gemsRandom CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* itemFitPercent READ get_itemFitPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* keepOriginalPercent READ get_keepOriginalPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelMinus READ get_levelMinus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelPlus READ get_levelPlus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* relativeCountMax READ get_relativeCountMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* relativeCountMin READ get_relativeCountMin CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* removeKnock READ get_removeKnock CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeat_sets READ get_repeat_sets CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeat_uniques READ get_repeat_uniques CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* replaceCharges READ get_replaceCharges CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* replaceSkills READ get_replaceSkills CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* setBonusMode READ get_setBonusMode CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* setRelativeCountMax READ get_setRelativeCountMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* setRelativeCountMin READ get_setRelativeCountMin CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* updateExisting READ get_updateExisting CONSTANT)

public:
    explicit Config_ModuleItemRandomizer_Qt(Config::ModuleItemRandomizer& data)
        : m_data(data)
        , m_affixRandom(data.getPropertyBool(0))
        , m_casterTypePercent(data.getPropertyInt(0))
        , m_frequencyAuras(data.getPropertyInt(1))
        , m_frequencyCharAttributes(data.getPropertyInt(2))
        , m_frequencyProcs(data.getPropertyInt(3))
        , m_frequencyResistances(data.getPropertyInt(4))
        , m_frequencySkills(data.getPropertyInt(5))
        , m_gemsRandom(data.getPropertyBool(1))
        , m_itemFitPercent(data.getPropertyInt(6))
        , m_keepOriginalPercent(data.getPropertyInt(7))
        , m_levelMinus(data.getPropertyInt(8))
        , m_levelPlus(data.getPropertyInt(9))
        , m_relativeCountMax(data.getPropertyInt(10))
        , m_relativeCountMin(data.getPropertyInt(11))
        , m_removeKnock(data.getPropertyBool(2))
        , m_repeat_sets(data.getPropertyInt(12))
        , m_repeat_uniques(data.getPropertyInt(13))
        , m_replaceCharges(data.getPropertyBool(3))
        , m_replaceSkills(data.getPropertyBool(4))
        , m_setBonusMode(data.getPropertyInt(14))
        , m_setRelativeCountMax(data.getPropertyInt(15))
        , m_setRelativeCountMin(data.getPropertyInt(16))
        , m_updateExisting(data.getPropertyBool(5))
    {
    }

    PropertyBool_Qt* get_affixRandom() { return &m_affixRandom; }

    PropertyInt_Qt* get_casterTypePercent() { return &m_casterTypePercent; }

    PropertyInt_Qt* get_frequencyAuras() { return &m_frequencyAuras; }

    PropertyInt_Qt* get_frequencyCharAttributes() { return &m_frequencyCharAttributes; }

    PropertyInt_Qt* get_frequencyProcs() { return &m_frequencyProcs; }

    PropertyInt_Qt* get_frequencyResistances() { return &m_frequencyResistances; }

    PropertyInt_Qt* get_frequencySkills() { return &m_frequencySkills; }

    PropertyBool_Qt* get_gemsRandom() { return &m_gemsRandom; }

    PropertyInt_Qt* get_itemFitPercent() { return &m_itemFitPercent; }

    PropertyInt_Qt* get_keepOriginalPercent() { return &m_keepOriginalPercent; }

    PropertyInt_Qt* get_levelMinus() { return &m_levelMinus; }

    PropertyInt_Qt* get_levelPlus() { return &m_levelPlus; }

    PropertyInt_Qt* get_relativeCountMax() { return &m_relativeCountMax; }

    PropertyInt_Qt* get_relativeCountMin() { return &m_relativeCountMin; }

    PropertyBool_Qt* get_removeKnock() { return &m_removeKnock; }

    PropertyInt_Qt* get_repeat_sets() { return &m_repeat_sets; }

    PropertyInt_Qt* get_repeat_uniques() { return &m_repeat_uniques; }

    PropertyBool_Qt* get_replaceCharges() { return &m_replaceCharges; }

    PropertyBool_Qt* get_replaceSkills() { return &m_replaceSkills; }

    PropertyInt_Qt* get_setBonusMode() { return &m_setBonusMode; }

    PropertyInt_Qt* get_setRelativeCountMax() { return &m_setRelativeCountMax; }

    PropertyInt_Qt* get_setRelativeCountMin() { return &m_setRelativeCountMin; }

    PropertyBool_Qt* get_updateExisting() { return &m_updateExisting; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleItemRandomizer& m_data;

    PropertyBool_Qt m_affixRandom;

    PropertyInt_Qt m_casterTypePercent;

    PropertyInt_Qt m_frequencyAuras;

    PropertyInt_Qt m_frequencyCharAttributes;

    PropertyInt_Qt m_frequencyProcs;

    PropertyInt_Qt m_frequencyResistances;

    PropertyInt_Qt m_frequencySkills;

    PropertyBool_Qt m_gemsRandom;

    PropertyInt_Qt m_itemFitPercent;

    PropertyInt_Qt m_keepOriginalPercent;

    PropertyInt_Qt m_levelMinus;

    PropertyInt_Qt m_levelPlus;

    PropertyInt_Qt m_relativeCountMax;

    PropertyInt_Qt m_relativeCountMin;

    PropertyBool_Qt m_removeKnock;

    PropertyInt_Qt m_repeat_sets;

    PropertyInt_Qt m_repeat_uniques;

    PropertyBool_Qt m_replaceCharges;

    PropertyBool_Qt m_replaceSkills;

    PropertyInt_Qt m_setBonusMode;

    PropertyInt_Qt m_setRelativeCountMax;

    PropertyInt_Qt m_setRelativeCountMin;

    PropertyBool_Qt m_updateExisting;
};

class Config_ModuleMonDensity_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* density READ get_density CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hellPacks READ get_hellPacks CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mon_groups READ get_mon_groups CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* packs READ get_packs CONSTANT)

public:
    explicit Config_ModuleMonDensity_Qt(Config::ModuleMonDensity& data)
        : m_data(data)
        , m_density(data.getPropertyInt(0))
        , m_hellPacks(data.getPropertyBool(0))
        , m_mon_groups(data.getPropertyInt(1))
        , m_packs(data.getPropertyInt(2))
    {
    }

    PropertyInt_Qt* get_density() { return &m_density; }

    PropertyBool_Qt* get_hellPacks() { return &m_hellPacks; }

    PropertyInt_Qt* get_mon_groups() { return &m_mon_groups; }

    PropertyInt_Qt* get_packs() { return &m_packs; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleMonDensity& m_data;

    PropertyInt_Qt m_density;

    PropertyBool_Qt m_hellPacks;

    PropertyInt_Qt m_mon_groups;

    PropertyInt_Qt m_packs;
};

class Config_ModuleMonRandomizer_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* hellResistances READ get_hellResistances CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* maxTypes READ get_maxTypes CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* randomizeResistances READ get_randomizeResistances CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* randomizeSpawns READ get_randomizeSpawns CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* spawnedCount READ get_spawnedCount CONSTANT)

public:
    explicit Config_ModuleMonRandomizer_Qt(Config::ModuleMonRandomizer& data)
        : m_data(data)
        , m_hellResistances(data.getPropertyBool(0))
        , m_maxTypes(data.getPropertyInt(0))
        , m_randomizeResistances(data.getPropertyBool(1))
        , m_randomizeSpawns(data.getPropertyBool(2))
        , m_spawnedCount(data.getPropertyInt(1))
    {
    }

    PropertyBool_Qt* get_hellResistances() { return &m_hellResistances; }

    PropertyInt_Qt* get_maxTypes() { return &m_maxTypes; }

    PropertyBool_Qt* get_randomizeResistances() { return &m_randomizeResistances; }

    PropertyBool_Qt* get_randomizeSpawns() { return &m_randomizeSpawns; }

    PropertyInt_Qt* get_spawnedCount() { return &m_spawnedCount; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleMonRandomizer& m_data;

    PropertyBool_Qt m_hellResistances;

    PropertyInt_Qt m_maxTypes;

    PropertyBool_Qt m_randomizeResistances;

    PropertyBool_Qt m_randomizeSpawns;

    PropertyInt_Qt m_spawnedCount;
};

class Config_ModuleMonStats_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* mon_ar READ get_mon_ar CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mon_dam READ get_mon_dam CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mon_def READ get_mon_def CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mon_hp READ get_mon_hp CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mon_xp READ get_mon_xp CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* max_resist READ get_max_resist CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rel_resist READ get_rel_resist CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* resist_apply_elem READ get_resist_apply_elem CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* resist_apply_magic READ get_resist_apply_magic CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* resist_apply_physical READ get_resist_apply_physical CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* resist_apply_poison READ get_resist_apply_poison CONSTANT)

public:
    explicit Config_ModuleMonStats_Qt(Config::ModuleMonStats& data)
        : m_data(data)
        , m_mon_ar(data.getPropertyInt(0))
        , m_mon_dam(data.getPropertyInt(1))
        , m_mon_def(data.getPropertyInt(2))
        , m_mon_hp(data.getPropertyInt(3))
        , m_mon_xp(data.getPropertyInt(4))
        , m_max_resist(data.getPropertyInt(5))
        , m_rel_resist(data.getPropertyInt(6))
        , m_resist_apply_elem(data.getPropertyBool(0))
        , m_resist_apply_magic(data.getPropertyBool(1))
        , m_resist_apply_physical(data.getPropertyBool(2))
        , m_resist_apply_poison(data.getPropertyBool(3))
    {
    }

    PropertyInt_Qt* get_mon_ar() { return &m_mon_ar; }

    PropertyInt_Qt* get_mon_dam() { return &m_mon_dam; }

    PropertyInt_Qt* get_mon_def() { return &m_mon_def; }

    PropertyInt_Qt* get_mon_hp() { return &m_mon_hp; }

    PropertyInt_Qt* get_mon_xp() { return &m_mon_xp; }

    PropertyInt_Qt* get_max_resist() { return &m_max_resist; }

    PropertyInt_Qt* get_rel_resist() { return &m_rel_resist; }

    PropertyBool_Qt* get_resist_apply_elem() { return &m_resist_apply_elem; }

    PropertyBool_Qt* get_resist_apply_magic() { return &m_resist_apply_magic; }

    PropertyBool_Qt* get_resist_apply_physical() { return &m_resist_apply_physical; }

    PropertyBool_Qt* get_resist_apply_poison() { return &m_resist_apply_poison; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleMonStats& m_data;

    PropertyInt_Qt m_mon_ar;

    PropertyInt_Qt m_mon_dam;

    PropertyInt_Qt m_mon_def;

    PropertyInt_Qt m_mon_hp;

    PropertyInt_Qt m_mon_xp;

    PropertyInt_Qt m_max_resist;

    PropertyInt_Qt m_rel_resist;

    PropertyBool_Qt m_resist_apply_elem;

    PropertyBool_Qt m_resist_apply_magic;

    PropertyBool_Qt m_resist_apply_physical;

    PropertyBool_Qt m_resist_apply_poison;
};

class Config_ModulePerfectRoll_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* affix_power_l1 READ get_affix_power_l1 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* affix_power_l50 READ get_affix_power_l50 CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* affixes READ get_affixes CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* armor READ get_armor CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* craft READ get_craft CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* crafted READ get_crafted CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* crafted_power READ get_crafted_power CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* gem_power READ get_gem_power CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* runeWords READ get_runeWords CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rw_power READ get_rw_power CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* setItems READ get_setItems CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* set_power_l1 READ get_set_power_l1 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* set_power_l50 READ get_set_power_l50 CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* uniques READ get_uniques CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniques_power_l1 READ get_uniques_power_l1 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniques_power_l50 READ get_uniques_power_l50 CONSTANT)

public:
    explicit Config_ModulePerfectRoll_Qt(Config::ModulePerfectRoll& data)
        : m_data(data)
        , m_affix_power_l1(data.getPropertyInt(0))
        , m_affix_power_l50(data.getPropertyInt(1))
        , m_affixes(data.getPropertyBool(0))
        , m_armor(data.getPropertyBool(1))
        , m_craft(data.getPropertyBool(2))
        , m_crafted(data.getPropertyBool(3))
        , m_crafted_power(data.getPropertyInt(2))
        , m_gem_power(data.getPropertyInt(3))
        , m_runeWords(data.getPropertyBool(4))
        , m_rw_power(data.getPropertyInt(4))
        , m_setItems(data.getPropertyBool(5))
        , m_set_power_l1(data.getPropertyInt(5))
        , m_set_power_l50(data.getPropertyInt(6))
        , m_uniques(data.getPropertyBool(6))
        , m_uniques_power_l1(data.getPropertyInt(7))
        , m_uniques_power_l50(data.getPropertyInt(8))
    {
    }

    PropertyInt_Qt* get_affix_power_l1() { return &m_affix_power_l1; }

    PropertyInt_Qt* get_affix_power_l50() { return &m_affix_power_l50; }

    PropertyBool_Qt* get_affixes() { return &m_affixes; }

    PropertyBool_Qt* get_armor() { return &m_armor; }

    PropertyBool_Qt* get_craft() { return &m_craft; }

    PropertyBool_Qt* get_crafted() { return &m_crafted; }

    PropertyInt_Qt* get_crafted_power() { return &m_crafted_power; }

    PropertyInt_Qt* get_gem_power() { return &m_gem_power; }

    PropertyBool_Qt* get_runeWords() { return &m_runeWords; }

    PropertyInt_Qt* get_rw_power() { return &m_rw_power; }

    PropertyBool_Qt* get_setItems() { return &m_setItems; }

    PropertyInt_Qt* get_set_power_l1() { return &m_set_power_l1; }

    PropertyInt_Qt* get_set_power_l50() { return &m_set_power_l50; }

    PropertyBool_Qt* get_uniques() { return &m_uniques; }

    PropertyInt_Qt* get_uniques_power_l1() { return &m_uniques_power_l1; }

    PropertyInt_Qt* get_uniques_power_l50() { return &m_uniques_power_l50; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModulePerfectRoll& m_data;

    PropertyInt_Qt m_affix_power_l1;

    PropertyInt_Qt m_affix_power_l50;

    PropertyBool_Qt m_affixes;

    PropertyBool_Qt m_armor;

    PropertyBool_Qt m_craft;

    PropertyBool_Qt m_crafted;

    PropertyInt_Qt m_crafted_power;

    PropertyInt_Qt m_gem_power;

    PropertyBool_Qt m_runeWords;

    PropertyInt_Qt m_rw_power;

    PropertyBool_Qt m_setItems;

    PropertyInt_Qt m_set_power_l1;

    PropertyInt_Qt m_set_power_l50;

    PropertyBool_Qt m_uniques;

    PropertyInt_Qt m_uniques_power_l1;

    PropertyInt_Qt m_uniques_power_l50;
};

class Config_ModuleQol_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* disableExpPenalty READ get_disableExpPenalty CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* keySize READ get_keySize CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* quiverSize READ get_quiverSize CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reduceCost READ get_reduceCost CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* showItemLevel READ get_showItemLevel CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* tomeSize READ get_tomeSize CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* uniqueCharmLimit READ get_uniqueCharmLimit CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* weakenTownSkills READ get_weakenTownSkills CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* weakenTownSkillsExt READ get_weakenTownSkillsExt CONSTANT)

public:
    explicit Config_ModuleQol_Qt(Config::ModuleQol& data)
        : m_data(data)
        , m_disableExpPenalty(data.getPropertyBool(0))
        , m_keySize(data.getPropertyBool(1))
        , m_quiverSize(data.getPropertyBool(2))
        , m_reduceCost(data.getPropertyInt(0))
        , m_showItemLevel(data.getPropertyBool(3))
        , m_tomeSize(data.getPropertyBool(4))
        , m_uniqueCharmLimit(data.getPropertyBool(5))
        , m_weakenTownSkills(data.getPropertyBool(6))
        , m_weakenTownSkillsExt(data.getPropertyBool(7))
    {
    }

    PropertyBool_Qt* get_disableExpPenalty() { return &m_disableExpPenalty; }

    PropertyBool_Qt* get_keySize() { return &m_keySize; }

    PropertyBool_Qt* get_quiverSize() { return &m_quiverSize; }

    PropertyInt_Qt* get_reduceCost() { return &m_reduceCost; }

    PropertyBool_Qt* get_showItemLevel() { return &m_showItemLevel; }

    PropertyBool_Qt* get_tomeSize() { return &m_tomeSize; }

    PropertyBool_Qt* get_uniqueCharmLimit() { return &m_uniqueCharmLimit; }

    PropertyBool_Qt* get_weakenTownSkills() { return &m_weakenTownSkills; }

    PropertyBool_Qt* get_weakenTownSkillsExt() { return &m_weakenTownSkillsExt; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleQol& m_data;

    PropertyBool_Qt m_disableExpPenalty;

    PropertyBool_Qt m_keySize;

    PropertyBool_Qt m_quiverSize;

    PropertyInt_Qt m_reduceCost;

    PropertyBool_Qt m_showItemLevel;

    PropertyBool_Qt m_tomeSize;

    PropertyBool_Qt m_uniqueCharmLimit;

    PropertyBool_Qt m_weakenTownSkills;

    PropertyBool_Qt m_weakenTownSkillsExt;
};

class Config_ModuleRequirements_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* affixItemLevel READ get_affixItemLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* affixReqLevel READ get_affixReqLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* equipItemLevel READ get_equipItemLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* equipReqLevel READ get_equipReqLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reqDex READ get_reqDex CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reqStr READ get_reqStr CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* skillsReqLevel READ get_skillsReqLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniquesItemLevel READ get_uniquesItemLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniquesReqLevel READ get_uniquesReqLevel CONSTANT)

public:
    explicit Config_ModuleRequirements_Qt(Config::ModuleRequirements& data)
        : m_data(data)
        , m_affixItemLevel(data.getPropertyInt(0))
        , m_affixReqLevel(data.getPropertyInt(1))
        , m_equipItemLevel(data.getPropertyInt(2))
        , m_equipReqLevel(data.getPropertyInt(3))
        , m_reqDex(data.getPropertyInt(4))
        , m_reqStr(data.getPropertyInt(5))
        , m_skillsReqLevel(data.getPropertyInt(6))
        , m_uniquesItemLevel(data.getPropertyInt(7))
        , m_uniquesReqLevel(data.getPropertyInt(8))
    {
    }

    PropertyInt_Qt* get_affixItemLevel() { return &m_affixItemLevel; }

    PropertyInt_Qt* get_affixReqLevel() { return &m_affixReqLevel; }

    PropertyInt_Qt* get_equipItemLevel() { return &m_equipItemLevel; }

    PropertyInt_Qt* get_equipReqLevel() { return &m_equipReqLevel; }

    PropertyInt_Qt* get_reqDex() { return &m_reqDex; }

    PropertyInt_Qt* get_reqStr() { return &m_reqStr; }

    PropertyInt_Qt* get_skillsReqLevel() { return &m_skillsReqLevel; }

    PropertyInt_Qt* get_uniquesItemLevel() { return &m_uniquesItemLevel; }

    PropertyInt_Qt* get_uniquesReqLevel() { return &m_uniquesReqLevel; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleRequirements& m_data;

    PropertyInt_Qt m_affixItemLevel;

    PropertyInt_Qt m_affixReqLevel;

    PropertyInt_Qt m_equipItemLevel;

    PropertyInt_Qt m_equipReqLevel;

    PropertyInt_Qt m_reqDex;

    PropertyInt_Qt m_reqStr;

    PropertyInt_Qt m_skillsReqLevel;

    PropertyInt_Qt m_uniquesItemLevel;

    PropertyInt_Qt m_uniquesReqLevel;
};

class Config_ModuleRuneDrops_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* countess_rune_higher READ get_countess_rune_higher CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* countess_rune_more READ get_countess_rune_more CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* highrune_switch READ get_highrune_switch CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rune_factor READ get_rune_factor CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* wraith_runes READ get_wraith_runes CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* zod_factor READ get_zod_factor CONSTANT)

public:
    explicit Config_ModuleRuneDrops_Qt(Config::ModuleRuneDrops& data)
        : m_data(data)
        , m_countess_rune_higher(data.getPropertyBool(0))
        , m_countess_rune_more(data.getPropertyBool(1))
        , m_highrune_switch(data.getPropertyBool(2))
        , m_rune_factor(data.getPropertyInt(0))
        , m_wraith_runes(data.getPropertyBool(3))
        , m_zod_factor(data.getPropertyInt(1))
    {
    }

    PropertyBool_Qt* get_countess_rune_higher() { return &m_countess_rune_higher; }

    PropertyBool_Qt* get_countess_rune_more() { return &m_countess_rune_more; }

    PropertyBool_Qt* get_highrune_switch() { return &m_highrune_switch; }

    PropertyInt_Qt* get_rune_factor() { return &m_rune_factor; }

    PropertyBool_Qt* get_wraith_runes() { return &m_wraith_runes; }

    PropertyInt_Qt* get_zod_factor() { return &m_zod_factor; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleRuneDrops& m_data;

    PropertyBool_Qt m_countess_rune_higher;

    PropertyBool_Qt m_countess_rune_more;

    PropertyBool_Qt m_highrune_switch;

    PropertyInt_Qt m_rune_factor;

    PropertyBool_Qt m_wraith_runes;

    PropertyInt_Qt m_zod_factor;
};

class Config_ModuleSkillRandomizer_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* ensureDifferent READ get_ensureDifferent CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* skillDamage READ get_skillDamage CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* skillTree READ get_skillTree CONSTANT)

public:
    explicit Config_ModuleSkillRandomizer_Qt(Config::ModuleSkillRandomizer& data)
        : m_data(data)
        , m_ensureDifferent(data.getPropertyBool(0))
        , m_skillDamage(data.getPropertyBool(1))
        , m_skillTree(data.getPropertyBool(2))
    {
    }

    PropertyBool_Qt* get_ensureDifferent() { return &m_ensureDifferent; }

    PropertyBool_Qt* get_skillDamage() { return &m_skillDamage; }

    PropertyBool_Qt* get_skillTree() { return &m_skillTree; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config::ModuleSkillRandomizer& m_data;

    PropertyBool_Qt m_ensureDifferent;

    PropertyBool_Qt m_skillDamage;

    PropertyBool_Qt m_skillTree;
};

class Config_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(Config_EnabledModules_Qt* enabledModules READ get_enabledModules CONSTANT)
    Q_PROPERTY(Config_ModuleMain_Qt* moduleMain READ get_moduleMain CONSTANT)
    Q_PROPERTY(Config_ModuleChallenge_Qt* moduleChallenge READ get_moduleChallenge CONSTANT)
    Q_PROPERTY(Config_ModuleCharacter_Qt* moduleCharacter READ get_moduleCharacter CONSTANT)
    Q_PROPERTY(Config_ModuleCube_Qt* moduleCube READ get_moduleCube CONSTANT)
    Q_PROPERTY(Config_ModuleDropFiltering_Qt* moduleDropFiltering READ get_moduleDropFiltering CONSTANT)
    Q_PROPERTY(Config_ModuleGambling_Qt* moduleGambling READ get_moduleGambling CONSTANT)
    Q_PROPERTY(Config_ModuleItemDrops_Qt* moduleItemDrops READ get_moduleItemDrops CONSTANT)
    Q_PROPERTY(Config_ModuleItemRandomizer_Qt* moduleItemRandomizer READ get_moduleItemRandomizer CONSTANT)
    Q_PROPERTY(Config_ModuleMonDensity_Qt* moduleMonDensity READ get_moduleMonDensity CONSTANT)
    Q_PROPERTY(Config_ModuleMonRandomizer_Qt* moduleMonRandomizer READ get_moduleMonRandomizer CONSTANT)
    Q_PROPERTY(Config_ModuleMonStats_Qt* moduleMonStats READ get_moduleMonStats CONSTANT)
    Q_PROPERTY(Config_ModulePerfectRoll_Qt* modulePerfectRoll READ get_modulePerfectRoll CONSTANT)
    Q_PROPERTY(Config_ModuleQol_Qt* moduleQol READ get_moduleQol CONSTANT)
    Q_PROPERTY(Config_ModuleRequirements_Qt* moduleRequirements READ get_moduleRequirements CONSTANT)
    Q_PROPERTY(Config_ModuleRuneDrops_Qt* moduleRuneDrops READ get_moduleRuneDrops CONSTANT)
    Q_PROPERTY(Config_ModuleSkillRandomizer_Qt* moduleSkillRandomizer READ get_moduleSkillRandomizer CONSTANT)

public:
    explicit Config_Qt(Config& data)
        : m_data(data)
        , m_enabledModules(data.m_enabledModules)
        , m_moduleMain(data.m_moduleMain)
        , m_moduleChallenge(data.m_moduleChallenge)
        , m_moduleCharacter(data.m_moduleCharacter)
        , m_moduleCube(data.m_moduleCube)
        , m_moduleDropFiltering(data.m_moduleDropFiltering)
        , m_moduleGambling(data.m_moduleGambling)
        , m_moduleItemDrops(data.m_moduleItemDrops)
        , m_moduleItemRandomizer(data.m_moduleItemRandomizer)
        , m_moduleMonDensity(data.m_moduleMonDensity)
        , m_moduleMonRandomizer(data.m_moduleMonRandomizer)
        , m_moduleMonStats(data.m_moduleMonStats)
        , m_modulePerfectRoll(data.m_modulePerfectRoll)
        , m_moduleQol(data.m_moduleQol)
        , m_moduleRequirements(data.m_moduleRequirements)
        , m_moduleRuneDrops(data.m_moduleRuneDrops)
        , m_moduleSkillRandomizer(data.m_moduleSkillRandomizer)
    {
    }

    Config_EnabledModules_Qt* get_enabledModules() { return &m_enabledModules; }

    Config_ModuleMain_Qt* get_moduleMain() { return &m_moduleMain; }

    Config_ModuleChallenge_Qt* get_moduleChallenge() { return &m_moduleChallenge; }

    Config_ModuleCharacter_Qt* get_moduleCharacter() { return &m_moduleCharacter; }

    Config_ModuleCube_Qt* get_moduleCube() { return &m_moduleCube; }

    Config_ModuleDropFiltering_Qt* get_moduleDropFiltering() { return &m_moduleDropFiltering; }

    Config_ModuleGambling_Qt* get_moduleGambling() { return &m_moduleGambling; }

    Config_ModuleItemDrops_Qt* get_moduleItemDrops() { return &m_moduleItemDrops; }

    Config_ModuleItemRandomizer_Qt* get_moduleItemRandomizer() { return &m_moduleItemRandomizer; }

    Config_ModuleMonDensity_Qt* get_moduleMonDensity() { return &m_moduleMonDensity; }

    Config_ModuleMonRandomizer_Qt* get_moduleMonRandomizer() { return &m_moduleMonRandomizer; }

    Config_ModuleMonStats_Qt* get_moduleMonStats() { return &m_moduleMonStats; }

    Config_ModulePerfectRoll_Qt* get_modulePerfectRoll() { return &m_modulePerfectRoll; }

    Config_ModuleQol_Qt* get_moduleQol() { return &m_moduleQol; }

    Config_ModuleRequirements_Qt* get_moduleRequirements() { return &m_moduleRequirements; }

    Config_ModuleRuneDrops_Qt* get_moduleRuneDrops() { return &m_moduleRuneDrops; }

    Config_ModuleSkillRandomizer_Qt* get_moduleSkillRandomizer() { return &m_moduleSkillRandomizer; }

    Q_INVOKABLE void clear() { m_data.clear(); }

    Q_INVOKABLE void syncValues() { m_data.syncValues(); }

    Q_INVOKABLE void activatePreset(int index) { m_data.activatePreset(index); }

private:
    Config& m_data;

    Config_EnabledModules_Qt m_enabledModules;

    Config_ModuleMain_Qt m_moduleMain;

    Config_ModuleChallenge_Qt m_moduleChallenge;

    Config_ModuleCharacter_Qt m_moduleCharacter;

    Config_ModuleCube_Qt m_moduleCube;

    Config_ModuleDropFiltering_Qt m_moduleDropFiltering;

    Config_ModuleGambling_Qt m_moduleGambling;

    Config_ModuleItemDrops_Qt m_moduleItemDrops;

    Config_ModuleItemRandomizer_Qt m_moduleItemRandomizer;

    Config_ModuleMonDensity_Qt m_moduleMonDensity;

    Config_ModuleMonRandomizer_Qt m_moduleMonRandomizer;

    Config_ModuleMonStats_Qt m_moduleMonStats;

    Config_ModulePerfectRoll_Qt m_modulePerfectRoll;

    Config_ModuleQol_Qt m_moduleQol;

    Config_ModuleRequirements_Qt m_moduleRequirements;

    Config_ModuleRuneDrops_Qt m_moduleRuneDrops;

    Config_ModuleSkillRandomizer_Qt m_moduleSkillRandomizer;
};

}
