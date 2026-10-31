#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "Config.hpp"
#include "StructBaseQt.hpp"

namespace D2ModGen {

class Config_EnabledModules_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool moduleChallenge READ get_moduleChallenge WRITE set_moduleChallenge NOTIFY dataChanged)
    Q_PROPERTY(bool moduleCharacter READ get_moduleCharacter WRITE set_moduleCharacter NOTIFY dataChanged)
    Q_PROPERTY(bool moduleCube READ get_moduleCube WRITE set_moduleCube NOTIFY dataChanged)
    Q_PROPERTY(bool moduleDropFiltering READ get_moduleDropFiltering WRITE set_moduleDropFiltering NOTIFY dataChanged)
    Q_PROPERTY(bool moduleGambling READ get_moduleGambling WRITE set_moduleGambling NOTIFY dataChanged)
    Q_PROPERTY(bool moduleItemDrops READ get_moduleItemDrops WRITE set_moduleItemDrops NOTIFY dataChanged)
    Q_PROPERTY(bool moduleItemRandomizer READ get_moduleItemRandomizer WRITE set_moduleItemRandomizer NOTIFY dataChanged)
    Q_PROPERTY(bool moduleMonDensity READ get_moduleMonDensity WRITE set_moduleMonDensity NOTIFY dataChanged)
    Q_PROPERTY(bool moduleMonRandomizer READ get_moduleMonRandomizer WRITE set_moduleMonRandomizer NOTIFY dataChanged)
    Q_PROPERTY(bool moduleMonStats READ get_moduleMonStats WRITE set_moduleMonStats NOTIFY dataChanged)
    Q_PROPERTY(bool modulePerfectRoll READ get_modulePerfectRoll WRITE set_modulePerfectRoll NOTIFY dataChanged)
    Q_PROPERTY(bool moduleQol READ get_moduleQol WRITE set_moduleQol NOTIFY dataChanged)
    Q_PROPERTY(bool moduleRequirements READ get_moduleRequirements WRITE set_moduleRequirements NOTIFY dataChanged)
    Q_PROPERTY(bool moduleRuneDrops READ get_moduleRuneDrops WRITE set_moduleRuneDrops NOTIFY dataChanged)
    Q_PROPERTY(bool moduleSkillRandomizer READ get_moduleSkillRandomizer WRITE set_moduleSkillRandomizer NOTIFY dataChanged)

public:
    explicit Config_EnabledModules_Qt(Config::EnabledModules& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_moduleChallenge() const { return get(m_data.m_moduleChallenge); }

    void set_moduleChallenge(bool val) { return set(m_data.m_moduleChallenge, val); }

    bool get_moduleCharacter() const { return get(m_data.m_moduleCharacter); }

    void set_moduleCharacter(bool val) { return set(m_data.m_moduleCharacter, val); }

    bool get_moduleCube() const { return get(m_data.m_moduleCube); }

    void set_moduleCube(bool val) { return set(m_data.m_moduleCube, val); }

    bool get_moduleDropFiltering() const { return get(m_data.m_moduleDropFiltering); }

    void set_moduleDropFiltering(bool val) { return set(m_data.m_moduleDropFiltering, val); }

    bool get_moduleGambling() const { return get(m_data.m_moduleGambling); }

    void set_moduleGambling(bool val) { return set(m_data.m_moduleGambling, val); }

    bool get_moduleItemDrops() const { return get(m_data.m_moduleItemDrops); }

    void set_moduleItemDrops(bool val) { return set(m_data.m_moduleItemDrops, val); }

    bool get_moduleItemRandomizer() const { return get(m_data.m_moduleItemRandomizer); }

    void set_moduleItemRandomizer(bool val) { return set(m_data.m_moduleItemRandomizer, val); }

    bool get_moduleMonDensity() const { return get(m_data.m_moduleMonDensity); }

    void set_moduleMonDensity(bool val) { return set(m_data.m_moduleMonDensity, val); }

    bool get_moduleMonRandomizer() const { return get(m_data.m_moduleMonRandomizer); }

    void set_moduleMonRandomizer(bool val) { return set(m_data.m_moduleMonRandomizer, val); }

    bool get_moduleMonStats() const { return get(m_data.m_moduleMonStats); }

    void set_moduleMonStats(bool val) { return set(m_data.m_moduleMonStats, val); }

    bool get_modulePerfectRoll() const { return get(m_data.m_modulePerfectRoll); }

    void set_modulePerfectRoll(bool val) { return set(m_data.m_modulePerfectRoll, val); }

    bool get_moduleQol() const { return get(m_data.m_moduleQol); }

    void set_moduleQol(bool val) { return set(m_data.m_moduleQol, val); }

    bool get_moduleRequirements() const { return get(m_data.m_moduleRequirements); }

    void set_moduleRequirements(bool val) { return set(m_data.m_moduleRequirements, val); }

    bool get_moduleRuneDrops() const { return get(m_data.m_moduleRuneDrops); }

    void set_moduleRuneDrops(bool val) { return set(m_data.m_moduleRuneDrops, val); }

    bool get_moduleSkillRandomizer() const { return get(m_data.m_moduleSkillRandomizer); }

    void set_moduleSkillRandomizer(bool val) { return set(m_data.m_moduleSkillRandomizer, val); }

private:
    Config::EnabledModules& m_data;
};

class Config_ModuleMain_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong d2rLocaleId READ get_d2rLocaleId WRITE set_d2rLocaleId NOTIFY dataChanged)
    Q_PROPERTY(qlonglong d2rLocaleId_min READ get_d2rLocaleId_min CONSTANT)
    Q_PROPERTY(qlonglong d2rLocaleId_max READ get_d2rLocaleId_max CONSTANT)
    Q_PROPERTY(bool d2rLocaleId_isPercent READ get_d2rLocaleId_isPercent CONSTANT)
    Q_PROPERTY(qlonglong inputMode READ get_inputMode WRITE set_inputMode NOTIFY dataChanged)
    Q_PROPERTY(qlonglong inputMode_min READ get_inputMode_min CONSTANT)
    Q_PROPERTY(qlonglong inputMode_max READ get_inputMode_max CONSTANT)
    Q_PROPERTY(bool inputMode_isPercent READ get_inputMode_isPercent CONSTANT)
    Q_PROPERTY(QString inputPath READ get_inputPath WRITE set_inputPath NOTIFY dataChanged)
    Q_PROPERTY(QString modname READ get_modname WRITE set_modname NOTIFY dataChanged)
    Q_PROPERTY(qlonglong outputMode READ get_outputMode WRITE set_outputMode NOTIFY dataChanged)
    Q_PROPERTY(qlonglong outputMode_min READ get_outputMode_min CONSTANT)
    Q_PROPERTY(qlonglong outputMode_max READ get_outputMode_max CONSTANT)
    Q_PROPERTY(bool outputMode_isPercent READ get_outputMode_isPercent CONSTANT)
    Q_PROPERTY(QString outputPath READ get_outputPath WRITE set_outputPath NOTIFY dataChanged)
    Q_PROPERTY(bool refreshSeed READ get_refreshSeed WRITE set_refreshSeed NOTIFY dataChanged)
    Q_PROPERTY(qlonglong seed READ get_seed WRITE set_seed NOTIFY dataChanged)
    Q_PROPERTY(qlonglong seed_min READ get_seed_min CONSTANT)
    Q_PROPERTY(qlonglong seed_max READ get_seed_max CONSTANT)
    Q_PROPERTY(bool seed_isPercent READ get_seed_isPercent CONSTANT)
    Q_PROPERTY(qlonglong versionD2R READ get_versionD2R WRITE set_versionD2R NOTIFY dataChanged)
    Q_PROPERTY(qlonglong versionD2R_min READ get_versionD2R_min CONSTANT)
    Q_PROPERTY(qlonglong versionD2R_max READ get_versionD2R_max CONSTANT)
    Q_PROPERTY(bool versionD2R_isPercent READ get_versionD2R_isPercent CONSTANT)
    Q_PROPERTY(qlonglong versionMajor READ get_versionMajor WRITE set_versionMajor NOTIFY dataChanged)
    Q_PROPERTY(qlonglong versionMajor_min READ get_versionMajor_min CONSTANT)
    Q_PROPERTY(qlonglong versionMajor_max READ get_versionMajor_max CONSTANT)
    Q_PROPERTY(bool versionMajor_isPercent READ get_versionMajor_isPercent CONSTANT)

public:
    explicit Config_ModuleMain_Qt(Config::ModuleMain& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_d2rLocaleId() const { return get(m_data.m_d2rLocaleId); }

    void set_d2rLocaleId(qlonglong val) { return set(m_data.m_d2rLocaleId, val); }

    qlonglong get_d2rLocaleId_min() const { return 0; }

    qlonglong get_d2rLocaleId_max() const { return 9000; }

    bool get_d2rLocaleId_isPercent() const { return false; }

    qlonglong get_inputMode() const { return get(m_data.m_inputMode); }

    void set_inputMode(qlonglong val) { return set(m_data.m_inputMode, val); }

    qlonglong get_inputMode_min() const { return 0; }

    qlonglong get_inputMode_max() const { return 9000; }

    bool get_inputMode_isPercent() const { return false; }

    QString get_inputPath() const { return get(m_data.m_inputPath); }

    void set_inputPath(const QString& val) { return set(m_data.m_inputPath, val); }

    QString get_modname() const { return get(m_data.m_modname); }

    void set_modname(const QString& val) { return set(m_data.m_modname, val); }

    qlonglong get_outputMode() const { return get(m_data.m_outputMode); }

    void set_outputMode(qlonglong val) { return set(m_data.m_outputMode, val); }

    qlonglong get_outputMode_min() const { return 0; }

    qlonglong get_outputMode_max() const { return 9000; }

    bool get_outputMode_isPercent() const { return false; }

    QString get_outputPath() const { return get(m_data.m_outputPath); }

    void set_outputPath(const QString& val) { return set(m_data.m_outputPath, val); }

    bool get_refreshSeed() const { return get(m_data.m_refreshSeed); }

    void set_refreshSeed(bool val) { return set(m_data.m_refreshSeed, val); }

    qlonglong get_seed() const { return get(m_data.m_seed); }

    void set_seed(qlonglong val) { return set(m_data.m_seed, val); }

    qlonglong get_seed_min() const { return 0; }

    qlonglong get_seed_max() const { return 4294967295; }

    bool get_seed_isPercent() const { return false; }

    qlonglong get_versionD2R() const { return get(m_data.m_versionD2R); }

    void set_versionD2R(qlonglong val) { return set(m_data.m_versionD2R, val); }

    qlonglong get_versionD2R_min() const { return 0; }

    qlonglong get_versionD2R_max() const { return 9000; }

    bool get_versionD2R_isPercent() const { return false; }

    qlonglong get_versionMajor() const { return get(m_data.m_versionMajor); }

    void set_versionMajor(qlonglong val) { return set(m_data.m_versionMajor, val); }

    qlonglong get_versionMajor_min() const { return 0; }

    qlonglong get_versionMajor_max() const { return 9000; }

    bool get_versionMajor_isPercent() const { return false; }

private:
    Config::ModuleMain& m_data;
};

