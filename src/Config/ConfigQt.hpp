#pragma once
// Do not edit, this file was generated from schema.yaml using update.py

#include "Config.hpp"
#include "StructBaseQt.hpp"

namespace D2ModGen {

class Config_TreasureClasses_Runes_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* countessSpecialRuneLevel READ get_countessSpecialRuneLevel CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* countessSpecialRuneMore READ get_countessSpecialRuneMore CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* wraithOnlyRunes READ get_wraithOnlyRunes CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* runeInGoodTC READ get_runeInGoodTC CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* zodFactor READ get_zodFactor CONSTANT)

public:
    explicit Config_TreasureClasses_Runes_Qt(Config::TreasureClasses::Runes& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_countessSpecialRuneLevel(data.getPropertyInt(0))
        , m_countessSpecialRuneMore(data.getPropertyBool(1))
        , m_wraithOnlyRunes(data.getPropertyBool(2))
        , m_runeInGoodTC(data.getPropertyInt(1))
        , m_zodFactor(data.getPropertyInt(2))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyInt_Qt* get_countessSpecialRuneLevel() { return &m_countessSpecialRuneLevel; }

    PropertyBool_Qt* get_countessSpecialRuneMore() { return &m_countessSpecialRuneMore; }

    PropertyBool_Qt* get_wraithOnlyRunes() { return &m_wraithOnlyRunes; }

    PropertyInt_Qt* get_runeInGoodTC() { return &m_runeInGoodTC; }

    PropertyInt_Qt* get_zodFactor() { return &m_zodFactor; }

    Q_INVOKABLE void assign(Config_TreasureClasses_Runes_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::TreasureClasses::Runes& m_data;

    PropertyBool_Qt m_enabled;

    PropertyInt_Qt m_countessSpecialRuneLevel;

    PropertyBool_Qt m_countessSpecialRuneMore;

    PropertyBool_Qt m_wraithOnlyRunes;

    PropertyInt_Qt m_runeInGoodTC;

    PropertyInt_Qt m_zodFactor;
};

class Config_TreasureClasses_Items_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* forceQuestDrops READ get_forceQuestDrops CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chanceForHighLevel READ get_chanceForHighLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* noneliteFactorIncrease READ get_noneliteFactorIncrease CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rareFactorIncrease READ get_rareFactorIncrease CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* setFactorIncrease READ get_setFactorIncrease CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniqueFactorIncrease READ get_uniqueFactorIncrease CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* inverseNodropPercent READ get_inverseNodropPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* equipPercent READ get_equipPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* goldPercent READ get_goldPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* goodPercent READ get_goodPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* junkPercent READ get_junkPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* championItemCount READ get_championItemCount CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* uniqueItemCount READ get_uniqueItemCount CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* superItemCount READ get_superItemCount CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* removeChampGold READ get_removeChampGold CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* removeElitePotions READ get_removeElitePotions CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* sameRarityForSetUniqueItems READ get_sameRarityForSetUniqueItems CONSTANT)

