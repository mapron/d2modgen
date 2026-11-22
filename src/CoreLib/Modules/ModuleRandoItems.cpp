/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleRandoItems.hpp"
#include "MernelPlatform/StringUtils.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"

#include "Logger.hpp"
#include "MernelPlatform/FileIOUtils.hpp"

#include <cstdlib>

namespace D2ModGen {

namespace {
constexpr const int s_maxUnbalanceLevel = 100;

void makeMonotonic(PermilleCurve& curve)
{
    for (int i = 1; i < LEVEL_GROUP_COUNT - 1; i++) {
        if (curve[i] == 0_perm && curve[i - 1] != 0_perm && curve[i + 1] != 0_perm)
            curve[i].raw = (curve[i - 1] + curve[i + 1]).raw / 2;
    }

    for (int i = 1; i < LEVEL_GROUP_COUNT; i++) {
        if (curve[i] < curve[i - 1])
            curve[i] = curve[i - 1];
    }
    for (int i = LEVEL_GROUP_COUNT - 2; i >= 0; i--) {
        if (curve[i] == 0_perm)
            curve[i] = curve[i + 1];
    }
};

std::string intToBase64(int value)
{
    std::string_view base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz"
                                    "0123456789_-";

    std::string result;
    for (int i = 0; i < 3; ++i) {
        result = base64_chars[value % 64] + result;
        value  = value / 64;
    }

    return result;
}

std::string makeKey(int value, char prefix)
{
    return prefix + intToBase64(value);
}

std::string makeValue(std::string old, int copy)
{
    old += ' ';
    old += ('a' + copy);
    return old;
}

}

void ModuleRandoItems::generate()
{
    if (!config.m_itemRandomizer.m_enabled)
        return;

    const auto& module = config.m_itemRandomizer;
    rng                = createRNG(module.m_seed);

    m_codeResolver = [this](const std::string& skillId) -> int {
        if (!skillId.empty()) {
            int intId = std::atoi(skillId.c_str());
            if (intId > 0) {
                return intId;
            }
            if (skillId == "0") {
                return 0;
            }
            auto it = output.skills.indexN.find(Mernel::strToLower(skillId));
            if (it != output.skills.indexN.cend())
                return it->second;
        }
        return -1;
    };
    m_skillGetter = [this](int index) -> DataRow_skills_ptr {
        if (index < 0)
            return nullptr;
        return &output.skills.rows[index];
    };

    std::set<std::string> skippedAttributes;
    std::set<std::string> unknownAttributes;

    m_verbose                 = env.extraTrace.contains("item_rando");
    const bool traceItemProps = env.extraTrace.contains("item_rando_props");

    Logger(m_verbose) << "Grab properties from game data...";
    const AllProps gameProps = grabAllProps(false, &unknownAttributes, &skippedAttributes);
    {
        std::ostringstream skipped, unknown;
        for (auto& str : skippedAttributes)
            skipped << str << ", ";
        for (auto& str : unknownAttributes)
            unknown << str << ", ";
        if (!unknownAttributes.empty())
            Logger(Logger::Warning) << "    Unknown attributes: " << unknown.str();
        if (!skippedAttributes.empty())
            Logger(m_verbose) << "Ignored attributes: " << skipped.str();
    }

    Logger(m_verbose) << "Analyze game properties, build statistic profile...";
    MagicContext magicContext;
    magicContext.actual = analyzeAll(gameProps);

    Logger(m_verbose) << "Init pre-compiled attributes...";

    Logger(m_verbose) << "precompiled=" << magicContext.precompiledSize
                      << ", actual=" << magicContext.actual.ids.size();

    Logger(m_verbose) << "Add some extra fine-tuned properties...";
    magicContext.loadPrecompiled();
    magicContext.addExtra(config, output.getClassesCount());

    StatisticsTable statisticsTable;

    const int64_t repeatUniquesMax = 5;
    const int64_t repeatSetsMax    = 5;
    const int64_t repeatUniques    = std::clamp(module.m_repeatUniques, int64_t(1), repeatUniquesMax);
    const int64_t repeatSets       = std::clamp(module.m_repeatSets, int64_t(1), repeatSetsMax);

    {
        if (!env.extraFile.empty())
            dumpAttributes(env.extraFile, magicContext.actual);
    }

    createNewItems(repeatUniques, repeatSets);

    if (m_verbose) {
        const AttributeStatistics attrib = makeStatistics(grabAllProps());
        statisticsTable.addSnapshot(attrib, 0);
    }

    PropRandomizer props(magicContext.actual, m_skillGetter);

    const auto& b                = config.m_balanceItems;
    const auto& uniqueSettings   = b.m_uniques.m_doOverride ? b.m_uniques : b.m_defaults;
    const auto& setsSettings     = b.m_sets.m_doOverride ? b.m_sets : b.m_defaults;
    const auto& setitemsSettings = b.m_setitems.m_doOverride ? b.m_setitems : b.m_defaults;
    const auto& affixesSettings  = b.m_affixes.m_doOverride ? b.m_affixes : b.m_defaults;
    const auto& runewordSettings = b.m_runewords.m_doOverride ? b.m_runewords : b.m_defaults;
    const auto& gemSettings      = b.m_gems.m_doOverride ? b.m_gems : b.m_defaults;
    const auto& craftedSettings  = b.m_crafted.m_doOverride ? b.m_crafted : b.m_defaults;
    //const auto& baseEquipSettings = b.m_equipment.m_doOverride ? b.m_equipment : b.m_defaults;

    RandoParams params;
    params.replaceSkills  = module.m_replaceSkills;
    params.replaceCharges = module.m_replaceCharges;
    params.removeKnock    = module.m_removeKnock;
    params.updateExisting = module.m_updateExisting;

    params.levelPlus       = module.m_levelPlus;
    params.levelMinus      = module.m_levelMinus;
    params.typeFitPercent  = module.m_itemFit;
    params.forceCasterType = module.m_forceCasterType;
    params.trashChance     = module.m_trashChance;
    params.luckyChance     = module.m_luckyChance;

    auto paramsSetitems              = params;
    paramsSetitems.common            = setitemsSettings;
    paramsSetitems.uniqueCheckAcross = true;

    auto paramsSets               = params;
    paramsSets.common             = setsSettings;
    paramsSets.supportRandomRange = false;
    paramsSets.uniqueCheckAcross  = true;

    auto paramsGems               = params;
    paramsGems.common             = gemSettings;
    paramsGems.supportRandomRange = false;
    paramsGems.enableGeneration   = module.m_gemsRandom;

    auto paramsAffix             = params;
    paramsAffix.common           = affixesSettings;
    paramsAffix.enableGeneration = module.m_affixRandom;

    auto paramsRunes   = params;
    paramsRunes.common = runewordSettings;

    auto paramsUniq   = params;
    paramsUniq.common = uniqueSettings;

    auto paramsCrafted             = params;
    paramsCrafted.common           = craftedSettings;
    paramsCrafted.enableGeneration = module.m_craftedRandom;
    paramsCrafted.emptyListAllowed = true;

    ItemStatistics allStat;

    auto fillProps = [&props, &allStat, this, traceItemProps]<typename Row>(DataTable<Row>& table, const RandoParams& params) {
        if (!params.enableGeneration)
            return;
        ItemStatistics tableStat;

        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;

            MagicPropItem item;
            item.rng            = &rng;
            item.propRandomizer = &props;
            item.randoParams    = &params;
            item.readParams(row);
            item.readAffixes(row, m_codeResolver, m_skillGetter, nullptr, nullptr);

            if (item.filter.props.slot == SlotType::None || item.filter.props.slot == SlotType::PD2Map)
                continue;

            item.process();
            item.writeTo(row);

            tableStat += item.stats;

            Logger(m_verbose) << "write: " << item.toString(traceItemProps);
        }
        allStat += tableStat;

        Logger(m_verbose) << "table '" << getTableIdString(Row::s_tableId) << "' " << tableStat.toString(ItemStatistics::Mode::Table);
    };

