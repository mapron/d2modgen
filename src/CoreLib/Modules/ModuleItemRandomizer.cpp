/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "MernelPlatform/StringUtils.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"
#include "TblParser.hpp"

#include "Logger.hpp"
#include "MernelPlatform/FileIOUtils.hpp"
#include "MernelPlatform/ChronoPoint.hpp"

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

ModuleItemRandomizer::ModuleItemRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "affixRandom", Mernel::PropertyTreeScalar{ true } },
          { "levelPlus", Mernel::PropertyTreeScalar{ 10 } },
          { "levelMinus", Mernel::PropertyTreeScalar{ 20 } },
          { "gemsRandom", Mernel::PropertyTreeScalar{ false } },

          { "casterTypePercent", Mernel::PropertyTreeScalar{ 25 } },
          { "itemFitPercent", Mernel::PropertyTreeScalar{ 100 } },
          { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 50 } },
          { "relativeCountMax", Mernel::PropertyTreeScalar{ 200 } },
          { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 200 } },
          { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },

          { "frequencyResistances", Mernel::PropertyTreeScalar{ 100 } },
          { "frequencyAuras", Mernel::PropertyTreeScalar{ 100 } },
          { "frequencyProcs", Mernel::PropertyTreeScalar{ 100 } },
          { "frequencySkills", Mernel::PropertyTreeScalar{ 100 } },
          { "frequencyCharAttributes", Mernel::PropertyTreeScalar{ 100 } },

          { "removeKnock", Mernel::PropertyTreeScalar{ true } },
          { "updateExisting", Mernel::PropertyTreeScalar{ true } },
          { "repeat_sets", Mernel::PropertyTreeScalar{ 5 } },
          { "repeat_uniques", Mernel::PropertyTreeScalar{ 5 } },
          { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
          { "replaceSkills", Mernel::PropertyTreeScalar{ false } },
          { "setBonusMode", Mernel::PropertyTreeScalar{ 1 } },
      })
{
    setPresets({
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 100 } },
            { "levelMinus", Mernel::PropertyTreeScalar{ 0 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 50 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "casterTypePercent", Mernel::PropertyTreeScalar{ 50 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "gemsRandom", Mernel::PropertyTreeScalar{ false } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 30 } },
            { "levelMinus", Mernel::PropertyTreeScalar{ 15 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 20 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 20 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "casterTypePercent", Mernel::PropertyTreeScalar{ 50 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "gemsRandom", Mernel::PropertyTreeScalar{ true } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 20 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 80 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "casterTypePercent", Mernel::PropertyTreeScalar{ 10 } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ false } },
            { "affixRandom", Mernel::PropertyTreeScalar{ false } },
            { "gemsRandom", Mernel::PropertyTreeScalar{ false } },
        } },
    });
}

void ModuleItemRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    m_codeResolver = [&output](const std::string& skillId) -> int {
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
    m_skillGetter = [&output](int index) -> DataRow_skills_ptr {
        if (index < 0)
            return nullptr;
        return &output.skills.rows[index];
    };
    m_extraLoc = {};

    std::set<std::string> skippedAttributes;
    std::set<std::string> unknownAttributes;

    const bool extraTrace  = !!getenv("MODGEN_RANDOMIZER_TRACE");
    const bool extraTrace2 = !!getenv("MODGEN_RANDOMIZER_TRACE_PROPS");

    Logger() << "Grab properties from game data...";
    const AllProps gameProps = grabAllProps(output, false, &unknownAttributes, &skippedAttributes);
    {
        std::ostringstream skipped, unknown;
        for (auto& str : skippedAttributes)
            skipped << str << ", ";
        for (auto& str : unknownAttributes)
            unknown << str << ", ";
        if (!unknownAttributes.empty())
            Logger(Logger::Warning) << "Unknown attributes: " << unknown.str();
        if (!skippedAttributes.empty())
            Logger() << "Ignored attributes: " << skipped.str();
    }

    Logger() << "Analyze game properties, build statistic profile...";
    AllAttributeData gameAttributes = analyzeAll(gameProps);

    Logger() << "Init pre-compiled attributes...";
    MagicContext magicContext(std::move(gameAttributes));
    Logger() << "precompiled=" << magicContext.precompiled.ids.size()
             << ", gameData=" << magicContext.gameData.ids.size()
             << ", actual=" << magicContext.actual.ids.size();

    Logger() << "Add some extra fine-tuned properties...";

    magicContext.addExtra(input);

    StatisticsTable statisticsTable;

    const int repeatUniquesMax = 5;
    const int repeatSetsMax    = 5;
    const int repeatUniques    = std::clamp(input.getInt("repeat_uniques"), 1, repeatUniquesMax);
    const int repeatSets       = std::clamp(input.getInt("repeat_sets"), 1, repeatSetsMax);

    {
        auto*      dumpFileName = getenv("MODGEN_ATTRIBUTE_DUMP_FILE");
        const bool dumpAll      = !!getenv("MODGEN_ATTRIBUTE_DUMP_ALL");
        if (dumpFileName) {
            const Mernel::std_path dumpFile = Mernel::string2path(dumpFileName);
            dumpAttributes(dumpFile, magicContext.actual, dumpAll ? std::set<AttributeId>{} : magicContext.precompiled.ids);
        }
    }

    createNewItems(output, rng, input, repeatUniques, repeatSets);

    if (extraTrace) {
        const AllAttributeData attrib = analyzeAll(grabAllProps(output));
        statisticsTable.addSnapshot(attrib, 0);
    }

    PropRandomizer props(magicContext.actual, m_skillGetter);

    RandoParams params;
    params.replaceSkills  = input.getInt("replaceSkills");
    params.replaceCharges = input.getInt("replaceCharges");
    params.removeKnock    = input.getInt("removeKnock");
    params.updateExisting = input.getInt("updateExisting");

    params.levelPlus           = input.getInt("levelPlus");
    params.levelMinus          = input.getInt("levelMinus");
    params.typeFitPercent      = input.getInt("itemFitPercent");
    params.casterTypePercent   = input.getInt("casterTypePercent");
    params.keepOriginalPercent = input.getInt("keepOriginalPercent");
    params.relativeCountMin    = input.getInt("relativeCountMin");
    params.relativeCountMax    = input.getInt("relativeCountMax");

    auto paramsSetitems              = params;
    paramsSetitems.relativeCountMin  = input.getInt("setRelativeCountMin");
    paramsSetitems.relativeCountMax  = input.getInt("setRelativeCountMax");
    paramsSetitems.uniqueCheckAcross = true;

    auto paramsSets               = params;
    paramsSets.supportRandomRange = false;
    paramsSets.uniqueCheckAcross  = true;

    auto paramsGems               = params;
    paramsGems.supportRandomRange = false;
    paramsGems.enableGeneration   = input.getInt("gemsRandom");

    auto paramsAffix             = params;
    paramsAffix.enableGeneration = input.getInt("affixRandom");

    auto paramsRunes = params;
    auto paramsUniq  = params;

    auto fillProps = [&props, &rng, this, extraTrace, extraTrace2]<typename Row>(DataTable<Row>& table, const RandoParams& params, RollType roll) {
        if (!params.enableGeneration)
            return;
        int rowsAffected   = 0;
        int totalKept      = 0;
        int totalGenerated = 0;

        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;

            MagicPropItem item;
            item.rng            = &rng;
            item.propRandomizer = &props;
            item.randoParams    = &params;
            item.readParams(row);
            item.readAffixes(row, m_codeResolver, m_skillGetter, nullptr, nullptr);

            if (extraTrace)
                Logger() << "read: " << item.toString(extraTrace2);

            if (item.filter.props.slot == SlotType::None || item.filter.props.slot == SlotType::PD2Map)
                continue;

            item.generate();
            item.writeTo(row);

            rowsAffected++;
            totalKept += item.stats.kept;
            totalGenerated += item.stats.genReq;
            if (extraTrace)
                Logger() << "write: " << item.toString(extraTrace2);
        }
        Logger(Logger::Notice) << "table '" << getTableIdString(Row::s_tableId) << "' rando-affected rows: " << rowsAffected << ", total kept affixes: " << totalKept << ", total generated: " << totalGenerated;
    };
    auto startFillProps = Mernel::ChronoPoint(true);

    fillProps(output.uniqueitems, paramsUniq, RollType::Rare);
    fillProps(output.setitems, paramsSetitems, RollType::Rare);
    fillProps(output.sets, paramsSets, RollType::Rare);
    fillProps(output.runes, paramsRunes, RollType::Rare);

    fillProps(output.gems, paramsGems, RollType::Common);
    fillProps(output.magicprefix, paramsAffix, RollType::Common);
    fillProps(output.magicsuffix, paramsAffix, RollType::Common);

    auto endFillProps = startFillProps.GetElapsedTime().ToProfilingTime();

    if (m_extraLoc.size() > 0) {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const Loc& loc : m_extraLoc) {
            totalKeySize += loc.key.size() + 1;
            totalValSize += loc.value.size() + 1;
        }
        Logger(Logger::Notice) << "Adding new " << m_extraLoc.size() << " translations, totalKeySize=" << totalKeySize << ", totalValSize=" << totalValSize;
        if (output.jsonFiles.contains(JsonId::ItemNames)) {
            auto& jsonDoc = output.jsonFiles[JsonId::ItemNames];
            updateD2Rtranslation(jsonDoc, input.m_env.d2rLocaleId);
        }
        if (output.blobs.contains(BlobId::PatchStringTbl)) {
            auto& blob = output.blobs[BlobId::PatchStringTbl];
            updateLoDtranslation(blob);
        }
    }

    if (extraTrace) {
        const AllAttributeData attrib = analyzeAll(grabAllProps(output));
        statisticsTable.addSnapshot(attrib, 1);
        statisticsTable.traceStatistics();
    }

    Logger(Logger::Notice) << "  item randomizer fill done in: " << endFillProps;
}