class Config_ModuleChallenge_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong hell_minus_res READ get_hell_minus_res WRITE set_hell_minus_res NOTIFY dataChanged)
    Q_PROPERTY(qlonglong hell_minus_res_min READ get_hell_minus_res_min CONSTANT)
    Q_PROPERTY(qlonglong hell_minus_res_max READ get_hell_minus_res_max CONSTANT)
    Q_PROPERTY(bool hell_minus_res_isPercent READ get_hell_minus_res_isPercent CONSTANT)
    Q_PROPERTY(qlonglong levelIncreaseHell READ get_levelIncreaseHell WRITE set_levelIncreaseHell NOTIFY dataChanged)
    Q_PROPERTY(qlonglong levelIncreaseHell_min READ get_levelIncreaseHell_min CONSTANT)
    Q_PROPERTY(qlonglong levelIncreaseHell_max READ get_levelIncreaseHell_max CONSTANT)
    Q_PROPERTY(bool levelIncreaseHell_isPercent READ get_levelIncreaseHell_isPercent CONSTANT)
    Q_PROPERTY(qlonglong levelIncreaseNightmare READ get_levelIncreaseNightmare WRITE set_levelIncreaseNightmare NOTIFY dataChanged)
    Q_PROPERTY(qlonglong levelIncreaseNightmare_min READ get_levelIncreaseNightmare_min CONSTANT)
    Q_PROPERTY(qlonglong levelIncreaseNightmare_max READ get_levelIncreaseNightmare_max CONSTANT)
    Q_PROPERTY(bool levelIncreaseNightmare_isPercent READ get_levelIncreaseNightmare_isPercent CONSTANT)
    Q_PROPERTY(bool levelIncreaseUltra READ get_levelIncreaseUltra WRITE set_levelIncreaseUltra NOTIFY dataChanged)
    Q_PROPERTY(qlonglong nightmare_minus_res READ get_nightmare_minus_res WRITE set_nightmare_minus_res NOTIFY dataChanged)
    Q_PROPERTY(qlonglong nightmare_minus_res_min READ get_nightmare_minus_res_min CONSTANT)
    Q_PROPERTY(qlonglong nightmare_minus_res_max READ get_nightmare_minus_res_max CONSTANT)
    Q_PROPERTY(bool nightmare_minus_res_isPercent READ get_nightmare_minus_res_isPercent CONSTANT)
    Q_PROPERTY(bool nodrop_hpsa READ get_nodrop_hpsa WRITE set_nodrop_hpsa NOTIFY dataChanged)
    Q_PROPERTY(bool nodrop_mpsa READ get_nodrop_mpsa WRITE set_nodrop_mpsa NOTIFY dataChanged)
    Q_PROPERTY(bool nodrop_rvs READ get_nodrop_rvs WRITE set_nodrop_rvs NOTIFY dataChanged)
    Q_PROPERTY(qlonglong normal_minus_res READ get_normal_minus_res WRITE set_normal_minus_res NOTIFY dataChanged)
    Q_PROPERTY(qlonglong normal_minus_res_min READ get_normal_minus_res_min CONSTANT)
    Q_PROPERTY(qlonglong normal_minus_res_max READ get_normal_minus_res_max CONSTANT)
    Q_PROPERTY(bool normal_minus_res_isPercent READ get_normal_minus_res_isPercent CONSTANT)

public:
    explicit Config_ModuleChallenge_Qt(Config::ModuleChallenge& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_hell_minus_res() const { return get(m_data.m_hell_minus_res); }

    void set_hell_minus_res(qlonglong val) { return set(m_data.m_hell_minus_res, val); }

    qlonglong get_hell_minus_res_min() const { return 0; }

    qlonglong get_hell_minus_res_max() const { return 9000; }

    bool get_hell_minus_res_isPercent() const { return false; }

    qlonglong get_levelIncreaseHell() const { return get(m_data.m_levelIncreaseHell); }

    void set_levelIncreaseHell(qlonglong val) { return set(m_data.m_levelIncreaseHell, val); }

    qlonglong get_levelIncreaseHell_min() const { return 0; }

    qlonglong get_levelIncreaseHell_max() const { return 9000; }

    bool get_levelIncreaseHell_isPercent() const { return false; }

    qlonglong get_levelIncreaseNightmare() const { return get(m_data.m_levelIncreaseNightmare); }

    void set_levelIncreaseNightmare(qlonglong val) { return set(m_data.m_levelIncreaseNightmare, val); }

    qlonglong get_levelIncreaseNightmare_min() const { return 0; }

    qlonglong get_levelIncreaseNightmare_max() const { return 9000; }

    bool get_levelIncreaseNightmare_isPercent() const { return false; }

    bool get_levelIncreaseUltra() const { return get(m_data.m_levelIncreaseUltra); }

    void set_levelIncreaseUltra(bool val) { return set(m_data.m_levelIncreaseUltra, val); }

    qlonglong get_nightmare_minus_res() const { return get(m_data.m_nightmare_minus_res); }

    void set_nightmare_minus_res(qlonglong val) { return set(m_data.m_nightmare_minus_res, val); }

    qlonglong get_nightmare_minus_res_min() const { return 0; }

    qlonglong get_nightmare_minus_res_max() const { return 9000; }

    bool get_nightmare_minus_res_isPercent() const { return false; }

    bool get_nodrop_hpsa() const { return get(m_data.m_nodrop_hpsa); }

    void set_nodrop_hpsa(bool val) { return set(m_data.m_nodrop_hpsa, val); }

    bool get_nodrop_mpsa() const { return get(m_data.m_nodrop_mpsa); }

    void set_nodrop_mpsa(bool val) { return set(m_data.m_nodrop_mpsa, val); }

    bool get_nodrop_rvs() const { return get(m_data.m_nodrop_rvs); }

    void set_nodrop_rvs(bool val) { return set(m_data.m_nodrop_rvs, val); }

    qlonglong get_normal_minus_res() const { return get(m_data.m_normal_minus_res); }

    void set_normal_minus_res(qlonglong val) { return set(m_data.m_normal_minus_res, val); }

    qlonglong get_normal_minus_res_min() const { return 0; }

    qlonglong get_normal_minus_res_max() const { return 9000; }

    bool get_normal_minus_res_isPercent() const { return false; }

private:
    Config::ModuleChallenge& m_data;
};

class Config_ModuleCharacter_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool addCube READ get_addCube WRITE set_addCube NOTIFY dataChanged)
    Q_PROPERTY(bool addTeleport READ get_addTeleport WRITE set_addTeleport NOTIFY dataChanged)
    Q_PROPERTY(bool idTome READ get_idTome WRITE set_idTome NOTIFY dataChanged)
    Q_PROPERTY(bool manaPots READ get_manaPots WRITE set_manaPots NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mercDam READ get_mercDam WRITE set_mercDam NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mercDam_min READ get_mercDam_min CONSTANT)
    Q_PROPERTY(qlonglong mercDam_max READ get_mercDam_max CONSTANT)
    Q_PROPERTY(bool mercDam_isPercent READ get_mercDam_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mercHP READ get_mercHP WRITE set_mercHP NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mercHP_min READ get_mercHP_min CONSTANT)
    Q_PROPERTY(qlonglong mercHP_max READ get_mercHP_max CONSTANT)
    Q_PROPERTY(bool mercHP_isPercent READ get_mercHP_isPercent CONSTANT)
    Q_PROPERTY(qlonglong skillPerLevel READ get_skillPerLevel WRITE set_skillPerLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong skillPerLevel_min READ get_skillPerLevel_min CONSTANT)
    Q_PROPERTY(qlonglong skillPerLevel_max READ get_skillPerLevel_max CONSTANT)
    Q_PROPERTY(bool skillPerLevel_isPercent READ get_skillPerLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong skillPointLimit READ get_skillPointLimit WRITE set_skillPointLimit NOTIFY dataChanged)
    Q_PROPERTY(qlonglong skillPointLimit_min READ get_skillPointLimit_min CONSTANT)
    Q_PROPERTY(qlonglong skillPointLimit_max READ get_skillPointLimit_max CONSTANT)
    Q_PROPERTY(bool skillPointLimit_isPercent READ get_skillPointLimit_isPercent CONSTANT)
    Q_PROPERTY(qlonglong statPerLevel READ get_statPerLevel WRITE set_statPerLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong statPerLevel_min READ get_statPerLevel_min CONSTANT)
    Q_PROPERTY(qlonglong statPerLevel_max READ get_statPerLevel_max CONSTANT)
    Q_PROPERTY(bool statPerLevel_isPercent READ get_statPerLevel_isPercent CONSTANT)

public:
    explicit Config_ModuleCharacter_Qt(Config::ModuleCharacter& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_addCube() const { return get(m_data.m_addCube); }

    void set_addCube(bool val) { return set(m_data.m_addCube, val); }

    bool get_addTeleport() const { return get(m_data.m_addTeleport); }

    void set_addTeleport(bool val) { return set(m_data.m_addTeleport, val); }

    bool get_idTome() const { return get(m_data.m_idTome); }

    void set_idTome(bool val) { return set(m_data.m_idTome, val); }

    bool get_manaPots() const { return get(m_data.m_manaPots); }

    void set_manaPots(bool val) { return set(m_data.m_manaPots, val); }

    qlonglong get_mercDam() const { return get(m_data.m_mercDam); }

    void set_mercDam(qlonglong val) { return set(m_data.m_mercDam, val); }

    qlonglong get_mercDam_min() const { return 0; }

    qlonglong get_mercDam_max() const { return 9000; }

    bool get_mercDam_isPercent() const { return false; }

    qlonglong get_mercHP() const { return get(m_data.m_mercHP); }

    void set_mercHP(qlonglong val) { return set(m_data.m_mercHP, val); }

    qlonglong get_mercHP_min() const { return 0; }

    qlonglong get_mercHP_max() const { return 9000; }

    bool get_mercHP_isPercent() const { return false; }

    qlonglong get_skillPerLevel() const { return get(m_data.m_skillPerLevel); }

    void set_skillPerLevel(qlonglong val) { return set(m_data.m_skillPerLevel, val); }

    qlonglong get_skillPerLevel_min() const { return 0; }

    qlonglong get_skillPerLevel_max() const { return 9000; }

    bool get_skillPerLevel_isPercent() const { return false; }

    qlonglong get_skillPointLimit() const { return get(m_data.m_skillPointLimit); }

    void set_skillPointLimit(qlonglong val) { return set(m_data.m_skillPointLimit, val); }

    qlonglong get_skillPointLimit_min() const { return 0; }

    qlonglong get_skillPointLimit_max() const { return 9000; }

    bool get_skillPointLimit_isPercent() const { return false; }

    qlonglong get_statPerLevel() const { return get(m_data.m_statPerLevel); }

    void set_statPerLevel(qlonglong val) { return set(m_data.m_statPerLevel, val); }

    qlonglong get_statPerLevel_min() const { return 0; }

    qlonglong get_statPerLevel_max() const { return 9000; }

    bool get_statPerLevel_isPercent() const { return false; }

private:
    Config::ModuleCharacter& m_data;
};

class Config_ModuleCube_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool cheatCharms READ get_cheatCharms WRITE set_cheatCharms NOTIFY dataChanged)
    Q_PROPERTY(bool craftHighIlvl READ get_craftHighIlvl WRITE set_craftHighIlvl NOTIFY dataChanged)
    Q_PROPERTY(bool craftNoRunes READ get_craftNoRunes WRITE set_craftNoRunes NOTIFY dataChanged)
    Q_PROPERTY(bool craftNoStrict READ get_craftNoStrict WRITE set_craftNoStrict NOTIFY dataChanged)
    Q_PROPERTY(bool noGemUpgrade READ get_noGemUpgrade WRITE set_noGemUpgrade NOTIFY dataChanged)
    Q_PROPERTY(bool quickPortals READ get_quickPortals WRITE set_quickPortals NOTIFY dataChanged)
    Q_PROPERTY(bool quickQuests READ get_quickQuests WRITE set_quickQuests NOTIFY dataChanged)
    Q_PROPERTY(bool socketing READ get_socketing WRITE set_socketing NOTIFY dataChanged)
    Q_PROPERTY(bool upgrading READ get_upgrading WRITE set_upgrading NOTIFY dataChanged)

