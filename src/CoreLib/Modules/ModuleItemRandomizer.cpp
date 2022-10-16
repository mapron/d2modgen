/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"

#include "Logger.hpp"
#include "ChronoPoint.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleItemRandomizer>();

constexpr const int s_maxUnbalanceLevel = 100;

const std::string s_itemsJson = "data\\local\\lng\\strings\\item-names.json";

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

void ModuleItemRandomizer::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy)
        return;

    const int repeatSets = input.getInt("repeat_sets");
    if (repeatSets > 1)
        output.m_extraJson.insert(s_itemsJson);
}

void ModuleItemRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    using Row      = TableView::RowView;
    auto& tableSet = output.tableSet;

    MagicPropUniverse props;
    {
        TableView view(tableSet.tables[TableId::itemtypes]);
        for (auto& row : view) {
            const auto codeStr  = row["Code"].str;
            const auto itemType = row["ItemType"].str;
            if (codeStr.empty())
                continue;

            const auto parent1 = row["Equiv1"].str;
            const auto parent2 = row["Equiv2"].str;

            ItemTypeInfo& info = props.itemTypeInfo[codeStr];
            if (row["Throwable"].str == "1")
                info.flags.insert(AttributeFlag::Quantity);
            else if (row["Repair"].str == "1")
                info.flags.insert(AttributeFlag::Durability);

            if (row["MaxSockets3"].str != "0")
                info.flags.insert(AttributeFlag::Sockets);
            if (!row["Shoots"].isEmpty())
                info.flags.insert(AttributeFlag::Missile);
            if (itemType.starts_with("Map"))
                info.flags.insert(AttributeFlag::PD2Map);

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
    for (const auto tableName : { TableId::armor, TableId::weapons, TableId::misc }) {
        TableView view(tableSet.tables[tableName]);
        for (auto& row : view) {
            const auto code = row["code"].str;
            const auto type = row["type"].str;
            if (!code.empty() && !type.empty()) {
                code2type[code] = type;
                assert(props.itemTypeInfo.count(type));
            }
        }
    }

    std::map<std::string, int> miscItemsLevels;
    {
        TableView view(tableSet.tables[TableId::misc]);
        for (auto& row : view) {
            const std::string& code     = row["code"].str;
            auto&              levelreq = row["levelreq"];
            auto&              level    = row["level"];
            if (!code.empty())
                miscItemsLevels[code] = std::max(levelreq.toInt(), level.toInt());
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

    const bool replaceSkills  = input.getInt("replaceSkills");
    const bool replaceCharges = input.getInt("replaceCharges");
    const bool removeKnock    = input.getInt("removeKnock");

    const int repeatUniques = input.getInt("repeat_uniques");
    const int repeatSets    = input.getInt("repeat_sets");

    const StringVector extraKnownCodesList = splitLine(input.getString("extraKnown"), ',', true);
    const StringSet    extraKnownCodes(extraKnownCodesList.cbegin(), extraKnownCodesList.cend());

    using LevelCallback               = std::function<int(const Row& row)>;
    using SupportedAttributesCallback = std::function<AttributeFlagSet(const Row& row)>;
    using ItemCodeFilterCallback      = std::function<ItemCodeFilter(const Row& row)>;

    auto postProcessRawList = [replaceSkills, replaceCharges, removeKnock](MagicPropRawList& rawList) {
        rawList.postProcess(replaceSkills, replaceCharges, removeKnock);
    };
    auto grabProps = [&props, &postProcessRawList, &extraKnownCodes](TableView&                      view,
                                                                     const std::vector<ColumnsDesc>& columnsList,
                                                                     const LevelCallback&            levelCb,
                                                                     const ItemCodeFilterCallback&   codeFilterCb) {
        for (auto& row : view) {
            const int level = levelCb(row);
            if (level <= 0)
                continue;

            auto types = codeFilterCb(row);

            for (const ColumnsDesc& columns : columnsList) {
                MagicPropRawList rawList;

                rawList.readFromRow(row, columns, extraKnownCodes, true);
                postProcessRawList(rawList);

                props.add(std::move(rawList), types, level);
            }
        }
    };

    auto commonLvlReq = [](const Row& row) { return row["lvl"].toInt(); };
    auto commonRWreq  = [&determineRWlevel](const Row& row) {
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
        auto&     table = tableSet.tables[TableId::uniqueitems];
        TableView view(table);
        grabProps(view, s_descUniques, commonLvlReq, uniqueType);
        if (repeatUniques > 1) {
            auto rows = table.rows;

            for (int i = 2; i <= repeatUniques; ++i)
                rows.insert(rows.end(), table.rows.cbegin(), table.rows.cend());
            table.rows = rows;
        }
    }

    {
        TableView view(tableSet.tables[TableId::runes]);
        grabProps(view, s_descRunewords, commonRWreq, rwTypes);
    }
    {
        auto&     table = tableSet.tables[TableId::setitems];
        TableView view(table);
        grabProps(view, s_descSetItems, commonLvlReq, setitemType);
        for (auto& row : view) {
            auto& setItem = row["index"];
            auto& setId   = row["set"];
            auto& lvl     = row["lvl"];
            if (setId.isEmpty())
                continue;
            setItemsKeys.push_back(setItem.str);
            setLevels[setId.str] = lvl.toInt();
        }

        const auto rowsOriginal = table.rows;
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
            table.rows.insert(table.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }
    {
        TableView view(tableSet.tables[TableId::gems]);

        grabProps(
            view, s_descGems, [&miscItemsLevels](const Row& row) {
                return mapValue(miscItemsLevels, row["code"].str);
            },
            uniqueType);
    }
    {
        for (const auto table : { TableId::magicprefix, TableId::magicsuffix }) {
            TableView view(tableSet.tables[table]);

            grabProps(
                view, s_descAffix, [](const Row& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                affixTypes);
        }
    }
    {
        auto&     table = tableSet.tables[TableId::sets];
        TableView view(table);
        grabProps(view, s_descSets, commonSetReq, setsTypes);
        for (auto& row : view) {
            auto& setId   = row["index"];
            auto& setName = row["name"];
            if (setName.isEmpty())
                continue;
            setItemsKeys.push_back(setId.str);
        }

        const auto rowsOriginal = table.rows;
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
            table.rows.insert(table.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
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
                      &extraKnownCodes,
                      itemFitPercent,
                      unbalance,
                      noDuplicates](TableView&                         view,
                                    const ColumnsDescList&             columnsList,
                                    const LevelCallback&               levelCb,
                                    const SupportedAttributesCallback& supportedAttributesCb,
                                    const ItemCodeFilterCallback&      codeFilterCb,
                                    const bool                         skipEmptyList,
                                    const bool                         supportMinMax,
                                    const bool                         isSet = false) {
        view.markModified();
        for (auto& row : view) {
            const int level = levelCb(row);
            if (level <= 0) {
                continue;
            }
            const auto supportedAttributes = supportedAttributesCb(row);
            const auto codeFilter          = codeFilterCb(row);

            int genCountPrev = 0;

            for (const ColumnsDesc& columns : columnsList) {
                MagicPropRawList rawList;
                rawList.readFromRow(row, columns, extraKnownCodes, false);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                postProcessRawList(rawList);

                const int originalCount  = rawList.getTotalSize();
                auto [keepCnt, genCount] = calcNewCount(originalCount, isSet);

                if (keepCnt + genCount == 0) {
                    genCount = 1;
                }
                const int maxGenCount = columns.m_cols.size() - keepCnt;
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
                if (rawList.getTotalSize() > columns.m_cols.size())
                    rawList.truncateRandom(rng, columns.m_cols.size());

                if (!supportMinMax)
                    rawList.makePerfect();

                rawList.writeToRow(row, columns);
            }
        }
    };
    auto commonTypeAll         = [](const Row&) -> AttributeFlagSet { return {}; };
    auto commonTypeEquipNoSock = [](const Row&) -> AttributeFlagSet { return { AttributeFlag::Durability }; };
    auto start                 = ChronoPoint(true);
    {
        TableView view(tableSet.tables[TableId::uniqueitems]);
        auto      code2flags = [&code2type, &props](const Row& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["code"].str);
            if (type.empty())
                return {};
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(view, s_descUniques, commonLvlReq, code2flags, uniqueType, true, true);
    }

    {
        TableView view(tableSet.tables[TableId::runes]);
        fillProps(view, s_descRunewords, commonRWreq, commonTypeEquipNoSock, rwTypes, true, false);
    }
    {
        TableView view(tableSet.tables[TableId::setitems]);
        auto      code2flags = [&code2type, &props](const Row& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["item"].str);
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(view, s_descSetItems, commonLvlReq, code2flags, setitemType, false, false, true);
    }
    if (input.getInt("gemsRandom")) {
        TableView view(tableSet.tables[TableId::gems]);
        for (const ColumnsDesc& desc : s_descGems) {
            fillProps(
                view, { desc }, [&miscItemsLevels](const Row& row) {
                    return mapValue(miscItemsLevels, row["code"].str);
                },
                commonTypeEquipNoSock,
                uniqueType,
                true,
                false);
        }
    }
    if (input.getInt("affixRandom")) {
        for (auto table : { TableId::magicprefix, TableId::magicsuffix }) {
            TableView view(tableSet.tables[table]);

            fillProps(
                view, s_descAffix, [&miscItemsLevels](const Row& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                commonTypeAll,
                affixTypes,
                true,
                true);
        }
    }
    {
        TableView view(tableSet.tables[TableId::sets]);
        fillProps(view, s_descSets, commonSetReq, commonTypeAll, setsTypes, false, false);
    }
    {
        if (repeatSets > 1 && output.jsonFiles.contains(s_itemsJson)) {
            auto&                               jsonDoc = output.jsonFiles[s_itemsJson];
            std::map<std::string, PropertyTree> itemNamesByKey;
            for (const auto& itemDesc : jsonDoc.getList()) {
                const std::string key = itemDesc["Key"].getScalar().toString();
                itemNamesByKey[key]   = itemDesc;
            }
            int id = 1000000;
            for (int i = 2; i <= repeatSets; ++i) {
                for (const auto& key : setItemsKeys) {
                    const std::string keyCopy  = key + " " + std::to_string(i);
                    PropertyTree      itemDesc = itemNamesByKey[key];
                    itemDesc["Key"]            = PropertyTreeScalar(keyCopy);
                    itemDesc["id"]             = PropertyTreeScalar(id++);
                    for (auto& [locKey, locValue] : itemDesc.getMap()) {
                        if (locKey == "Key" || locKey == "id")
                            continue;
                        const auto str = locValue.getScalar().toString();
                        locValue       = PropertyTreeScalar(str + " " + std::to_string(i));
                    }
                    jsonDoc.append(itemDesc);
                }
            }
        }
    }
    Logger() << "generate() take:" << start.GetElapsedTime().ToProfilingTime();
}

}
