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
          { "levelPlus", Mernel::PropertyTreeScalar{ 20 } },
          { "levelMinus", Mernel::PropertyTreeScalar{ 10 } },
          { "extraKnown", Mernel::PropertyTreeScalar{} },
          { "gemsRandom", Mernel::PropertyTreeScalar{ false } },

          { "casterTypePercent", Mernel::PropertyTreeScalar{ 25 } },
          { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "removeKnock", Mernel::PropertyTreeScalar{ true } },
          { "updateExisting", Mernel::PropertyTreeScalar{ true } },
          { "repeat_sets", Mernel::PropertyTreeScalar{ 5 } },
          { "repeat_uniques", Mernel::PropertyTreeScalar{ 5 } },
          { "replaceCharges", Mernel::PropertyTreeScalar{ false } },
          { "replaceSkills", Mernel::PropertyTreeScalar{ false } },
          { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "setBonusMode", Mernel::PropertyTreeScalar{ 0 } },
      })
{
    setPresets({
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 100 } },
            { "levelMinus", Mernel::PropertyTreeScalar{ 0 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "gemsRandom", Mernel::PropertyTreeScalar{ true } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 30 } },
            { "levelMinus", Mernel::PropertyTreeScalar{ 15 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 50 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 20 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "levelPlus", Mernel::PropertyTreeScalar{ 10 } },
            { "levelMinus", Mernel::PropertyTreeScalar{ 10 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 50 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 120 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 150 } },
        } },
    });
}

void ModuleItemRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    MagicContext context;
    context.generatedNonPropAttributes = globalNonPropAttributes();
    context.generatedPropAttributes    = globalPropAttributes();
    context.generatedSlotScale         = globalSlotScale();

    ExtraLoc extraLoc;

    AllProps allProps;

    grabAllProps(allProps, output, context);
    createNewItems(extraLoc, output, rng, input);

    context.eraseNonGame();
    {
        // Fill skilltab values
        auto fillPropList = [&input](AffixPropLeveledList& props, int level, int min, int max) {
            for (int i = 0, cnt = input.m_env.isRotW ? 24 : 21; i < cnt; ++i) {
                props.push_back(AffixPropLeveled{ .level = level, .par = std::to_string(i), .min = min, .max = max });
            }
        };
        PropAttributeRecord skilltabs{
            .code        = AttributeId::skilltab,
            .freqPercent = 75,
        };
        auto fillPropList2 = [&fillPropList, &skilltabs]<size_t size>(SlotType type, const std::array<int, size> valuesMin, const std::array<int, size> valuesMax) {
            std::vector<int> levels{ 21, 41, 61, 81 };
            while (levels.size() > size)
                levels.erase(levels.begin());
            for (size_t i = 0; i < size; ++i) {
                fillPropList(skilltabs.slotValues[type], levels[i], valuesMin[i], valuesMax[i]);
            }
        };
        fillPropList2(SlotType::MainArmor, std::array{ 1, 1, 2, 3 }, std::array{ 1, 1, 3, 5 });
        fillPropList2(SlotType::OtherArmor, std::array{ 1, 1, 2, 3 }, std::array{ 1, 2, 3, 3 });
        fillPropList2(SlotType::Amulet, std::array{ 1, 2, 3, 3 }, std::array{ 1, 2, 3, 4 });
        fillPropList2(SlotType::Ring, std::array{ 1, 2 }, std::array{ 2, 3 });
        fillPropList2(SlotType::Weapon, std::array{ 2, 2, 3, 5 }, std::array{ 2, 3, 4, 7 });
        fillPropList2(SlotType::Charm, std::array{ 1, 1, 1 }, std::array{ 1, 1, 2 });

        context.generatedPropAttributes.push_back(std::move(skilltabs));
    }
    context.calculateConfigAttributes();

    {
        NonPropAttributeRecordList nonPropList;
        PropAttributeRecordList    propList;

        ApproximationResultsSimpleMap approxMapSimple;

        for (const auto& [code, codeProps] : allProps) {
            if (context.configAttributes.contains(code))
                continue;
            Logger() << "analyzing property values for attr='" << idToString(code) << "'";
            analyzeAllPropsForDump(code, codeProps, nonPropList, propList, approxMapSimple);
        }

        dumpAttributes(nonPropList, propList, approxMapSimple);

        context.generatedNonPropAttributes.insert(context.generatedNonPropAttributes.cend(), nonPropList.cbegin(), nonPropList.cend());
        context.generatedPropAttributes.insert(context.generatedPropAttributes.cend(), propList.cbegin(), propList.cend());
        context.calculateConfigAttributes();
    }

    MagicPropUniverse props(context);

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

    auto paramsSets              = params;
    paramsSets.uniqueCheckAcross = true;

    auto paramsGems               = params;
    paramsGems.supportRandomRange = false;
    paramsGems.enableGeneration   = input.getInt("gemsRandom");

    auto paramsAffix             = params;
    paramsAffix.enableGeneration = input.getInt("affixRandom");

    auto paramsRunes = params;
    auto paramsUniq  = params;

    auto skillResolver = [&output](const std::string& skillId) -> DataRow_skills_ptr {
        if (!skillId.empty()) {
            int intId = std::atoi(skillId.c_str());
            if (intId > 0) {
                return &output.skills.rows[intId];
            }
            if (skillId == "0") {
                return &output.skills.rows[0];
            }
            auto it = output.skills.index.find(skillId);
            if (it != output.skills.index.cend())
                return it->second;
        }
        return nullptr;
    };

    auto fillProps = [&props, &rng, &skillResolver]<typename Row>(DataTable<Row>& table, const RandoParams& params) {
        if (!params.enableGeneration)
            return;

        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;

            MagicPropItem item;
            item.skillResolver = skillResolver;
            item.readFrom(row);
            if (item.slot == SlotType::Undefined || item.slot == SlotType::PD2Map)
                continue;

            item.generate(rng, props, params);
            item.writeTo(row);
            //Logger() << item.toString();
        }
    };
    auto startFillProps = Mernel::ChronoPoint(true);

    fillProps(output.uniqueitems, paramsUniq);
    fillProps(output.runes, paramsRunes);
    fillProps(output.setitems, paramsSetitems);
    fillProps(output.gems, paramsGems);
    fillProps(output.magicprefix, paramsAffix);
    fillProps(output.magicsuffix, paramsAffix);
    fillProps(output.sets, paramsSets);

    auto endFillProps = startFillProps.GetElapsedTime().ToProfilingTime();

    if (extraLoc.size() > 0) {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const Loc& loc : extraLoc) {
            totalKeySize += loc.key.size() + 1;
            totalValSize += loc.value.size() + 1;
        }
        Logger(Logger::Notice) << "Adding new " << extraLoc.size() << " translations, totalKeySize=" << totalKeySize << ", totalValSize=" << totalValSize;
        if (output.jsonFiles.contains(JsonId::ItemNames)) {
            auto& jsonDoc = output.jsonFiles[JsonId::ItemNames];
            updateD2Rtranslation(jsonDoc, extraLoc, input.m_env.d2rLocaleId);
        }
        if (output.blobs.contains(BlobId::PatchStringTbl)) {
            auto& blob = output.blobs[BlobId::PatchStringTbl];
            updateLoDtranslation(blob, extraLoc);
        }
    }

    Logger(Logger::Notice) << "  item randomizer fill done in: " << endFillProps;
}

void ModuleItemRandomizer::grabAllProps(AllProps& allProps, DataContext& output, MagicContext& context) const
{
    std::set<std::string> skippedAttributes;
    std::set<std::string> unknownAttributes;

    auto grabPropsRow = [&context, &allProps, &skippedAttributes, &unknownAttributes](const AffixPropList* affixList, SlotType slot, int level) {
        assert(level >= -1 && level < 260);
        for (const auto& prop : *affixList) {
            auto id = getAttributeId(prop.code);
            if (id == AttributeId::INVALID) {
                unknownAttributes.insert(prop.code);
            } else if (id == AttributeId::SKIP) {
                skippedAttributes.insert(prop.code);
            } else {
                context.existingGameAttributes.insert(id);
                allProps[id][slot][level].push_back(prop);
            }
        }
    };

    auto grabProps = [&grabPropsRow]<typename Row>(const DataTable<Row>& table) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int level = row.getLevel();

            ItemCodeList types = row.getItemFilter();
            auto         slot  = types.determineSlot();
            if (slot == SlotType::PD2Map) {
                continue;
            }
            if (slot == SlotType::Undefined) {
                Logger(Logger::Warning) << "Indefined slot type for item: " << row.getId();
                continue;
            }

            for (const AffixPropList* affixList : row.affixLists()) {
                grabPropsRow(affixList, slot, level);
            }
        }
    };

    grabProps(output.uniqueitems);
    grabProps(output.runes);
    grabProps(output.setitems);
    grabProps(output.gems);
    grabProps(output.magicprefix);
    grabProps(output.magicsuffix);
    grabProps(output.sets);

    const std::vector<std::pair<std::string, SlotType>> hints{
        { "shield", SlotType::MainArmor },
        { "quiver", SlotType::Weapon },
        { "body", SlotType::MainArmor },
        { "amulet", SlotType::Amulet },
        { "ring", SlotType::Ring },
        { "weapon", SlotType::Weapon },
        { "boots", SlotType::OtherArmor },
        { "helm", SlotType::MainArmor },
        { "belt", SlotType::OtherArmor },
        { "gloves", SlotType::OtherArmor },
    };

    for (auto& row : output.cubemain.rows) {
        if (!row.isCrafted())
            continue;
        auto      desc         = Mernel::strToLower(row.description);
        const int craftedLevel = 50;
        SlotType  slot         = SlotType::OtherArmor;

        for (const auto& hint : hints)
            if (desc.ends_with(hint.first)) {
                slot = hint.second;
            }

        for (const AffixPropList* affixList : row.affixLists()) {
            grabPropsRow(affixList, slot, craftedLevel);
        }
    }

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
}

