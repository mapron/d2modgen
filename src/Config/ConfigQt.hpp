#pragma once
// Do not edit, this file was generated from Config.yaml using update.py

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

class Config_ItemRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* seed READ get_seed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* forceCasterType READ get_forceCasterType CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* itemFit READ get_itemFit CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelMinus READ get_levelMinus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* levelPlus READ get_levelPlus CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeatSets READ get_repeatSets CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* repeatUniques READ get_repeatUniques CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* affixRandom READ get_affixRandom CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* gemsRandom READ get_gemsRandom CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* craftedRandom READ get_craftedRandom CONSTANT)
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
    Q_PROPERTY(PropertyBool_Qt* sameRarityForSetUniqueItems READ get_sameRarityForSetUniqueItems CONSTANT)

public:
    explicit Config_ItemRandomizer_Qt(Config::ItemRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_seed(data.getPropertyInt(0))
        , m_forceCasterType(data.getPropertyInt(1))
        , m_itemFit(data.getPropertyInt(2))
        , m_levelMinus(data.getPropertyInt(3))
        , m_levelPlus(data.getPropertyInt(4))
        , m_repeatSets(data.getPropertyInt(5))
        , m_repeatUniques(data.getPropertyInt(6))
        , m_affixRandom(data.getPropertyBool(1))
        , m_gemsRandom(data.getPropertyBool(2))
        , m_craftedRandom(data.getPropertyBool(3))
        , m_removeKnock(data.getPropertyBool(4))
        , m_replaceCharges(data.getPropertyBool(5))
        , m_replaceSkills(data.getPropertyBool(6))
        , m_updateExisting(data.getPropertyBool(7))
        , m_setBonusMode(data.getPropertyInt(7))
        , m_frequencyAuras(data.getPropertyInt(8))
        , m_frequencyCharAttributes(data.getPropertyInt(9))
        , m_frequencyProcs(data.getPropertyInt(10))
        , m_frequencyResistances(data.getPropertyInt(11))
        , m_frequencySkills(data.getPropertyInt(12))
        , m_sameRarityForSetUniqueItems(data.getPropertyBool(8))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyInt_Qt* get_seed() { return &m_seed; }

    PropertyInt_Qt* get_forceCasterType() { return &m_forceCasterType; }

    PropertyInt_Qt* get_itemFit() { return &m_itemFit; }

    PropertyInt_Qt* get_levelMinus() { return &m_levelMinus; }

    PropertyInt_Qt* get_levelPlus() { return &m_levelPlus; }

    PropertyInt_Qt* get_repeatSets() { return &m_repeatSets; }

    PropertyInt_Qt* get_repeatUniques() { return &m_repeatUniques; }

    PropertyBool_Qt* get_affixRandom() { return &m_affixRandom; }

    PropertyBool_Qt* get_gemsRandom() { return &m_gemsRandom; }

    PropertyBool_Qt* get_craftedRandom() { return &m_craftedRandom; }

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

    PropertyBool_Qt* get_sameRarityForSetUniqueItems() { return &m_sameRarityForSetUniqueItems; }

    Q_INVOKABLE void assign(Config_ItemRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ItemRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyInt_Qt m_seed;

    PropertyInt_Qt m_forceCasterType;

    PropertyInt_Qt m_itemFit;

    PropertyInt_Qt m_levelMinus;

    PropertyInt_Qt m_levelPlus;

    PropertyInt_Qt m_repeatSets;

    PropertyInt_Qt m_repeatUniques;

    PropertyBool_Qt m_affixRandom;

    PropertyBool_Qt m_gemsRandom;

    PropertyBool_Qt m_craftedRandom;

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

    PropertyBool_Qt m_sameRarityForSetUniqueItems;
};

class Config_MonsterRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* seed READ get_seed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* numMonExtra READ get_numMonExtra CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* spawnKeepOriginal READ get_spawnKeepOriginal CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* superuniqueRando READ get_superuniqueRando CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* guestsMin READ get_guestsMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* guestsMax READ get_guestsMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chimeraAIMin READ get_chimeraAIMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chimeraAIMax READ get_chimeraAIMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rollResistancesMin READ get_rollResistancesMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rollResistancesMax READ get_rollResistancesMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rollStatMin READ get_rollStatMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* rollStatMax READ get_rollStatMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* densityMin READ get_densityMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* densityMax READ get_densityMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* elitePacksMin READ get_elitePacksMin CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* elitePacksMax READ get_elitePacksMax CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monGroupIncreaseSize READ get_monGroupIncreaseSize CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hellResistances READ get_hellResistances CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* hellPacks READ get_hellPacks CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* maxPacks READ get_maxPacks CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* minAbsDensity READ get_minAbsDensity CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* minAbsPacks READ get_minAbsPacks CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* minNormalZoneId READ get_minNormalZoneId CONSTANT)