ModuleItemRandomizer::AllProps ModuleItemRandomizer::grabAllProps(const DataContext&     output,
                                                                  bool                   onlySetUniques,
                                                                  std::set<std::string>* unknownList,
                                                                  std::set<std::string>* skippedList) const
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

                auto&          byRoll  = byId.propsByRoll[affixlist.rollTypeHighestGrab];
                auto&          bySlot  = byRoll.propsBySlot[slot];
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

    for (auto& row : output.cubemain.rows) {
        if (!row.isCrafted())
            continue;
        auto      desc         = Mernel::strToLower(row.description);
        const int craftedLevel = 25;

        MagicPropItem item;
        item.id                          = row.description;
        item.originalLevel               = craftedLevel;
        item.filter.props.slot           = row.slotType;
        item.filter.props.levelBracket20 = 1;
        item.readAffixes(row, m_codeResolver, m_skillGetter, unknownList, skippedList);

        for (auto& affixlist : item.affixlists) {
            for (auto& prop : affixlist.parsedProps) {
                auto& byId = allProps.propsById[prop.id];

                auto&          byRoll  = byId.propsByRoll[affixlist.rollTypeHighestGrab];
                auto&          bySlot  = byRoll.propsBySlot[row.slotType];
                MagicPropList& byLevel = bySlot.propsByLevel[craftedLevel];
                byLevel.push_back(prop);
            }
        }
    }
    return allProps;
}