    fillProps(output.uniqueitems, paramsUniq);
    fillProps(output.setitems, paramsSetitems);
    fillProps(output.sets, paramsSets);
    fillProps(output.runes, paramsRunes);

    fillProps(output.gems, paramsGems);
    fillProps(output.magicprefix, paramsAffix);
    fillProps(output.magicsuffix, paramsAffix);
    fillProps(output.cubemain, paramsCrafted);

    Logger(Logger::Notice) << "    " << allStat.toString(ItemStatistics::Mode::All);

    if (module.m_sameRarityForSetUniqueItems) {
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

    if (m_verbose) {
        const AttributeStatistics attrib = makeStatistics(grabAllProps());
        statisticsTable.addSnapshot(attrib, 1);
        statisticsTable.traceStatistics();
    }
}

ModuleRandoItems::AllProps ModuleRandoItems::grabAllProps(bool                   onlySetUniques,
                                                          std::set<std::string>* unknownList,
                                                          std::set<std::string>* skippedList)
{
    AllProps allProps;

    auto grabPropsRow = [this, &allProps, unknownList, skippedList]<typename Row>(const Row& row, SlotType slot, int level) -> int {
        MagicPropItem item;
        item.readParams(row);
        item.readAffixes(row, m_codeResolver, m_skillGetter, unknownList, skippedList);
        int inserted = 0;
        for (auto& affixlist : item.affixlists) {
            for (auto& prop : affixlist.parsedProps) {
                auto& byId = allProps.propsById[prop.id];

                //auto&          byRoll  = byId.propsByRoll[RollType::Common];
                auto&          bySlot  = byId.propsBySlot[slot];
                MagicPropList& byLevel = bySlot.propsByLevel[level];

                byLevel.push_back(prop);
                inserted++;
            }
        }
        return inserted;
    };

    auto grabProps = [this, &grabPropsRow]<typename Row>(const DataTable<Row>& table) -> int {
        int result = 0;
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int level = row.getLevel();

            ItemCodeList types = row.getItemFilter();
            auto         slot  = types.determineSlot();
            if (slot == SlotType::PD2Map) {
                continue;
            }
            if (slot == SlotType::None) {
                Logger(Logger::Warning) << "Undefined slot type for item: " << row.getId();
                continue;
            }

            result += grabPropsRow(row, slot, level);
        }
        return result;
    };
    grabProps(output.uniqueitems);
    grabProps(output.setitems);
    if (onlySetUniques)
        return allProps;