public:
    explicit Config_TreasureClasses_Items_Qt(Config::TreasureClasses::Items& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_forceQuestDrops(data.getPropertyBool(1))
        , m_chanceForHighLevel(data.getPropertyInt(0))
        , m_noneliteFactorIncrease(data.getPropertyInt(1))
        , m_rareFactorIncrease(data.getPropertyInt(2))
        , m_setFactorIncrease(data.getPropertyInt(3))
        , m_uniqueFactorIncrease(data.getPropertyInt(4))
        , m_inverseNodropPercent(data.getPropertyInt(5))
        , m_equipPercent(data.getPropertyInt(6))
        , m_goldPercent(data.getPropertyInt(7))
        , m_goodPercent(data.getPropertyInt(8))
        , m_junkPercent(data.getPropertyInt(9))
        , m_championItemCount(data.getPropertyInt(10))
        , m_uniqueItemCount(data.getPropertyInt(11))
        , m_superItemCount(data.getPropertyInt(12))
        , m_removeChampGold(data.getPropertyBool(2))
        , m_removeElitePotions(data.getPropertyBool(3))
        , m_sameRarityForSetUniqueItems(data.getPropertyBool(4))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyBool_Qt* get_forceQuestDrops() { return &m_forceQuestDrops; }

    PropertyInt_Qt* get_chanceForHighLevel() { return &m_chanceForHighLevel; }

    PropertyInt_Qt* get_noneliteFactorIncrease() { return &m_noneliteFactorIncrease; }

    PropertyInt_Qt* get_rareFactorIncrease() { return &m_rareFactorIncrease; }

    PropertyInt_Qt* get_setFactorIncrease() { return &m_setFactorIncrease; }

    PropertyInt_Qt* get_uniqueFactorIncrease() { return &m_uniqueFactorIncrease; }

    PropertyInt_Qt* get_inverseNodropPercent() { return &m_inverseNodropPercent; }

    PropertyInt_Qt* get_equipPercent() { return &m_equipPercent; }

    PropertyInt_Qt* get_goldPercent() { return &m_goldPercent; }

    PropertyInt_Qt* get_goodPercent() { return &m_goodPercent; }

    PropertyInt_Qt* get_junkPercent() { return &m_junkPercent; }

    PropertyInt_Qt* get_championItemCount() { return &m_championItemCount; }

    PropertyInt_Qt* get_uniqueItemCount() { return &m_uniqueItemCount; }

    PropertyInt_Qt* get_superItemCount() { return &m_superItemCount; }

    PropertyBool_Qt* get_removeChampGold() { return &m_removeChampGold; }

    PropertyBool_Qt* get_removeElitePotions() { return &m_removeElitePotions; }

    PropertyBool_Qt* get_sameRarityForSetUniqueItems() { return &m_sameRarityForSetUniqueItems; }

    Q_INVOKABLE void assign(Config_TreasureClasses_Items_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::TreasureClasses::Items& m_data;

    PropertyBool_Qt m_enabled;

    PropertyBool_Qt m_forceQuestDrops;

    PropertyInt_Qt m_chanceForHighLevel;

    PropertyInt_Qt m_noneliteFactorIncrease;

    PropertyInt_Qt m_rareFactorIncrease;

    PropertyInt_Qt m_setFactorIncrease;

    PropertyInt_Qt m_uniqueFactorIncrease;

    PropertyInt_Qt m_inverseNodropPercent;

    PropertyInt_Qt m_equipPercent;

    PropertyInt_Qt m_goldPercent;

    PropertyInt_Qt m_goodPercent;

    PropertyInt_Qt m_junkPercent;

    PropertyInt_Qt m_championItemCount;

    PropertyInt_Qt m_uniqueItemCount;

    PropertyInt_Qt m_superItemCount;

    PropertyBool_Qt m_removeChampGold;

    PropertyBool_Qt m_removeElitePotions;

    PropertyBool_Qt m_sameRarityForSetUniqueItems;
};

class Config_TreasureClasses_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_TreasureClasses_Runes_Qt* runes READ get_runes CONSTANT)
    Q_PROPERTY(Config_TreasureClasses_Items_Qt* items READ get_items CONSTANT)

public:
    explicit Config_TreasureClasses_Qt(Config::TreasureClasses& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_runes(data.m_runes)
        , m_items(data.m_items)
    {
    }

    Config_TreasureClasses_Runes_Qt* get_runes() { return &m_runes; }

    Config_TreasureClasses_Items_Qt* get_items() { return &m_items; }

    Q_INVOKABLE void assign(Config_TreasureClasses_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::TreasureClasses& m_data;

    Config_TreasureClasses_Runes_Qt m_runes;

    Config_TreasureClasses_Items_Qt m_items;
};

class Config_ItemBalance_CommonSettings_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* doOverride READ get_doOverride CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rollMode READ get_rollMode CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* itemLevel READ get_itemLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reqLevel READ get_reqLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* randoKeepOriginalPercent READ get_randoKeepOriginalPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* randoGenerateMin READ get_randoGenerateMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* randoGenerateMax READ get_randoGenerateMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* powerScaleLevel1 READ get_powerScaleLevel1 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* powerScaleLevel50 READ get_powerScaleLevel50 CONSTANT)

public:
    explicit Config_ItemBalance_CommonSettings_Qt(Config::ItemBalance::CommonSettings& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_doOverride(data.getPropertyBool(0))
        , m_rollMode(data.getPropertyInt(0))
        , m_itemLevel(data.getPropertyInt(1))
        , m_reqLevel(data.getPropertyInt(2))
        , m_randoKeepOriginalPercent(data.getPropertyInt(3))
        , m_randoGenerateMin(data.getPropertyInt(4))
        , m_randoGenerateMax(data.getPropertyInt(5))
        , m_powerScaleLevel1(data.getPropertyInt(6))
        , m_powerScaleLevel50(data.getPropertyInt(7))
    {
    }

    PropertyBool_Qt* get_doOverride() { return &m_doOverride; }

    PropertyInt_Qt* get_rollMode() { return &m_rollMode; }

    PropertyInt_Qt* get_itemLevel() { return &m_itemLevel; }

    PropertyInt_Qt* get_reqLevel() { return &m_reqLevel; }

    PropertyInt_Qt* get_randoKeepOriginalPercent() { return &m_randoKeepOriginalPercent; }

    PropertyInt_Qt* get_randoGenerateMin() { return &m_randoGenerateMin; }

    PropertyInt_Qt* get_randoGenerateMax() { return &m_randoGenerateMax; }

    PropertyInt_Qt* get_powerScaleLevel1() { return &m_powerScaleLevel1; }

    PropertyInt_Qt* get_powerScaleLevel50() { return &m_powerScaleLevel50; }

    Q_INVOKABLE void assign(Config_ItemBalance_CommonSettings_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ItemBalance::CommonSettings& m_data;

    PropertyBool_Qt m_doOverride;

    PropertyInt_Qt m_rollMode;

    PropertyInt_Qt m_itemLevel;

    PropertyInt_Qt m_reqLevel;

    PropertyInt_Qt m_randoKeepOriginalPercent;

    PropertyInt_Qt m_randoGenerateMin;

    PropertyInt_Qt m_randoGenerateMax;

    PropertyInt_Qt m_powerScaleLevel1;

    PropertyInt_Qt m_powerScaleLevel50;
};

class Config_ItemBalance_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* defaults READ get_defaults CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* uniques READ get_uniques CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* setitems READ get_setitems CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* sets READ get_sets CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* affixes READ get_affixes CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* crafted READ get_crafted CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* runewords READ get_runewords CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* gems READ get_gems CONSTANT)
    Q_PROPERTY(Config_ItemBalance_CommonSettings_Qt* equipment READ get_equipment CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reqDex READ get_reqDex CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reqStr READ get_reqStr CONSTANT)