void ModuleItemRandomizer::createNewItems(DataContext& output, RandomGenerator& rng, const InputContext& input, int repeatUniques, int repeatSets) const
{
    const int setBonusMode = input.getInt("setBonusMode");

    if (repeatUniques == 1 && repeatSets == 1)
        return;

    Logger() << "Create additional records for setitems/uniqueitems...";

    const std::vector<std::string> colors{ "whit", "lgry", "dgry", "blac", "lblu", "dblu", "cblu", "lred", "dred", "cred", "lgrn", "dgrn", "cgrn", "lyel", "dyel", "lgld", "dgld", "lpur", "dpur", "oran", "bwht" };

    auto getRandomColor = [&rng, &colors]() -> std::string {
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
        auto getNewLevelAndCode = [&rng, &output, validArmor, validWeapon](std::optional<int>& lvl,
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
            std::vector<Row> copyRows;
            copyRows.reserve(table.rows.size() + validRows.size() * (copies - 1));
            copyRows.insert(copyRows.end(), table.rows.cbegin(), table.rows.cend());

            for (Row* row_ : validRows) {
                for (int i = 1; i < copies; ++i) {
                    Row row = *row_;
                    row._ser_unusedCells.clear();

                    afterCopy(int(copyRows.size()), i, row, *row_);
                    copyRows.push_back(std::move(row));
                }
            }
            std::swap(copyRows, table.rows);
        };

        auto beforeCopyUniq = [](DataRow_uniqueitems& row) -> bool {
            row.disableChronicle = true;
            if (row.eqipmentType->category != EquipmentCategory::Misc)
                row.lvlreq = 2;
            else {
                auto slot = row.getItemFilter().determineSlot();
                if (slot == SlotType::Amulet || slot == SlotType::Charm || slot == SlotType::Ring || slot == SlotType::Weapon)
                    return true;
                Logger() << "uniq copy skip: " << row.getId();
                return false;
            }
            return true;
        };
        auto afterCopyUniq = [&input, this, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_uniqueitems& row, const DataRow_uniqueitems& old) {
            row.index       = makeKey(id, 'u');
            row.displayName = makeValue(old.index, i);
            m_extraLoc.push_back({ row.index, row.displayName });

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
        auto afterCopySetitem = [&input, this, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_setitems& row, const DataRow_setitems& old) {
            row.index       = makeKey(id, 's');
            row.displayName = makeValue(old.index, i);
            m_extraLoc.push_back({ row.index, row.displayName });

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
            m_extraLoc.push_back({ row.index, row.index });

            row.name = row.index;
        };

        processTableWithCopies(output.sets, repeatSets, beforeCopySet, afterCopySet);
    }
}

AllAttributeData ModuleItemRandomizer::analyzeAll(const AllProps& allProps) const
{
    AllAttributeData  result;
    NonParRecordList& nonPropList = result.nonPar;
    ParRecordList&    propList    = result.par;

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
            NonParRecord nonParRecord;
            nonParRecord.code = code;
            analyzeNonPar(codeProps, nonParRecord, result.statistics[code]);
            nonPropList.push_back(std::move(nonParRecord));
        } else {
            ParRecord parRecord;
            parRecord.code = code;
            analyzePar(codeProps, parRecord, result.statistics[code]);
            propList.push_back(std::move(parRecord));
        }
    }
    result.refreshIds();
    return result;
}

void ModuleItemRandomizer::analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecord& record, NonParValueSummary& stat) const
{
    NonParValueList     allBadds;
    NonParValueByLevel* prev1 = nullptr;
    NonParValueList     all;

    for (const auto& [roll, rollProps] : allPropsVal.propsByRoll) {
        assert(roll != RollType::Negative);
        NonParValueByLevel& nonParByLevel = record.get(roll);
        NonParValueByLevel* prev          = prev1;
        prev1                             = &nonParByLevel;

        std::map<SlotType, NonParValueByLevel> approxMapForSlot;
        NonParValueList                        valuesGood;
        for (const auto& [slot, slotProps] : rollProps.propsBySlot) {
            record.slotTypes.insert(slot);

            NonParValueList valuesSlot;
            for (const auto& [level, levelProps] : slotProps.propsByLevel) {
                assert(level >= -1 && level < 260);
                for (auto prop : levelProps) {
                    const auto min = prop.min.value_or(0);
                    const auto max = prop.max.value_or(0);
                    const auto par = prop.par.value_or(0);

                    if (min < 0 || max < 0) {
                        allBadds.push_back({ level, min, max, par });
                    } else {
                        valuesSlot.push_back({ level, min, max, par });
                        valuesGood.push_back({ level, min, max, par });
                    }

                    all.push_back({ level, min, max, par });
                }
            }

            approxMapForSlot[slot].makeApprox(valuesSlot);
        }
        if (valuesGood.size()) {
            nonParByLevel.makeApprox(valuesGood, prev);
            nonParByLevel.makeMonotonic();

            for (const auto& [slot, approxSlot] : approxMapForSlot) {
                record.addScale(slot, approxSlot.total, nonParByLevel.total);
            };
        }
    }
    record.badd.makeApprox(allBadds);
    stat.make(all);

    record.cleanupScale();
}