void ModuleItemRandomizer::createNewItems(ExtraLoc& extraLoc, DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const int setBonusMode     = input.getInt("setBonusMode");
    const int repeatUniquesMax = 5;
    const int repeatSetsMax    = 5;
    const int repeatUniques    = std::clamp(input.getInt("repeat_uniques"), 1, repeatUniquesMax);
    const int repeatSets       = std::clamp(input.getInt("repeat_sets"), 1, repeatSetsMax);

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
        auto afterCopyUniq = [&input, &extraLoc, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_uniqueitems& row, const DataRow_uniqueitems& old) {
            row.index        = makeKey(id, 'u');
            row.originalName = makeValue(old.index, i);
            extraLoc.push_back({ row.index, row.originalName });

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
        auto afterCopySetitem = [&input, &extraLoc, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_setitems& row, const DataRow_setitems& old) {
            row.index        = makeKey(id, 's');
            row.originalName = makeValue(old.index, i);
            extraLoc.push_back({ row.index, row.originalName });

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
        auto afterCopySet = [&extraLoc, &getRandomColor, &getNewLevelAndCode](int id, int i, DataRow_sets& row, const DataRow_sets& old) {
            row.index = makeValue(old.index, i);
            extraLoc.push_back({ row.index, row.index });

            row.name = row.index;
        };

        processTableWithCopies(output.sets, repeatSets, beforeCopySet, afterCopySet);
    }
}

void ModuleItemRandomizer::analyzeAllPropsForDump(
    AttributeId                    code,
    const AllPropsVal&             allPropsVal,
    NonPropAttributeRecordList&    nonPropList,
    PropAttributeRecordList&       propList,
    ApproximationResultsSimpleMap& approxMapSimple) const
{
    const auto& desc = getAttributeDesc(code);

    const bool ignore = desc.flags.contains(AttributeFlag::Keep) || desc.flags.contains(AttributeFlag::PD2Map);
    if (ignore)
        return;

    const bool isPositive      = !desc.flags.contains(AttributeFlag::Negative);
    const bool hasMinMaxLimits = desc.flags.contains(AttributeFlag::HasRange);
    const bool hasNoRange      = desc.flags.contains(AttributeFlag::NoRange);
    const bool hasSkillPar     = desc.flags.contains(AttributeFlag::SkillPar);

    ApproximationResultsMap approxMap;
    LevelValueList          allValues;
    LevelValueList          allExceptions;
    AffixPropLeveledMap     regularMap;

    bool parEmptyAll = true;
    for (const auto& [slot, slotProps] : allPropsVal) {
        LevelValueList slotValues;

        bool parEmpty = true;
        auto add      = [&allValues, hasMinMaxLimits, hasNoRange, hasSkillPar, &allExceptions, &slotValues, &regularMap, slot, isPositive, &parEmpty, &parEmptyAll](AffixProp prop, int level) {
            if (hasMinMaxLimits && !hasSkillPar) {
                if (!prop.hasRange() && !prop.moveParToMinMax())
                    return;
                prop.par.clear();
            }

            assert(level >= -1 && level < 260);
            if (!hasNoRange && !prop.hasPar() && prop.hasRange()) {
                const auto min = prop.min.value();
                const auto max = prop.max.value();

                if (isPositive) {
                    if (min <= 0 || max <= 0) {
                        allExceptions.push_back({ level, min, max });
                        return;
                    }
                } else {
                    if (min >= 0 || max >= 0) {
                        allExceptions.push_back({ level, min, max });
                        return;
                    }
                }
                allValues.push_back({ level, min, max });
                slotValues.push_back({ level, min, max });

            } else {
                parEmpty    = false;
                parEmptyAll = false;
            }
            regularMap[slot].push_back(AffixPropLeveled{
                .level = level,
                .par   = prop.par,
                .min   = prop.min,
                .max   = prop.max,
            });
        };
        for (const auto& [level, props] : slotProps) {
            for (const auto& prop : props) {
                add(prop, level);
            }
        }
        if (parEmpty && slotValues.size()) {
            approxMap[slot].linearApprox(slotValues);
        }
    }
    if (parEmptyAll) {
        ApproximationResults approxGlobal;
        ApproximationResults approxGlobalBad;
        if (allValues.size()) {
            approxGlobal.linearApprox(allValues);
            approxGlobal.makeMonotonic(isPositive);
        }
        if (allExceptions.size()) {
            approxGlobalBad.linearApprox(allExceptions);
        }
        for (const auto& [slot, approxSlot] : approxMap) {
            if (approxSlot.frequency && approxGlobal.frequency)
                approxMapSimple[slot][code] = buildResultsSimple(approxSlot, approxGlobal);
        };
        nonPropList.push_back(NonPropAttributeRecord{
            .code       = code,
            .frequency  = allValues.size() + allExceptions.size(),
            .goodValues = std::move(approxGlobal),
            .badValues  = std::move(approxGlobalBad),
        });

    } else {
        propList.push_back(PropAttributeRecord{
            .code       = code,
            .slotValues = std::move(regularMap),
        });
    }
}

void ModuleItemRandomizer::dumpAttributes(const NonPropAttributeRecordList& nonPropList, const PropAttributeRecordList& propList, const ApproximationResultsSimpleMap& approxMapSimple) const
{
    auto* dumpFileName = getenv("MODGEN_ATTRIBUTE_DUMP");
    if (!dumpFileName)
        return;

    const Mernel::std_path dumpFile = Mernel::string2path(dumpFileName);
    std::string            oldData;
    Mernel::readFileIntoBufferNoexcept(dumpFileName, oldData);

    std::ostringstream dump;

    dump << "#pragma once\n"
         << "// this is generated, do not edit\n\n"
         << "#include \"AttributeValues.hpp\"\n\n"
         << "namespace D2ModGen {\n\n"
         << "// clang-format off\n"
         << "static inline const NonPropAttributeRecordList g_generatedNonPropAttributes1{\n";
    ;
    for (const auto& prop : nonPropList)
        dump << prop.toString();
    dump << "\n};\n\n";

    dump << "static inline const ApproximationResultsSimpleMap g_generatedSlotScale1" << printMap(approxMapSimple);
    dump << ";\n\n";

    dump << "static inline const PropAttributeRecordList g_generatedPropAttributes1{\n";

    for (const auto& prop : propList)
        dump << prop.toString();
    dump << "\n};\n";

    dump << "// clang-format on\n}\n";

    if (oldData != dump.str())
        Mernel::writeFileFromBufferNoexcept(dumpFileName, dump.str());
}
}

void D2ModGen::ModuleItemRandomizer::updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const ExtraLoc& extraLoc, const std::string& localeId) const
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
    for (const Loc& loc : extraLoc) {
        Mernel::PropertyTree itemDesc;
        itemDesc["Key"]    = Mernel::PropertyTreeScalar(loc.key);
        itemDesc["id"]     = Mernel::PropertyTreeScalar(id++);
        itemDesc[localeId] = Mernel::PropertyTreeScalar(loc.value);

        jsonDoc.append(itemDesc);
    }
}

void D2ModGen::ModuleItemRandomizer::updateLoDtranslation(std::string& patchstringTlb, const ExtraLoc& extraLoc) const
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

    for (const Loc& loc : extraLoc) {
        tbl.m_rows.push_back({ .Key = loc.key, .Val = loc.value });
    }
    tbl.writeToBuffer(patchstringTlb);
}