public:
    explicit Config_ItemBalance_Qt(Config::ItemBalance& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_defaults(data.m_defaults)
        , m_uniques(data.m_uniques)
        , m_setitems(data.m_setitems)
        , m_sets(data.m_sets)
        , m_affixes(data.m_affixes)
        , m_crafted(data.m_crafted)
        , m_runewords(data.m_runewords)
        , m_gems(data.m_gems)
        , m_equipment(data.m_equipment)
        , m_reqDex(data.getPropertyInt(0))
        , m_reqStr(data.getPropertyInt(1))
    {
    }

    Config_ItemBalance_CommonSettings_Qt* get_defaults() { return &m_defaults; }

    Config_ItemBalance_CommonSettings_Qt* get_uniques() { return &m_uniques; }

    Config_ItemBalance_CommonSettings_Qt* get_setitems() { return &m_setitems; }

    Config_ItemBalance_CommonSettings_Qt* get_sets() { return &m_sets; }

    Config_ItemBalance_CommonSettings_Qt* get_affixes() { return &m_affixes; }

    Config_ItemBalance_CommonSettings_Qt* get_crafted() { return &m_crafted; }

    Config_ItemBalance_CommonSettings_Qt* get_runewords() { return &m_runewords; }

    Config_ItemBalance_CommonSettings_Qt* get_gems() { return &m_gems; }

    Config_ItemBalance_CommonSettings_Qt* get_equipment() { return &m_equipment; }

    PropertyInt_Qt* get_reqDex() { return &m_reqDex; }

    PropertyInt_Qt* get_reqStr() { return &m_reqStr; }

    Q_INVOKABLE void assign(Config_ItemBalance_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ItemBalance& m_data;

    Config_ItemBalance_CommonSettings_Qt m_defaults;

    Config_ItemBalance_CommonSettings_Qt m_uniques;

    Config_ItemBalance_CommonSettings_Qt m_setitems;

    Config_ItemBalance_CommonSettings_Qt m_sets;

    Config_ItemBalance_CommonSettings_Qt m_affixes;

    Config_ItemBalance_CommonSettings_Qt m_crafted;

    Config_ItemBalance_CommonSettings_Qt m_runewords;

    Config_ItemBalance_CommonSettings_Qt m_gems;

    Config_ItemBalance_CommonSettings_Qt m_equipment;

    PropertyInt_Qt m_reqDex;

    PropertyInt_Qt m_reqStr;
};

class Config_ModuleMain_Qt : public CommonStruct_Qt {
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
    Q_PROPERTY(PropertyBool_Qt* dumpIn READ get_dumpIn CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* dumpOut READ get_dumpOut CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* dumpDiff READ get_dumpDiff CONSTANT)

public:
    explicit Config_ModuleMain_Qt(Config::ModuleMain& data)
        : CommonStruct_Qt(data)
        , m_data(data)
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
        , m_dumpIn(data.getPropertyBool(1))
        , m_dumpOut(data.getPropertyBool(2))
        , m_dumpDiff(data.getPropertyBool(3))
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

    PropertyBool_Qt* get_dumpIn() { return &m_dumpIn; }

    PropertyBool_Qt* get_dumpOut() { return &m_dumpOut; }

    PropertyBool_Qt* get_dumpDiff() { return &m_dumpDiff; }

    Q_INVOKABLE void assign(Config_ModuleMain_Qt* another) { m_data.assign(another->m_data, true); }

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

    PropertyBool_Qt m_dumpIn;

    PropertyBool_Qt m_dumpOut;

    PropertyBool_Qt m_dumpDiff;
};

class Config_ModuleChallenge_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* levelIncreaseHell READ get_levelIncreaseHell CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelIncreaseNightmare READ get_levelIncreaseNightmare CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* levelIncreaseOverBound READ get_levelIncreaseOverBound CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* normalResPenalty READ get_normalResPenalty CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* nightmareResPenalty READ get_nightmareResPenalty CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* hellResPenalty READ get_hellResPenalty CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* disableDropsHealthPot READ get_disableDropsHealthPot CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* disableDropsManaPot READ get_disableDropsManaPot CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* disableDropsRejuvPot READ get_disableDropsRejuvPot CONSTANT)