public:
    explicit Config_MonsterRandomizer_Qt(Config::MonsterRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_seed(data.getPropertyInt(0))
        , m_numMonExtra(data.getPropertyInt(1))
        , m_spawnKeepOriginal(data.getPropertyInt(2))
        , m_superuniqueRando(data.getPropertyInt(3))
        , m_guestsMin(data.getPropertyInt(4))
        , m_guestsMax(data.getPropertyInt(5))
        , m_chimeraAIMin(data.getPropertyInt(6))
        , m_chimeraAIMax(data.getPropertyInt(7))
        , m_rollResistancesMin(data.getPropertyInt(8))
        , m_rollResistancesMax(data.getPropertyInt(9))
        , m_rollStatMin(data.getPropertyInt(10))
        , m_rollStatMax(data.getPropertyInt(11))
        , m_densityMin(data.getPropertyInt(12))
        , m_densityMax(data.getPropertyInt(13))
        , m_elitePacksMin(data.getPropertyInt(14))
        , m_elitePacksMax(data.getPropertyInt(15))
        , m_monGroupIncreaseSize(data.getPropertyInt(16))
        , m_hellResistances(data.getPropertyBool(1))
        , m_hellPacks(data.getPropertyBool(2))
        , m_maxPacks(data.getPropertyBool(3))
        , m_minAbsDensity(data.getPropertyInt(17))
        , m_minAbsPacks(data.getPropertyInt(18))
        , m_minNormalZoneId(data.getPropertyInt(19))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyInt_Qt* get_seed() { return &m_seed; }

    PropertyInt_Qt* get_numMonExtra() { return &m_numMonExtra; }

    PropertyInt_Qt* get_spawnKeepOriginal() { return &m_spawnKeepOriginal; }

    PropertyInt_Qt* get_superuniqueRando() { return &m_superuniqueRando; }

    PropertyInt_Qt* get_guestsMin() { return &m_guestsMin; }

    PropertyInt_Qt* get_guestsMax() { return &m_guestsMax; }

    PropertyInt_Qt* get_chimeraAIMin() { return &m_chimeraAIMin; }

    PropertyInt_Qt* get_chimeraAIMax() { return &m_chimeraAIMax; }

    PropertyInt_Qt* get_rollResistancesMin() { return &m_rollResistancesMin; }

    PropertyInt_Qt* get_rollResistancesMax() { return &m_rollResistancesMax; }

    PropertyInt_Qt* get_rollStatMin() { return &m_rollStatMin; }

    PropertyInt_Qt* get_rollStatMax() { return &m_rollStatMax; }

    PropertyInt_Qt* get_densityMin() { return &m_densityMin; }

    PropertyInt_Qt* get_densityMax() { return &m_densityMax; }

    PropertyInt_Qt* get_elitePacksMin() { return &m_elitePacksMin; }

    PropertyInt_Qt* get_elitePacksMax() { return &m_elitePacksMax; }

    PropertyInt_Qt* get_monGroupIncreaseSize() { return &m_monGroupIncreaseSize; }

    PropertyBool_Qt* get_hellResistances() { return &m_hellResistances; }

    PropertyBool_Qt* get_hellPacks() { return &m_hellPacks; }

    PropertyBool_Qt* get_maxPacks() { return &m_maxPacks; }

    PropertyInt_Qt* get_minAbsDensity() { return &m_minAbsDensity; }

    PropertyInt_Qt* get_minAbsPacks() { return &m_minAbsPacks; }

    PropertyInt_Qt* get_minNormalZoneId() { return &m_minNormalZoneId; }

    Q_INVOKABLE void assign(Config_MonsterRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::MonsterRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyInt_Qt m_seed;

    PropertyInt_Qt m_numMonExtra;

    PropertyInt_Qt m_spawnKeepOriginal;

    PropertyInt_Qt m_superuniqueRando;

    PropertyInt_Qt m_guestsMin;

    PropertyInt_Qt m_guestsMax;

    PropertyInt_Qt m_chimeraAIMin;

    PropertyInt_Qt m_chimeraAIMax;

    PropertyInt_Qt m_rollResistancesMin;

    PropertyInt_Qt m_rollResistancesMax;

    PropertyInt_Qt m_rollStatMin;

    PropertyInt_Qt m_rollStatMax;

    PropertyInt_Qt m_densityMin;

    PropertyInt_Qt m_densityMax;

    PropertyInt_Qt m_elitePacksMin;

    PropertyInt_Qt m_elitePacksMax;

    PropertyInt_Qt m_monGroupIncreaseSize;

    PropertyBool_Qt m_hellResistances;

    PropertyBool_Qt m_hellPacks;

    PropertyBool_Qt m_maxPacks;

    PropertyInt_Qt m_minAbsDensity;

    PropertyInt_Qt m_minAbsPacks;

    PropertyInt_Qt m_minNormalZoneId;
};

class Config_SkillRandomizer_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* enabled READ get_enabled CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* seed READ get_seed CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* chanceElemChange READ get_chanceElemChange CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* maximumElements READ get_maximumElements CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* randoMode READ get_randoMode CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* specificClass READ get_specificClass CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* keepOriginal READ get_keepOriginal CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* generate READ get_generate CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* globalHitShift READ get_globalHitShift CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightAma READ get_weightAma CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightSor READ get_weightSor CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightNec READ get_weightNec CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightPal READ get_weightPal CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightBar READ get_weightBar CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightDru READ get_weightDru CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightAss READ get_weightAss CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* weightWar READ get_weightWar CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow1 READ get_lvlreqRow1 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow2 READ get_lvlreqRow2 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow3 READ get_lvlreqRow3 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow4 READ get_lvlreqRow4 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow5 READ get_lvlreqRow5 CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* lvlreqRow6 READ get_lvlreqRow6 CONSTANT)

