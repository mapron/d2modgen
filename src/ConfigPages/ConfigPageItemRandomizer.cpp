/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageItemRandomizer.hpp"
#include "RandoUtils.hpp"

#include "HelpToolButton.hpp"

#include <QBoxLayout>
#include <QLabel>

namespace D2ModGen {

namespace {
constexpr const int   s_maxUnbalanceLevel = 100;
const ColumnsDescList s_descUniques{ ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12) };
const ColumnsDescList s_descRunewords{ ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7) };
const ColumnsDescList s_descSetItems{
    ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9),
    ColumnsDesc("aprop%1a", "apar%1a", "amin%1a", "amax%1a", 5),
    ColumnsDesc("aprop%1b", "apar%1b", "amin%1b", "amax%1b", 5),
};

const ColumnsDescList s_descGems{
    ColumnsDesc("weaponMod%1Code", "weaponMod%1Param", "weaponMod%1Min", "weaponMod%1Max", 3),
    ColumnsDesc("helmMod%1Code", "helmMod%1Param", "helmMod%1Min", "helmMod%1Max", 3),
    ColumnsDesc("shieldMod%1Code", "shieldMod%1Param", "shieldMod%1Min", "shieldMod%1Max", 3),
};

const ColumnsDescList s_descAffix{ ColumnsDesc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3) };

const ColumnsDescList s_descSets{
    ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2),
    ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2),
    ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8),
};

}

ConfigPageItemRandomizer::ConfigPageItemRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidgetMinMax(tr("Crazy-ness (or 'NON-balance level', lower = more balance, 100=chaos)"), "crazyLevel", 5, s_maxUnbalanceLevel, 20, this),
                          tr("Crazyness level - determine level difference to be used when selecting new properties for item/rune/etc.\n"
                             "With '10' it will select between level-10 and level+10 at first, if there are <50 candidates,\n"
                             "then it will select level-30..level+30, and finally it will try fully random. \n"
                             "In short, lower value = more balance in terms of original affix level and item level."))
               << addHelp(new SliderWidgetMinMax(tr("Item type fit percent (0% = fully random, 100% = all according to item type)"), "itemFitPercent", 0, 100, 70, this),
                          tr("Item fit slider allow you to select how much item affixes will be related to original item type.\n"
                             "For example, if you choose 80%, then 4 of 5 affixes will be selected to pool for specific item type\n"
                             "Item can have have several pools related to its type - say, scepter is a rod and a melee weapon.\n"
                             "Item type-specific properties will be picked in proportion to all types."))
               << addHelp(new SliderWidgetMinMax(tr("How many original properties to keep, percent"), "keepOriginalPercent", 0, 100, 50, this),
                          tr("You can select how many properties of original item you want to keep.\n"
                             "If 0, then every item will be fully randomized.\n"
                             "If 50, then half of genereted properties will be original, and half randomized.\n"
                             "If 100, then every property will be property of original item. (you can reduce an amount of props)"))
               << addHelp(new SliderWidget(tr("Minimum relative property count, compared to the original"), "relativeCountMin", 4, 10, this),
                          tr("This and the next options determine new property count will be after generation.\n"
                             "If Min=Max=100%%, then property count will be exactly as original (except rare corner cases).\n"
                             "If Min=50%% and Max=200%%, then new property count will be at least half as original, and twice at best.\n"
                             "For example, if item has 5 properties, then worst case is 2, and best case is 9 (because 9 is maximum for Uniques)"))
               << new SliderWidget(tr("Maximum relative property count, compared to the original"), "relativeCountMax", 4, 10, 1000, this)
               << addHelp(new SliderWidgetMinMax(tr("Number of versions of each unique"), "repeat_uniques", 1, 20, 10, this),
                          tr("allow you to have different uniques with same name and level, but different properties, \n"
                             "you will have N different uniques with differnet stats;\n"
                             "so you have an opportunity to pick same item again to check it out.\n"
                             "This works only with Uniques, not Sets.")));

    addEditors(QList<IValueWidget*>()
               << addHelp(new CheckboxWidget(tr("Always perfect rolls"), "perfectRoll", false, this),
                          tr("Always perfect means simply minimum roll becomes maximum (where it makes sense)."))
               << new CheckboxWidget(tr("Prevent duplicate properties on items"), "noDuplicates", true, this)
               << addHelp(new CheckboxWidget(tr("Randomize magix/rare affixes"), "affixRandom", true, this),
                          tr("This will modify rare and magic suffixes - \n"
                             "so they can include properties of any other item in the game. \n"
                             "Note that their properties are read even without this option."))
               << addHelp(new CheckboxWidget(tr("Randomize gem and runes properties"), "gemsRandom", true, this),
                          tr("This will modify gem and rune properties - \n"
                             "so they can include properties of any other item in the game. \n"
                             "Note that their properties are read even without this option."))
               << addHelp(new CheckboxWidget(tr("Replace skills with oskills"), "replaceSkills", false, this),
                          tr(""))
               << addHelp(new CheckboxWidget(tr("Replace charges with oskills"), "replaceCharges", false, this),
                          tr(""))
               << addHelp(new CheckboxWidget(tr("Remove Knockback/Monster flee"), "removeKnock", false, this),
                          tr("")));
    closeLayout();
}