void ModuleItemRandomizer::analyzePar(const AllPropsVal& allPropsVal, ParRecord& record, NonParValueSummary& stat) const
{
    NonParValueList all;
    for (const auto& [roll, rollProps] : allPropsVal.propsByRoll) {
        assert(roll != RollType::Negative);

        for (const auto& [slot, slotProps] : rollProps.propsBySlot) {
            record.slotTypes.insert(slot);

            PropListByLevel& propValues = record.get(roll);
            for (const auto& [level, levelProps] : slotProps.propsByLevel) {
                const size_t bracket20 = std::clamp(level, 0, 99) / 20;

                for (auto prop : levelProps) {
                    all.push_back({ level, prop.min.value_or(0), prop.max.value_or(0), prop.par.value_or(0) });
                    propValues.freq++;
                    propValues.levels[bracket20].push_back({ prop.par, prop.min, prop.max });
                }
            }
        }
    }
    stat.make(all);
}

void ModuleItemRandomizer::dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData, const std::set<AttributeId>& skip) const
{
    std::string oldData;
    Mernel::readFileIntoBufferNoexcept(dumpFile, oldData);

    std::ostringstream dump;
    bool               isEmpty = true;

    dump << "#pragma once\n"
         << "// this is generated, do not edit\n\n"
         << "#include \"AttributeValues.hpp\"\n\n"
         << "namespace D2ModGen {\n\n"
         << "static inline const NonParRecord g_generatedNonParRecordList1[]{\n";
    ;
    for (const auto& prop : attributeData.nonPar) {
        if (skip.contains(prop.code))
            continue;
        isEmpty = false;
        dump << prop.toString();
    }
    if (isEmpty) {
        dump << "    {},\n";
    }
    dump << "\n};\n\n";

    isEmpty = true;
    dump << "static inline const ParRecord g_generatedParRecordList1[]{\n";

    for (const auto& prop : attributeData.par) {
        if (skip.contains(prop.code))
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

void ModuleItemRandomizer::updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const std::string& localeId) const
{
    {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const auto& itemDesc : jsonDoc.getList()) {
            const std::string key = itemDesc["Key"].getScalar().toString();
            const std::string val = itemDesc[localeId].getScalar().toString();
            totalKeySize += key.size() + 1;
            totalValSize += val.size() + 1;
        }
        Logger(Logger::Notice) << "Existing: totalKeySize=" << totalKeySize << " totalValSize=" << totalValSize;
    }

    int id = 1000000;
    for (const Loc& loc : m_extraLoc) {
        Mernel::PropertyTree itemDesc;
        itemDesc["Key"]    = Mernel::PropertyTreeScalar(loc.key);
        itemDesc["id"]     = Mernel::PropertyTreeScalar(id++);
        itemDesc[localeId] = Mernel::PropertyTreeScalar(loc.value);

        jsonDoc.append(itemDesc);
    }
}

void ModuleItemRandomizer::updateLoDtranslation(std::string& patchstringTlb) const
{
    TblStructure tbl;
    tbl.readFromBuffer(patchstringTlb);

    {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const auto& itemDesc : tbl.m_rows) {
            totalKeySize += itemDesc.Key.size() + 1;
            totalValSize += itemDesc.Val.size() + 1;
        }
        Logger(Logger::Notice) << "Existing: totalKeySize=" << totalKeySize << " totalValSize=" << totalValSize;
    }

    for (const Loc& loc : m_extraLoc) {
        tbl.m_rows.push_back({ .Key = loc.key, .Val = loc.value });
    }
    tbl.writeToBuffer(patchstringTlb);
}