    grabProps(output.sets);
    grabProps(output.runes);
    grabProps(output.gems);
    grabProps(output.magicprefix);
    grabProps(output.magicsuffix);
    grabProps(output.automagic);
    grabProps(output.cubemain);

    return allProps;
}

void ModuleRandoItems::createNewItems(int repeatUniques, int repeatSets)
{
    const int setBonusMode = config.m_itemRandomizer.m_setBonusMode;

    if (repeatUniques == 1 && repeatSets == 1)
        return;

    Logger(m_verbose) << "Create additional records for setitems/uniqueitems...";

    const std::vector<std::string> colors{ "whit", "lgry", "dgry", "blac", "lblu", "dblu", "cblu", "lred", "dred", "cred", "lgrn", "dgrn", "cgrn", "lyel", "dyel", "lgld", "dgld", "lpur", "dpur", "oran", "bwht" };

    auto getRandomColor = [this, &colors]() -> std::string {
        const int index = rng(50);
        if (index < colors.size()) {
            return colors[index];
        }
        return "";
    };
    std::vector<EquipmentType> validArmor;
    std::vector<EquipmentType> validWeapon;
    for (auto& row : output.armor.rows) {
        if (row.getTier() != Tier::None)
            validArmor.push_back(&row);
    }
    for (auto& row : output.weapons.rows) {
        if (row.getTier() != Tier::None)
            validWeapon.push_back(&row);
    }

    {
        auto getNewLevelAndCode = [this, validArmor, validWeapon](std::optional<int>& lvl,
                                                                  EquipmentType&      eqipmentType,
                                                                  std::string&        eqipmentCode,
                                                                  std::string&        eqipmentName) {
            int base_ilvl = -1;
            if (eqipmentType->category == EquipmentCategory::Armor) {
                eqipmentType = validArmor[rng(validArmor.size())];
                base_ilvl    = eqipmentType->lvl.value();
            } else if (eqipmentType->category == EquipmentCategory::Weapon) {
                eqipmentType = validWeapon[rng(validWeapon.size())];
                base_ilvl    = eqipmentType->lvl.value();
            }
            const int oldLvl = lvl.value_or(1);

            const int oldLvl1 = base_ilvl == -1 ? oldLvl : (oldLvl + base_ilvl) / 2;
            const int minLvl  = std::max(2, oldLvl1 - 10);
            const int maxLvl  = base_ilvl == -1 ? 87 : std::min(87, oldLvl1 + 20);
            const int newLvl  = minLvl + rng(maxLvl - minLvl);
            lvl               = newLvl;

            eqipmentName = eqipmentType->name;
            eqipmentCode = eqipmentType->code;
        };

        auto processTableWithCopies = []<typename Row>(DataTable<Row>& table, int copies, auto&& beforeCopy, auto&& afterCopy) {
            std::vector<Row*> validRows;
            validRows.reserve(table.rows.size());
            for (int id = 0; auto& row : table.rows) {
                if (row.isValid()) {
                    if (beforeCopy(row))
                        validRows.push_back(&row);
                }
                id++;
            }
            table.extra.reserve(validRows.size() * (copies - 1));

            for (Row* row_ : validRows) {
                for (int i = 1; i < copies; ++i) {
                    Row row = *row_;
                    row._ser_unusedCells.clear();

                    afterCopy(int(table.extra.size() + table.rows.size()), i, row, *row_);
                    table.extra.push_back(std::move(row));
                }
            }
            table.commitExtraRows();
        };

        auto beforeCopyUniq = [this](DataRow_uniqueitems& row) -> bool {
            row.disableChronicle = true;
            if (row.eqipmentType->category != EquipmentCategory::Misc)
                row.lvlreq = 2;
            else {
                auto slot = row.getItemFilter().determineSlot();
                if (slot == SlotType::Amulet || slot == SlotType::Charm || slot == SlotType::Ring || slot == SlotType::Weapon)
                    return true;
                Logger(m_verbose) << "uniq copy skip: " << row.getId();
                return false;
            }
            return true;
        };
        auto afterCopyUniq = [this, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_uniqueitems& row, const DataRow_uniqueitems& old) {
            row.index       = makeKey(id, 'u');
            row.displayName = makeValue(old.index, i);
            output.newLoc.items.push_back({ row.index, row.displayName });

            row.version         = 100;
            row.ux              = {};
            row.ux.id           = id;
            row.ux.chrtransform = row.ux.invtransform = getRandomColor();
            getNewLevelAndCode(row.lvl, row.eqipmentType, row.eqipmentCode, row.ux.itemName);
        };

        processTableWithCopies(output.uniqueitems, repeatUniques, beforeCopyUniq, afterCopyUniq);

        auto beforeCopySetitem = [&setBonusMode](DataRow_setitems& row) {
            row.disableChronicle = true;
            if (row.eqipmentType->category != EquipmentCategory::Misc)
                row.lvlreq = 2;

            // addfunc:
            // 0 -> no green properties on item (apropxx will appear as a blue attribute on the list instead).
            // 1 -> green properties (apropxx) depend on which other items from the set are equipped. (only Civerb's Ward)
            // 2 -> green properties (apropxx) depend on how many other items from the set are equipped.
            if (setBonusMode == 1) {
                row.addfunc = 0;
            }
            if (setBonusMode == 2) {
                row.addfunc = 2;
            }
            return true;
        };
        auto afterCopySetitem = [this, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_setitems& row, const DataRow_setitems& old) {
            row.index       = makeKey(id, 's');
            row.displayName = makeValue(old.index, i);
            output.newLoc.items.push_back({ row.index, row.displayName });

            row.setIndex = makeValue(old.setIndex, i);

            row.ux              = {};
            row.ux.id           = id;
            row.ux.chrtransform = row.ux.invtransform = getRandomColor();
            getNewLevelAndCode(row.lvl, row.eqipmentType, row.eqipmentCode, row.ux.itemName);
        };

        processTableWithCopies(output.setitems, repeatSets, beforeCopySetitem, afterCopySetitem);

        auto beforeCopySet = [](DataRow_sets& row) {
            return true;
        };
        auto afterCopySet = [this, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_sets& row, const DataRow_sets& old) {
            row.index = makeValue(old.index, i);
            output.newLoc.items.push_back({ row.index, row.index });

            row.name = row.index;
        };

        processTableWithCopies(output.sets, repeatSets, beforeCopySet, afterCopySet);
    }
}