public:
    explicit Config_SkillRandomizer_Qt(Config::SkillRandomizer& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_enabled(data.getPropertyBool(0))
        , m_seed(data.getPropertyInt(0))
        , m_chanceElemChange(data.getPropertyInt(1))
        , m_maximumElements(data.getPropertyInt(2))
        , m_randoMode(data.getPropertyInt(3))
        , m_specificClass(data.getPropertyInt(4))
        , m_keepOriginal(data.getPropertyInt(5))
        , m_generate(data.getPropertyInt(6))
        , m_globalHitShift(data.getPropertyInt(7))
        , m_weightAma(data.getPropertyInt(8))
        , m_weightSor(data.getPropertyInt(9))
        , m_weightNec(data.getPropertyInt(10))
        , m_weightPal(data.getPropertyInt(11))
        , m_weightBar(data.getPropertyInt(12))
        , m_weightDru(data.getPropertyInt(13))
        , m_weightAss(data.getPropertyInt(14))
        , m_weightWar(data.getPropertyInt(15))
        , m_lvlreqRow1(data.getPropertyInt(16))
        , m_lvlreqRow2(data.getPropertyInt(17))
        , m_lvlreqRow3(data.getPropertyInt(18))
        , m_lvlreqRow4(data.getPropertyInt(19))
        , m_lvlreqRow5(data.getPropertyInt(20))
        , m_lvlreqRow6(data.getPropertyInt(21))
    {
    }

    PropertyBool_Qt* get_enabled() { return &m_enabled; }

    PropertyInt_Qt* get_seed() { return &m_seed; }

    PropertyInt_Qt* get_chanceElemChange() { return &m_chanceElemChange; }

    PropertyInt_Qt* get_maximumElements() { return &m_maximumElements; }

    PropertyInt_Qt* get_randoMode() { return &m_randoMode; }

    PropertyInt_Qt* get_specificClass() { return &m_specificClass; }

    PropertyInt_Qt* get_keepOriginal() { return &m_keepOriginal; }

    PropertyInt_Qt* get_generate() { return &m_generate; }

    PropertyInt_Qt* get_globalHitShift() { return &m_globalHitShift; }

    PropertyInt_Qt* get_weightAma() { return &m_weightAma; }

    PropertyInt_Qt* get_weightSor() { return &m_weightSor; }

    PropertyInt_Qt* get_weightNec() { return &m_weightNec; }

    PropertyInt_Qt* get_weightPal() { return &m_weightPal; }

    PropertyInt_Qt* get_weightBar() { return &m_weightBar; }

    PropertyInt_Qt* get_weightDru() { return &m_weightDru; }

    PropertyInt_Qt* get_weightAss() { return &m_weightAss; }

    PropertyInt_Qt* get_weightWar() { return &m_weightWar; }

    PropertyInt_Qt* get_lvlreqRow1() { return &m_lvlreqRow1; }

    PropertyInt_Qt* get_lvlreqRow2() { return &m_lvlreqRow2; }

    PropertyInt_Qt* get_lvlreqRow3() { return &m_lvlreqRow3; }

    PropertyInt_Qt* get_lvlreqRow4() { return &m_lvlreqRow4; }

    PropertyInt_Qt* get_lvlreqRow5() { return &m_lvlreqRow5; }

    PropertyInt_Qt* get_lvlreqRow6() { return &m_lvlreqRow6; }

    Q_INVOKABLE void assign(Config_SkillRandomizer_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::SkillRandomizer& m_data;

    PropertyBool_Qt m_enabled;

    PropertyInt_Qt m_seed;

    PropertyInt_Qt m_chanceElemChange;

    PropertyInt_Qt m_maximumElements;

    PropertyInt_Qt m_randoMode;

    PropertyInt_Qt m_specificClass;

    PropertyInt_Qt m_keepOriginal;

    PropertyInt_Qt m_generate;

    PropertyInt_Qt m_globalHitShift;

    PropertyInt_Qt m_weightAma;

    PropertyInt_Qt m_weightSor;

    PropertyInt_Qt m_weightNec;

    PropertyInt_Qt m_weightPal;

    PropertyInt_Qt m_weightBar;

    PropertyInt_Qt m_weightDru;

    PropertyInt_Qt m_weightAss;

    PropertyInt_Qt m_weightWar;

    PropertyInt_Qt m_lvlreqRow1;

    PropertyInt_Qt m_lvlreqRow2;

    PropertyInt_Qt m_lvlreqRow3;

    PropertyInt_Qt m_lvlreqRow4;

    PropertyInt_Qt m_lvlreqRow5;

    PropertyInt_Qt m_lvlreqRow6;
};

class Config_Challenge_Qt : public CommonStruct_Qt {
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
    explicit Config_Challenge_Qt(Config::Challenge& data)
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

    Q_INVOKABLE void assign(Config_Challenge_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::Challenge& m_data;

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

class Config_Character_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* addCube READ get_addCube CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* addTeleport READ get_addTeleport CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* idTome READ get_idTome CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* tpTome READ get_tpTome CONSTANT)
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
    explicit Config_Character_Qt(Config::Character& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_addCube(data.getPropertyBool(0))
        , m_addTeleport(data.getPropertyBool(1))
        , m_idTome(data.getPropertyBool(2))
        , m_tpTome(data.getPropertyBool(3))
        , m_manaPots(data.getPropertyBool(4))
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

    PropertyBool_Qt* get_tpTome() { return &m_tpTome; }

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

    Q_INVOKABLE void assign(Config_Character_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::Character& m_data;

    PropertyBool_Qt m_addCube;

    PropertyBool_Qt m_addTeleport;

    PropertyBool_Qt m_idTome;

    PropertyBool_Qt m_tpTome;

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

class Config_HoradricCube_Qt : public CommonStruct_Qt {
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
    explicit Config_HoradricCube_Qt(Config::HoradricCube& data)
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

    Q_INVOKABLE void assign(Config_HoradricCube_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::HoradricCube& m_data;

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

class Config_Gambling_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyBool_Qt* charmGamble READ get_charmGamble CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioElite READ get_ratioElite CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioExc READ get_ratioExc CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioRare READ get_ratioRare CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioSet READ get_ratioSet CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* ratioUnique READ get_ratioUnique CONSTANT)

public:
    explicit Config_Gambling_Qt(Config::Gambling& data)
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

    Q_INVOKABLE void assign(Config_Gambling_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::Gambling& m_data;

    PropertyBool_Qt m_charmGamble;

    PropertyInt_Qt m_ratioElite;

    PropertyInt_Qt m_ratioExc;

    PropertyInt_Qt m_ratioRare;

    PropertyInt_Qt m_ratioSet;

    PropertyInt_Qt m_ratioUnique;
};

class Config_MonsterStats_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* monsterAttack READ get_monsterAttack CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monsterDamage READ get_monsterDamage CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monsterDefense READ get_monsterDefense CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monsterHealth READ get_monsterHealth CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* monsterExperience READ get_monsterExperience CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* immunityLimit READ get_immunityLimit CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* relativeResistDmg READ get_relativeResistDmg CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyFire READ get_applyFire CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyCold READ get_applyCold CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyLight READ get_applyLight CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyPois READ get_applyPois CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyMag READ get_applyMag CONSTANT)
    Q_PROPERTY(PropertyBool_Qt* applyPhys READ get_applyPhys CONSTANT)

public:
    explicit Config_MonsterStats_Qt(Config::MonsterStats& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_monsterAttack(data.getPropertyInt(0))
        , m_monsterDamage(data.getPropertyInt(1))
        , m_monsterDefense(data.getPropertyInt(2))
        , m_monsterHealth(data.getPropertyInt(3))
        , m_monsterExperience(data.getPropertyInt(4))
        , m_immunityLimit(data.getPropertyInt(5))
        , m_relativeResistDmg(data.getPropertyInt(6))
        , m_applyFire(data.getPropertyBool(0))
        , m_applyCold(data.getPropertyBool(1))
        , m_applyLight(data.getPropertyBool(2))
        , m_applyPois(data.getPropertyBool(3))
        , m_applyMag(data.getPropertyBool(4))
        , m_applyPhys(data.getPropertyBool(5))
    {
    }

    PropertyInt_Qt* get_monsterAttack() { return &m_monsterAttack; }

    PropertyInt_Qt* get_monsterDamage() { return &m_monsterDamage; }

    PropertyInt_Qt* get_monsterDefense() { return &m_monsterDefense; }

    PropertyInt_Qt* get_monsterHealth() { return &m_monsterHealth; }

    PropertyInt_Qt* get_monsterExperience() { return &m_monsterExperience; }

    PropertyInt_Qt* get_immunityLimit() { return &m_immunityLimit; }

    PropertyInt_Qt* get_relativeResistDmg() { return &m_relativeResistDmg; }

    PropertyBool_Qt* get_applyFire() { return &m_applyFire; }

    PropertyBool_Qt* get_applyCold() { return &m_applyCold; }

    PropertyBool_Qt* get_applyLight() { return &m_applyLight; }

    PropertyBool_Qt* get_applyPois() { return &m_applyPois; }

    PropertyBool_Qt* get_applyMag() { return &m_applyMag; }

    PropertyBool_Qt* get_applyPhys() { return &m_applyPhys; }

    Q_INVOKABLE void assign(Config_MonsterStats_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::MonsterStats& m_data;

    PropertyInt_Qt m_monsterAttack;

    PropertyInt_Qt m_monsterDamage;

    PropertyInt_Qt m_monsterDefense;

    PropertyInt_Qt m_monsterHealth;

    PropertyInt_Qt m_monsterExperience;

    PropertyInt_Qt m_immunityLimit;

    PropertyInt_Qt m_relativeResistDmg;

    PropertyBool_Qt m_applyFire;

    PropertyBool_Qt m_applyCold;

    PropertyBool_Qt m_applyLight;

    PropertyBool_Qt m_applyPois;

    PropertyBool_Qt m_applyMag;

    PropertyBool_Qt m_applyPhys;
};

class Config_QualityOfLife_Qt : public CommonStruct_Qt {
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
    explicit Config_QualityOfLife_Qt(Config::QualityOfLife& data)
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

    Q_INVOKABLE void assign(Config_QualityOfLife_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::QualityOfLife& m_data;

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

class Config_ExpandedInventory_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* inventoryHeight READ get_inventoryHeight CONSTANT)
    Q_PROPERTY(PropertyInt_Qt* inventoryWidth READ get_inventoryWidth CONSTANT)

public:
    explicit Config_ExpandedInventory_Qt(Config::ExpandedInventory& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_inventoryHeight(data.getPropertyInt(0))
        , m_inventoryWidth(data.getPropertyInt(1))
    {
    }

    PropertyInt_Qt* get_inventoryHeight() { return &m_inventoryHeight; }

    PropertyInt_Qt* get_inventoryWidth() { return &m_inventoryWidth; }

    Q_INVOKABLE void assign(Config_ExpandedInventory_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::ExpandedInventory& m_data;

    PropertyInt_Qt m_inventoryHeight;

    PropertyInt_Qt m_inventoryWidth;
};

class Config_Maps_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(PropertyInt_Qt* mazeMode READ get_mazeMode CONSTANT)

public:
    explicit Config_Maps_Qt(Config::Maps& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_mazeMode(data.getPropertyInt(0))
    {
    }

    PropertyInt_Qt* get_mazeMode() { return &m_mazeMode; }

    Q_INVOKABLE void assign(Config_Maps_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config::Maps& m_data;

    PropertyInt_Qt m_mazeMode;
};

class Config_Qt : public CommonStruct_Qt {
    Q_OBJECT
    Q_PROPERTY(Config_TreasureClasses_Qt* treasureClasses READ get_treasureClasses CONSTANT)
    Q_PROPERTY(Config_ItemBalance_Qt* itemBalance READ get_itemBalance CONSTANT)
    Q_PROPERTY(Config_ItemRandomizer_Qt* itemRandomizer READ get_itemRandomizer CONSTANT)
    Q_PROPERTY(Config_MonsterRandomizer_Qt* monsterRandomizer READ get_monsterRandomizer CONSTANT)
    Q_PROPERTY(Config_SkillRandomizer_Qt* skillRandomizer READ get_skillRandomizer CONSTANT)
    Q_PROPERTY(Config_Challenge_Qt* challenge READ get_challenge CONSTANT)
    Q_PROPERTY(Config_Character_Qt* character READ get_character CONSTANT)
    Q_PROPERTY(Config_HoradricCube_Qt* horadricCube READ get_horadricCube CONSTANT)
    Q_PROPERTY(Config_Gambling_Qt* gambling READ get_gambling CONSTANT)
    Q_PROPERTY(Config_MonsterStats_Qt* monsterStats READ get_monsterStats CONSTANT)
    Q_PROPERTY(Config_QualityOfLife_Qt* qualityOfLife READ get_qualityOfLife CONSTANT)
    Q_PROPERTY(Config_ExpandedInventory_Qt* expandedInventory READ get_expandedInventory CONSTANT)
    Q_PROPERTY(Config_Maps_Qt* maps READ get_maps CONSTANT)

public:
    explicit Config_Qt(Config& data)
        : CommonStruct_Qt(data)
        , m_data(data)
        , m_treasureClasses(data.m_treasureClasses)
        , m_itemBalance(data.m_itemBalance)
        , m_itemRandomizer(data.m_itemRandomizer)
        , m_monsterRandomizer(data.m_monsterRandomizer)
        , m_skillRandomizer(data.m_skillRandomizer)
        , m_challenge(data.m_challenge)
        , m_character(data.m_character)
        , m_horadricCube(data.m_horadricCube)
        , m_gambling(data.m_gambling)
        , m_monsterStats(data.m_monsterStats)
        , m_qualityOfLife(data.m_qualityOfLife)
        , m_expandedInventory(data.m_expandedInventory)
        , m_maps(data.m_maps)
    {
    }

    Config_TreasureClasses_Qt* get_treasureClasses() { return &m_treasureClasses; }

    Config_ItemBalance_Qt* get_itemBalance() { return &m_itemBalance; }

    Config_ItemRandomizer_Qt* get_itemRandomizer() { return &m_itemRandomizer; }

    Config_MonsterRandomizer_Qt* get_monsterRandomizer() { return &m_monsterRandomizer; }

    Config_SkillRandomizer_Qt* get_skillRandomizer() { return &m_skillRandomizer; }

    Config_Challenge_Qt* get_challenge() { return &m_challenge; }

    Config_Character_Qt* get_character() { return &m_character; }

    Config_HoradricCube_Qt* get_horadricCube() { return &m_horadricCube; }

    Config_Gambling_Qt* get_gambling() { return &m_gambling; }

    Config_MonsterStats_Qt* get_monsterStats() { return &m_monsterStats; }

    Config_QualityOfLife_Qt* get_qualityOfLife() { return &m_qualityOfLife; }

    Config_ExpandedInventory_Qt* get_expandedInventory() { return &m_expandedInventory; }

    Config_Maps_Qt* get_maps() { return &m_maps; }

    Q_INVOKABLE void assign(Config_Qt* another) { m_data.assign(another->m_data, true); }

private:
    Config& m_data;

    Config_TreasureClasses_Qt m_treasureClasses;

    Config_ItemBalance_Qt m_itemBalance;

    Config_ItemRandomizer_Qt m_itemRandomizer;

    Config_MonsterRandomizer_Qt m_monsterRandomizer;

    Config_SkillRandomizer_Qt m_skillRandomizer;

    Config_Challenge_Qt m_challenge;

    Config_Character_Qt m_character;

    Config_HoradricCube_Qt m_horadricCube;

    Config_Gambling_Qt m_gambling;

    Config_MonsterStats_Qt m_monsterStats;

    Config_QualityOfLife_Qt m_qualityOfLife;

    Config_ExpandedInventory_Qt m_expandedInventory;

    Config_Maps_Qt m_maps;
};

}