public:
    explicit Config_ModuleChallenge_Qt(Config::ModuleChallenge& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_levelIncreaseHell(data.getPropertyInt(0))
        , m_levelIncreaseNightmare(data.getPropertyInt(1))
        , m_levelIncreaseOverBound(data.getPropertyBool(0))
        , m_normalResPenalty(data.getPropertyInt(2))
        , m_nightmareResPenalty(data.getPropertyInt(3))
        , m_hellResPenalty(data.getPropertyInt(4))
        , m_disableDropsHealthPot(data.getPropertyBool(1))
        , m_disableDropsManaPot(data.getPropertyBool(2))
        , m_disableDropsRejuvPot(data.getPropertyBool(3))
    {
    }

    PropertyInt_Qt* get_levelIncreaseHell() { return &m_levelIncreaseHell; }

    PropertyInt_Qt* get_levelIncreaseNightmare() { return &m_levelIncreaseNightmare; }

    PropertyBool_Qt* get_levelIncreaseOverBound() { return &m_levelIncreaseOverBound; }

    PropertyInt_Qt* get_normalResPenalty() { return &m_normalResPenalty; }

    PropertyInt_Qt* get_nightmareResPenalty() { return &m_nightmareResPenalty; }

    PropertyInt_Qt* get_hellResPenalty() { return &m_hellResPenalty; }

    PropertyBool_Qt* get_disableDropsHealthPot() { return &m_disableDropsHealthPot; }

    PropertyBool_Qt* get_disableDropsManaPot() { return &m_disableDropsManaPot; }

    PropertyBool_Qt* get_disableDropsRejuvPot() { return &m_disableDropsRejuvPot; }

    Q_INVOKABLE void assign(Config_ModuleChallenge_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleChallenge& m_data;

    PropertyInt_Qt m_levelIncreaseHell;

    PropertyInt_Qt m_levelIncreaseNightmare;

    PropertyBool_Qt m_levelIncreaseOverBound;

    PropertyInt_Qt m_normalResPenalty;

    PropertyInt_Qt m_nightmareResPenalty;

    PropertyInt_Qt m_hellResPenalty;

    PropertyBool_Qt m_disableDropsHealthPot;

    PropertyBool_Qt m_disableDropsManaPot;

    PropertyBool_Qt m_disableDropsRejuvPot;
};

class Config_ModuleCharacter_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* addCube READ get_addCube CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* addTeleport READ get_addTeleport CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* idTome READ get_idTome CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* manaPots READ get_manaPots CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mercDam READ get_mercDam CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mercHP READ get_mercHP CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* skillPointLimit READ get_skillPointLimit CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* skillsReqLevel READ get_skillsReqLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* extraSkillPerLevel READ get_extraSkillPerLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* extraStatPerLevel READ get_extraStatPerLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* extraHpPerLevel READ get_extraHpPerLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* extraManaPerLevel READ get_extraManaPerLevel CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* hpPotEfficacy READ get_hpPotEfficacy CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* mpPotEfficacy READ get_mpPotEfficacy CONSTANT)