AllAttributeData ModuleRandoItems::analyzeAll(const AllProps& allProps) const
{
    AllAttributeData                   result;
    NonParRecordGroupMap&              groupMap = result.groupMap;
    NonParRecordMap&                   idMap    = result.idMap;
    ParRecordMap&                      parMap   = result.parMap;
    AllProps                           trivialNoGroup;
    std::map<AttributeGroup, AllProps> trivial;

    NonParValueList all;

    for (const auto& [code, codeProps] : allProps.propsById) {
        const auto& desc = getAttributeDesc(code);

        const bool ignore = desc.flags.contains(AttributeFlag::Keep) || desc.flags.contains(AttributeFlag::PD2Map);
        if (ignore)
            continue;

        const bool hasPar      = desc.parMode != ParMode::None;
        const bool hasAnyRange = desc.rangeMode != RangeMode::None;
        if (!hasPar && !hasAnyRange)
            continue;

        const bool isTrivialPar = desc.parMode == ParMode::PureValue || desc.parMode == ParMode::None;

        if (isTrivialPar) {
            if (desc.group == AttributeGroup::None) {
                trivialNoGroup.propsById[code] = codeProps;
            } else {
                trivial[desc.group].propsById[code] = codeProps;
            }

        } else {
            ParRecord& parRecord = parMap[code];
            parRecord.id         = code;
            std::map<int, SlotPropsVal> byPar;
            for (const auto& [slot, slotProps] : codeProps.propsBySlot) {
                parRecord.slotTypes.insert(slot);
                for (const auto& [level, levelProps] : slotProps.propsByLevel) {
                    for (const MagicProp& prop : levelProps) {
                        byPar[prop.par.value_or(0)].propsByLevel[level].push_back(prop);
                    }
                }
            }
            for (const auto& [par, slotProps] : byPar) {
                ParRecordParam parRecordParam;
                parRecordParam.par = par;
                analyzePar(slotProps, parRecordParam);
                if (parRecordParam.minLevel < 999) {
                    parRecordParam.minLevel = std::min(81, parRecordParam.minLevel);
                    parRecord.params.push_back(std::move(parRecordParam));
                }
            }
        }
    }

    for (const auto& [groupId, trivialProps] : trivial) {
        NonParRecord record;
        record.group = groupId;
        AllPropsVal allPropsVal;
        for (const auto& [attrId, allVal] : trivialProps.propsById) {
            record.ids.insert(attrId);
            for (const auto& [slot, slotProps] : allVal.propsBySlot) {
                for (const auto& [level, levelProps] : slotProps.propsByLevel) {
                    allPropsVal.propsBySlot[slot].propsByLevel[level].append_range(levelProps);
                }
            }
        }

        analyzeNonPar(allPropsVal, record);
        record.freq /= record.ids.size();
        if (record.minLevel < 999) {
            record.minLevel   = std::min(81, record.minLevel);
            groupMap[groupId] = std::move(record);
        }
    }
    for (const auto& [code, codeProps] : trivialNoGroup.propsById) {
        NonParRecord record;
        record.ids.insert(code);

        analyzeNonPar(codeProps, record);

        if (record.minLevel < 999) {
            record.minLevel = std::min(81, record.minLevel);
            idMap[code]     = std::move(record);
        }
    }

    result.refreshIds();
    return result;
}

