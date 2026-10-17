/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"

#include "Logger.hpp"
#include "MernelPlatform/ChronoPoint.hpp"

namespace D2ModGen {

namespace {
constexpr const int s_maxUnbalanceLevel = 100;

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return ch < 0 || !std::isspace(ch);
            }));
}
inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return ch < 0 || !std::isspace(ch);
            }).base(),
            s.end());
}

inline StringVector splitLine(const std::string& line, char sep, bool skipEmpty = false)
{
    std::vector<std::string> result;
    std::string              token;
    std::istringstream       ss(line);
    while (std::getline(ss, token, sep)) {
        ltrim(token);
        rtrim(token);
        if (!token.empty() || !skipEmpty)
            result.push_back(token);
    }
    return result;
}

}

ModuleItemRandomizer::ModuleItemRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "affixRandom", Mernel::PropertyTreeScalar{ true } },
          { "crazyLevel", Mernel::PropertyTreeScalar{ 20 } },
          { "extraKnown", Mernel::PropertyTreeScalar{} },
          { "gemsRandom", Mernel::PropertyTreeScalar{ false } },
          { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "noDuplicates", Mernel::PropertyTreeScalar{ true } },
          { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "removeKnock", Mernel::PropertyTreeScalar{ true } },
          { "repeat_sets", Mernel::PropertyTreeScalar{ 5 } },
          { "repeat_uniques", Mernel::PropertyTreeScalar{ 10 } },
          { "replaceCharges", Mernel::PropertyTreeScalar{ false } },
          { "replaceSkills", Mernel::PropertyTreeScalar{ false } },
          { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
      })
{
    setPresets({
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "crazyLevel", Mernel::PropertyTreeScalar{ 100 } },
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
            { "crazyLevel", Mernel::PropertyTreeScalar{ 30 } },
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
            { "crazyLevel", Mernel::PropertyTreeScalar{ 10 } },
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
#if 0
    MagicPropUniverse props;
    {
        for (auto& row : output.itemtypes.rows) {
            const auto codeStr  = row.code;
                                 const auto itemType = row.itemType;
            if (codeStr.empty())
                continue;

            const auto parent1 = row.equiv1;
            const auto parent2 = row.equiv2;

            ItemTypeInfo& info = props.itemTypeInfo[codeStr];
            info.flags = row.flags;

            if (!parent1.empty()) {
                info.parents.insert(parent1);
                props.itemTypeInfo[parent1].nested.insert(codeStr);
            }
            if (!parent2.empty()) {
                info.parents.insert(parent2);
                props.itemTypeInfo[parent2].nested.insert(codeStr);
            }
        }
        props.itemTypeInfo[("SETS")] = {};

        auto expandNested = [&props]() -> bool {
            bool result = false;
            for (auto& typeInfo : props.itemTypeInfo) {
                const auto ncopy = typeInfo.second.nested;
                for (const auto& nestedCode : ncopy) {
                    appendToSet(typeInfo.second.nested, props.itemTypeInfo[nestedCode].nested);
                }
                const auto pcopy = typeInfo.second.parents;
                for (const auto& pCode : pcopy) {
                    appendToSet(typeInfo.second.parents, props.itemTypeInfo[pCode].parents);
                }
                result = result || ncopy != typeInfo.second.nested || pcopy != typeInfo.second.parents;
            }
            return result;
        };
        while (expandNested()) {
            ;
        }
        //        for (auto& typeInfo : props.itemTypeInfo) {
        //            Logger() << typeInfo.first << " -> " << typeInfo.second.nested;
        //            Logger() << typeInfo.first << " <- " << typeInfo.second.parents;
        //        }
    }

    std::map<std::string, std::string> code2type;

    auto typeBuilder = [&code2type, &props]<typename Row>(const DataTable<Row>& table) {
        for (auto& row : table.rows) {
            const auto code = row.code;
            const auto type = row.type;
            if (!code.empty() && !type.empty()) {
                code2type[code] = type;
                assert(props.itemTypeInfo.count(type));
            }
        }
    };
    typeBuilder(output.armor);
    typeBuilder(output.weapons);
    typeBuilder(output.misc);

    std::map<std::string, int> miscItemsLevels;
    {
        for (auto& row : output.misc.rows) {
            if (!row.code.empty())
                miscItemsLevels[row.code] = std::max(row.lvlreq.value_or(0), row.lvl.value_or(0));
        }
    }
    std::map<std::string, int> setLevels;
    auto                       determineRWlevel = [&miscItemsLevels](const StringVector& runes) {
        int level = 0;
        for (const std::string& rune : runes)
            level = std::max(level, mapValue(miscItemsLevels, rune));
        return level;
    };

    std::vector<std::string> setItemsKeys;
    std::vector<std::string> allItemsKeys;

    const bool replaceSkills  = input.getInt("replaceSkills");
    const bool replaceCharges = input.getInt("replaceCharges");
    const bool removeKnock    = input.getInt("removeKnock");

    const int repeatUniques = input.getInt("repeat_uniques");
    const int repeatSets    = input.getInt("repeat_sets");

    using LevelCallback               = std::function<int(const Row& row)>;
    using SupportedAttributesCallback = std::function<AttributeFlagSet(const Row& row)>;
    using ItemCodeFilterCallback      = std::function<ItemCodeFilter(const Row& row)>;

    auto postProcessRawList = [replaceSkills, replaceCharges, removeKnock](MagicPropRawList& rawList) {
        rawList.postProcess(replaceSkills, replaceCharges, removeKnock);
    };
    auto grabProps = [&props, &postProcessRawList]<typename Row>(const DataTable<Row>&         table,
                                                                 const LevelCallback&          levelCb,
                                                                 const ItemCodeFilterCallback& codeFilterCb) {
        for (auto& row : table.rows) {
            const int level = levelCb(row);
            if (level <= 0)
                continue;

            auto types = codeFilterCb(row);

            for (AffixPropList* affixList : row.affixLists()) {
                MagicPropRawList rawList;

                rawList.readFromRow(*affixList, true);
                postProcessRawList(rawList);

                props.add(std::move(rawList), types, level);
            }
        }
    };

    auto commonLvlReq = [](const Row& row) {
        int ilvl = row["lvl"].toInt();
        return ilvl > 1 ? ilvl : row["lvl req"].toInt();
    };
    auto commonRWreq = [&determineRWlevel](const Row& row) {
        return determineRWlevel({ row["Rune1"].str, row["Rune2"].str, row["Rune3"].str, row["Rune4"].str, row["Rune5"].str, row["Rune6"].str });
    };
    auto commonSetReq = [&setLevels](const Row& row) {
        return mapValue(setLevels, row["name"].str);
    };
    auto uniqueType = [&code2type](const Row& row) -> ItemCodeFilter {
        return { { mapValue(code2type, row["code"].str) }, {} };
    };
    auto setitemType = [&code2type](const Row& row) -> ItemCodeFilter {
        return { { mapValue(code2type, row["item"].str) }, {} };
    };
    auto rwTypes = [](const Row& row) -> ItemCodeFilter {
        ItemCodeSet result;
        for (int i = 1; i <= 6; ++i) {
            const auto& itype = row[argCompat("itype%1", i)];
            if (itype.isEmpty())
                break;
            result.insert(itype.str);
        }
        return { result, {} };
    };
    auto affixTypes = [](const Row& row) -> ItemCodeFilter {
        ItemCodeFilter result;
        for (int i = 1; i <= 7; ++i) {
            const auto col = argCompat("itype%1", i);
            if (!row.hasColumn(col))
                break;
            const auto& itype = row[col];
            if (itype.isEmpty())
                break;
            result.include.insert(itype.str);
        }
        for (int i = 1; i <= 5; ++i) {
            const auto col = argCompat("etype%1", i);
            if (!row.hasColumn(col))
                break;
            const auto& etype = row[col];
            if (etype.isEmpty())
                break;
            result.exclude.insert(etype.str);
        }
        return result;
    };
    auto setsTypes = [](const Row& row) -> ItemCodeFilter { return { { std::string("SETS") }, {} }; };
    using namespace Tables;
    {
        grabProps(output.uniqueitems, commonLvlReq, uniqueType);
        for (auto& row : output.uniqueitems.rows) {
            auto& item = row["index"];
            auto& lvl  = row["lvl"];
            if (item.isEmpty() || lvl.isEmpty())
                continue;
            allItemsKeys.push_back(item.str);
        }
        if (repeatUniques > 1) {
            auto rows = output.uniqueitems.rows;

            for (int i = 2; i <= repeatUniques; ++i)
                rows.insert(rows.end(), output.uniqueitems.rows.cbegin(), output.uniqueitems.rows.cend());
            output.uniqueitems.rows = rows;
        }
    }

    {
        grabProps(output.runes, commonRWreq, rwTypes);
    }
    {
        grabProps(output.setitems, commonLvlReq, setitemType);
        for (auto& row : output.setitems.rows) {
            auto& setItem = row["index"];
            auto& setId   = row["set"];
            auto& lvl     = row["lvl"];
            if (setId.isEmpty())
                continue;
            setItemsKeys.push_back(setItem.str);
            allItemsKeys.push_back(setItem.str);
            setLevels[setId.str] = lvl.toInt();
        }

        const auto rowsOriginal = output.setitems.rows;
        for (int i = 2; i <= repeatSets; ++i) {
            auto rowsCopy = rowsOriginal;
            for (auto& row : rowsCopy) {
                auto& firstCol = row.data[0]; // index
                auto& thirdCol = row.data[2]; // set
                if (firstCol.isEmpty() || thirdCol.isEmpty())
                    continue;
                firstCol.str += " " + std::to_string(i);
                thirdCol.str += " " + std::to_string(i);
            }
            output.setitems.rows.insert(output.setitems.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }
    {
        grabProps(
            output.gems, [&miscItemsLevels](const Row& row) {
                return mapValue(miscItemsLevels, row["code"].str);
            },
            uniqueType);
    }
    {
        auto affixCb = [](const Row& row) {
            return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
        };

        grabProps(output.magicprefix, affixCb, affixTypes);
        grabProps(output.magicsuffix, affixCb, affixTypes);
    }
    {
        grabProps(output.sets, commonSetReq, setsTypes);
        for (auto& row : output.sets.rows) {
            auto& setId   = row["index"];
            auto& setName = row["name"];
            if (setName.isEmpty())
                continue;
            setItemsKeys.push_back(setId.str);
            allItemsKeys.push_back(setId.str);
        }

        const auto rowsOriginal = output.sets.rows;
        for (int i = 2; i <= repeatSets; ++i) {
            auto rowsCopy = rowsOriginal;
            for (auto& row : rowsCopy) {
                auto& firstCol  = row.data[0]; // index
                auto& secondCol = row.data[1]; // name
                if (firstCol.isEmpty() || secondCol.isEmpty())
                    continue;
                firstCol.str += " " + std::to_string(i);
                secondCol.str += " " + std::to_string(i);
            }
            output.sets.rows.insert(output.sets.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }

    const int  unbalance               = input.getInt("crazyLevel");
    const int  itemFitPercent          = input.getInt("itemFitPercent");
    const int  keepOriginalPercent     = input.getInt("keepOriginalPercent");
    const int  relativeGeneralCountMin = input.getInt("relativeCountMin");
    const int  relativeGeneralCountMax = input.getInt("relativeCountMax");
    const int  relativeSetCountMin     = input.getInt("setRelativeCountMin");
    const int  relativeSetCountMax     = input.getInt("setRelativeCountMax");
    const bool noDuplicates            = input.getInt("noDuplicates");

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

    auto fillProps = [&props,
                      &postProcessRawList,
                      &rng,
                      &calcNewCount,
                      itemFitPercent,
                      unbalance,
                      noDuplicates]<typename Row>(const DataTable<Row>&              table,
                                                  const LevelCallback&               levelCb,
                                                  const SupportedAttributesCallback& supportedAttributesCb,
                                                  const ItemCodeFilterCallback&      codeFilterCb,
                                                  const bool                         skipEmptyList,
                                                  const bool                         supportMinMax,
                                                  const bool                         isSet = false) {
        for (auto& row : table.rows) {
            const int level = levelCb(row);
            if (level <= 0) {
                continue;
            }
            const auto supportedAttributes = supportedAttributesCb(row);
            const auto codeFilter          = codeFilterCb(row);

            int genCountPrev = 0;

            for (AffixPropList* affixList : row.affixLists()) {
                MagicPropRawList rawList;
                rawList.readFromRow(*affixList, false);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                postProcessRawList(rawList);

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

                auto newProps = props.generate(rng,
                                               rawList.getAllCodes(),
                                               supportedAttributes,
                                               codeFilter,
                                               itemFitPercent,
                                               genCount,
                                               level,
                                               unbalance == s_maxUnbalanceLevel ? -1 : unbalance,
                                               noDuplicates);

                rawList.append(std::move(newProps));
                if (rawList.getTotalSize() > affixList->getLimit())
                    rawList.truncateRandom(rng, affixList->getLimit());

                if (!supportMinMax)
                    rawList.makePerfect();

                rawList.writeToRow(*affixList);
            }
        }
    };
    auto commonTypeAll         = [](const Row&) -> AttributeFlagSet { return {}; };
    auto commonTypeEquipNoSock = [](const Row&) -> AttributeFlagSet { return { AttributeFlag::Durability }; };
    auto start                 = Mernel::ChronoPoint(true);
    {
        auto code2flags = [&code2type, &props](const Row& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["code"].str);
            if (type.empty())
                return {};
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(output.uniqueitems, commonLvlReq, code2flags, uniqueType, true, true);
    }

    {
        fillProps(output.runes, commonRWreq, commonTypeEquipNoSock, rwTypes, true, false);
    }
    {
        auto code2flags = [&code2type, &props](const Row& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["item"].str);
            return props.itemTypeInfo.contains(type) ? props.itemTypeInfo.at(type).flags : AttributeFlagSet{};
        };
        fillProps(output.setitems, commonLvlReq, code2flags, setitemType, false, false, true);
    }
    if (input.getInt("gemsRandom")) {
        fillProps(
            output.gems, [&miscItemsLevels](const Row& row) {
                return mapValue(miscItemsLevels, row["code"].str);
            },
            commonTypeEquipNoSock,
            uniqueType,
            true,
            false);
    }
    if (input.getInt("affixRandom")) {
        auto affixFill = [&miscItemsLevels, &fillProps, &commonTypeAll, &affixTypes](auto& table) {
            fillProps(
                table, [&miscItemsLevels](const Row& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                commonTypeAll,
                affixTypes,
                true,
                true);
        };
        affixFill(output.magicprefix);
        affixFill(output.magicsuffix);
    }
    {
        fillProps(output.sets, commonSetReq, commonTypeAll, setsTypes, false, false);
    }
    {
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
    }
    Logger() << "generate() take:" << start.GetElapsedTime().ToProfilingTime();
#endif
}

}