public:
    explicit Config_ModuleCube_Qt(Config::ModuleCube& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_cheatCharms() const { return get(m_data.m_cheatCharms); }

    void set_cheatCharms(bool val) { return set(m_data.m_cheatCharms, val); }

    bool get_craftHighIlvl() const { return get(m_data.m_craftHighIlvl); }

    void set_craftHighIlvl(bool val) { return set(m_data.m_craftHighIlvl, val); }

    bool get_craftNoRunes() const { return get(m_data.m_craftNoRunes); }

    void set_craftNoRunes(bool val) { return set(m_data.m_craftNoRunes, val); }

    bool get_craftNoStrict() const { return get(m_data.m_craftNoStrict); }

    void set_craftNoStrict(bool val) { return set(m_data.m_craftNoStrict, val); }

    bool get_noGemUpgrade() const { return get(m_data.m_noGemUpgrade); }

    void set_noGemUpgrade(bool val) { return set(m_data.m_noGemUpgrade, val); }

    bool get_quickPortals() const { return get(m_data.m_quickPortals); }

    void set_quickPortals(bool val) { return set(m_data.m_quickPortals, val); }

    bool get_quickQuests() const { return get(m_data.m_quickQuests); }

    void set_quickQuests(bool val) { return set(m_data.m_quickQuests, val); }

    bool get_socketing() const { return get(m_data.m_socketing); }

    void set_socketing(bool val) { return set(m_data.m_socketing, val); }

    bool get_upgrading() const { return get(m_data.m_upgrading); }

    void set_upgrading(bool val) { return set(m_data.m_upgrading, val); }

private:
    Config::ModuleCube& m_data;
};

class Config_ModuleDropFiltering_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool compact_pots READ get_compact_pots WRITE set_compact_pots NOTIFY dataChanged)
    Q_PROPERTY(bool compact_scrolls READ get_compact_scrolls WRITE set_compact_scrolls NOTIFY dataChanged)
    Q_PROPERTY(bool hide_ammo READ get_hide_ammo WRITE set_hide_ammo NOTIFY dataChanged)
    Q_PROPERTY(bool hide_hps READ get_hide_hps WRITE set_hide_hps NOTIFY dataChanged)
    Q_PROPERTY(bool hide_hpsa READ get_hide_hpsa WRITE set_hide_hpsa NOTIFY dataChanged)
    Q_PROPERTY(bool hide_isc READ get_hide_isc WRITE set_hide_isc NOTIFY dataChanged)
    Q_PROPERTY(bool hide_junks READ get_hide_junks WRITE set_hide_junks NOTIFY dataChanged)
    Q_PROPERTY(bool hide_lowq READ get_hide_lowq WRITE set_hide_lowq NOTIFY dataChanged)
    Q_PROPERTY(bool hide_mps READ get_hide_mps WRITE set_hide_mps NOTIFY dataChanged)
    Q_PROPERTY(bool hide_mpsa READ get_hide_mpsa WRITE set_hide_mpsa NOTIFY dataChanged)
    Q_PROPERTY(bool hide_rvl READ get_hide_rvl WRITE set_hide_rvl NOTIFY dataChanged)
    Q_PROPERTY(bool hide_rvs READ get_hide_rvs WRITE set_hide_rvs NOTIFY dataChanged)
    Q_PROPERTY(bool hide_stam READ get_hide_stam WRITE set_hide_stam NOTIFY dataChanged)
    Q_PROPERTY(bool hide_tsc READ get_hide_tsc WRITE set_hide_tsc NOTIFY dataChanged)

public:
    explicit Config_ModuleDropFiltering_Qt(Config::ModuleDropFiltering& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_compact_pots() const { return get(m_data.m_compact_pots); }

    void set_compact_pots(bool val) { return set(m_data.m_compact_pots, val); }

    bool get_compact_scrolls() const { return get(m_data.m_compact_scrolls); }

    void set_compact_scrolls(bool val) { return set(m_data.m_compact_scrolls, val); }

    bool get_hide_ammo() const { return get(m_data.m_hide_ammo); }

    void set_hide_ammo(bool val) { return set(m_data.m_hide_ammo, val); }

    bool get_hide_hps() const { return get(m_data.m_hide_hps); }

    void set_hide_hps(bool val) { return set(m_data.m_hide_hps, val); }

    bool get_hide_hpsa() const { return get(m_data.m_hide_hpsa); }

    void set_hide_hpsa(bool val) { return set(m_data.m_hide_hpsa, val); }

    bool get_hide_isc() const { return get(m_data.m_hide_isc); }

    void set_hide_isc(bool val) { return set(m_data.m_hide_isc, val); }

    bool get_hide_junks() const { return get(m_data.m_hide_junks); }

    void set_hide_junks(bool val) { return set(m_data.m_hide_junks, val); }

    bool get_hide_lowq() const { return get(m_data.m_hide_lowq); }

    void set_hide_lowq(bool val) { return set(m_data.m_hide_lowq, val); }

    bool get_hide_mps() const { return get(m_data.m_hide_mps); }

    void set_hide_mps(bool val) { return set(m_data.m_hide_mps, val); }

    bool get_hide_mpsa() const { return get(m_data.m_hide_mpsa); }

    void set_hide_mpsa(bool val) { return set(m_data.m_hide_mpsa, val); }

    bool get_hide_rvl() const { return get(m_data.m_hide_rvl); }

    void set_hide_rvl(bool val) { return set(m_data.m_hide_rvl, val); }

    bool get_hide_rvs() const { return get(m_data.m_hide_rvs); }

    void set_hide_rvs(bool val) { return set(m_data.m_hide_rvs, val); }

    bool get_hide_stam() const { return get(m_data.m_hide_stam); }

    void set_hide_stam(bool val) { return set(m_data.m_hide_stam, val); }

    bool get_hide_tsc() const { return get(m_data.m_hide_tsc); }

    void set_hide_tsc(bool val) { return set(m_data.m_hide_tsc, val); }

private:
    Config::ModuleDropFiltering& m_data;
};

class Config_ModuleGambling_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool charmGamble READ get_charmGamble WRITE set_charmGamble NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioElite READ get_ratioElite WRITE set_ratioElite NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioElite_min READ get_ratioElite_min CONSTANT)
    Q_PROPERTY(qlonglong ratioElite_max READ get_ratioElite_max CONSTANT)
    Q_PROPERTY(bool ratioElite_isPercent READ get_ratioElite_isPercent CONSTANT)
    Q_PROPERTY(qlonglong ratioExc READ get_ratioExc WRITE set_ratioExc NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioExc_min READ get_ratioExc_min CONSTANT)
    Q_PROPERTY(qlonglong ratioExc_max READ get_ratioExc_max CONSTANT)
    Q_PROPERTY(bool ratioExc_isPercent READ get_ratioExc_isPercent CONSTANT)
    Q_PROPERTY(qlonglong ratioRare READ get_ratioRare WRITE set_ratioRare NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioRare_min READ get_ratioRare_min CONSTANT)
    Q_PROPERTY(qlonglong ratioRare_max READ get_ratioRare_max CONSTANT)
    Q_PROPERTY(bool ratioRare_isPercent READ get_ratioRare_isPercent CONSTANT)
    Q_PROPERTY(qlonglong ratioSet READ get_ratioSet WRITE set_ratioSet NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioSet_min READ get_ratioSet_min CONSTANT)
    Q_PROPERTY(qlonglong ratioSet_max READ get_ratioSet_max CONSTANT)
    Q_PROPERTY(bool ratioSet_isPercent READ get_ratioSet_isPercent CONSTANT)
    Q_PROPERTY(qlonglong ratioUnique READ get_ratioUnique WRITE set_ratioUnique NOTIFY dataChanged)
    Q_PROPERTY(qlonglong ratioUnique_min READ get_ratioUnique_min CONSTANT)
    Q_PROPERTY(qlonglong ratioUnique_max READ get_ratioUnique_max CONSTANT)
    Q_PROPERTY(bool ratioUnique_isPercent READ get_ratioUnique_isPercent CONSTANT)

public:
    explicit Config_ModuleGambling_Qt(Config::ModuleGambling& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_charmGamble() const { return get(m_data.m_charmGamble); }

    void set_charmGamble(bool val) { return set(m_data.m_charmGamble, val); }

    qlonglong get_ratioElite() const { return get(m_data.m_ratioElite); }

    void set_ratioElite(qlonglong val) { return set(m_data.m_ratioElite, val); }

    qlonglong get_ratioElite_min() const { return 0; }

    qlonglong get_ratioElite_max() const { return 9000; }

    bool get_ratioElite_isPercent() const { return false; }

    qlonglong get_ratioExc() const { return get(m_data.m_ratioExc); }

    void set_ratioExc(qlonglong val) { return set(m_data.m_ratioExc, val); }

    qlonglong get_ratioExc_min() const { return 0; }

    qlonglong get_ratioExc_max() const { return 9000; }

    bool get_ratioExc_isPercent() const { return false; }

    qlonglong get_ratioRare() const { return get(m_data.m_ratioRare); }

    void set_ratioRare(qlonglong val) { return set(m_data.m_ratioRare, val); }

    qlonglong get_ratioRare_min() const { return 0; }

    qlonglong get_ratioRare_max() const { return 9000; }

    bool get_ratioRare_isPercent() const { return false; }

    qlonglong get_ratioSet() const { return get(m_data.m_ratioSet); }

    void set_ratioSet(qlonglong val) { return set(m_data.m_ratioSet, val); }

    qlonglong get_ratioSet_min() const { return 0; }

    qlonglong get_ratioSet_max() const { return 9000; }

    bool get_ratioSet_isPercent() const { return false; }

    qlonglong get_ratioUnique() const { return get(m_data.m_ratioUnique); }

    void set_ratioUnique(qlonglong val) { return set(m_data.m_ratioUnique, val); }

    qlonglong get_ratioUnique_min() const { return 0; }

    qlonglong get_ratioUnique_max() const { return 9000; }

    bool get_ratioUnique_isPercent() const { return false; }

private:
    Config::ModuleGambling& m_data;
};