AttributeStatistics ModuleRandoItems::makeStatistics(const AllProps& allProps) const
{
    AttributeStatistics result;

    for (const auto& [code, codeProps] : allProps.propsById) {
        const auto& desc = getAttributeDesc(code);

        const bool ignore = desc.flags.contains(AttributeFlag::Keep) || desc.flags.contains(AttributeFlag::PD2Map);
        if (ignore)
            continue;

        NonParValueList all;
        for (const auto& [slot, slotProps] : codeProps.propsBySlot) {
            for (const auto& [level, levelProps] : slotProps.propsByLevel) {
                for (auto prop : levelProps) {
                    all.push_back({ level, prop.min.value_or(0), prop.max.value_or(0), prop.par.value_or(0) });
                }
            }
        }

        result[code].make(all);
    }

    return result;
}

void ModuleRandoItems::analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecord& record) const
{
    NonParValueList                     all;
    NonParValueList                     penalty;
    std::map<SlotType, NonParValueList> bySlot;

    for (const auto& [slot, slotProps] : allPropsVal.propsBySlot) {
        NonParValueList& valuesSlot = bySlot[slot];
        for (const auto& [level, levelProps] : slotProps.propsByLevel) {
            assert(level >= -1 && level < 260);
            for (auto prop : levelProps) {
                const auto min = prop.min.value_or(0);
                const auto max = prop.max.value_or(0);
                const auto par = prop.par.value_or(0);

                if (min >= 0 && max > 0) {
                    valuesSlot.push_back({ level, min, max, par });
                    all.push_back({ level, min, max, par });
                }
                if (max < 0) {
                    penalty.push_back({ level, min, max, par });
                }
            }
        }
    }
    {
        NonParValueSummary summAll;
        summAll.make(all);
        record.freq      = all.size() * FREQ_PER_ONE_PROP;
        record.minMedian = summAll.low.p50;
        record.maxMedian = summAll.high.p50;
        record.limit     = summAll.high.max;
    }
    {
        NonParValueSummary summpenalty;
        summpenalty.make(penalty);
        record.freq += penalty.size() * FREQ_PER_ONE_PROP;
        record.penMax = std::min(summpenalty.high.p50, summpenalty.low.p50);
    }

    NonParValueList valuesGoodScaledToMedian;
    for (auto& [slot, slotProps] : bySlot) {
        NonParValueSummary summ;
        summ.make(slotProps);

        int slotMedian = summ.high.p50;
        int val        = slotMedian * 1000 / record.maxMedian;
        if (val < 900 || val > 1100)
            record.slotScale.insert(slot, Permille{ val });

        record.slotTypes.insert(slot);

        for (auto& val : slotProps) {
            val.max = val.max * 1000 / slotMedian;
            val.min = val.min * 1000 / slotMedian;
        }
        valuesGoodScaledToMedian.append_range(slotProps);
    }

    record.minLevel = 999;
    record.medCurve = NonParRecord::s_emptyCurve;

    record.ratioP000 = NonParRecord::s_emptyCurve;
    record.ratioP010 = NonParRecord::s_emptyCurve;
    record.ratioP090 = NonParRecord::s_emptyCurve;
    record.ratioP100 = NonParRecord::s_emptyCurve;
    std::vector<NonParValueList> levelGroups(LEVEL_GROUP_COUNT);
    for (const NonParValue& value : valuesGoodScaledToMedian) {
        int levelGroup = makeTier(value.level);
        levelGroups[levelGroup].push_back(value);
        record.minLevel = std::min(record.minLevel, value.level);
    }
    for (int i = 0; i < LEVEL_GROUP_COUNT; i++) {
        NonParValueSummary summ;
        summ.make(levelGroups[i]);
        if (!summ.freq)
            continue;

        record.medCurve[i] = Permille{ summ.high.p50 };

        record.ratioP000[i] = Permille{ summ.high.min };
        record.ratioP010[i] = Permille{ summ.high.p10 };
        record.ratioP090[i] = Permille{ summ.high.p90 };
        record.ratioP100[i] = Permille{ summ.high.max };
    }

    makeMonotonic(record.medCurve);

    makeMonotonic(record.ratioP000);
    makeMonotonic(record.ratioP010);
    makeMonotonic(record.ratioP090);
    makeMonotonic(record.ratioP100);

    for (int i = 0; i < LEVEL_GROUP_COUNT; i++) {
        auto med            = record.medCurve[i].raw;
        record.ratioP000[i] = Permille{ record.ratioP000[i].raw * 1000 / med };
        record.ratioP010[i] = Permille{ record.ratioP010[i].raw * 1000 / med };
        record.ratioP090[i] = Permille{ record.ratioP090[i].raw * 1000 / med };
        record.ratioP100[i] = Permille{ record.ratioP100[i].raw * 1000 / med };
    }
}

