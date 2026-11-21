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
    AllAttributeData gameAttributes = analyzeAll(gameProps);

    Logger(m_verbose) << "Init pre-compiled attributes...";
    MagicContext magicContext(std::move(gameAttributes));
    Logger(m_verbose) << "precompiled=" << magicContext.precompiled.ids.size()
                      << ", gameData=" << magicContext.gameData.ids.size()
                      << ", actual=" << magicContext.actual.ids.size();

    Logger(m_verbose) << "Add some extra fine-tuned properties...";

    magicContext.addExtra(config, output);

    StatisticsTable statisticsTable;

    const int64_t repeatUniquesMax = 5;
    const int64_t repeatSetsMax    = 5;
    const int64_t repeatUniques    = std::clamp(module.m_repeatUniques, int64_t(1), repeatUniquesMax);
    const int64_t repeatSets       = std::clamp(module.m_repeatSets, int64_t(1), repeatSetsMax);

    {
        if (!env.extraFile.empty()) {
            const bool dumpAll = env.extraTrace.contains("item_rando_all_attr");
            dumpAttributes(env.extraFile, magicContext.actual, dumpAll ? std::set<AttributeId>{} : magicContext.precompiled.ids);
            return;
        }
    }

    createNewItems(repeatUniques, repeatSets);

    if (m_verbose) {
        const AllAttributeData attrib = analyzeAll(grabAllProps());
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
        const AllAttributeData attrib = analyzeAll(grabAllProps());
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

AllAttributeData ModuleRandoItems::analyzeAll(const AllProps& allProps)
{
    AllAttributeData                   result;
    NonParRecordList&                  nonPropList = result.nonPar;
    ParRecordList&                     propList    = result.par;
    NonParRecordNewGroupMap&           groupMap    = result.groupMap;
    NonParRecordNewMap&                idMap       = result.idMap;
    AllProps                           trivialNoGroup;
    std::map<AttributeGroup, AllProps> trivial;

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
            ParRecord parRecord;
            parRecord.code = code;
            analyzePar(codeProps, parRecord, result.statistics[code]);
            propList.push_back(std::move(parRecord));
        }
    }

    for (const auto& [groupId, trivialProps] : trivial) {
        NonParRecordNew& record = groupMap[groupId];
        record.group            = groupId;
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
    }
    for (const auto& [code, codeProps] : trivialNoGroup.propsById) {
        NonParRecordNew& record = idMap[code];
        record.ids.insert(code);

        analyzeNonPar(codeProps, record);
    }

    result.refreshIds();
    return result;
}