class Config_ModuleItemDrops_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool boss_quest_drops READ get_boss_quest_drops WRITE set_boss_quest_drops NOTIFY dataChanged)
    Q_PROPERTY(qlonglong chance_highlevel READ get_chance_highlevel WRITE set_chance_highlevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong chance_highlevel_min READ get_chance_highlevel_min CONSTANT)
    Q_PROPERTY(qlonglong chance_highlevel_max READ get_chance_highlevel_max CONSTANT)
    Q_PROPERTY(bool chance_highlevel_isPercent READ get_chance_highlevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong chance_rare READ get_chance_rare WRITE set_chance_rare NOTIFY dataChanged)
    Q_PROPERTY(qlonglong chance_rare_min READ get_chance_rare_min CONSTANT)
    Q_PROPERTY(qlonglong chance_rare_max READ get_chance_rare_max CONSTANT)
    Q_PROPERTY(bool chance_rare_isPercent READ get_chance_rare_isPercent CONSTANT)
    Q_PROPERTY(qlonglong chance_set READ get_chance_set WRITE set_chance_set NOTIFY dataChanged)
    Q_PROPERTY(qlonglong chance_set_min READ get_chance_set_min CONSTANT)
    Q_PROPERTY(qlonglong chance_set_max READ get_chance_set_max CONSTANT)
    Q_PROPERTY(bool chance_set_isPercent READ get_chance_set_isPercent CONSTANT)
    Q_PROPERTY(qlonglong chance_uni READ get_chance_uni WRITE set_chance_uni NOTIFY dataChanged)
    Q_PROPERTY(qlonglong chance_uni_min READ get_chance_uni_min CONSTANT)
    Q_PROPERTY(qlonglong chance_uni_max READ get_chance_uni_max CONSTANT)
    Q_PROPERTY(bool chance_uni_isPercent READ get_chance_uni_isPercent CONSTANT)
    Q_PROPERTY(qlonglong drop_percent READ get_drop_percent WRITE set_drop_percent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong drop_percent_min READ get_drop_percent_min CONSTANT)
    Q_PROPERTY(qlonglong drop_percent_max READ get_drop_percent_max CONSTANT)
    Q_PROPERTY(bool drop_percent_isPercent READ get_drop_percent_isPercent CONSTANT)
    Q_PROPERTY(bool equal_uniques READ get_equal_uniques WRITE set_equal_uniques NOTIFY dataChanged)
    Q_PROPERTY(qlonglong equip_percent READ get_equip_percent WRITE set_equip_percent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong equip_percent_min READ get_equip_percent_min CONSTANT)
    Q_PROPERTY(qlonglong equip_percent_max READ get_equip_percent_max CONSTANT)
    Q_PROPERTY(bool equip_percent_isPercent READ get_equip_percent_isPercent CONSTANT)
    Q_PROPERTY(qlonglong gold_percent READ get_gold_percent WRITE set_gold_percent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong gold_percent_min READ get_gold_percent_min CONSTANT)
    Q_PROPERTY(qlonglong gold_percent_max READ get_gold_percent_max CONSTANT)
    Q_PROPERTY(bool gold_percent_isPercent READ get_gold_percent_isPercent CONSTANT)
    Q_PROPERTY(qlonglong good_percent READ get_good_percent WRITE set_good_percent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong good_percent_min READ get_good_percent_min CONSTANT)
    Q_PROPERTY(qlonglong good_percent_max READ get_good_percent_max CONSTANT)
    Q_PROPERTY(bool good_percent_isPercent READ get_good_percent_isPercent CONSTANT)
    Q_PROPERTY(bool high_elite_drops READ get_high_elite_drops WRITE set_high_elite_drops NOTIFY dataChanged)
    Q_PROPERTY(qlonglong junk_percent READ get_junk_percent WRITE set_junk_percent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong junk_percent_min READ get_junk_percent_min CONSTANT)
    Q_PROPERTY(qlonglong junk_percent_max READ get_junk_percent_max CONSTANT)
    Q_PROPERTY(bool junk_percent_isPercent READ get_junk_percent_isPercent CONSTANT)

public:
    explicit Config_ModuleItemDrops_Qt(Config::ModuleItemDrops& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_boss_quest_drops() const { return get(m_data.m_boss_quest_drops); }

    void set_boss_quest_drops(bool val) { return set(m_data.m_boss_quest_drops, val); }

    qlonglong get_chance_highlevel() const { return get(m_data.m_chance_highlevel); }

    void set_chance_highlevel(qlonglong val) { return set(m_data.m_chance_highlevel, val); }

    qlonglong get_chance_highlevel_min() const { return 0; }

    qlonglong get_chance_highlevel_max() const { return 9000; }

    bool get_chance_highlevel_isPercent() const { return false; }

    qlonglong get_chance_rare() const { return get(m_data.m_chance_rare); }

    void set_chance_rare(qlonglong val) { return set(m_data.m_chance_rare, val); }

    qlonglong get_chance_rare_min() const { return 0; }

    qlonglong get_chance_rare_max() const { return 9000; }

    bool get_chance_rare_isPercent() const { return false; }

    qlonglong get_chance_set() const { return get(m_data.m_chance_set); }

    void set_chance_set(qlonglong val) { return set(m_data.m_chance_set, val); }

    qlonglong get_chance_set_min() const { return 0; }

    qlonglong get_chance_set_max() const { return 9000; }

    bool get_chance_set_isPercent() const { return false; }

    qlonglong get_chance_uni() const { return get(m_data.m_chance_uni); }

    void set_chance_uni(qlonglong val) { return set(m_data.m_chance_uni, val); }

    qlonglong get_chance_uni_min() const { return 0; }

    qlonglong get_chance_uni_max() const { return 9000; }

    bool get_chance_uni_isPercent() const { return false; }

    qlonglong get_drop_percent() const { return get(m_data.m_drop_percent); }

    void set_drop_percent(qlonglong val) { return set(m_data.m_drop_percent, val); }

    qlonglong get_drop_percent_min() const { return 0; }

    qlonglong get_drop_percent_max() const { return 9000; }

    bool get_drop_percent_isPercent() const { return false; }

    bool get_equal_uniques() const { return get(m_data.m_equal_uniques); }

    void set_equal_uniques(bool val) { return set(m_data.m_equal_uniques, val); }

    qlonglong get_equip_percent() const { return get(m_data.m_equip_percent); }

    void set_equip_percent(qlonglong val) { return set(m_data.m_equip_percent, val); }

    qlonglong get_equip_percent_min() const { return 0; }

    qlonglong get_equip_percent_max() const { return 9000; }

    bool get_equip_percent_isPercent() const { return false; }

    qlonglong get_gold_percent() const { return get(m_data.m_gold_percent); }

    void set_gold_percent(qlonglong val) { return set(m_data.m_gold_percent, val); }

    qlonglong get_gold_percent_min() const { return 0; }

    qlonglong get_gold_percent_max() const { return 9000; }

    bool get_gold_percent_isPercent() const { return false; }

    qlonglong get_good_percent() const { return get(m_data.m_good_percent); }

    void set_good_percent(qlonglong val) { return set(m_data.m_good_percent, val); }

    qlonglong get_good_percent_min() const { return 0; }

    qlonglong get_good_percent_max() const { return 9000; }

    bool get_good_percent_isPercent() const { return false; }

    bool get_high_elite_drops() const { return get(m_data.m_high_elite_drops); }

    void set_high_elite_drops(bool val) { return set(m_data.m_high_elite_drops, val); }

    qlonglong get_junk_percent() const { return get(m_data.m_junk_percent); }

    void set_junk_percent(qlonglong val) { return set(m_data.m_junk_percent, val); }

    qlonglong get_junk_percent_min() const { return 0; }

    qlonglong get_junk_percent_max() const { return 9000; }

    bool get_junk_percent_isPercent() const { return false; }

private:
    Config::ModuleItemDrops& m_data;
};

class Config_ModuleItemRandomizer_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool affixRandom READ get_affixRandom WRITE set_affixRandom NOTIFY dataChanged)
    Q_PROPERTY(qlonglong casterTypePercent READ get_casterTypePercent WRITE set_casterTypePercent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong casterTypePercent_min READ get_casterTypePercent_min CONSTANT)
    Q_PROPERTY(qlonglong casterTypePercent_max READ get_casterTypePercent_max CONSTANT)
    Q_PROPERTY(bool casterTypePercent_isPercent READ get_casterTypePercent_isPercent CONSTANT)
    Q_PROPERTY(qlonglong frequencyAuras READ get_frequencyAuras WRITE set_frequencyAuras NOTIFY dataChanged)
    Q_PROPERTY(qlonglong frequencyAuras_min READ get_frequencyAuras_min CONSTANT)
    Q_PROPERTY(qlonglong frequencyAuras_max READ get_frequencyAuras_max CONSTANT)
    Q_PROPERTY(bool frequencyAuras_isPercent READ get_frequencyAuras_isPercent CONSTANT)
    Q_PROPERTY(qlonglong frequencyCharAttributes READ get_frequencyCharAttributes WRITE set_frequencyCharAttributes NOTIFY dataChanged)
    Q_PROPERTY(qlonglong frequencyCharAttributes_min READ get_frequencyCharAttributes_min CONSTANT)
    Q_PROPERTY(qlonglong frequencyCharAttributes_max READ get_frequencyCharAttributes_max CONSTANT)
    Q_PROPERTY(bool frequencyCharAttributes_isPercent READ get_frequencyCharAttributes_isPercent CONSTANT)
    Q_PROPERTY(qlonglong frequencyProcs READ get_frequencyProcs WRITE set_frequencyProcs NOTIFY dataChanged)
    Q_PROPERTY(qlonglong frequencyProcs_min READ get_frequencyProcs_min CONSTANT)
    Q_PROPERTY(qlonglong frequencyProcs_max READ get_frequencyProcs_max CONSTANT)
    Q_PROPERTY(bool frequencyProcs_isPercent READ get_frequencyProcs_isPercent CONSTANT)
    Q_PROPERTY(qlonglong frequencyResistances READ get_frequencyResistances WRITE set_frequencyResistances NOTIFY dataChanged)
    Q_PROPERTY(qlonglong frequencyResistances_min READ get_frequencyResistances_min CONSTANT)
    Q_PROPERTY(qlonglong frequencyResistances_max READ get_frequencyResistances_max CONSTANT)
    Q_PROPERTY(bool frequencyResistances_isPercent READ get_frequencyResistances_isPercent CONSTANT)
    Q_PROPERTY(qlonglong frequencySkills READ get_frequencySkills WRITE set_frequencySkills NOTIFY dataChanged)
    Q_PROPERTY(qlonglong frequencySkills_min READ get_frequencySkills_min CONSTANT)
    Q_PROPERTY(qlonglong frequencySkills_max READ get_frequencySkills_max CONSTANT)
    Q_PROPERTY(bool frequencySkills_isPercent READ get_frequencySkills_isPercent CONSTANT)
    Q_PROPERTY(bool gemsRandom READ get_gemsRandom WRITE set_gemsRandom NOTIFY dataChanged)
    Q_PROPERTY(qlonglong itemFitPercent READ get_itemFitPercent WRITE set_itemFitPercent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong itemFitPercent_min READ get_itemFitPercent_min CONSTANT)
    Q_PROPERTY(qlonglong itemFitPercent_max READ get_itemFitPercent_max CONSTANT)
    Q_PROPERTY(bool itemFitPercent_isPercent READ get_itemFitPercent_isPercent CONSTANT)
    Q_PROPERTY(qlonglong keepOriginalPercent READ get_keepOriginalPercent WRITE set_keepOriginalPercent NOTIFY dataChanged)
    Q_PROPERTY(qlonglong keepOriginalPercent_min READ get_keepOriginalPercent_min CONSTANT)
    Q_PROPERTY(qlonglong keepOriginalPercent_max READ get_keepOriginalPercent_max CONSTANT)
    Q_PROPERTY(bool keepOriginalPercent_isPercent READ get_keepOriginalPercent_isPercent CONSTANT)
    Q_PROPERTY(qlonglong levelMinus READ get_levelMinus WRITE set_levelMinus NOTIFY dataChanged)
    Q_PROPERTY(qlonglong levelMinus_min READ get_levelMinus_min CONSTANT)
    Q_PROPERTY(qlonglong levelMinus_max READ get_levelMinus_max CONSTANT)
    Q_PROPERTY(bool levelMinus_isPercent READ get_levelMinus_isPercent CONSTANT)
    Q_PROPERTY(qlonglong levelPlus READ get_levelPlus WRITE set_levelPlus NOTIFY dataChanged)
    Q_PROPERTY(qlonglong levelPlus_min READ get_levelPlus_min CONSTANT)
    Q_PROPERTY(qlonglong levelPlus_max READ get_levelPlus_max CONSTANT)
    Q_PROPERTY(bool levelPlus_isPercent READ get_levelPlus_isPercent CONSTANT)
    Q_PROPERTY(qlonglong relativeCountMax READ get_relativeCountMax WRITE set_relativeCountMax NOTIFY dataChanged)
    Q_PROPERTY(qlonglong relativeCountMax_min READ get_relativeCountMax_min CONSTANT)
    Q_PROPERTY(qlonglong relativeCountMax_max READ get_relativeCountMax_max CONSTANT)
    Q_PROPERTY(bool relativeCountMax_isPercent READ get_relativeCountMax_isPercent CONSTANT)
    Q_PROPERTY(qlonglong relativeCountMin READ get_relativeCountMin WRITE set_relativeCountMin NOTIFY dataChanged)
    Q_PROPERTY(qlonglong relativeCountMin_min READ get_relativeCountMin_min CONSTANT)
    Q_PROPERTY(qlonglong relativeCountMin_max READ get_relativeCountMin_max CONSTANT)
    Q_PROPERTY(bool relativeCountMin_isPercent READ get_relativeCountMin_isPercent CONSTANT)
    Q_PROPERTY(bool removeKnock READ get_removeKnock WRITE set_removeKnock NOTIFY dataChanged)
    Q_PROPERTY(qlonglong repeat_sets READ get_repeat_sets WRITE set_repeat_sets NOTIFY dataChanged)
    Q_PROPERTY(qlonglong repeat_sets_min READ get_repeat_sets_min CONSTANT)
    Q_PROPERTY(qlonglong repeat_sets_max READ get_repeat_sets_max CONSTANT)
    Q_PROPERTY(bool repeat_sets_isPercent READ get_repeat_sets_isPercent CONSTANT)
    Q_PROPERTY(qlonglong repeat_uniques READ get_repeat_uniques WRITE set_repeat_uniques NOTIFY dataChanged)
    Q_PROPERTY(qlonglong repeat_uniques_min READ get_repeat_uniques_min CONSTANT)
    Q_PROPERTY(qlonglong repeat_uniques_max READ get_repeat_uniques_max CONSTANT)
    Q_PROPERTY(bool repeat_uniques_isPercent READ get_repeat_uniques_isPercent CONSTANT)
    Q_PROPERTY(bool replaceCharges READ get_replaceCharges WRITE set_replaceCharges NOTIFY dataChanged)
    Q_PROPERTY(bool replaceSkills READ get_replaceSkills WRITE set_replaceSkills NOTIFY dataChanged)
    Q_PROPERTY(qlonglong setBonusMode READ get_setBonusMode WRITE set_setBonusMode NOTIFY dataChanged)
    Q_PROPERTY(qlonglong setBonusMode_min READ get_setBonusMode_min CONSTANT)
    Q_PROPERTY(qlonglong setBonusMode_max READ get_setBonusMode_max CONSTANT)
    Q_PROPERTY(bool setBonusMode_isPercent READ get_setBonusMode_isPercent CONSTANT)
    Q_PROPERTY(qlonglong setRelativeCountMax READ get_setRelativeCountMax WRITE set_setRelativeCountMax NOTIFY dataChanged)
    Q_PROPERTY(qlonglong setRelativeCountMax_min READ get_setRelativeCountMax_min CONSTANT)
    Q_PROPERTY(qlonglong setRelativeCountMax_max READ get_setRelativeCountMax_max CONSTANT)
    Q_PROPERTY(bool setRelativeCountMax_isPercent READ get_setRelativeCountMax_isPercent CONSTANT)
    Q_PROPERTY(qlonglong setRelativeCountMin READ get_setRelativeCountMin WRITE set_setRelativeCountMin NOTIFY dataChanged)
    Q_PROPERTY(qlonglong setRelativeCountMin_min READ get_setRelativeCountMin_min CONSTANT)
    Q_PROPERTY(qlonglong setRelativeCountMin_max READ get_setRelativeCountMin_max CONSTANT)
    Q_PROPERTY(bool setRelativeCountMin_isPercent READ get_setRelativeCountMin_isPercent CONSTANT)
    Q_PROPERTY(bool updateExisting READ get_updateExisting WRITE set_updateExisting NOTIFY dataChanged)