void ModuleRandoItems::analyzePar(const SlotPropsVal& allPropsVal, ParRecordParam& record) const
{
    NonParValueList all;

    for (const auto& [level, levelProps] : allPropsVal.propsByLevel) {
        assert(level >= -1 && level < 260);
        for (auto prop : levelProps) {
            const auto min = prop.min.value_or(0);
            const auto max = prop.max.value_or(0);

            if (min >= 0 && max > 0) {
                all.push_back({ level, min, max, record.par });
            }
            if (max < 0) {
                assert(0);
            }
        }
    }

    {
        NonParValueSummary summAll;
        summAll.make(all);
        record.freq      = summAll.freq;
        record.minMedian = summAll.low.p90;
        record.maxMedian = summAll.high.p90;
    }

    record.minLevel = 999;
    std::vector<NonParValueList> levelGroups(LEVEL_GROUP_COUNT);
    for (NonParValue val : all) {
        val.max        = val.max * 1000 / record.maxMedian;
        val.min        = val.min * 1000 / record.minMedian;
        int levelGroup = makeTier(val.level);
        levelGroups[levelGroup].push_back(val);
        record.minLevel = std::min(record.minLevel, val.level);
    }

    for (int i = 0; i < LEVEL_GROUP_COUNT; i++) {
        NonParValueSummary summ;
        summ.make(levelGroups[i]);
        if (!summ.freq)
            continue;

        record.minCurve[i] = Permille{ summ.low.p50 };
        record.maxCurve[i] = Permille{ summ.high.p50 };
    }

    makeMonotonic(record.minCurve);
    makeMonotonic(record.maxCurve);
}

