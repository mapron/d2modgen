/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"

#include <QStringList>

#include "Logger.hpp"
#include "ChronoPoint.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleItemRandomizer>();

constexpr const int s_maxUnbalanceLevel = 100;

StringVector split(const std::string& str, char sep)
{
    const QStringList parts = QString::fromStdString(str).split(sep, Qt::SkipEmptyParts);
    StringVector      result;
    result.reserve(parts.size());
    for (const QString& part : parts)
        result.push_back(part.toStdString());
    return result;
}

}

IModule::PresetList ModuleItemRandomizer::presets() const
{
    return {
        PropertyTreeScalarMap{
            { "crazyLevel", s_maxUnbalanceLevel },
            { "itemFitPercent", 0 },
            { "keepOriginalPercent", 0 },
            { "relativeCountMin", 1000 },
            { "relativeCountMax", 1000 },
            { "affixRandom", true },
            { "gemsRandom", true },
            { "replaceSkills", true },
            { "replaceCharges", true },
        },
        PropertyTreeScalarMap{
            { "crazyLevel", 30 },
            { "itemFitPercent", 50 },
            { "keepOriginalPercent", 20 },
            { "relativeCountMin", 100 },
            { "relativeCountMax", 1000 },
            { "affixRandom", true },
            { "replaceSkills", true },
            { "replaceCharges", true },
        },
        PropertyTreeScalarMap{
            { "crazyLevel", 10 },
            { "itemFitPercent", 70 },
            { "keepOriginalPercent", 50 },
            { "relativeCountMin", 100 },
            { "relativeCountMax", 120 },
        },
    };
}

PropertyTreeScalarMap ModuleItemRandomizer::defaultValues() const
{
    return {
        { "crazyLevel", 20 },
        { "itemFitPercent", 70 },
        { "keepOriginalPercent", 70 },
        { "relativeCountMin", 100 },
        { "relativeCountMax", 1000 },
        { "repeat_uniques", 10 },
        { "noDuplicates", true },
        { "affixRandom", true },
        { "gemsRandom", false },
        { "replaceSkills", false },
        { "replaceCharges", false },
        { "removeKnock", true },
        { "extraKnown", std::string() },
    };
}

IModule::UiControlHintMap ModuleItemRandomizer::uiHints() const
{
    return {
        { "crazyLevel", UiControlHintSliderMinMax(5, s_maxUnbalanceLevel) },
        { "itemFitPercent", UiControlHintSliderMinMax(0, 100) },
        { "keepOriginalPercent", UiControlHintSliderMinMax(0, 100) },
        { "relativeCountMin", UiControlHintSlider(4, 10) },
        { "relativeCountMax", UiControlHintSlider(4, 10) },
        { "repeat_uniques", UiControlHintSliderMinMax(1, 20) },
        { "extraKnown", UiControlHint{ UiControlHint::Control::LineEdit } },
    };
}

void ModuleItemRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    auto& tableSet = output.tableSet;

    MagicPropUniverse props;
    {
        TableView view(tableSet.tables["itemtypes"]);
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
    for (const auto* tableName : { "armor", "weapons", "misc" }) {
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
        TableView view(tableSet.tables["misc"]);
        for (auto& row : view) {
            const std::string& code     = row["code"].str;
            auto&              levelreq = row["levelreq"];
            auto&              level    = row["level"];
            if (!code.empty())
                miscItemsLevels[code] = std::max(levelreq.toInt(), level.toInt());
        }
    }
    std::map<std::string, int> setLevels;
    auto                   determineRWlevel = [&miscItemsLevels](const StringVector& runes) {
        int level = 0;
        for (const std::string& rune : runes)
            level = std::max(level, mapValue(miscItemsLevels, rune));
        return level;
    };

    const bool replaceSkills  = input.getInt("replaceSkills");
    const bool replaceCharges = input.getInt("replaceCharges");
    const bool removeKnock    = input.getInt("removeKnock");

    const StringVector          extraKnownCodesList = split(input.getString("extraKnown"), ',');
    const StringSet extraKnownCodes(extraKnownCodesList.cbegin(), extraKnownCodesList.cend());

    using LevelCallback               = std::function<int(const TableView::RowView& row)>;
    using SupportedAttributesCallback = std::function<AttributeFlagSet(const TableView::RowView& row)>;
    using ItemCodeFilterCallback      = std::function<ItemCodeFilter(const TableView::RowView& row)>;

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

                rawList.readFromRow(row, columns, extraKnownCodes);
                postProcessRawList(rawList);

                props.add(std::move(rawList), types, level);
            }
        }
    };

    auto commonLvlReq = [](const TableView::RowView& row) { return row["lvl"].toInt(); };
    auto commonRWreq  = [&determineRWlevel](const TableView::RowView& row) {
        return determineRWlevel({ row["Rune1"].str, row["Rune2"].str, row["Rune3"].str, row["Rune4"].str, row["Rune5"].str, row["Rune6"].str });
    };
    auto commonSetReq = [&setLevels](const TableView::RowView& row) {
        return mapValue(setLevels, row["name"].str);
    };
    auto uniqueType = [&code2type](const TableView::RowView& row) -> ItemCodeFilter {
        assert(code2type.contains(row["code"].str));
        return { { mapValue(code2type, row["code"].str) }, {} };
    };
    auto setitemType = [&code2type](const TableView::RowView& row) -> ItemCodeFilter {
        assert(code2type.contains(row["item"].str));
        return { { mapValue(code2type, row["item"].str) }, {} };
    };
    auto rwTypes = [](const TableView::RowView& row) -> ItemCodeFilter {
        ItemCodeSet result;
        for (int i = 1; i <= 6; ++i) {
            const auto& itype = row[argCompat("itype%1", i)];
            if (itype.isEmpty())
                break;
            result.insert(itype.str);
        }
        return { result, {} };
    };
    auto affixTypes = [](const TableView::RowView& row) -> ItemCodeFilter {
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
    auto setsTypes = [](const TableView::RowView& row) -> ItemCodeFilter { return { { std::string("SETS") }, {} }; };
    using namespace Tables;
    {
        auto&     table = tableSet.tables["uniqueitems"];
        TableView view(table);
        grabProps(view, s_descUniques, commonLvlReq, uniqueType);
        const int repeatUniques = input.getInt("repeat_uniques");
        if (repeatUniques > 1) {
            auto rows = table.rows;

            for (int i = 2; i <= repeatUniques; ++i)
                rows.insert(rows.end(), table.rows.cbegin(), table.rows.cend());
            table.rows = rows;
        }
    }

    {
        TableView view(tableSet.tables["runes"]);
        grabProps(view, s_descRunewords, commonRWreq, rwTypes);
    }
    {
        TableView view(tableSet.tables["setitems"]);
        grabProps(view, s_descSetItems, commonLvlReq, setitemType);
        for (auto& row : view) {
            auto& setId          = row["set"];
            auto& lvl            = row["lvl"];
            setLevels[setId.str] = lvl.toInt();
        }
    }
    {
        TableView view(tableSet.tables["gems"]);

        grabProps(
            view, s_descGems, [&miscItemsLevels](const TableView::RowView& row) {
                return mapValue(miscItemsLevels, row["code"].str);
            },
            uniqueType);
    }
    {
        for (const char* table : { "magicprefix", "magicsuffix" }) {
            TableView view(tableSet.tables[table]);

            grabProps(
                view, s_descAffix, [](const TableView::RowView& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                affixTypes);
        }
    }
    {
        TableView view(tableSet.tables["sets"]);
        grabProps(view, s_descSets, commonSetReq, setsTypes);
    }

    const int  unbalance           = input.getInt("crazyLevel");
    const int  itemFitPercent      = input.getInt("itemFitPercent");
    const int  keepOriginalPercent = input.getInt("keepOriginalPercent");
    const int  relativeCountMax    = input.getInt("relativeCountMax");
    const int  relativeCountMin    = input.getInt("relativeCountMin");
    const bool noDuplicates        = input.getInt("noDuplicates");

    auto calcNewCount = [&rng,
                         relativeCountMin,
                         relativeCountMax,
                         keepOriginalPercent](const int originalCount, int columnsCount) -> std::pair<int, int> {
        if (relativeCountMin == 100 && relativeCountMax == 100) {
            if (keepOriginalPercent == 100)
                return { originalCount, 0 };
            if (keepOriginalPercent == 0)
                return { 0, originalCount };
        }

        const int relativeCountBase     = std::min(relativeCountMin, relativeCountMax);
        const int relativeCountBonusRng = std::max(relativeCountMin, relativeCountMax) - relativeCountBase;
        const int relativeCount         = relativeCountBase + (relativeCountBonusRng > 0 ? rng(relativeCountBonusRng) : 0);
        const int absCountInHundreds    = std::min(columnsCount * 100, originalCount * relativeCount);
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
                                    const bool                         supportMinMax) {
        view.markModified();
        for (auto& row : view) {
            const int level = levelCb(row);
            if (level <= 0) {
                continue;
            }
            const auto supportedAttributes = supportedAttributesCb(row);
            const auto codeFilter          = codeFilterCb(row);

            for (const ColumnsDesc& columns : columnsList) {
                MagicPropRawList rawList;
                rawList.readFromRow(row, columns, extraKnownCodes);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                postProcessRawList(rawList);

                const int originalCount  = rawList.getTotalSize();
                auto [keepCnt, genCount] = calcNewCount(originalCount, columns.m_cols.size());
                if (keepCnt + genCount == 0) {
                    genCount = 1;
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
    auto commonTypeAll         = [](const TableView::RowView&) -> AttributeFlagSet { return {}; };
    auto commonTypeEquipNoSock = [](const TableView::RowView&) -> AttributeFlagSet { return { AttributeFlag::Durability }; };
    auto start                 = ChronoPoint(true);
    {
        TableView view(tableSet.tables["uniqueitems"]);
        auto      code2flags = [&code2type, &props](const TableView::RowView& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["code"].str);
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(view, s_descUniques, commonLvlReq, code2flags, uniqueType, true, true);
    }

    {
        TableView view(tableSet.tables["runes"]);
        fillProps(view, s_descRunewords, commonRWreq, commonTypeEquipNoSock, rwTypes, true, false);
    }
    {
        TableView view(tableSet.tables["setitems"]);
        auto      code2flags = [&code2type, &props](const TableView::RowView& row) -> AttributeFlagSet {
            const auto type = mapValue(code2type, row["item"].str);
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(view, s_descSetItems, commonLvlReq, code2flags, setitemType, false, false);
    }
    if (input.getInt("gemsRandom")) {
        TableView view(tableSet.tables["gems"]);
        for (const ColumnsDesc& desc : s_descGems) {
            fillProps(
                view, { desc }, [&miscItemsLevels](const TableView::RowView& row) {
                    return mapValue(miscItemsLevels, row["code"].str);
                },
                commonTypeEquipNoSock,
                uniqueType,
                true,
                false);
        }
    }
    if (input.getInt("affixRandom")) {
        for (const char* table : { "magicprefix", "magicsuffix" }) {
            TableView view(tableSet.tables[table]);

            fillProps(
                view, s_descAffix, [&miscItemsLevels](const TableView::RowView& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                commonTypeAll,
                affixTypes,
                true,
                true);
        }
    }
    {
        TableView view(tableSet.tables["sets"]);
        fillProps(view, s_descSets, commonSetReq, commonTypeAll, setsTypes, false, false);
    }
    Logger() << "generate() take:" << start.GetElapsedTime().ToProfilingTime();
}

}