public:
    explicit Config_ModuleItemRandomizer_Qt(Config::ModuleItemRandomizer& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_affixRandom() const { return get(m_data.m_affixRandom); }

    void set_affixRandom(bool val) { return set(m_data.m_affixRandom, val); }

    qlonglong get_casterTypePercent() const { return get(m_data.m_casterTypePercent); }

    void set_casterTypePercent(qlonglong val) { return set(m_data.m_casterTypePercent, val); }

    qlonglong get_casterTypePercent_min() const { return 0; }

    qlonglong get_casterTypePercent_max() const { return 9000; }

    bool get_casterTypePercent_isPercent() const { return false; }

    qlonglong get_frequencyAuras() const { return get(m_data.m_frequencyAuras); }

    void set_frequencyAuras(qlonglong val) { return set(m_data.m_frequencyAuras, val); }

    qlonglong get_frequencyAuras_min() const { return 0; }

    qlonglong get_frequencyAuras_max() const { return 9000; }

    bool get_frequencyAuras_isPercent() const { return false; }

    qlonglong get_frequencyCharAttributes() const { return get(m_data.m_frequencyCharAttributes); }

    void set_frequencyCharAttributes(qlonglong val) { return set(m_data.m_frequencyCharAttributes, val); }

    qlonglong get_frequencyCharAttributes_min() const { return 0; }

    qlonglong get_frequencyCharAttributes_max() const { return 9000; }

    bool get_frequencyCharAttributes_isPercent() const { return false; }

    qlonglong get_frequencyProcs() const { return get(m_data.m_frequencyProcs); }

    void set_frequencyProcs(qlonglong val) { return set(m_data.m_frequencyProcs, val); }

    qlonglong get_frequencyProcs_min() const { return 0; }

    qlonglong get_frequencyProcs_max() const { return 9000; }

    bool get_frequencyProcs_isPercent() const { return false; }

    qlonglong get_frequencyResistances() const { return get(m_data.m_frequencyResistances); }

    void set_frequencyResistances(qlonglong val) { return set(m_data.m_frequencyResistances, val); }

    qlonglong get_frequencyResistances_min() const { return 0; }

    qlonglong get_frequencyResistances_max() const { return 9000; }

    bool get_frequencyResistances_isPercent() const { return false; }

    qlonglong get_frequencySkills() const { return get(m_data.m_frequencySkills); }

    void set_frequencySkills(qlonglong val) { return set(m_data.m_frequencySkills, val); }

    qlonglong get_frequencySkills_min() const { return 0; }

    qlonglong get_frequencySkills_max() const { return 9000; }

    bool get_frequencySkills_isPercent() const { return false; }

    bool get_gemsRandom() const { return get(m_data.m_gemsRandom); }

    void set_gemsRandom(bool val) { return set(m_data.m_gemsRandom, val); }

    qlonglong get_itemFitPercent() const { return get(m_data.m_itemFitPercent); }

    void set_itemFitPercent(qlonglong val) { return set(m_data.m_itemFitPercent, val); }

    qlonglong get_itemFitPercent_min() const { return 0; }

    qlonglong get_itemFitPercent_max() const { return 9000; }

    bool get_itemFitPercent_isPercent() const { return false; }

    qlonglong get_keepOriginalPercent() const { return get(m_data.m_keepOriginalPercent); }

    void set_keepOriginalPercent(qlonglong val) { return set(m_data.m_keepOriginalPercent, val); }

    qlonglong get_keepOriginalPercent_min() const { return 0; }

    qlonglong get_keepOriginalPercent_max() const { return 9000; }

    bool get_keepOriginalPercent_isPercent() const { return false; }

    qlonglong get_levelMinus() const { return get(m_data.m_levelMinus); }

    void set_levelMinus(qlonglong val) { return set(m_data.m_levelMinus, val); }

    qlonglong get_levelMinus_min() const { return 0; }

    qlonglong get_levelMinus_max() const { return 9000; }

    bool get_levelMinus_isPercent() const { return false; }

    qlonglong get_levelPlus() const { return get(m_data.m_levelPlus); }

    void set_levelPlus(qlonglong val) { return set(m_data.m_levelPlus, val); }

    qlonglong get_levelPlus_min() const { return 0; }

    qlonglong get_levelPlus_max() const { return 9000; }

    bool get_levelPlus_isPercent() const { return false; }

    qlonglong get_relativeCountMax() const { return get(m_data.m_relativeCountMax); }

    void set_relativeCountMax(qlonglong val) { return set(m_data.m_relativeCountMax, val); }

    qlonglong get_relativeCountMax_min() const { return 0; }

    qlonglong get_relativeCountMax_max() const { return 9000; }

    bool get_relativeCountMax_isPercent() const { return false; }

    qlonglong get_relativeCountMin() const { return get(m_data.m_relativeCountMin); }

    void set_relativeCountMin(qlonglong val) { return set(m_data.m_relativeCountMin, val); }

    qlonglong get_relativeCountMin_min() const { return 0; }

    qlonglong get_relativeCountMin_max() const { return 9000; }

    bool get_relativeCountMin_isPercent() const { return false; }

    bool get_removeKnock() const { return get(m_data.m_removeKnock); }

    void set_removeKnock(bool val) { return set(m_data.m_removeKnock, val); }

    qlonglong get_repeat_sets() const { return get(m_data.m_repeat_sets); }

    void set_repeat_sets(qlonglong val) { return set(m_data.m_repeat_sets, val); }

    qlonglong get_repeat_sets_min() const { return 0; }

    qlonglong get_repeat_sets_max() const { return 9000; }

    bool get_repeat_sets_isPercent() const { return false; }

    qlonglong get_repeat_uniques() const { return get(m_data.m_repeat_uniques); }

    void set_repeat_uniques(qlonglong val) { return set(m_data.m_repeat_uniques, val); }

    qlonglong get_repeat_uniques_min() const { return 0; }

    qlonglong get_repeat_uniques_max() const { return 9000; }

    bool get_repeat_uniques_isPercent() const { return false; }

    bool get_replaceCharges() const { return get(m_data.m_replaceCharges); }

    void set_replaceCharges(bool val) { return set(m_data.m_replaceCharges, val); }

    bool get_replaceSkills() const { return get(m_data.m_replaceSkills); }

    void set_replaceSkills(bool val) { return set(m_data.m_replaceSkills, val); }

    qlonglong get_setBonusMode() const { return get(m_data.m_setBonusMode); }

    void set_setBonusMode(qlonglong val) { return set(m_data.m_setBonusMode, val); }

    qlonglong get_setBonusMode_min() const { return 0; }

    qlonglong get_setBonusMode_max() const { return 9000; }

    bool get_setBonusMode_isPercent() const { return false; }

    qlonglong get_setRelativeCountMax() const { return get(m_data.m_setRelativeCountMax); }

    void set_setRelativeCountMax(qlonglong val) { return set(m_data.m_setRelativeCountMax, val); }

    qlonglong get_setRelativeCountMax_min() const { return 0; }

    qlonglong get_setRelativeCountMax_max() const { return 9000; }

    bool get_setRelativeCountMax_isPercent() const { return false; }

    qlonglong get_setRelativeCountMin() const { return get(m_data.m_setRelativeCountMin); }

    void set_setRelativeCountMin(qlonglong val) { return set(m_data.m_setRelativeCountMin, val); }

    qlonglong get_setRelativeCountMin_min() const { return 0; }

    qlonglong get_setRelativeCountMin_max() const { return 9000; }

    bool get_setRelativeCountMin_isPercent() const { return false; }

    bool get_updateExisting() const { return get(m_data.m_updateExisting); }

    void set_updateExisting(bool val) { return set(m_data.m_updateExisting, val); }

private:
    Config::ModuleItemRandomizer& m_data;
};