void ModuleRandoItems::dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData) const
{
    std::string oldData;
    Mernel::readFileIntoBufferNoexcept(dumpFile, oldData);

    std::ostringstream dump;
    bool               isEmpty = true;

    dump << "#pragma once\n"
         << "// this is generated, do not edit\n\n"
         << "#include \"AttributeValues.hpp\"\n\n"
         << "namespace D2ModGen {\n\n"
         << "static inline const NonParRecordList g_generatedNonParRecordsGrouped1{\n";
    ;
    for (const auto& [key, prop] : attributeData.groupMap) {
        isEmpty = false;
        dump << prop.toString();
    }
    if (isEmpty) {
        dump << "    {},\n";
    }
    dump << "\n};\n\n";

    isEmpty = true;
    dump << "static inline const NonParRecordList g_generatedNonParRecords1{\n";

    for (const auto& [key, prop] : attributeData.idMap) {
        isEmpty = false;
        dump << prop.toString();
    }
    if (isEmpty) {
        dump << "    {},\n";
    }
    dump << "\n};\n";

    isEmpty = true;
    dump << "static inline const ParRecordList g_generatedParRecords1{\n";

    for (const auto& [key, prop] : attributeData.parMap) {
        isEmpty = false;
        dump << prop.toString();
    }
    if (isEmpty) {
        dump << "    {},\n";
    }
    dump << "\n};\n";

    dump << "\n}\n";

    if (oldData != dump.str())
        Mernel::writeFileFromBufferNoexcept(dumpFile, dump.str());
}

