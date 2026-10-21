/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"
#include "MernelPlatform/FileIOUtils.hpp"

#include "Logger.hpp"
#include "MernelPlatform/ChronoPoint.hpp"

#include <cstdlib>

namespace D2ModGen {

namespace {
constexpr const int s_maxUnbalanceLevel = 100;
}

ModuleItemRandomizer::ModuleItemRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "affixRandom", Mernel::PropertyTreeScalar{ true } },
          { "levelPlus", Mernel::PropertyTreeScalar{ 20 } },
          { "levelMinus", Mernel::PropertyTreeScalar{ 10 } },
          { "extraKnown", Mernel::PropertyTreeScalar{} },
          { "gemsRandom", Mernel::PropertyTreeScalar{ false } },
          { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "removeKnock", Mernel::PropertyTreeScalar{ true } },
          { "updateRange", Mernel::PropertyTreeScalar{ true } },
          { "repeat_sets", Mernel::PropertyTreeScalar{ 5 } },
          { "repeat_uniques", Mernel::PropertyTreeScalar{ 10 } },
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

    std::vector<std::string> setItemsKeys;
    std::vector<std::string> allItemsKeys;

    const bool replaceSkills  = input.getInt("replaceSkills");
    const bool replaceCharges = input.getInt("replaceCharges");
    const bool removeKnock    = input.getInt("removeKnock");
    const bool updateRange    = input.getInt("updateRange");

    const int setBonusMode  = input.getInt("setBonusMode");
    const int repeatUniques = input.getInt("repeat_uniques");
    const int repeatSets    = input.m_env.isLegacy ? 0 : input.getInt("repeat_sets");

    auto postProcessRawList = [replaceSkills, replaceCharges, removeKnock](MagicPropRawList& rawList) {
        rawList.postProcess(replaceSkills, replaceCharges, removeKnock);
    };

    std::map<AttributeId, std::map<SlotType, std::map<int, AffixPropList>>> allProps;

    std::set<std::string> skippedAttributes;
    std::set<std::string> unknownAttributes;

    auto grabProps = [&skippedAttributes, &unknownAttributes, &context, &allProps]<typename Row>(const DataTable<Row>& table) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int level = row.getLevel();
            assert(level >= -1 && level < 260);

            ItemCodeFilter types = row.getItemFilter();
            auto           slot  = types.determineSlot();
            assert(slot != SlotType::Undefined);

            for (const AffixPropList* affixList : row.affixLists()) {
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
            }
        }
    };

    {
        grabProps(output.uniqueitems);
        std::vector<DataRow_uniqueitems> validRows;
        for (auto& row : output.uniqueitems.rows) {
            if (!row.isValid())
                continue;
            allItemsKeys.push_back(row.index);
            row.disableChronicle = true;
            validRows.push_back(row);
        }
        if (repeatUniques > 1) {
            for (int i = 2; i <= repeatUniques; ++i)
                output.uniqueitems.rows.insert(output.uniqueitems.rows.end(), validRows.cbegin(), validRows.cend());
        }
    }

    {
        grabProps(output.runes);
    }
    {
        grabProps(output.setitems);
        std::vector<DataRow_setitems> validRows;
        for (auto& row : output.setitems.rows) {
            if (!row.isValid())
                continue;
            setItemsKeys.push_back(row.index);
            allItemsKeys.push_back(row.index);
            row.disableChronicle = true;
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
            validRows.push_back(row);
        }

        for (int i = 2; i <= repeatSets; ++i) {
            auto setSuffix = " " + std::to_string(i);
            auto rowsCopy  = validRows;
            for (auto& row : rowsCopy) {
                row.index += setSuffix;
                row.setIndex += setSuffix;
            }
            output.setitems.rows.insert(output.setitems.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }
    {
        grabProps(output.gems);
    }
    {
        grabProps(output.magicprefix);
        grabProps(output.magicsuffix);
    }
    {
        grabProps(output.sets);
        std::vector<DataRow_sets> validRows;
        for (auto& row : output.sets.rows) {
            if (!row.isValid())
                continue;
            setItemsKeys.push_back(row.index);
            allItemsKeys.push_back(row.name);
            validRows.push_back(row);
        }

        const auto rowsOriginal = output.sets.rows;
        for (int i = 2; i <= repeatSets; ++i) {
            auto setSuffix = " " + std::to_string(i);
            auto rowsCopy  = validRows;
            for (auto& row : rowsCopy) {
                row.index += setSuffix;
                row.name += setSuffix;
            }
            output.sets.rows.insert(output.sets.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
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
            Logger(Logger::Warning) << "Unknown attributes: " << skipped.str();
    }
    context.eraseNonGame();
    {
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
            const auto& desc = getAttributeDesc(code);

            const bool ignore = desc.flags.contains(AttributeFlag::Keep) || desc.flags.contains(AttributeFlag::PD2Map);
            if (ignore)
                continue;

            const bool isPositive      = !desc.flags.contains(AttributeFlag::Negative);
            const bool hasMinMaxLimits = desc.flags.contains(AttributeFlag::HasRange);
            const bool hasNoRange      = desc.flags.contains(AttributeFlag::NoRange);
            const bool hasSkillPar     = desc.flags.contains(AttributeFlag::SkillPar);

            ApproximationResultsMap approxMap;
            LevelValueList          allValues;
            LevelValueList          allExceptions;
            AffixPropLeveledMap     regularMap;

            bool parEmptyAll = true;
            for (const auto& [slot, slotProps] : codeProps) {
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

        auto* dumpFileName = getenv("MODGEN_ATTRIBUTE_DUMP");
        if (dumpFileName) {
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

        context.generatedNonPropAttributes.insert(context.generatedNonPropAttributes.cend(), nonPropList.cbegin(), nonPropList.cend());
        context.generatedPropAttributes.insert(context.generatedPropAttributes.cend(), propList.cbegin(), propList.cend());
        context.calculateConfigAttributes();
    }

    MagicPropUniverse props(context);

    const int levelPlus               = input.getInt("levelPlus");
    const int levelMinus              = input.getInt("levelMinus");
    const int itemFitPercent          = input.getInt("itemFitPercent");
    const int keepOriginalPercent     = input.getInt("keepOriginalPercent");
    const int relativeGeneralCountMin = input.getInt("relativeCountMin");
    const int relativeGeneralCountMax = input.getInt("relativeCountMax");
    const int relativeSetCountMin     = input.getInt("setRelativeCountMin");
    const int relativeSetCountMax     = input.getInt("setRelativeCountMax");

    auto calcNewCount = [&rng,
                         relativeGeneralCountMin,
                         relativeGeneralCountMax,
                         relativeSetCountMin,
                         relativeSetCountMax,
                         keepOriginalPercent](const int originalCount, bool isSet) -> std::pair<int, int> {
        const auto relativeCountMin = isSet ? relativeSetCountMin : relativeGeneralCountMin;
        const auto relativeCountMax = isSet ? relativeSetCountMax : relativeGeneralCountMax;
        if (relativeCountMin == 100 && relativeCountMax == 100) {
            if (keepOriginalPercent == 100)
                return { originalCount, 0 };
            if (keepOriginalPercent == 0)
                return { 0, originalCount };
        }

        const int relativeCountBase     = std::min(relativeCountMin, relativeCountMax);
        const int relativeCountBonusRng = std::max(relativeCountMin, relativeCountMax) - relativeCountBase;
        const int relativeCount         = relativeCountBase + (relativeCountBonusRng > 0 ? rng(relativeCountBonusRng) : 0);
        const int absCountInHundreds    = originalCount * relativeCount;
        if (keepOriginalPercent == 0)
            return { 0, absCountInHundreds / 100 };
        const int  keepCountInHundreds = keepOriginalPercent * originalCount;
        const auto rngFraction         = [&rng](const int hundredsCount) -> int {
            const int fraction = hundredsCount % 100;
            const int whole    = hundredsCount / 100;
            if (fraction < 2)
                return whole;
            return whole + (0 == rng(fraction));
        };
        const int keepCount          = rngFraction(keepCountInHundreds);
        const int genCountInHundreds = absCountInHundreds - keepCount * 100;
        const int generatedCount     = rngFraction(genCountInHundreds);
        return { keepCount, generatedCount };
    };

    auto fillProps = [&postProcessRawList,
                      &props,
                      replaceSkills,
                      &rng,
                      &calcNewCount,
                      updateRange,
                      itemFitPercent,
                      levelPlus,
                      levelMinus]<typename Row>(DataTable<Row>& table,
                                                const bool      skipEmptyList,
                                                const bool      supportMinMax,
                                                const bool      isSet = false) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int              originalLevel       = row.getLevel();
            const AttributeFlagSet supportedAttributes = row.getAttributes();
            const int              targetLevel         = [originalLevel, &rng, levelPlus, levelMinus]() {
                if (rng(2)) {
                    return levelPlus ? originalLevel + rng(levelPlus) : originalLevel;
                } else {
                    return levelMinus ? originalLevel - rng(levelMinus) : originalLevel;
                }
            }();
            const size_t levelBracket20 = std::clamp(targetLevel, 0, 99) / 20;

            const ItemCodeFilter types    = row.getItemFilter();
            const SlotType       slotType = types.determineSlot();

            Character existingClass = types.determineClass();

            int genCountPrev = 0;

            UniqueAttributeChecker attrChecker;

            for (AffixPropList* affixList : row.affixLists()) {
                MagicPropRawList rawList;
                rawList.readFromRow(*affixList);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                const int originalCount  = rawList.getTotalSize();
                auto [keepCnt, genCount] = calcNewCount(originalCount, isSet);

                if (keepCnt + genCount == 0) {
                    genCount = 1;
                }
                const int maxGenCount = affixList->getLimit() - keepCnt;
                genCount += genCountPrev;
                if (genCount > maxGenCount) {
                    genCountPrev = genCount - maxGenCount;
                    genCount     = maxGenCount;
                }

                if (keepCnt < originalCount)
                    rawList.truncateRandom(rng, keepCnt);

                if (updateRange)
                    props.update(rawList, rng, slotType, levelBracket20);

                if (existingClass == Character::none)
                    existingClass = rawList.determineClass(true);
                if (!isSet) {
                    attrChecker = {};
                }
                rawList.fill(attrChecker);

                auto newProps = props.generate(rng,
                                               attrChecker,
                                               existingClass,
                                               supportedAttributes,
                                               slotType,
                                               itemFitPercent,
                                               genCount,
                                               levelBracket20);

                rawList.append(std::move(newProps));
                if (rawList.getTotalSize() > affixList->getLimit())
                    rawList.truncateRandom(rng, affixList->getLimit());

                if (!supportMinMax)
                    rawList.makePerfect();

                postProcessRawList(rawList);
                rawList.writeToRow(*affixList);
            }
        }
    };
    auto startFillProps = Mernel::ChronoPoint(true);

    fillProps(output.uniqueitems, true, true);

    fillProps(output.runes, true, true);

    fillProps(output.setitems, false, true, true);

    if (input.getInt("gemsRandom"))
        fillProps(output.gems, true, false);

    if (input.getInt("affixRandom")) {
        fillProps(output.magicprefix, true, true);
        fillProps(output.magicsuffix, true, true);
    }

    fillProps(output.sets, false, false, true);

    auto endFillProps = startFillProps.GetElapsedTime().ToProfilingTime();

    if (repeatSets > 1 && output.jsonFiles.contains(JsonId::ItemNames)) {
        auto&                                       jsonDoc = output.jsonFiles[JsonId::ItemNames];
        std::map<std::string, Mernel::PropertyTree> itemNamesByKey;
        for (const auto& itemDesc : jsonDoc.getList()) {
            const std::string key = itemDesc["Key"].getScalar().toString();
            itemNamesByKey[key]   = itemDesc;
        }
        int id = 1000000;
        for (const auto& key : allItemsKeys) {
            if (itemNamesByKey.contains(key))
                continue;
            Mernel::PropertyTree itemDesc;
            itemDesc["Key"]  = Mernel::PropertyTreeScalar(key);
            itemDesc["enUS"] = Mernel::PropertyTreeScalar(key);
            itemDesc["id"]   = Mernel::PropertyTreeScalar(id++);
            jsonDoc.append(itemDesc);
            itemNamesByKey[key] = itemDesc;
        }
        for (int i = 2; i <= repeatSets; ++i) {
            for (const auto& key : setItemsKeys) {
                const std::string    keyCopy  = key + " " + std::to_string(i);
                Mernel::PropertyTree itemDesc = itemNamesByKey[key];
                itemDesc["Key"]               = Mernel::PropertyTreeScalar(keyCopy);
                itemDesc["id"]                = Mernel::PropertyTreeScalar(id++);
                for (auto& [locKey, locValue] : itemDesc.getMap()) {
                    if (locKey == "Key" || locKey == "id")
                        continue;
                    const auto str = locValue.getScalar().toString();
                    locValue       = Mernel::PropertyTreeScalar(str + " " + std::to_string(i));
                }
                jsonDoc.append(itemDesc);
            }
        }
    }

    Logger() << "  randomizer::generate fillprops done in: " << endFillProps;
}

}