class Config_ModuleMonDensity_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong density READ get_density WRITE set_density NOTIFY dataChanged)
    Q_PROPERTY(qlonglong density_min READ get_density_min CONSTANT)
    Q_PROPERTY(qlonglong density_max READ get_density_max CONSTANT)
    Q_PROPERTY(bool density_isPercent READ get_density_isPercent CONSTANT)
    Q_PROPERTY(bool hellPacks READ get_hellPacks WRITE set_hellPacks NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_groups READ get_mon_groups WRITE set_mon_groups NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_groups_min READ get_mon_groups_min CONSTANT)
    Q_PROPERTY(qlonglong mon_groups_max READ get_mon_groups_max CONSTANT)
    Q_PROPERTY(bool mon_groups_isPercent READ get_mon_groups_isPercent CONSTANT)
    Q_PROPERTY(qlonglong packs READ get_packs WRITE set_packs NOTIFY dataChanged)
    Q_PROPERTY(qlonglong packs_min READ get_packs_min CONSTANT)
    Q_PROPERTY(qlonglong packs_max READ get_packs_max CONSTANT)
    Q_PROPERTY(bool packs_isPercent READ get_packs_isPercent CONSTANT)

public:
    explicit Config_ModuleMonDensity_Qt(Config::ModuleMonDensity& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_density() const { return get(m_data.m_density); }

    void set_density(qlonglong val) { return set(m_data.m_density, val); }

    qlonglong get_density_min() const { return 0; }

    qlonglong get_density_max() const { return 9000; }

    bool get_density_isPercent() const { return false; }

    bool get_hellPacks() const { return get(m_data.m_hellPacks); }

    void set_hellPacks(bool val) { return set(m_data.m_hellPacks, val); }

    qlonglong get_mon_groups() const { return get(m_data.m_mon_groups); }

    void set_mon_groups(qlonglong val) { return set(m_data.m_mon_groups, val); }

    qlonglong get_mon_groups_min() const { return 0; }

    qlonglong get_mon_groups_max() const { return 9000; }

    bool get_mon_groups_isPercent() const { return false; }

    qlonglong get_packs() const { return get(m_data.m_packs); }

    void set_packs(qlonglong val) { return set(m_data.m_packs, val); }

    qlonglong get_packs_min() const { return 0; }

    qlonglong get_packs_max() const { return 9000; }

    bool get_packs_isPercent() const { return false; }

private:
    Config::ModuleMonDensity& m_data;
};

class Config_ModuleMonRandomizer_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool hellResistances READ get_hellResistances WRITE set_hellResistances NOTIFY dataChanged)
    Q_PROPERTY(qlonglong maxTypes READ get_maxTypes WRITE set_maxTypes NOTIFY dataChanged)
    Q_PROPERTY(qlonglong maxTypes_min READ get_maxTypes_min CONSTANT)
    Q_PROPERTY(qlonglong maxTypes_max READ get_maxTypes_max CONSTANT)
    Q_PROPERTY(bool maxTypes_isPercent READ get_maxTypes_isPercent CONSTANT)
    Q_PROPERTY(bool randomizeResistances READ get_randomizeResistances WRITE set_randomizeResistances NOTIFY dataChanged)
    Q_PROPERTY(bool randomizeSpawns READ get_randomizeSpawns WRITE set_randomizeSpawns NOTIFY dataChanged)
    Q_PROPERTY(qlonglong spawnedCount READ get_spawnedCount WRITE set_spawnedCount NOTIFY dataChanged)
    Q_PROPERTY(qlonglong spawnedCount_min READ get_spawnedCount_min CONSTANT)
    Q_PROPERTY(qlonglong spawnedCount_max READ get_spawnedCount_max CONSTANT)
    Q_PROPERTY(bool spawnedCount_isPercent READ get_spawnedCount_isPercent CONSTANT)

public:
    explicit Config_ModuleMonRandomizer_Qt(Config::ModuleMonRandomizer& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_hellResistances() const { return get(m_data.m_hellResistances); }

    void set_hellResistances(bool val) { return set(m_data.m_hellResistances, val); }

    qlonglong get_maxTypes() const { return get(m_data.m_maxTypes); }

    void set_maxTypes(qlonglong val) { return set(m_data.m_maxTypes, val); }

    qlonglong get_maxTypes_min() const { return 0; }

    qlonglong get_maxTypes_max() const { return 9000; }

    bool get_maxTypes_isPercent() const { return false; }

    bool get_randomizeResistances() const { return get(m_data.m_randomizeResistances); }

    void set_randomizeResistances(bool val) { return set(m_data.m_randomizeResistances, val); }

    bool get_randomizeSpawns() const { return get(m_data.m_randomizeSpawns); }

    void set_randomizeSpawns(bool val) { return set(m_data.m_randomizeSpawns, val); }

    qlonglong get_spawnedCount() const { return get(m_data.m_spawnedCount); }

    void set_spawnedCount(qlonglong val) { return set(m_data.m_spawnedCount, val); }

    qlonglong get_spawnedCount_min() const { return 0; }

    qlonglong get_spawnedCount_max() const { return 9000; }

    bool get_spawnedCount_isPercent() const { return false; }

private:
    Config::ModuleMonRandomizer& m_data;
};

class Config_ModuleMonStats_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong max_resist READ get_max_resist WRITE set_max_resist NOTIFY dataChanged)
    Q_PROPERTY(qlonglong max_resist_min READ get_max_resist_min CONSTANT)
    Q_PROPERTY(qlonglong max_resist_max READ get_max_resist_max CONSTANT)
    Q_PROPERTY(bool max_resist_isPercent READ get_max_resist_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mon_ar READ get_mon_ar WRITE set_mon_ar NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_ar_min READ get_mon_ar_min CONSTANT)
    Q_PROPERTY(qlonglong mon_ar_max READ get_mon_ar_max CONSTANT)
    Q_PROPERTY(bool mon_ar_isPercent READ get_mon_ar_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mon_dam READ get_mon_dam WRITE set_mon_dam NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_dam_min READ get_mon_dam_min CONSTANT)
    Q_PROPERTY(qlonglong mon_dam_max READ get_mon_dam_max CONSTANT)
    Q_PROPERTY(bool mon_dam_isPercent READ get_mon_dam_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mon_def READ get_mon_def WRITE set_mon_def NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_def_min READ get_mon_def_min CONSTANT)
    Q_PROPERTY(qlonglong mon_def_max READ get_mon_def_max CONSTANT)
    Q_PROPERTY(bool mon_def_isPercent READ get_mon_def_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mon_hp READ get_mon_hp WRITE set_mon_hp NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_hp_min READ get_mon_hp_min CONSTANT)
    Q_PROPERTY(qlonglong mon_hp_max READ get_mon_hp_max CONSTANT)
    Q_PROPERTY(bool mon_hp_isPercent READ get_mon_hp_isPercent CONSTANT)
    Q_PROPERTY(qlonglong mon_xp READ get_mon_xp WRITE set_mon_xp NOTIFY dataChanged)
    Q_PROPERTY(qlonglong mon_xp_min READ get_mon_xp_min CONSTANT)
    Q_PROPERTY(qlonglong mon_xp_max READ get_mon_xp_max CONSTANT)
    Q_PROPERTY(bool mon_xp_isPercent READ get_mon_xp_isPercent CONSTANT)
    Q_PROPERTY(qlonglong rel_resist READ get_rel_resist WRITE set_rel_resist NOTIFY dataChanged)
    Q_PROPERTY(qlonglong rel_resist_min READ get_rel_resist_min CONSTANT)
    Q_PROPERTY(qlonglong rel_resist_max READ get_rel_resist_max CONSTANT)
    Q_PROPERTY(bool rel_resist_isPercent READ get_rel_resist_isPercent CONSTANT)
    Q_PROPERTY(bool resist_apply_elem READ get_resist_apply_elem WRITE set_resist_apply_elem NOTIFY dataChanged)
    Q_PROPERTY(bool resist_apply_magic READ get_resist_apply_magic WRITE set_resist_apply_magic NOTIFY dataChanged)
    Q_PROPERTY(bool resist_apply_physical READ get_resist_apply_physical WRITE set_resist_apply_physical NOTIFY dataChanged)
    Q_PROPERTY(bool resist_apply_poison READ get_resist_apply_poison WRITE set_resist_apply_poison NOTIFY dataChanged)

public:
    explicit Config_ModuleMonStats_Qt(Config::ModuleMonStats& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_max_resist() const { return get(m_data.m_max_resist); }

    void set_max_resist(qlonglong val) { return set(m_data.m_max_resist, val); }

    qlonglong get_max_resist_min() const { return 0; }

    qlonglong get_max_resist_max() const { return 9000; }

    bool get_max_resist_isPercent() const { return false; }

    qlonglong get_mon_ar() const { return get(m_data.m_mon_ar); }

    void set_mon_ar(qlonglong val) { return set(m_data.m_mon_ar, val); }

    qlonglong get_mon_ar_min() const { return 0; }

    qlonglong get_mon_ar_max() const { return 9000; }

    bool get_mon_ar_isPercent() const { return false; }

    qlonglong get_mon_dam() const { return get(m_data.m_mon_dam); }

    void set_mon_dam(qlonglong val) { return set(m_data.m_mon_dam, val); }

    qlonglong get_mon_dam_min() const { return 0; }

    qlonglong get_mon_dam_max() const { return 9000; }

    bool get_mon_dam_isPercent() const { return false; }

    qlonglong get_mon_def() const { return get(m_data.m_mon_def); }

    void set_mon_def(qlonglong val) { return set(m_data.m_mon_def, val); }

    qlonglong get_mon_def_min() const { return 0; }

    qlonglong get_mon_def_max() const { return 9000; }

    bool get_mon_def_isPercent() const { return false; }

    qlonglong get_mon_hp() const { return get(m_data.m_mon_hp); }

    void set_mon_hp(qlonglong val) { return set(m_data.m_mon_hp, val); }

    qlonglong get_mon_hp_min() const { return 0; }

    qlonglong get_mon_hp_max() const { return 9000; }

    bool get_mon_hp_isPercent() const { return false; }

    qlonglong get_mon_xp() const { return get(m_data.m_mon_xp); }

    void set_mon_xp(qlonglong val) { return set(m_data.m_mon_xp, val); }

    qlonglong get_mon_xp_min() const { return 0; }

    qlonglong get_mon_xp_max() const { return 9000; }

    bool get_mon_xp_isPercent() const { return false; }

    qlonglong get_rel_resist() const { return get(m_data.m_rel_resist); }

    void set_rel_resist(qlonglong val) { return set(m_data.m_rel_resist, val); }

    qlonglong get_rel_resist_min() const { return 0; }

    qlonglong get_rel_resist_max() const { return 9000; }

    bool get_rel_resist_isPercent() const { return false; }

    bool get_resist_apply_elem() const { return get(m_data.m_resist_apply_elem); }

    void set_resist_apply_elem(bool val) { return set(m_data.m_resist_apply_elem, val); }

    bool get_resist_apply_magic() const { return get(m_data.m_resist_apply_magic); }

    void set_resist_apply_magic(bool val) { return set(m_data.m_resist_apply_magic, val); }

    bool get_resist_apply_physical() const { return get(m_data.m_resist_apply_physical); }

    void set_resist_apply_physical(bool val) { return set(m_data.m_resist_apply_physical, val); }

    bool get_resist_apply_poison() const { return get(m_data.m_resist_apply_poison); }

    void set_resist_apply_poison(bool val) { return set(m_data.m_resist_apply_poison, val); }

private:
    Config::ModuleMonStats& m_data;
};