void ModuleRandoItems::MagicContext::loadPrecompiled()
{
    precompiledSize        = 0;
    const auto precompiled = getPrecompiledData();
    for (const auto& [groupId, record] : precompiled.groupMap) {
        precompiledSize += record.ids.size();
        if (auto it = actual.groupMap.find(groupId); it != actual.groupMap.cend()) {
            NonParRecord& actualRec = it->second;
            auto          freq      = actualRec.freq;
            auto          ids       = actualRec.ids;
            auto          limit     = actualRec.limit;

            actualRec = record;

            actualRec.freq  = freq;
            actualRec.ids   = ids;
            actualRec.limit = limit;
        }
    }
}

void ModuleRandoItems::MagicContext::addExtra(const Config& config, const int classCount)
{
    using A = AttributeId;

    const auto& module = config.m_itemRandomizer;

    ParRecord& skilltabs = actual.parMap.at(A::skilltab);

    int maxFreq = 0;
    for (auto& param : skilltabs.params)
        maxFreq = std::max(maxFreq, param.freq);

    skilltabs.slotScale = { { SlotType::Charm, 500_perm }, { SlotType::Weapon, 1500_perm }, { SlotType::Amulet, 1500_perm } };
    skilltabs.params.clear();
    for (int i = 0, maxSkillTabs = classCount * 3; i < maxSkillTabs; i++) {
        skilltabs.params.push_back(ParRecordParam{
            .freq      = maxFreq,
            .par       = i,
            .minLevel  = 15,
            .minMedian = 1,
            .maxMedian = 1,
            .minCurve  = { 1000_perm, 2000_perm, 3000_perm, 3000_perm, 4000_perm },
            .maxCurve  = { 1000_perm, 3000_perm, 4000_perm, 5000_perm, 5000_perm },
        });
    }

    if (auto* rec = actual.findExistingNonPar(A::skill_rand)) {
        rec->freq *= 20;
    }
    if (auto* rec = actual.findExistingNonPar(A::explosivearrow)) {
        rec->freq *= 5;
    }
    if (auto* rec = actual.findExistingNonPar(A::pierce)) {
        rec->freq *= 5;
    }
    if (auto* rec = actual.findExistingNonPar(A::rep_quant)) {
        rec->freq *= 10;
    }

    //---
    if (auto* rec = actual.findExistingPar(A::charged)) {
        rec->freqMult.raw /= 3;
    }
    if (auto* rec = actual.findExistingPar(A::skill)) {
        rec->freqMult.raw *= 3;
    }
    if (auto* rec = actual.findExistingPar(A::aura)) {
        rec->freqMult.raw *= 2;
    }

    scaleSettings(module.m_frequencyResistances, { A::res_fire, A::res_ltng, A::res_cold, A::res_mag, A::res_pois, A::res_all });
    scaleSettings(module.m_frequencyAuras, { A::aura });
    scaleSettings(module.m_frequencyProcs, {
                                               A::att_skill,
                                               A::hit_skill,
                                               A::gethit_skill,
                                               A::kill_skill,
                                               A::death_skill,
                                               A::levelup_skill,
                                               A::block_skill,
                                               A::cast_skill,
                                           });
    scaleSettings(module.m_frequencySkills, { A::skill, A::skilltab, A::allskills, A::ama, A::pal, A::nec, A::sor, A::bar, A::dru, A::ass, A::war });
    scaleSettings(module.m_frequencyCharAttributes, {
                                                        A::str,
                                                        A::dex,
                                                        A::vit,
                                                        A::enr,
                                                        A::mana,
                                                        A::mana_percent,
                                                        A::hp,
                                                        A::hp_percent,
                                                        A::hp_perlvl,
                                                        A::mana_perlvl,
                                                        A::str_perlvl,
                                                        A::dex_perlvl,
                                                        A::enr_perlvl,
                                                        A::vit_perlvl,
                                                        A::att_perlvl,

                                                    });
}

void ModuleRandoItems::MagicContext::scaleSettings(Percent scalePercent, const std::vector<AttributeId>& attrs)
{
    /*
    for (auto id : attrs) {
        if (auto* rec = actual.findExistingNonPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        } else if (auto* rec = actual.findExistingPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        }
    }*/
}

}