public:
    explicit Config_ModuleCharacter_Qt(Config::ModuleCharacter& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_addCube(data.getPropertyBool(0))
        , m_addTeleport(data.getPropertyBool(1))
        , m_idTome(data.getPropertyBool(2))
        , m_manaPots(data.getPropertyBool(3))
        , m_mercDam(data.getPropertyInt(0))
        , m_mercHP(data.getPropertyInt(1))
        , m_skillPointLimit(data.getPropertyInt(2))
        , m_skillsReqLevel(data.getPropertyInt(3))
        , m_extraSkillPerLevel(data.getPropertyInt(4))
        , m_extraStatPerLevel(data.getPropertyInt(5))
        , m_extraHpPerLevel(data.getPropertyInt(6))
        , m_extraManaPerLevel(data.getPropertyInt(7))
        , m_hpPotEfficacy(data.getPropertyInt(8))
        , m_mpPotEfficacy(data.getPropertyInt(9))
    {
    }

    PropertyBool_Qt* get_addCube() { return &m_addCube; }

    PropertyBool_Qt* get_addTeleport() { return &m_addTeleport; }

    PropertyBool_Qt* get_idTome() { return &m_idTome; }

    PropertyBool_Qt* get_manaPots() { return &m_manaPots; }

    PropertyInt_Qt* get_mercDam() { return &m_mercDam; }

    PropertyInt_Qt* get_mercHP() { return &m_mercHP; }

    PropertyInt_Qt* get_skillPointLimit() { return &m_skillPointLimit; }

    PropertyInt_Qt* get_skillsReqLevel() { return &m_skillsReqLevel; }

    PropertyInt_Qt* get_extraSkillPerLevel() { return &m_extraSkillPerLevel; }

    PropertyInt_Qt* get_extraStatPerLevel() { return &m_extraStatPerLevel; }

    PropertyInt_Qt* get_extraHpPerLevel() { return &m_extraHpPerLevel; }

    PropertyInt_Qt* get_extraManaPerLevel() { return &m_extraManaPerLevel; }

    PropertyInt_Qt* get_hpPotEfficacy() { return &m_hpPotEfficacy; }

    PropertyInt_Qt* get_mpPotEfficacy() { return &m_mpPotEfficacy; }

    Q_INVOKABLE void assign(Config_ModuleCharacter_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleCharacter& m_data;

    PropertyBool_Qt m_addCube;

    PropertyBool_Qt m_addTeleport;

    PropertyBool_Qt m_idTome;

    PropertyBool_Qt m_manaPots;

    PropertyInt_Qt m_mercDam;

    PropertyInt_Qt m_mercHP;

    PropertyInt_Qt m_skillPointLimit;

    PropertyInt_Qt m_skillsReqLevel;

    PropertyInt_Qt m_extraSkillPerLevel;

    PropertyInt_Qt m_extraStatPerLevel;

    PropertyInt_Qt m_extraHpPerLevel;

    PropertyInt_Qt m_extraManaPerLevel;

    PropertyInt_Qt m_hpPotEfficacy;

    PropertyInt_Qt m_mpPotEfficacy;
};

class Config_ModuleCube_Qt : public CommonStruct_Qt {
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
        : CommonStruct_Qt(data)
        , m_data(data)
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

    Q_INVOKABLE void assign(Config_ModuleCube_Qt* another) { m_data.assign(another->m_data, true); }

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

class Config_ModuleGambling_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* charmGamble READ get_charmGamble CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioElite READ get_ratioElite CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioExc READ get_ratioExc CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioRare READ get_ratioRare CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioSet READ get_ratioSet CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioUnique READ get_ratioUnique CONSTANT)

public:
    explicit Config_ModuleGambling_Qt(Config::ModuleGambling& data)
        : CommonStruct_Qt(data)
        , m_data(data)
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

    Q_INVOKABLE void assign(Config_ModuleGambling_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleGambling& m_data;

    PropertyBool_Qt m_charmGamble;

    PropertyInt_Qt m_ratioElite;

    PropertyInt_Qt m_ratioExc;

    PropertyInt_Qt m_ratioRare;

    PropertyInt_Qt m_ratioSet;

    PropertyInt_Qt m_ratioUnique;
};

class Config_ModuleItemRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* casterTypePercent READ get_casterTypePercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* itemFitPercent READ get_itemFitPercent CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelMinus READ get_levelMinus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelPlus READ get_levelPlus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeatSets READ get_repeatSets CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeatUniques READ get_repeatUniques CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* affixRandom READ get_affixRandom CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* gemsRandom READ get_gemsRandom CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* removeKnock READ get_removeKnock CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* replaceCharges READ get_replaceCharges CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* replaceSkills READ get_replaceSkills CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* updateExisting READ get_updateExisting CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* setBonusMode READ get_setBonusMode CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyAuras READ get_frequencyAuras CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyCharAttributes READ get_frequencyCharAttributes CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyProcs READ get_frequencyProcs CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencyResistances READ get_frequencyResistances CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* frequencySkills READ get_frequencySkills CONSTANT)