QString ConfigPageItemRandomizer::pageHelp() const
{
    return tr("What item randomizer does in short - it reads all possible item properties from Uniques, Sets, etc, \n"
              "And then reassign properties back, but in random order (also it does not mean every original will be used).\n"
              "For details, check descriptions of every option.");
}

void ConfigPageItemRandomizer::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto& tableSet = output.tableSet;

    MagicPropUniverse props;
    {
        TableView view(tableSet.tables["itemtypes"]);
        for (auto& row : view) {
            QString& codeStr = row["Code"];
            if (codeStr.isEmpty())
                continue;

            const auto parent1 = (row["Equiv1"]);
            const auto parent2 = (row["Equiv2"]);

            ItemTypeInfo& info = props.itemTypeInfo[codeStr];
            if (row["Throwable"] == "1")
                info.flags << AttributeFlag::Quantity;
            else if (row["Repair"] == "1")
                info.flags << AttributeFlag::Durability;

            if (row["MaxSockets3"] != "0")
                info.flags << AttributeFlag::Sockets;
            if (!row["Shoots"].isEmpty())
                info.flags << AttributeFlag::Missile;

            if (!parent1.isEmpty()) {
                info.parents << parent1;
                props.itemTypeInfo[parent1].nested << codeStr;
            }
            if (!parent2.isEmpty()) {
                info.parents << parent2;
                props.itemTypeInfo[parent2].nested << codeStr;
            }
        }
        props.itemTypeInfo[("SETS")] = {};

        auto expandNested = [&props]() -> bool {
            bool result = false;
            for (auto& typeInfo : props.itemTypeInfo) {
                const auto ncopy = typeInfo.second.nested;
                for (QString nestedCode : ncopy) {
                    typeInfo.second.nested += props.itemTypeInfo[nestedCode].nested;
                }
                const auto pcopy = typeInfo.second.parents;
                for (QString pCode : pcopy) {
                    typeInfo.second.parents += props.itemTypeInfo[pCode].parents;
                }
                result = result || ncopy != typeInfo.second.nested || pcopy != typeInfo.second.parents;
            }
            return result;
        };
        while (expandNested()) {
            ;
        }
        //        for (auto& typeInfo : props.itemTypeInfo) {
        //            qDebug() << typeInfo.first << " -> " << typeInfo.second.nested;
        //            qDebug() << typeInfo.first << " <- " << typeInfo.second.parents;
        //        }
    }

    QMap<QString, QString> code2type;
    for (const auto* tableName : { "armor", "weapons", "misc" }) {
        TableView view(tableSet.tables[tableName]);
        for (auto& row : view) {
            QString& code = row["code"];
            QString& type = row["type"];
            if (!code.isEmpty() && !type.isEmpty()) {
                code2type[code] = type;
                assert(props.itemTypeInfo.count(type));
            }
        }
    }

    QMap<QString, int> miscItemsLevels;
    {
        TableView view(tableSet.tables["misc"]);
        for (auto& row : view) {
            QString& code     = row["code"];
            QString& levelreq = row["levelreq"];
            QString& level    = row["level"];
            if (!code.isEmpty())
                miscItemsLevels[code] = std::max(levelreq.toInt(), level.toInt());
        }
    }
    QMap<QString, int> setLevels;
    auto               determineRWlevel = [&miscItemsLevels](const QStringList& runes) {
        int level = 0;
        for (const QString& rune : runes)
            level = std::max(level, miscItemsLevels.value(rune));
        return level;
    };

    const bool replaceSkills  = getWidgetValue("replaceSkills");
    const bool replaceCharges = getWidgetValue("replaceCharges");
    const bool removeKnock    = getWidgetValue("removeKnock");
    const bool perfectRoll    = getWidgetValue("perfectRoll");

    using LevelCallback               = std::function<int(const TableView::RowView& row)>;
    using SupportedAttributesCallback = std::function<AttributeFlagSet(const TableView::RowView& row)>;
    using ItemCodeFilterCallback      = std::function<ItemCodeFilter(const TableView::RowView& row)>;

    auto postProcessRawList = [replaceSkills, replaceCharges, removeKnock](MagicPropRawList& rawList, bool makePerfect) {
        rawList.postProcess(replaceSkills, replaceCharges, removeKnock);
        if (makePerfect)
            rawList.makePerfect();
    };
    auto grabProps = [&props, &postProcessRawList, perfectRoll](TableView&                      view,
                                                                const std::vector<ColumnsDesc>& columnsList,
                                                                const LevelCallback&            levelCb,
                                                                const ItemCodeFilterCallback&   codeFilterCb,
                                                                bool                            supportMinMax) {
        for (auto& row : view) {
            const int level = levelCb(row);
            if (level <= 0)
                continue;

            auto types = codeFilterCb(row);

            for (const ColumnsDesc& columns : columnsList) {
                MagicPropRawList rawList;

                rawList.readFromRow(row, columns);
                postProcessRawList(rawList, perfectRoll || !supportMinMax);

                props.add(std::move(rawList), types, level);
            }
        }
    };
    auto commonLvlReq = [](const TableView::RowView& row) { return row["lvl"].toInt(); };
    auto commonRWreq  = [&determineRWlevel](const TableView::RowView& row) {
        return determineRWlevel({ row["Rune1"], row["Rune2"], row["Rune3"], row["Rune4"], row["Rune5"], row["Rune6"] });
    };
    auto commonSetReq = [&setLevels](const TableView::RowView& row) {
        return setLevels.value(row["name"]);
    };
    auto uniqueType = [&code2type](const TableView::RowView& row) -> ItemCodeFilter {
        assert(code2type.contains(row["code"]));
        return { { code2type.value(row["code"]) }, {} };
    };
    auto setitemType = [&code2type](const TableView::RowView& row) -> ItemCodeFilter {
        assert(code2type.contains(row["item"]));
        return { { code2type.value(row["item"]) }, {} };
    };
    auto rwTypes = [](const TableView::RowView& row) -> ItemCodeFilter {
        ItemCodeSet result;
        for (int i = 1; i <= 6; ++i) {
            const QString& itype = row[QString("itype%1").arg(i)];
            if (itype.isEmpty())
                break;
            result << itype;
        }
        return { result, {} };
    };
    auto affixTypes = [](const TableView::RowView& row) -> ItemCodeFilter {
        ItemCodeFilter result;
        for (int i = 1; i <= 7; ++i) {
            const auto col = QString("itype%1").arg(i);
            if (!row.hasColumn(col))
                break;
            const QString& itype = row[col];
            if (itype.isEmpty())
                break;
            result.include += itype;
        }
        for (int i = 1; i <= 5; ++i) {
            const auto col = QString("etype%1").arg(i);
            if (!row.hasColumn(col))
                break;
            const QString& itype = row[col];
            if (itype.isEmpty())
                break;
            result.exclude += itype;
        }
        return result;
    };
    auto setsTypes = [](const TableView::RowView& row) -> ItemCodeFilter { return { { QString("SETS") }, {} }; };

    {
        auto&     table = tableSet.tables["uniqueitems"];
        TableView view(table);
        grabProps(view, s_descUniques, commonLvlReq, uniqueType, true);
        const int repeatUniques = getWidgetValue("repeat_uniques");
        if (repeatUniques > 1) {
            auto rows = table.rows;

            for (int i = 2; i <= repeatUniques; ++i)
                rows.append(table.rows);
            table.rows = rows;
        }
    }

    {
        TableView view(tableSet.tables["runes"]);
        grabProps(view, s_descRunewords, commonRWreq, rwTypes, true);
    }
    {
        TableView view(tableSet.tables["setitems"]);
        grabProps(view, s_descSetItems, commonLvlReq, setitemType, true);
        for (auto& row : view) {
            QString& setId   = row["set"];
            QString& lvl     = row["lvl"];
            setLevels[setId] = lvl.toInt();
        }
    }
    {
        TableView view(tableSet.tables["gems"]);

        grabProps(
            view, s_descGems, [&miscItemsLevels](const TableView::RowView& row) {
                return miscItemsLevels.value(row["code"]);
            },
            uniqueType,
            false);
    }
    {
        for (const char* table : { "magicprefix", "magicsuffix" }) {
            TableView view(tableSet.tables[table]);

            grabProps(
                view, s_descAffix, [](const TableView::RowView& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                affixTypes,
                true);
        }
    }
    {
        TableView view(tableSet.tables["sets"]);
        grabProps(view, s_descSets, commonSetReq, setsTypes, false);
    }

    const int  unbalance           = getWidgetValue("crazyLevel");
    const int  itemFitPercent      = getWidgetValue("itemFitPercent");
    const int  keepOriginalPercent = getWidgetValue("keepOriginalPercent");
    const int  relativeCountMax    = getWidgetValue("relativeCountMax");
    const int  relativeCountMin    = getWidgetValue("relativeCountMin");
    const bool noDuplicates        = getWidgetValue("noDuplicates");

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
        const int relativeCount         = relativeCountBase + (relativeCountBonusRng > 0 ? rng.bounded(relativeCountBonusRng) : 0);
        const int absCountInHundreds    = std::min(columnsCount * 100, originalCount * relativeCount);
        if (keepOriginalPercent == 0)
            return { 0, absCountInHundreds / 100 };
        const int  keepCountInHundreds = keepOriginalPercent * originalCount;
        const auto rngFraction         = [&rng](const int hundredsCount) -> int {
            const int fraction = hundredsCount % 100;
            const int whole    = hundredsCount / 100;
            if (fraction < 2)
                return whole;
            return whole + (0 == rng.bounded(fraction));
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
                      noDuplicates,
                      perfectRoll](TableView&                         view,
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
                rawList.readFromRow(row, columns);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                postProcessRawList(rawList, perfectRoll || !supportMinMax);

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

                rawList.writeToRow(row, columns);
            }
        }
    };
    auto commonTypeAll         = [](const TableView::RowView&) -> AttributeFlagSet { return {}; };
    auto commonTypeEquipNoSock = [](const TableView::RowView&) -> AttributeFlagSet { return { AttributeFlag::Durability }; };
    auto start                 = QDateTime::currentMSecsSinceEpoch();
    {
        TableView view(tableSet.tables["uniqueitems"]);
        auto      code2flags = [&code2type, &props](const TableView::RowView& row) -> AttributeFlagSet {
            const auto type = code2type.value(row["code"]);
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
            const auto type = code2type.value(row["item"]);
            assert(props.itemTypeInfo.contains(type));
            return props.itemTypeInfo.at(type).flags;
        };
        fillProps(view, s_descSetItems, commonLvlReq, code2flags, setitemType, false, false);
    }
    if (getWidgetValue("gemsRandom")) {
        TableView view(tableSet.tables["gems"]);
        for (const ColumnsDesc& desc : s_descGems) {
            fillProps(
                view, { desc }, [&miscItemsLevels](const TableView::RowView& row) {
                    return miscItemsLevels.value(row["code"]);
                },
                commonTypeEquipNoSock,
                uniqueType,
                true,
                false);
        }
    }
    if (getWidgetValue("affixRandom")) {
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
    qDebug() << "generate() take:" << (QDateTime::currentMSecsSinceEpoch() - start) << " ms.";
}

}