void ModuleRandoItems::analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecordNew& record)
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
    record.medCurve = NonParRecordNew::s_emptyCurve;

    record.ratioP000 = NonParRecordNew::s_emptyCurve;
    record.ratioP010 = NonParRecordNew::s_emptyCurve;
    record.ratioP090 = NonParRecordNew::s_emptyCurve;
    record.ratioP100 = NonParRecordNew::s_emptyCurve;
    std::vector<NonParValueList> levelGroups(LEVEL_GROUP_COUNT);
    for (const NonParValue& value : valuesGoodScaledToMedian) {
        int levelGroup = makeTier(value.level);
        levelGroups[levelGroup].push_back(value);
        record.minLevel = std::min(record.minLevel, value.level);
    }
    int nonEmpty = 0;
    for (int i = 0; i < LEVEL_GROUP_COUNT; i++) {
        NonParValueSummary summ;
        summ.make(levelGroups[i]);
        if (!summ.freq)
            continue;

        record.medCurve[i] = Permille{ summ.high.p50 };

        record.ratioP000[i] = Permille{ summ.high.min * 1000 / summ.high.p50 };
        record.ratioP010[i] = Permille{ summ.high.p10 * 1000 / summ.high.p50 };
        record.ratioP090[i] = Permille{ summ.high.p90 * 1000 / summ.high.p50 };
        record.ratioP100[i] = Permille{ summ.high.max * 1000 / summ.high.p50 };
    }

    auto makeMonotonic = [](PermilleCurve& curve) {
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

void ModuleRandoItems::analyzePar(const AllPropsVal& allPropsVal, ParRecord& record, NonParValueSummary& stat)
{
    NonParValueList all;
    //for (const auto& [roll, rollProps] : allPropsVal.propsByRoll) {
    //  assert(roll != RollType::Negative);

    for (const auto& [slot, slotProps] : allPropsVal.propsBySlot) {
        record.slotTypes.insert(slot);

        PropListByLevel& propValues = record.comm;
        for (const auto& [level, levelProps] : slotProps.propsByLevel) {
            const int levelTier = makeTier(level);

            for (auto prop : levelProps) {
                all.push_back({ level, prop.min.value_or(0), prop.max.value_or(0), prop.par.value_or(0) });
                propValues.freq++;
                propValues.levels[levelTier].push_back({ prop.par, prop.min, prop.max });
            }
        }
    }
    // }
    stat.make(all);
}

void ModuleRandoItems::dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData, const std::set<AttributeId>& skip)
{
    std::string oldData;
    Mernel::readFileIntoBufferNoexcept(dumpFile, oldData);

    std::ostringstream dump;
    bool               isEmpty = true;

    dump << "#pragma once\n"
         << "// this is generated, do not edit\n\n"
         << "#include \"AttributeValues.hpp\"\n\n"
         << "namespace D2ModGen {\n\n"
         << "static inline const NonParRecordNew g_generatedNonParRecordGrouped1[]{\n";
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
    dump << "static inline const NonParRecordNew g_generatedNonParRecordIds1[]{\n";

    for (const auto& [key, prop] : attributeData.idMap) {
        isEmpty = false;
        dump << prop.toString();
    }
    if (isEmpty) {
        dump << "    {},\n";
    }
    dump << "\n};\n";

    isEmpty = true;
    dump << "static inline const ParRecord g_generatedParRecordList1[]{\n";

    for (const auto& prop : attributeData.par) {
        if (skip.contains(prop.code) || 1)
            continue;
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

void ModuleRandoItems::MagicContext::addExtra(const Config& config, const DataContext& output)
{
    using A = AttributeId;
    //actual.erase(A::skilltab);

    const auto& module = config.m_itemRandomizer;

    const int classCount = output.getClassesCount();
#if 0
    // Fill skilltab values
    auto fillPropList = [classCount](PropWithRangeList& props, int min, int max) -> int {
        if (!min || !max)
            return 0;
        const int maxSkillTabs = classCount * 3;
        for (int skilltabId = 0; skilltabId < maxSkillTabs; ++skilltabId) {
            props.push_back(PropWithRange{ .par = skilltabId, .min = min, .max = max });
        }
        return maxSkillTabs;
    };

    auto fillPropList2 = [&fillPropList, this]<size_t size = NonParValueByLevel::s_segments>(RollType roll, const std::array<int, size> valuesMin, const std::array<int, size> valuesMax, SlotType type) {
        ParRecord skilltabs{
            .code       = A::skilltab,
            .freqFactor = FREQ_PER_ONE_PROP * DEFAULT_FREQ * 3 / 4,
        };

        skilltabs.slotTypes.insert(type);
        auto& pool = skilltabs.get(roll);
        pool.freq++;
        for (size_t i = 0; i < size; ++i) {
            pool.freq += fillPropList(pool.levels[i], valuesMin[i], valuesMax[i]);
        }
        actual.add(std::move(skilltabs));
    };
    for (SlotType slot : { SlotType::MainArmor, SlotType::OtherArmor, SlotType::Amulet, SlotType::Weapon, SlotType::Charm })
        fillPropList2(RollType::Common, std::array{ 1, 1, 1, 1, 1 }, std::array{ 1, 1, 1, 1, 1 }, slot);

    fillPropList2(RollType::Rare, std::array{ 0, 0, 0, 2, 3 }, std::array{ 0, 0, 0, 3, 4 }, SlotType::MainArmor);
    fillPropList2(RollType::Rare, std::array{ 0, 0, 1, 2, 3 }, std::array{ 0, 0, 2, 3, 3 }, SlotType::OtherArmor);
    fillPropList2(RollType::Rare, std::array{ 0, 0, 2, 3, 3 }, std::array{ 0, 0, 2, 3, 3 }, SlotType::Amulet);
    fillPropList2(RollType::Rare, std::array{ 0, 0, 0, 1, 2 }, std::array{ 0, 0, 0, 2, 3 }, SlotType::Ring);
    fillPropList2(RollType::Rare, std::array{ 0, 1, 2, 3, 3 }, std::array{ 0, 2, 3, 3, 4 }, SlotType::Weapon);
    fillPropList2(RollType::Rare, std::array{ 0, 0, 0, 0, 1 }, std::array{ 0, 0, 0, 0, 2 }, SlotType::Charm);

    fillPropList2(RollType::Epic, std::array{ 1, 2, 3, 4, 5 }, std::array{ 2, 3, 5, 6, 7 }, SlotType::MainArmor);
    fillPropList2(RollType::Epic, std::array{ 1, 2, 3, 4, 4 }, std::array{ 2, 3, 5, 6, 5 }, SlotType::Amulet);
    fillPropList2(RollType::Epic, std::array{ 1, 2, 3, 4, 5 }, std::array{ 2, 3, 5, 6, 7 }, SlotType::Weapon);
#endif
    if (auto* rec = actual.findExistingNonPar(A::skill_rand); rec) {
        rec->freqFactor *= 20;
    }
    if (auto* rec = actual.findExistingNonPar(A::explosivearrow); rec) {
        rec->freqFactor *= 5;
    }
    if (auto* rec = actual.findExistingNonPar(A::pierce); rec) {
        rec->freqFactor *= 5;
    }
    if (auto* rec = actual.findExistingNonPar(A::rep_quant); rec) {
        rec->freqFactor *= 10;
    }

    //---
    if (auto* rec = actual.findExistingPar(A::charged); rec) {
        rec->freqFactor /= 3;
    }
    if (auto* rec = actual.findExistingPar(A::skill); rec) {
        rec->freqFactor *= 3;
    }
    if (auto* rec = actual.findExistingPar(A::aura); rec) {
        rec->freqFactor *= 5;
    }

    if (auto* rec = actual.findExistingPar(A::res_mag); rec) {
        rec->freqFactor /= 3;
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
    for (auto id : attrs) {
        if (auto* rec = actual.findExistingNonPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        } else if (auto* rec = actual.findExistingPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        }
    }
}

ModuleRandoItems::MagicContext::MagicContext(AllAttributeData gameData_)
    : precompiled(getPrecompiledData())
    , gameData(std::move(gameData_))
{
    actual         = precompiled;
    auto notInGame = [this](const auto& rec) {
        return !gameData.ids.contains(rec.code);
    };
    std::copy_if(precompiled.par.cbegin(), precompiled.par.cend(), std::back_inserter(precompiledSkipped.par), notInGame);
    std::copy_if(precompiled.nonPar.cbegin(), precompiled.nonPar.cend(), std::back_inserter(precompiledSkipped.nonPar), notInGame);

    std::erase_if(actual.par, notInGame);
    std::erase_if(actual.nonPar, notInGame);

    for (auto& rec : gameData.par) {
        if (actual.ids.contains(rec.code)) {
            gameDataSkipped.add(rec);
            continue;
        }
        actual.add(rec);
    }
    for (auto& rec : gameData.nonPar) {
        if (actual.ids.contains(rec.code)) {
            gameDataSkipped.add(rec);
            continue;
        }
        actual.add(rec);
    }
    for (auto& [groupId, rec] : gameData.groupMap) {
        /*if (actual.ids.contains(rec.code)) {
            gameDataSkipped.add(rec);
            continue;
        }*/
        actual.addGrouped(rec);
    }
    for (auto& [groupId, rec] : gameData.idMap) {
        /*if (actual.ids.contains(rec.code)) {
            gameDataSkipped.add(rec);
            continue;
        }*/
        actual.addIdd(rec);
    }
}

}