public:
    explicit Config_ModuleItemRandomizer_Qt(Config::ModuleItemRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_casterTypePercent(data.getPropertyInt(0))
        , m_itemFitPercent(data.getPropertyInt(1))
        , m_levelMinus(data.getPropertyInt(2))
        , m_levelPlus(data.getPropertyInt(3))
        , m_repeatSets(data.getPropertyInt(4))
        , m_repeatUniques(data.getPropertyInt(5))
        , m_affixRandom(data.getPropertyBool(1))
        , m_gemsRandom(data.getPropertyBool(2))
        , m_removeKnock(data.getPropertyBool(3))
        , m_replaceCharges(data.getPropertyBool(4))
        , m_replaceSkills(data.getPropertyBool(5))
        , m_updateExisting(data.getPropertyBool(6))
        , m_setBonusMode(data.getPropertyInt(6))
        , m_frequencyAuras(data.getPropertyInt(7))
        , m_frequencyCharAttributes(data.getPropertyInt(8))
        , m_frequencyProcs(data.getPropertyInt(9))
        , m_frequencyResistances(data.getPropertyInt(10))
        , m_frequencySkills(data.getPropertyInt(11))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyInt_Qt* get_casterTypePercent() { return &m_casterTypePercent; }

    PropertyInt_Qt* get_itemFitPercent() { return &m_itemFitPercent; }

    PropertyInt_Qt* get_levelMinus() { return &m_levelMinus; }

    PropertyInt_Qt* get_levelPlus() { return &m_levelPlus; }

    PropertyInt_Qt* get_repeatSets() { return &m_repeatSets; }

    PropertyInt_Qt* get_repeatUniques() { return &m_repeatUniques; }

    PropertyBool_Qt* get_affixRandom() { return &m_affixRandom; }

    PropertyBool_Qt* get_gemsRandom() { return &m_gemsRandom; }

    PropertyBool_Qt* get_removeKnock() { return &m_removeKnock; }

    PropertyBool_Qt* get_replaceCharges() { return &m_replaceCharges; }

    PropertyBool_Qt* get_replaceSkills() { return &m_replaceSkills; }

    PropertyBool_Qt* get_updateExisting() { return &m_updateExisting; }

    PropertyInt_Qt* get_setBonusMode() { return &m_setBonusMode; }

    PropertyInt_Qt* get_frequencyAuras() { return &m_frequencyAuras; }

    PropertyInt_Qt* get_frequencyCharAttributes() { return &m_frequencyCharAttributes; }

    PropertyInt_Qt* get_frequencyProcs() { return &m_frequencyProcs; }

    PropertyInt_Qt* get_frequencyResistances() { return &m_frequencyResistances; }

    PropertyInt_Qt* get_frequencySkills() { return &m_frequencySkills; }

    Q_INVOKABLE void assign(Config_ModuleItemRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleItemRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyInt_Qt m_casterTypePercent;

    PropertyInt_Qt m_itemFitPercent;

    PropertyInt_Qt m_levelMinus;

    PropertyInt_Qt m_levelPlus;

    PropertyInt_Qt m_repeatSets;

    PropertyInt_Qt m_repeatUniques;

    PropertyBool_Qt m_affixRandom;

    PropertyBool_Qt m_gemsRandom;

    PropertyBool_Qt m_removeKnock;

    PropertyBool_Qt m_replaceCharges;

    PropertyBool_Qt m_replaceSkills;

    PropertyBool_Qt m_updateExisting;

    PropertyInt_Qt m_setBonusMode;

    PropertyInt_Qt m_frequencyAuras;

    PropertyInt_Qt m_frequencyCharAttributes;

    PropertyInt_Qt m_frequencyProcs;

    PropertyInt_Qt m_frequencyResistances;

    PropertyInt_Qt m_frequencySkills;
};

class Config_ModuleMonDensity_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* density READ get_density CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hellPacks READ get_hellPacks CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monGroups READ get_monGroups CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* packs READ get_packs CONSTANT)

public:
    explicit Config_ModuleMonDensity_Qt(Config::ModuleMonDensity& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_density(data.getPropertyInt(0))
        , m_hellPacks(data.getPropertyBool(0))
        , m_monGroups(data.getPropertyInt(1))
        , m_packs(data.getPropertyInt(2))
    {
    }

    PropertyInt_Qt* get_density() { return &m_density; }

    PropertyBool_Qt* get_hellPacks() { return &m_hellPacks; }

    PropertyInt_Qt* get_monGroups() { return &m_monGroups; }

    PropertyInt_Qt* get_packs() { return &m_packs; }

    Q_INVOKABLE void assign(Config_ModuleMonDensity_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleMonDensity& m_data;

    PropertyInt_Qt m_density;

    PropertyBool_Qt m_hellPacks;

    PropertyInt_Qt m_monGroups;

    PropertyInt_Qt m_packs;
};

class Config_ModuleMonRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hellResistances READ get_hellResistances CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* maxTypes READ get_maxTypes CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* randomizeResistances READ get_randomizeResistances CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* randomizeSpawns READ get_randomizeSpawns CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* spawnedCount READ get_spawnedCount CONSTANT)