class Config_ModulePerfectRoll_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong affix_power_l1 READ get_affix_power_l1 WRITE set_affix_power_l1 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong affix_power_l1_min READ get_affix_power_l1_min CONSTANT)
    Q_PROPERTY(qlonglong affix_power_l1_max READ get_affix_power_l1_max CONSTANT)
    Q_PROPERTY(bool affix_power_l1_isPercent READ get_affix_power_l1_isPercent CONSTANT)
    Q_PROPERTY(qlonglong affix_power_l50 READ get_affix_power_l50 WRITE set_affix_power_l50 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong affix_power_l50_min READ get_affix_power_l50_min CONSTANT)
    Q_PROPERTY(qlonglong affix_power_l50_max READ get_affix_power_l50_max CONSTANT)
    Q_PROPERTY(bool affix_power_l50_isPercent READ get_affix_power_l50_isPercent CONSTANT)
    Q_PROPERTY(bool affixes READ get_affixes WRITE set_affixes NOTIFY dataChanged)
    Q_PROPERTY(bool armor READ get_armor WRITE set_armor NOTIFY dataChanged)
    Q_PROPERTY(bool craft READ get_craft WRITE set_craft NOTIFY dataChanged)
    Q_PROPERTY(bool crafted READ get_crafted WRITE set_crafted NOTIFY dataChanged)
    Q_PROPERTY(qlonglong crafted_power READ get_crafted_power WRITE set_crafted_power NOTIFY dataChanged)
    Q_PROPERTY(qlonglong crafted_power_min READ get_crafted_power_min CONSTANT)
    Q_PROPERTY(qlonglong crafted_power_max READ get_crafted_power_max CONSTANT)
    Q_PROPERTY(bool crafted_power_isPercent READ get_crafted_power_isPercent CONSTANT)
    Q_PROPERTY(qlonglong gem_power READ get_gem_power WRITE set_gem_power NOTIFY dataChanged)
    Q_PROPERTY(qlonglong gem_power_min READ get_gem_power_min CONSTANT)
    Q_PROPERTY(qlonglong gem_power_max READ get_gem_power_max CONSTANT)
    Q_PROPERTY(bool gem_power_isPercent READ get_gem_power_isPercent CONSTANT)
    Q_PROPERTY(bool runeWords READ get_runeWords WRITE set_runeWords NOTIFY dataChanged)
    Q_PROPERTY(qlonglong rw_power READ get_rw_power WRITE set_rw_power NOTIFY dataChanged)
    Q_PROPERTY(qlonglong rw_power_min READ get_rw_power_min CONSTANT)
    Q_PROPERTY(qlonglong rw_power_max READ get_rw_power_max CONSTANT)
    Q_PROPERTY(bool rw_power_isPercent READ get_rw_power_isPercent CONSTANT)
    Q_PROPERTY(bool setItems READ get_setItems WRITE set_setItems NOTIFY dataChanged)
    Q_PROPERTY(qlonglong set_power_l1 READ get_set_power_l1 WRITE set_set_power_l1 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong set_power_l1_min READ get_set_power_l1_min CONSTANT)
    Q_PROPERTY(qlonglong set_power_l1_max READ get_set_power_l1_max CONSTANT)
    Q_PROPERTY(bool set_power_l1_isPercent READ get_set_power_l1_isPercent CONSTANT)
    Q_PROPERTY(qlonglong set_power_l50 READ get_set_power_l50 WRITE set_set_power_l50 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong set_power_l50_min READ get_set_power_l50_min CONSTANT)
    Q_PROPERTY(qlonglong set_power_l50_max READ get_set_power_l50_max CONSTANT)
    Q_PROPERTY(bool set_power_l50_isPercent READ get_set_power_l50_isPercent CONSTANT)
    Q_PROPERTY(bool uniques READ get_uniques WRITE set_uniques NOTIFY dataChanged)
    Q_PROPERTY(qlonglong uniques_power_l1 READ get_uniques_power_l1 WRITE set_uniques_power_l1 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong uniques_power_l1_min READ get_uniques_power_l1_min CONSTANT)
    Q_PROPERTY(qlonglong uniques_power_l1_max READ get_uniques_power_l1_max CONSTANT)
    Q_PROPERTY(bool uniques_power_l1_isPercent READ get_uniques_power_l1_isPercent CONSTANT)
    Q_PROPERTY(qlonglong uniques_power_l50 READ get_uniques_power_l50 WRITE set_uniques_power_l50 NOTIFY dataChanged)
    Q_PROPERTY(qlonglong uniques_power_l50_min READ get_uniques_power_l50_min CONSTANT)
    Q_PROPERTY(qlonglong uniques_power_l50_max READ get_uniques_power_l50_max CONSTANT)
    Q_PROPERTY(bool uniques_power_l50_isPercent READ get_uniques_power_l50_isPercent CONSTANT)

public:
    explicit Config_ModulePerfectRoll_Qt(Config::ModulePerfectRoll& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_affix_power_l1() const { return get(m_data.m_affix_power_l1); }

    void set_affix_power_l1(qlonglong val) { return set(m_data.m_affix_power_l1, val); }

    qlonglong get_affix_power_l1_min() const { return 0; }

    qlonglong get_affix_power_l1_max() const { return 9000; }

    bool get_affix_power_l1_isPercent() const { return false; }

    qlonglong get_affix_power_l50() const { return get(m_data.m_affix_power_l50); }

    void set_affix_power_l50(qlonglong val) { return set(m_data.m_affix_power_l50, val); }

    qlonglong get_affix_power_l50_min() const { return 0; }

    qlonglong get_affix_power_l50_max() const { return 9000; }

    bool get_affix_power_l50_isPercent() const { return false; }

    bool get_affixes() const { return get(m_data.m_affixes); }

    void set_affixes(bool val) { return set(m_data.m_affixes, val); }

    bool get_armor() const { return get(m_data.m_armor); }

    void set_armor(bool val) { return set(m_data.m_armor, val); }

    bool get_craft() const { return get(m_data.m_craft); }

    void set_craft(bool val) { return set(m_data.m_craft, val); }

    bool get_crafted() const { return get(m_data.m_crafted); }

    void set_crafted(bool val) { return set(m_data.m_crafted, val); }

    qlonglong get_crafted_power() const { return get(m_data.m_crafted_power); }

    void set_crafted_power(qlonglong val) { return set(m_data.m_crafted_power, val); }

    qlonglong get_crafted_power_min() const { return 0; }

    qlonglong get_crafted_power_max() const { return 9000; }

    bool get_crafted_power_isPercent() const { return false; }

    qlonglong get_gem_power() const { return get(m_data.m_gem_power); }

    void set_gem_power(qlonglong val) { return set(m_data.m_gem_power, val); }

    qlonglong get_gem_power_min() const { return 0; }

    qlonglong get_gem_power_max() const { return 9000; }

    bool get_gem_power_isPercent() const { return false; }

    bool get_runeWords() const { return get(m_data.m_runeWords); }

    void set_runeWords(bool val) { return set(m_data.m_runeWords, val); }

    qlonglong get_rw_power() const { return get(m_data.m_rw_power); }

    void set_rw_power(qlonglong val) { return set(m_data.m_rw_power, val); }

    qlonglong get_rw_power_min() const { return 0; }

    qlonglong get_rw_power_max() const { return 9000; }

    bool get_rw_power_isPercent() const { return false; }

    bool get_setItems() const { return get(m_data.m_setItems); }

    void set_setItems(bool val) { return set(m_data.m_setItems, val); }

    qlonglong get_set_power_l1() const { return get(m_data.m_set_power_l1); }

    void set_set_power_l1(qlonglong val) { return set(m_data.m_set_power_l1, val); }

    qlonglong get_set_power_l1_min() const { return 0; }

    qlonglong get_set_power_l1_max() const { return 9000; }

    bool get_set_power_l1_isPercent() const { return false; }

    qlonglong get_set_power_l50() const { return get(m_data.m_set_power_l50); }

    void set_set_power_l50(qlonglong val) { return set(m_data.m_set_power_l50, val); }

    qlonglong get_set_power_l50_min() const { return 0; }

    qlonglong get_set_power_l50_max() const { return 9000; }

    bool get_set_power_l50_isPercent() const { return false; }

    bool get_uniques() const { return get(m_data.m_uniques); }

    void set_uniques(bool val) { return set(m_data.m_uniques, val); }

    qlonglong get_uniques_power_l1() const { return get(m_data.m_uniques_power_l1); }

    void set_uniques_power_l1(qlonglong val) { return set(m_data.m_uniques_power_l1, val); }

    qlonglong get_uniques_power_l1_min() const { return 0; }

    qlonglong get_uniques_power_l1_max() const { return 9000; }

    bool get_uniques_power_l1_isPercent() const { return false; }

    qlonglong get_uniques_power_l50() const { return get(m_data.m_uniques_power_l50); }

    void set_uniques_power_l50(qlonglong val) { return set(m_data.m_uniques_power_l50, val); }

    qlonglong get_uniques_power_l50_min() const { return 0; }

    qlonglong get_uniques_power_l50_max() const { return 9000; }

    bool get_uniques_power_l50_isPercent() const { return false; }

private:
    Config::ModulePerfectRoll& m_data;
};

class Config_ModuleQol_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool disableExpPenalty READ get_disableExpPenalty WRITE set_disableExpPenalty NOTIFY dataChanged)
    Q_PROPERTY(bool keySize READ get_keySize WRITE set_keySize NOTIFY dataChanged)
    Q_PROPERTY(bool quiverSize READ get_quiverSize WRITE set_quiverSize NOTIFY dataChanged)
    Q_PROPERTY(qlonglong reduceCost READ get_reduceCost WRITE set_reduceCost NOTIFY dataChanged)
    Q_PROPERTY(qlonglong reduceCost_min READ get_reduceCost_min CONSTANT)
    Q_PROPERTY(qlonglong reduceCost_max READ get_reduceCost_max CONSTANT)
    Q_PROPERTY(bool reduceCost_isPercent READ get_reduceCost_isPercent CONSTANT)
    Q_PROPERTY(bool showItemLevel READ get_showItemLevel WRITE set_showItemLevel NOTIFY dataChanged)
    Q_PROPERTY(bool tomeSize READ get_tomeSize WRITE set_tomeSize NOTIFY dataChanged)
    Q_PROPERTY(bool uniqueCharmLimit READ get_uniqueCharmLimit WRITE set_uniqueCharmLimit NOTIFY dataChanged)
    Q_PROPERTY(bool weakenTownSkills READ get_weakenTownSkills WRITE set_weakenTownSkills NOTIFY dataChanged)
    Q_PROPERTY(bool weakenTownSkillsExt READ get_weakenTownSkillsExt WRITE set_weakenTownSkillsExt NOTIFY dataChanged)

public:
    explicit Config_ModuleQol_Qt(Config::ModuleQol& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_disableExpPenalty() const { return get(m_data.m_disableExpPenalty); }

    void set_disableExpPenalty(bool val) { return set(m_data.m_disableExpPenalty, val); }

    bool get_keySize() const { return get(m_data.m_keySize); }

    void set_keySize(bool val) { return set(m_data.m_keySize, val); }

    bool get_quiverSize() const { return get(m_data.m_quiverSize); }

    void set_quiverSize(bool val) { return set(m_data.m_quiverSize, val); }

    qlonglong get_reduceCost() const { return get(m_data.m_reduceCost); }

    void set_reduceCost(qlonglong val) { return set(m_data.m_reduceCost, val); }

    qlonglong get_reduceCost_min() const { return 0; }

    qlonglong get_reduceCost_max() const { return 9000; }

    bool get_reduceCost_isPercent() const { return false; }

    bool get_showItemLevel() const { return get(m_data.m_showItemLevel); }

    void set_showItemLevel(bool val) { return set(m_data.m_showItemLevel, val); }

    bool get_tomeSize() const { return get(m_data.m_tomeSize); }

    void set_tomeSize(bool val) { return set(m_data.m_tomeSize, val); }

    bool get_uniqueCharmLimit() const { return get(m_data.m_uniqueCharmLimit); }

    void set_uniqueCharmLimit(bool val) { return set(m_data.m_uniqueCharmLimit, val); }

    bool get_weakenTownSkills() const { return get(m_data.m_weakenTownSkills); }

    void set_weakenTownSkills(bool val) { return set(m_data.m_weakenTownSkills, val); }

    bool get_weakenTownSkillsExt() const { return get(m_data.m_weakenTownSkillsExt); }

    void set_weakenTownSkillsExt(bool val) { return set(m_data.m_weakenTownSkillsExt, val); }

private:
    Config::ModuleQol& m_data;
};