void ModuleItemRandomizer::MagicContext::addExtra(const InputContext& input)
{
    using A = AttributeId;
    actual.erase(A::skilltab);

    Character lastCharacter = input.m_env.isRotW ? Character::War : Character::Ass;
    const int classCount    = static_cast<int>(lastCharacter) + 1;

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

    homogenizeNonPar(200, { A::ama, A::pal, A::nec, A::sor, A::bar, A::dru, A::ass, A::war });
    homogenizeNonPar(50, { A::res_fire, A::res_ltng, A::res_cold, A::res_mag, A::res_pois });
    homogenizeNonPar(300, { A::res_fire_max, A::res_ltng_max, A::res_cold_max, A::res_pois_max });
    homogenizeNonPar(300, { A::abs_fire_percent, A::abs_ltng_percent, A::abs_mag_percent, A::abs_cold_percent });
    homogenizeNonPar(300, { A::abs_fire, A::abs_ltng, A::abs_mag, A::abs_cold });
    homogenizeNonPar(200, { A::fireskill, A::poisskill, A::coldskill, A::magskill, A::ltngskill });
    homogenizeNonPar(200, { A::pierce_fire, A::pierce_ltng, A::pierce_cold, A::pierce_pois, A::pierce_mag });
    homogenizeNonPar(200, { A::extra_fire, A::extra_ltng, A::extra_cold, A::extra_pois, A::extra_mag });

    homogenizeNonPar(300, { A::dmg_cold_perlvl, A::dmg_fire_perlvl, A::dmg_ltng_perlvl, A::dmg_pois_perlvl });
    homogenizeNonPar(300, { A::res_cold_perlvl, A::res_fire_perlvl, A::res_ltng_perlvl, A::res_pois_perlvl });
    homogenizeNonPar(300, { A::abs_cold_perlvl, A::abs_fire_perlvl, A::abs_ltng_perlvl, A::abs_pois_perlvl });

    if (auto* rec = actual.findExistingPar(A::res_mag); rec) {
        rec->freqFactor /= 3;
    }

    scaleSettings(input, "frequencyResistances", { A::res_fire, A::res_ltng, A::res_cold, A::res_mag, A::res_pois, A::res_all });
    scaleSettings(input, "frequencyAuras", { A::aura });
    scaleSettings(input, "frequencyProcs", {
                                               A::att_skill,
                                               A::hit_skill,
                                               A::gethit_skill,
                                               A::kill_skill,
                                               A::death_skill,
                                               A::levelup_skill,
                                               A::block_skill,
                                               A::cast_skill,
                                           });
    scaleSettings(input, "frequencySkills", { A::skill, A::skilltab, A::allskills, A::ama, A::pal, A::nec, A::sor, A::bar, A::dru, A::ass, A::war });
    scaleSettings(input, "frequencyCharAttributes", {
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

void ModuleItemRandomizer::MagicContext::homogenizeNonPar(int scalePercent, const std::vector<AttributeId>& attrs)
{
    std::vector<const NonParRecord*> sources;
    std::vector<NonParRecord*>       edit;
    for (auto id : attrs) {
        if (auto* rec = actual.findExistingNonPar(id)) {
            sources.push_back(rec);

            edit.push_back(rec);
        }
    }
    if (sources.empty())
        return;

    NonParRecord homogen;
    homogen.doHomogen(sources);
    homogen.rescaleFreq(scalePercent, sources.size());

    std::map<NonParRecord*, NonParRecord> old2new;
    for (NonParRecord* rec : edit) {
        rec->comm           = homogen.comm;
        rec->rare           = homogen.rare;
        rec->epic           = homogen.epic;
        rec->slotTypes      = homogen.slotTypes;
        rec->slotTypesScale = homogen.slotTypesScale;
    }
}

void ModuleItemRandomizer::MagicContext::scaleSettings(const InputContext& input, const std::string& key, const std::vector<AttributeId>& attrs)
{
    int scalePercent = input.getInt(key);
    for (auto id : attrs) {
        if (auto* rec = actual.findExistingNonPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        } else if (auto* rec = actual.findExistingPar(id)) {
            rec->rescaleFreq(scalePercent, 1);
        }
    }
}

ModuleItemRandomizer::MagicContext::MagicContext(AllAttributeData gameData_)
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
}

}