public:
    explicit Config_ModuleMonRandomizer_Qt(Config::ModuleMonRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_hellResistances(data.getPropertyBool(1))
        , m_maxTypes(data.getPropertyInt(0))
        , m_randomizeResistances(data.getPropertyBool(2))
        , m_randomizeSpawns(data.getPropertyBool(3))
        , m_spawnedCount(data.getPropertyInt(1))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyBool_Qt* get_hellResistances() { return &m_hellResistances; }

    PropertyInt_Qt* get_maxTypes() { return &m_maxTypes; }

    PropertyBool_Qt* get_randomizeResistances() { return &m_randomizeResistances; }

    PropertyBool_Qt* get_randomizeSpawns() { return &m_randomizeSpawns; }

    PropertyInt_Qt* get_spawnedCount() { return &m_spawnedCount; }

    Q_INVOKABLE void assign(Config_ModuleMonRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleMonRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyBool_Qt m_hellResistances;

    PropertyInt_Qt m_maxTypes;

    PropertyBool_Qt m_randomizeResistances;

    PropertyBool_Qt m_randomizeSpawns;

    PropertyInt_Qt m_spawnedCount;
};

class Config_ModuleMonStats_Qt : public CommonStruct_Qt {
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
        : CommonStruct_Qt(data)
        , m_data(data)
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

    Q_INVOKABLE void assign(Config_ModuleMonStats_Qt* another) { m_data.assign(another->m_data, true); }

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

class Config_ModuleQol_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* disableExpPenalty READ get_disableExpPenalty CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* showItemLevel READ get_showItemLevel CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* uniqueCharmLimit READ get_uniqueCharmLimit CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* weakenTownSkills READ get_weakenTownSkills CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* weakenTownSkillsExt READ get_weakenTownSkillsExt CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* tomeSize READ get_tomeSize CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* quiverSize READ get_quiverSize CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* keySize READ get_keySize CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* reduceCost READ get_reduceCost CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* compactPots READ get_compactPots CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* compactScrolls READ get_compactScrolls CONSTANT)

public:
    explicit Config_ModuleQol_Qt(Config::ModuleQol& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_disableExpPenalty(data.getPropertyBool(0))
        , m_showItemLevel(data.getPropertyBool(1))
        , m_uniqueCharmLimit(data.getPropertyBool(2))
        , m_weakenTownSkills(data.getPropertyBool(3))
        , m_weakenTownSkillsExt(data.getPropertyBool(4))
        , m_tomeSize(data.getPropertyInt(0))
        , m_quiverSize(data.getPropertyInt(1))
        , m_keySize(data.getPropertyInt(2))
        , m_reduceCost(data.getPropertyInt(3))
        , m_compactPots(data.getPropertyBool(5))
        , m_compactScrolls(data.getPropertyBool(6))
    {
    }

    PropertyBool_Qt* get_disableExpPenalty() { return &m_disableExpPenalty; }

    PropertyBool_Qt* get_showItemLevel() { return &m_showItemLevel; }

    PropertyBool_Qt* get_uniqueCharmLimit() { return &m_uniqueCharmLimit; }

    PropertyBool_Qt* get_weakenTownSkills() { return &m_weakenTownSkills; }

    PropertyBool_Qt* get_weakenTownSkillsExt() { return &m_weakenTownSkillsExt; }

    PropertyInt_Qt* get_tomeSize() { return &m_tomeSize; }

    PropertyInt_Qt* get_quiverSize() { return &m_quiverSize; }

    PropertyInt_Qt* get_keySize() { return &m_keySize; }

    PropertyInt_Qt* get_reduceCost() { return &m_reduceCost; }

    PropertyBool_Qt* get_compactPots() { return &m_compactPots; }

    PropertyBool_Qt* get_compactScrolls() { return &m_compactScrolls; }

    Q_INVOKABLE void assign(Config_ModuleQol_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleQol& m_data;

    PropertyBool_Qt m_disableExpPenalty;

    PropertyBool_Qt m_showItemLevel;

    PropertyBool_Qt m_uniqueCharmLimit;

    PropertyBool_Qt m_weakenTownSkills;

    PropertyBool_Qt m_weakenTownSkillsExt;

    PropertyInt_Qt m_tomeSize;

    PropertyInt_Qt m_quiverSize;

    PropertyInt_Qt m_keySize;

    PropertyInt_Qt m_reduceCost;

    PropertyBool_Qt m_compactPots;

    PropertyBool_Qt m_compactScrolls;
};

class Config_ModuleSkillRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* ensureDifferent READ get_ensureDifferent CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* skillDamage READ get_skillDamage CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* skillTree READ get_skillTree CONSTANT)

public:
    explicit Config_ModuleSkillRandomizer_Qt(Config::ModuleSkillRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_ensureDifferent(data.getPropertyBool(1))
        , m_skillDamage(data.getPropertyBool(2))
        , m_skillTree(data.getPropertyBool(3))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyBool_Qt* get_ensureDifferent() { return &m_ensureDifferent; }

    PropertyBool_Qt* get_skillDamage() { return &m_skillDamage; }

    PropertyBool_Qt* get_skillTree() { return &m_skillTree; }

    Q_INVOKABLE void assign(Config_ModuleSkillRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ModuleSkillRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyBool_Qt m_ensureDifferent;

    PropertyBool_Qt m_skillDamage;

    PropertyBool_Qt m_skillTree;
};

class Config_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_TreasureClasses_Qt* treasureClasses READ get_treasureClasses CONSTANT)
    Q_PROPERTY(Config_ItemBalance_Qt* itemBalance READ get_itemBalance CONSTANT)
    Q_PROPERTY(Config_ModuleMain_Qt* moduleMain READ get_moduleMain CONSTANT)
    Q_PROPERTY(Config_ModuleChallenge_Qt* moduleChallenge READ get_moduleChallenge CONSTANT)
    Q_PROPERTY(Config_ModuleCharacter_Qt* moduleCharacter READ get_moduleCharacter CONSTANT)
    Q_PROPERTY(Config_ModuleCube_Qt* moduleCube READ get_moduleCube CONSTANT)
    Q_PROPERTY(Config_ModuleGambling_Qt* moduleGambling READ get_moduleGambling CONSTANT)
    Q_PROPERTY(Config_ModuleItemRandomizer_Qt* moduleItemRandomizer READ get_moduleItemRandomizer CONSTANT)
    Q_PROPERTY(Config_ModuleMonDensity_Qt* moduleMonDensity READ get_moduleMonDensity CONSTANT)
    Q_PROPERTY(Config_ModuleMonRandomizer_Qt* moduleMonRandomizer READ get_moduleMonRandomizer CONSTANT)
    Q_PROPERTY(Config_ModuleMonStats_Qt* moduleMonStats READ get_moduleMonStats CONSTANT)
    Q_PROPERTY(Config_ModuleQol_Qt* moduleQol READ get_moduleQol CONSTANT)
    Q_PROPERTY(Config_ModuleSkillRandomizer_Qt* moduleSkillRandomizer READ get_moduleSkillRandomizer CONSTANT)

public:
    explicit Config_Qt(Config& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_treasureClasses(data.m_treasureClasses)
        , m_itemBalance(data.m_itemBalance)
        , m_moduleMain(data.m_moduleMain)
        , m_moduleChallenge(data.m_moduleChallenge)
        , m_moduleCharacter(data.m_moduleCharacter)
        , m_moduleCube(data.m_moduleCube)
        , m_moduleGambling(data.m_moduleGambling)
        , m_moduleItemRandomizer(data.m_moduleItemRandomizer)
        , m_moduleMonDensity(data.m_moduleMonDensity)
        , m_moduleMonRandomizer(data.m_moduleMonRandomizer)
        , m_moduleMonStats(data.m_moduleMonStats)
        , m_moduleQol(data.m_moduleQol)
        , m_moduleSkillRandomizer(data.m_moduleSkillRandomizer)
    {
    }

    Config_TreasureClasses_Qt* get_treasureClasses() { return &m_treasureClasses; }

    Config_ItemBalance_Qt* get_itemBalance() { return &m_itemBalance; }

    Config_ModuleMain_Qt* get_moduleMain() { return &m_moduleMain; }

    Config_ModuleChallenge_Qt* get_moduleChallenge() { return &m_moduleChallenge; }

    Config_ModuleCharacter_Qt* get_moduleCharacter() { return &m_moduleCharacter; }

    Config_ModuleCube_Qt* get_moduleCube() { return &m_moduleCube; }

    Config_ModuleGambling_Qt* get_moduleGambling() { return &m_moduleGambling; }

    Config_ModuleItemRandomizer_Qt* get_moduleItemRandomizer() { return &m_moduleItemRandomizer; }

    Config_ModuleMonDensity_Qt* get_moduleMonDensity() { return &m_moduleMonDensity; }

    Config_ModuleMonRandomizer_Qt* get_moduleMonRandomizer() { return &m_moduleMonRandomizer; }

    Config_ModuleMonStats_Qt* get_moduleMonStats() { return &m_moduleMonStats; }

    Config_ModuleQol_Qt* get_moduleQol() { return &m_moduleQol; }

    Config_ModuleSkillRandomizer_Qt* get_moduleSkillRandomizer() { return &m_moduleSkillRandomizer; }

    Q_INVOKABLE void assign(Config_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config& m_data;

    Config_TreasureClasses_Qt m_treasureClasses;

    Config_ItemBalance_Qt m_itemBalance;

    Config_ModuleMain_Qt m_moduleMain;

    Config_ModuleChallenge_Qt m_moduleChallenge;

    Config_ModuleCharacter_Qt m_moduleCharacter;

    Config_ModuleCube_Qt m_moduleCube;

    Config_ModuleGambling_Qt m_moduleGambling;

    Config_ModuleItemRandomizer_Qt m_moduleItemRandomizer;

    Config_ModuleMonDensity_Qt m_moduleMonDensity;

    Config_ModuleMonRandomizer_Qt m_moduleMonRandomizer;

    Config_ModuleMonStats_Qt m_moduleMonStats;

    Config_ModuleQol_Qt m_moduleQol;

    Config_ModuleSkillRandomizer_Qt m_moduleSkillRandomizer;
};

}