class Config_ModuleRequirements_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(qlonglong affixItemLevel READ get_affixItemLevel WRITE set_affixItemLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong affixItemLevel_min READ get_affixItemLevel_min CONSTANT)
    Q_PROPERTY(qlonglong affixItemLevel_max READ get_affixItemLevel_max CONSTANT)
    Q_PROPERTY(bool affixItemLevel_isPercent READ get_affixItemLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong affixReqLevel READ get_affixReqLevel WRITE set_affixReqLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong affixReqLevel_min READ get_affixReqLevel_min CONSTANT)
    Q_PROPERTY(qlonglong affixReqLevel_max READ get_affixReqLevel_max CONSTANT)
    Q_PROPERTY(bool affixReqLevel_isPercent READ get_affixReqLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong equipItemLevel READ get_equipItemLevel WRITE set_equipItemLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong equipItemLevel_min READ get_equipItemLevel_min CONSTANT)
    Q_PROPERTY(qlonglong equipItemLevel_max READ get_equipItemLevel_max CONSTANT)
    Q_PROPERTY(bool equipItemLevel_isPercent READ get_equipItemLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong equipReqLevel READ get_equipReqLevel WRITE set_equipReqLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong equipReqLevel_min READ get_equipReqLevel_min CONSTANT)
    Q_PROPERTY(qlonglong equipReqLevel_max READ get_equipReqLevel_max CONSTANT)
    Q_PROPERTY(bool equipReqLevel_isPercent READ get_equipReqLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong reqDex READ get_reqDex WRITE set_reqDex NOTIFY dataChanged)
    Q_PROPERTY(qlonglong reqDex_min READ get_reqDex_min CONSTANT)
    Q_PROPERTY(qlonglong reqDex_max READ get_reqDex_max CONSTANT)
    Q_PROPERTY(bool reqDex_isPercent READ get_reqDex_isPercent CONSTANT)
    Q_PROPERTY(qlonglong reqStr READ get_reqStr WRITE set_reqStr NOTIFY dataChanged)
    Q_PROPERTY(qlonglong reqStr_min READ get_reqStr_min CONSTANT)
    Q_PROPERTY(qlonglong reqStr_max READ get_reqStr_max CONSTANT)
    Q_PROPERTY(bool reqStr_isPercent READ get_reqStr_isPercent CONSTANT)
    Q_PROPERTY(qlonglong skillsReqLevel READ get_skillsReqLevel WRITE set_skillsReqLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong skillsReqLevel_min READ get_skillsReqLevel_min CONSTANT)
    Q_PROPERTY(qlonglong skillsReqLevel_max READ get_skillsReqLevel_max CONSTANT)
    Q_PROPERTY(bool skillsReqLevel_isPercent READ get_skillsReqLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong uniquesItemLevel READ get_uniquesItemLevel WRITE set_uniquesItemLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong uniquesItemLevel_min READ get_uniquesItemLevel_min CONSTANT)
    Q_PROPERTY(qlonglong uniquesItemLevel_max READ get_uniquesItemLevel_max CONSTANT)
    Q_PROPERTY(bool uniquesItemLevel_isPercent READ get_uniquesItemLevel_isPercent CONSTANT)
    Q_PROPERTY(qlonglong uniquesReqLevel READ get_uniquesReqLevel WRITE set_uniquesReqLevel NOTIFY dataChanged)
    Q_PROPERTY(qlonglong uniquesReqLevel_min READ get_uniquesReqLevel_min CONSTANT)
    Q_PROPERTY(qlonglong uniquesReqLevel_max READ get_uniquesReqLevel_max CONSTANT)
    Q_PROPERTY(bool uniquesReqLevel_isPercent READ get_uniquesReqLevel_isPercent CONSTANT)

public:
    explicit Config_ModuleRequirements_Qt(Config::ModuleRequirements& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    qlonglong get_affixItemLevel() const { return get(m_data.m_affixItemLevel); }

    void set_affixItemLevel(qlonglong val) { return set(m_data.m_affixItemLevel, val); }

    qlonglong get_affixItemLevel_min() const { return 0; }

    qlonglong get_affixItemLevel_max() const { return 9000; }

    bool get_affixItemLevel_isPercent() const { return false; }

    qlonglong get_affixReqLevel() const { return get(m_data.m_affixReqLevel); }

    void set_affixReqLevel(qlonglong val) { return set(m_data.m_affixReqLevel, val); }

    qlonglong get_affixReqLevel_min() const { return 0; }

    qlonglong get_affixReqLevel_max() const { return 9000; }

    bool get_affixReqLevel_isPercent() const { return false; }

    qlonglong get_equipItemLevel() const { return get(m_data.m_equipItemLevel); }

    void set_equipItemLevel(qlonglong val) { return set(m_data.m_equipItemLevel, val); }

    qlonglong get_equipItemLevel_min() const { return 0; }

    qlonglong get_equipItemLevel_max() const { return 9000; }

    bool get_equipItemLevel_isPercent() const { return false; }

    qlonglong get_equipReqLevel() const { return get(m_data.m_equipReqLevel); }

    void set_equipReqLevel(qlonglong val) { return set(m_data.m_equipReqLevel, val); }

    qlonglong get_equipReqLevel_min() const { return 0; }

    qlonglong get_equipReqLevel_max() const { return 9000; }

    bool get_equipReqLevel_isPercent() const { return false; }

    qlonglong get_reqDex() const { return get(m_data.m_reqDex); }

    void set_reqDex(qlonglong val) { return set(m_data.m_reqDex, val); }

    qlonglong get_reqDex_min() const { return 0; }

    qlonglong get_reqDex_max() const { return 9000; }

    bool get_reqDex_isPercent() const { return false; }

    qlonglong get_reqStr() const { return get(m_data.m_reqStr); }

    void set_reqStr(qlonglong val) { return set(m_data.m_reqStr, val); }

    qlonglong get_reqStr_min() const { return 0; }

    qlonglong get_reqStr_max() const { return 9000; }

    bool get_reqStr_isPercent() const { return false; }

    qlonglong get_skillsReqLevel() const { return get(m_data.m_skillsReqLevel); }

    void set_skillsReqLevel(qlonglong val) { return set(m_data.m_skillsReqLevel, val); }

    qlonglong get_skillsReqLevel_min() const { return 0; }

    qlonglong get_skillsReqLevel_max() const { return 9000; }

    bool get_skillsReqLevel_isPercent() const { return false; }

    qlonglong get_uniquesItemLevel() const { return get(m_data.m_uniquesItemLevel); }

    void set_uniquesItemLevel(qlonglong val) { return set(m_data.m_uniquesItemLevel, val); }

    qlonglong get_uniquesItemLevel_min() const { return 0; }

    qlonglong get_uniquesItemLevel_max() const { return 9000; }

    bool get_uniquesItemLevel_isPercent() const { return false; }

    qlonglong get_uniquesReqLevel() const { return get(m_data.m_uniquesReqLevel); }

    void set_uniquesReqLevel(qlonglong val) { return set(m_data.m_uniquesReqLevel, val); }

    qlonglong get_uniquesReqLevel_min() const { return 0; }

    qlonglong get_uniquesReqLevel_max() const { return 9000; }

    bool get_uniquesReqLevel_isPercent() const { return false; }

private:
    Config::ModuleRequirements& m_data;
};

class Config_ModuleRuneDrops_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool countess_rune_higher READ get_countess_rune_higher WRITE set_countess_rune_higher NOTIFY dataChanged)
    Q_PROPERTY(bool countess_rune_more READ get_countess_rune_more WRITE set_countess_rune_more NOTIFY dataChanged)
    Q_PROPERTY(bool highrune_switch READ get_highrune_switch WRITE set_highrune_switch NOTIFY dataChanged)
    Q_PROPERTY(qlonglong rune_factor READ get_rune_factor WRITE set_rune_factor NOTIFY dataChanged)
    Q_PROPERTY(qlonglong rune_factor_min READ get_rune_factor_min CONSTANT)
    Q_PROPERTY(qlonglong rune_factor_max READ get_rune_factor_max CONSTANT)
    Q_PROPERTY(bool rune_factor_isPercent READ get_rune_factor_isPercent CONSTANT)
    Q_PROPERTY(bool wraith_runes READ get_wraith_runes WRITE set_wraith_runes NOTIFY dataChanged)
    Q_PROPERTY(qlonglong zod_factor READ get_zod_factor WRITE set_zod_factor NOTIFY dataChanged)
    Q_PROPERTY(qlonglong zod_factor_min READ get_zod_factor_min CONSTANT)
    Q_PROPERTY(qlonglong zod_factor_max READ get_zod_factor_max CONSTANT)
    Q_PROPERTY(bool zod_factor_isPercent READ get_zod_factor_isPercent CONSTANT)

public:
    explicit Config_ModuleRuneDrops_Qt(Config::ModuleRuneDrops& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_countess_rune_higher() const { return get(m_data.m_countess_rune_higher); }

    void set_countess_rune_higher(bool val) { return set(m_data.m_countess_rune_higher, val); }

    bool get_countess_rune_more() const { return get(m_data.m_countess_rune_more); }

    void set_countess_rune_more(bool val) { return set(m_data.m_countess_rune_more, val); }

    bool get_highrune_switch() const { return get(m_data.m_highrune_switch); }

    void set_highrune_switch(bool val) { return set(m_data.m_highrune_switch, val); }

    qlonglong get_rune_factor() const { return get(m_data.m_rune_factor); }

    void set_rune_factor(qlonglong val) { return set(m_data.m_rune_factor, val); }

    qlonglong get_rune_factor_min() const { return 0; }

    qlonglong get_rune_factor_max() const { return 9000; }

    bool get_rune_factor_isPercent() const { return false; }

    bool get_wraith_runes() const { return get(m_data.m_wraith_runes); }

    void set_wraith_runes(bool val) { return set(m_data.m_wraith_runes, val); }

    qlonglong get_zod_factor() const { return get(m_data.m_zod_factor); }

    void set_zod_factor(qlonglong val) { return set(m_data.m_zod_factor, val); }

    qlonglong get_zod_factor_min() const { return 0; }

    qlonglong get_zod_factor_max() const { return 9000; }

    bool get_zod_factor_isPercent() const { return false; }

private:
    Config::ModuleRuneDrops& m_data;
};

class Config_ModuleSkillRandomizer_Qt : public BaseStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(bool ensureDifferent READ get_ensureDifferent WRITE set_ensureDifferent NOTIFY dataChanged)
    Q_PROPERTY(bool skillDamage READ get_skillDamage WRITE set_skillDamage NOTIFY dataChanged)
    Q_PROPERTY(bool skillTree READ get_skillTree WRITE set_skillTree NOTIFY dataChanged)

public:
    explicit Config_ModuleSkillRandomizer_Qt(Config::ModuleSkillRandomizer& data)
        : BaseStruct_Qt(data)
        , m_data(data)
    {
    }

    bool get_ensureDifferent() const { return get(m_data.m_ensureDifferent); }

    void set_ensureDifferent(bool val) { return set(m_data.m_ensureDifferent, val); }

    bool get_skillDamage() const { return get(m_data.m_skillDamage); }

    void set_skillDamage(bool val) { return set(m_data.m_skillDamage, val); }

    bool get_skillTree() const { return get(m_data.m_skillTree); }

    void set_skillTree(bool val) { return set(m_data.m_skillTree, val); }

private:
    Config::ModuleSkillRandomizer& m_data;
};

class Config_Qt : public BaseStruct_Qt {
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
        : BaseStruct_Qt(data)
        , m_data(data)
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
