/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageItemRandomizer.hpp"
#include "AttributeHelper.hpp"

#include <QBoxLayout>
#include <QLabel>

namespace D2ModGen {

namespace {
constexpr const int s_maxBalanceLevel = 99;
constexpr const int s_maxIngameLevel  = 110;
}

void ConfigPageItemRandomizer::MagicPropBucket::postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock)
{
    if (!replaceSkills && !replaceCharges)
        return;

    for (MagicPropBundle& bundle : bundles) {
        std::vector<MagicProp> newProps;
        for (MagicProp prop : bundle.props) {
            if (replaceSkills && prop.code == "skill") {
                if (prop.par == "132") // Leap
                {
                    newProps.push_back(std::move(prop));
                    continue;
                }
                prop.code = "oskill";
            }
            if (replaceCharges && prop.code == "charged") {
                if (prop.par == "132") // Leap
                {
                    newProps.push_back(std::move(prop));
                    continue;
                }
                prop.code = "oskill";
                if (prop.max.startsWith("-")) {
                    prop.min = prop.max = prop.max.mid(1);
                }
            }
            if (skipKnock && (prop.code == "knock" || prop.code == "howl"))
                continue;
            newProps.push_back(std::move(prop));
        }
        bundle.props = std::move(newProps);
    }
}

void ConfigPageItemRandomizer::MagicPropBucket::sortByLevel()
{
    std::sort(bundles.begin(), bundles.end(), [](const MagicPropBundle& l, const MagicPropBundle& r) { return l.level < r.level; });
    int lastLevel = -1;
    for (int i = 0; i < bundles.size(); ++i) {
        int level = bundles[i].level;
        for (int j = lastLevel; j <= level; j++) {
            lowerLevelBounds[j] = i;
        }
        lastLevel = level + 1;
    }
    for (; lastLevel <= s_maxIngameLevel; ++lastLevel)
        lowerLevelBounds[lastLevel] = bundles.size();
}

std::pair<int, int> ConfigPageItemRandomizer::MagicPropBucket::getBounds(int level, int balance, int minRange) const
{
    if (balance >= s_maxBalanceLevel)
        return { 0, static_cast<int>(bundles.size()) };

    int lowerIndex  = lowerLevelBounds.value(std::clamp(level - balance, 0, s_maxIngameLevel));
    int higherIndex = lowerLevelBounds.value(std::clamp(level + (balance / 2), 0, s_maxIngameLevel));
    if (higherIndex - lowerIndex >= minRange)
        return { lowerIndex, higherIndex };
    lowerIndex = lowerLevelBounds.value(std::clamp(level - balance * 2, 0, s_maxIngameLevel));
    if (higherIndex - lowerIndex >= minRange)
        return { lowerIndex, higherIndex };
    lowerIndex = 0;
    if (higherIndex - lowerIndex >= minRange)
        return { lowerIndex, higherIndex };
    return { 0, static_cast<int>(bundles.size()) };
}

const ConfigPageItemRandomizer::MagicPropBundle& ConfigPageItemRandomizer::MagicPropBucket::getRandomBundle(QRandomGenerator& rng, int level, int balance) const
{
    const auto [lowerBound, upperBound] = getBounds(level, balance, 2);
    const int index                     = rng.bounded(lowerBound, upperBound);
    return bundles[index];
}

void ConfigPageItemRandomizer::MagicPropBucket::addParsedBundle(MagicPropBundle inBundle)
{
    if (inBundle.props.empty())
        return;
    static const QList<QSet<QString>> s_bundledIds{
        QSet<QString>{ "ltng-min", "ltng-max" },
        QSet<QString>{ "fire-min", "fire-max" },
        QSet<QString>{ "cold-min", "cold-max", "cold-len" },
        QSet<QString>{ "pois-min", "pois-max", "pois-len" },
    };
    auto isBundled = [](const QString& code) {
        for (const auto& pack : s_bundledIds)
            if (pack.contains(code))
                return true;
        return false;
    };
    auto consume = [&inBundle, &isBundled, this]() -> bool {
        if (inBundle.props.empty())
            return false;
        auto prop = inBundle.props[0];
        inBundle.removeAt(0);
        MagicPropBundle bundle;
        bundle.level     = prop.level;
        bundle.itemTypes = prop.itemTypes;
        bundle.props.push_back(std::move(prop));
        if (!isBundled(prop.code)) {
            bundles.push_back(std::move(bundle));
            return true;
        }
        for (int i = inBundle.props.size() - 1; i >= 0; --i) {
            auto& prop = inBundle.props[i];
            if (isBundled(prop.code)) {
                bundle.props.push_back(std::move(prop));
                inBundle.removeAt(i);
            }
        }
        bundles.push_back(std::move(bundle));

        return true;
    };
    while (consume()) {
        ;
    }
}

void ConfigPageItemRandomizer::MagicPropSet::addParsedBundle(MagicPropBundle inBundle)
{
    if (inBundle.props.empty())
        return;

    for (auto code : inBundle.itemTypes.include)
        assert(code);

    auto splitByProperty = [this](MagicPropBundle& inBundle, AttributeFlag flag) {
        MagicPropBundle hasProperty;
        hasProperty.level     = inBundle.level;
        hasProperty.itemTypes = inBundle.itemTypes;

        for (int i = inBundle.props.size() - 1; i >= 0; --i) {
            auto&       prop = inBundle.props[i];
            const auto& desc = getAttributeDesc(prop.code);
            if (desc.flags.contains(flag)) {
                hasProperty.props.push_back(std::move(prop));
                inBundle.removeAt(i);
            }
        }
        bucketByType[flag].addParsedBundle(std::move(hasProperty));
    };

    splitByProperty(inBundle, AttributeFlag::Durability);
    splitByProperty(inBundle, AttributeFlag::Quantity);
    splitByProperty(inBundle, AttributeFlag::Sockets);
    splitByProperty(inBundle, AttributeFlag::Missile);

    bucketByType[AttributeFlag::ANY].addParsedBundle(std::move(inBundle));
}

void ConfigPageItemRandomizer::MagicPropSet::postProcess(bool replaceSkills, bool replaceCharges, bool skipKnock)
{
    for (auto&& bucket : bucketByType) {
        bucket.second.postProcess(replaceSkills, replaceCharges, skipKnock);
        bucket.second.sortByLevel();
    }
}

QList<const ConfigPageItemRandomizer::MagicPropBundle*> ConfigPageItemRandomizer::MagicPropSet::getRandomBundles(const AttributeFlagSet& allowedTypes,
                                                                                                         QRandomGenerator&       rng,
                                                                                                         int                     count,
                                                                                                         int                     level,
                                                                                                         int                     balance) const
{
    if (!count)
        return {};

    QList<const ConfigPageItemRandomizer::MagicPropBundle*> result;

    std::vector<BucketRange> branges;
    int                      totalSize = 0;

    for (const auto type : allowedTypes) {
        if (!bucketByType.contains(type))
            continue;
        const auto p          = bucketByType.at(type).getBounds(level, balance, 2);
        const auto lowerBound = p.first;
        const auto upperBound = p.second;
        const auto size       = upperBound - lowerBound;
        if (size <= 0)
            continue;
        branges.push_back(BucketRange{ type, lowerBound, size });
        totalSize += size;
    }
    assert(totalSize > 0);
    for (int i = 0; i < count; ++i) {
        int index = rng.bounded(totalSize);
        for (const BucketRange& range : branges) {
            if (index >= range.size) {
                index -= range.size;
                continue;
            }
            result << &(bucketByType.at(range.type).bundles[index]);
            break;
        }
    }
    assert(!result.isEmpty());
    return result;
}

QList<const ConfigPageItemRandomizer::MagicPropBundle*> ConfigPageItemRandomizer::MagicPropUniverse::getRandomBundles(const AttributeFlagSet& allowedTypes,
                                                                                                              const ItemCodeFilter&   query,
                                                                                                              int                     specificItemUsage,
                                                                                                              QRandomGenerator&       rng,
                                                                                                              int                     count,
                                                                                                              int                     level,
                                                                                                              int                     balance) const
{
    if (!specificItemUsage)
        return all.getRandomBundles(allowedTypes, rng, count, level, balance);

    QList<const ConfigPageItemRandomizer::MagicPropBundle*> result;
    const ItemCodeSet                                   codeSet       = query.include;
    const ItemCode                                      code          = (*codeSet.begin());
    int                                                 specificCount = 0;
    for (int i = 0; i < count; ++i) {
        if (rng.bounded(100) < specificItemUsage)
            specificCount++;
    }
    {
        QList<const ConfigPageItemRandomizer::MagicPropBundle*> resultSpec;
        for (auto code : codeSet) {
            if (!propSetByCode.contains(code))
                continue;
            resultSpec += propSetByCode.at(code).getRandomBundles(allowedTypes, rng, specificCount, level, balance);
        }
        QSet<const ConfigPageItemRandomizer::MagicPropBundle*> resultSpecSet;
        if (!resultSpec.isEmpty()) {
            for (int i = 0; i < specificCount; ++i)
                resultSpecSet << resultSpec[rng.bounded(resultSpec.size())];
        }
        for (auto* bundle : resultSpecSet)
            result << bundle;
    }
    result += all.getRandomBundles(allowedTypes, rng, count - result.size(), level, balance);
    return result;
}

ConfigPageItemRandomizer::ItemCodeSet ConfigPageItemRandomizer::MagicPropUniverse::processQuery(const ItemCodeFilter& query) const
{
    ItemCodeSet codeSet;
    codeSet += query.include;
    for (const auto code : query.include)
        codeSet += itemTypeInfo.at(code).nested;
    codeSet -= query.exclude;
    for (const auto code : query.exclude)
        codeSet -= itemTypeInfo.at(code).nested;
    return codeSet;
}

void ConfigPageItemRandomizer::MagicPropUniverse::fillPropSets()
{
    for (const auto& bucket : all.bucketByType) {
        for (const MagicPropBundle& bundle : bucket.second.bundles) {
            const ItemCodeSet codeSet = processQuery(bundle.itemTypes);

            for (const auto code : codeSet) {
                propSetByCode[code].bucketByType[bucket.first].bundles.push_back(bundle);
            }
        }
    }
}

ConfigPageItemRandomizer::ConfigPageItemRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax("Balance level (lower = more balance, 99=chaos)", "balance", 5, s_maxBalanceLevel, s_maxBalanceLevel, this)
               << new SliderWidgetMinMax("Item type fit percent (0% = all affixes fully random, 100% = all according to item type)", "itemFitPercent", 0, 100, 0, this)
               << new SliderWidgetMinMax("Number of versions of each unique (you will have N different uniques with differnet stats)", "repeat_uniques", 1, 20, 10, this));

    auto* keepCount = new CheckboxWidget("Keep original property count", "keepCount", false, this);
    addEditors(QList<IValueWidget*>() << keepCount);
    auto addMinimax = [this, keepCount](int minValue, int maxValue, int midValue, const QString& minKey, const QString& maxKey, const QString& overallTitle) {
        IValueWidget* minw = new SliderWidgetMinMax("Min", minKey, minValue, maxValue, midValue, true, this);
        IValueWidget* maxw = new SliderWidgetMinMax("Max", maxKey, minValue, maxValue, maxValue, true, this);

        QWidget*     wrapper    = new QWidget(this);
        QHBoxLayout* mainLayout = new QHBoxLayout(wrapper);
        mainLayout->setContentsMargins(0, 5, 0, 5);
        QHBoxLayout* bottom = new QHBoxLayout();
        auto*        l      = new QLabel(overallTitle, this);
        l->setMinimumWidth(110);
        mainLayout->addWidget(l);
        mainLayout->addLayout(bottom);
        bottom->addWidget(minw);
        bottom->addWidget(maxw);

        addWidget(wrapper);
        addEditorsPlain({ minw, maxw });
        connect(keepCount, &CheckboxWidget::toggled, wrapper, &QWidget::setDisabled);
    };
    addMinimax(1, 12, 3, "min_uniq_props", "max_uniq_props", "Unique properties");
    addMinimax(1, 7, 3, "min_rw_props", "max_rw_props", "RW properties");
    addMinimax(1, 19, 3, "min_set_props", "max_set_props", "Set items properties");

    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Always perfect rolls", "perfectRoll", false, this)
               << new CheckboxWidget("Randomize magix/rare affixes", "affixRandom", false, this)
               << new CheckboxWidget("Randomize gem and runes properties", "gemsRandom", false, this)
               << new CheckboxWidget("Replace skills with oskills", "replaceSkills", false, this)
               << new CheckboxWidget("Replace charges with oskills", "replaceCharges", false, this)
               << new CheckboxWidget("Remove Knockback/Monster flee", "removeKnock", false, this));
    closeLayout();
}

KeySet ConfigPageItemRandomizer::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    auto&  tableSet = output.tableSet;
    KeySet result;
    result << "uniqueitems"
           << "runes"
           << "setitems"
           << "sets";

    MagicPropUniverse props;
    {
        TableView view(tableSet.tables["itemtypes"]);
        for (auto& row : view) {
            QString& codeStr = row["Code"];
            if (codeStr.isEmpty())
                continue;
            ItemTypeInfo info;
            if (row["Throwable"] == "1")
                info.flags << AttributeFlag::Quantity;
            else if (row["Repair"] == "1")
                info.flags << AttributeFlag::Durability;

            if (row.hasColumn("MaxSockets3") && row["MaxSockets3"] != "0"
                || row.hasColumn("MaxSock40") && row["MaxSock40"] != "0")
                info.flags << AttributeFlag::Sockets;
            if (!row["Shoots"].isEmpty())
                info.flags << AttributeFlag::Missile;

            const auto parent1 = makeCode(row["Equiv1"]);
            const auto parent2 = makeCode(row["Equiv2"]);
            const auto code    = makeCode(codeStr);
            if (parent1) {
                info.parents << parent1;
                props.itemTypeInfo[parent1].nested << code;
            }
            if (parent2) {
                info.parents << parent2;
                props.itemTypeInfo[parent2].nested << code;
            }

            props.itemTypeInfo[code] = info;
        }
        props.itemTypeInfo[makeCode("SETS")] = {};

        auto expandNested = [&props]() -> bool {
            bool result = false;
            for (auto& typeInfo : props.itemTypeInfo) {
                const auto ncopy = typeInfo.second.nested;
                for (uint32_t nestedCode : ncopy) {
                    typeInfo.second.nested += props.itemTypeInfo[nestedCode].nested;
                }
                const auto pcopy = typeInfo.second.parents;
                for (uint32_t pCode : pcopy) {
                    typeInfo.second.parents += props.itemTypeInfo[pCode].parents;
                }
                result = result || ncopy != typeInfo.second.nested || pcopy != typeInfo.second.parents;
            }
            return result;
        };
        while (expandNested()) {
            ;
        }
        //        for (auto& typeInfo : itemTypeInfo) {
        //            qDebug() << printCode(typeInfo.first) << " -> " << printCodes(typeInfo.second.nested);
        //            qDebug() << printCode(typeInfo.first) << " <- " << printCodes(typeInfo.second.parents);
        //        }
    }

    QMap<QString, ItemCode> code2type;
    for (const auto* tableName : { "armor", "weapons", "misc" }) {
        TableView view(tableSet.tables[tableName]);
        for (auto& row : view) {
            QString& code = row["code"];
            QString& type = row["type"];
            if (!code.isEmpty() && !type.isEmpty()) {
                auto tc         = makeCode(type);
                code2type[code] = tc;
                assert(props.itemTypeInfo.count(tc));
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

    using LevelCallback     = std::function<int(const TableView::RowView& row)>;
    using FlagsCallback     = std::function<AttributeFlagSet(const TableView::RowView& row)>;
    using ItemTypesCallback = std::function<ItemCodeFilter(const TableView::RowView& row)>;
    auto grabProps          = [&props](TableView&               view,
                              const ColumnsDesc&       columns,
                              const LevelCallback&     levelCb,
                              const ItemTypesCallback& typeCb) {
        for (auto& row : view) {
            const int level = levelCb(row);
            if (level <= 0)
                continue;
            const auto itemTypes = typeCb(row);

            MagicPropBundle bundle;

            for (const auto& col : columns.m_cols) {
                MagicProp mp;
                mp.code      = row[col.code];
                mp.par       = row[col.par];
                mp.min       = row[col.min];
                mp.max       = row[col.max];
                mp.level     = level;
                mp.itemTypes = itemTypes;
                if (mp.code.isEmpty())
                    break;
                if (isUnusedAttribute(mp.code))
                    continue;
                bundle.props.push_back(std::move(mp));
            }
            props.all.addParsedBundle(std::move(bundle));
        }
    };
    auto commonLvlReq = [](const TableView::RowView& row) { return row["lvl"].toInt(); };
    auto commonRWreq  = [&determineRWlevel](const TableView::RowView& row) {
        return determineRWlevel({ row["Rune1"], row["Rune2"], row["Rune3"], row["Rune4"], row["Rune5"], row["Rune6"] });
    };
    auto commonSetReq = [&setLevels](const TableView::RowView& row) {
        return setLevels.value(row["name"]);
    };
    auto commonTypeAll = [](const TableView::RowView&) -> AttributeFlagSet {
        return { AttributeFlag::ANY };
    };
    auto commonTypeEquipNoSock = [](const TableView::RowView&) -> AttributeFlagSet {
        return { AttributeFlag::ANY, AttributeFlag::Durability };
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
            auto tc = makeCode(itype);
            assert(tc);
            result << tc;
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
            auto tc = makeCode(itype);
            assert(tc);
            result.include += tc;
        }
        for (int i = 1; i <= 5; ++i) {
            const auto col = QString("etype%1").arg(i);
            if (!row.hasColumn(col))
                break;
            const QString& itype = row[col];
            if (itype.isEmpty())
                break;
            auto tc = makeCode(itype);
            assert(tc);
            result.exclude += tc;
        }
        return result;
    };
    auto setsTypes = [](const TableView::RowView& row) -> ItemCodeFilter { return { { makeCode("SETS") }, {} }; };

    {
        auto&     table = tableSet.tables["uniqueitems"];
        TableView view(table);
        grabProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12), commonLvlReq, uniqueType);
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
        grabProps(view, ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7), commonRWreq, rwTypes);
    }
    {
        TableView view(tableSet.tables["setitems"]);
        grabProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9), commonLvlReq, setitemType);
        grabProps(view, ColumnsDesc("aprop%1a", "apar%1a", "amin%1a", "amax%1a", 5), commonLvlReq, setitemType);
        grabProps(view, ColumnsDesc("aprop%1b", "apar%1b", "amin%1b", "amax%1b", 5), commonLvlReq, setitemType);
        for (auto& row : view) {
            QString& setId   = row["set"];
            QString& lvl     = row["lvl"];
            setLevels[setId] = lvl.toInt();
        }
    }
    {
        TableView view(tableSet.tables["gems"]);
        for (QString prefix : QStringList{ "weaponMod", "helmMod", "shieldMod" }) {
            const ColumnsDesc desc(prefix + "%1Code", prefix + "%1Param", prefix + "%1Min", prefix + "%1Max", 3);
            grabProps(
                view, desc, [&miscItemsLevels](const TableView::RowView& row) {
                    return miscItemsLevels.value(row["code"]);
                },
                uniqueType);
        }
    }
    {
        for (const char* table : { "magicprefix", "magicsuffix" }) {
            TableView         view(tableSet.tables[table]);
            const ColumnsDesc desc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3);
            grabProps(
                view, desc, [](const TableView::RowView& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                affixTypes);
        }
    }
    {
        TableView view(tableSet.tables["sets"]);

        grabProps(view, ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2), commonSetReq, setsTypes);
        grabProps(view, ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2), commonSetReq, setsTypes);
        grabProps(view, ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8), commonSetReq, setsTypes);
    }
    props.all.postProcess(getWidgetValue("replaceSkills"), getWidgetValue("replaceCharges"), getWidgetValue("removeKnock"));

    const int  balance        = getWidgetValue("balance");
    const int  itemFitPercent = getWidgetValue("itemFitPercent");
    const bool perfectRoll    = getWidgetValue("perfectRoll");
    const bool keepCount      = getWidgetValue("keepCount");
    if (itemFitPercent)
        props.fillPropSets();

    auto fillProps = [&props, &rng, balance, keepCount](TableView&               view,
                                                        const ColumnsDesc&       columns,
                                                        const LevelCallback&     levelCb,
                                                        const FlagsCallback&     flagsCb,
                                                        const ItemTypesCallback& typesCb,
                                                        const int                minProps,
                                                        const int                maxProps,
                                                        const int                itemFitPercent,
                                                        const bool               isPerfect,
                                                        const bool               commonSkip) {
        for (auto& row : view) {
            QString& firstPar = row[columns.m_cols[0].code];
            if (commonSkip && firstPar.isEmpty())
                continue;

            const int level = levelCb(row);
            if (level <= 0) {
                continue;
            }
            int originalCount = 0;
            for (const auto& col : columns.m_cols) {
                if (row[col.code].isEmpty())
                    break;
                originalCount++;
            }
            const auto allowedTypes = flagsCb(row);
            const auto filter       = typesCb(row);
            const int  newCnt       = keepCount ? originalCount : rng.bounded(minProps, maxProps + 1);
            const auto bundles      = props.getRandomBundles(allowedTypes, filter, itemFitPercent, rng, newCnt, level, balance);

            int col = 0;
            for (auto* bundle : bundles) {
                for (int j = 0; j < bundle->props.size(); ++j) {
                    if (col >= columns.m_cols.size())
                        break;
                    const auto& colDesc = columns.m_cols[col];
                    const auto& prop    = bundle->props[j];
                    auto&       code    = row[colDesc.code];
                    auto&       par     = row[colDesc.par];
                    auto&       min     = row[colDesc.min];
                    auto&       max     = row[colDesc.max];
                    code                = prop.code;
                    par                 = prop.par;
                    min                 = isPerfect && isMinMaxRange(code) ? prop.max : prop.min;
                    max                 = prop.max;
                    col++;
                }
            }
            for (; col < columns.m_cols.size(); ++col) {
                const auto& colDesc = columns.m_cols[col];
                row[colDesc.code]   = "";
                row[colDesc.par]    = "";
                row[colDesc.min]    = "";
                row[colDesc.max]    = "";
            }
        }
    };

    {
        const int minProps = getWidgetValue("min_uniq_props");
        const int maxProps = std::max(minProps, getWidgetValue("max_uniq_props"));
        TableView view(tableSet.tables["uniqueitems"]);
        auto      code2flags = [&code2type, &props](const TableView::RowView& row) -> AttributeFlagSet {
            AttributeFlagSet result{ AttributeFlag::ANY };
            const auto       type = code2type.value(row["code"]);
            assert(props.itemTypeInfo.contains(type));
            const ItemTypeInfo& info = props.itemTypeInfo.at(type);
            result += info.flags;
            return result;
        };
        fillProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12), commonLvlReq, code2flags, uniqueType, minProps, maxProps, itemFitPercent, perfectRoll, true);
    }
    {
        const int minProps = getWidgetValue("min_rw_props");
        const int maxProps = std::max(minProps, getWidgetValue("max_rw_props"));
        TableView view(tableSet.tables["runes"]);
        fillProps(view, ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7), commonRWreq, commonTypeEquipNoSock, rwTypes, minProps, maxProps, itemFitPercent, perfectRoll, true);
    }
    {
        const int minProps = getWidgetValue("min_set_props");
        const int maxProps = std::max(minProps, getWidgetValue("max_set_props"));
        TableView view(tableSet.tables["setitems"]);
        auto      code2flags = [&code2type, &props](const TableView::RowView& row) -> AttributeFlagSet {
            AttributeFlagSet result{ AttributeFlag::ANY };
            const auto       type = code2type.value(row["item"]);
            assert(props.itemTypeInfo.contains(type));
            const ItemTypeInfo& info = props.itemTypeInfo.at(type);
            result += info.flags;
            return result;
        };
        fillProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9), commonLvlReq, code2flags, setitemType, minProps / 2, maxProps / 2, itemFitPercent, perfectRoll, false);
        fillProps(view, ColumnsDesc("aprop%1a", "apar%1a", "amin%1a", "amax%1a", 5), commonLvlReq, code2flags, setitemType, (minProps / 4) + 1, (maxProps / 4) + 1, itemFitPercent, perfectRoll, false);
        fillProps(view, ColumnsDesc("aprop%1b", "apar%1b", "amin%1b", "amax%1b", 5), commonLvlReq, code2flags, setitemType, (minProps / 4) + 1, (maxProps / 4) + 1, itemFitPercent, perfectRoll, false);
    }
    if (getWidgetValue("gemsRandom")) {
        const int minProps = 1;
        const int maxProps = 3;
        TableView view(tableSet.tables["gems"]);

        static const QStringList s_prefixes{ "weaponMod", "helmMod", "shieldMod" };
        for (const QString& prefix : s_prefixes) {
            const ColumnsDesc desc(prefix + "%1Code", prefix + "%1Param", prefix + "%1Min", prefix + "%1Max", 3);
            fillProps(
                view, desc, [&miscItemsLevels](const TableView::RowView& row) {
                    return miscItemsLevels.value(row["code"]);
                },
                commonTypeEquipNoSock,
                uniqueType,
                minProps,
                maxProps,
                0, // @todo: itemFitPercent
                true,
                true);
        }
        result << "gems";
    }
    if (getWidgetValue("affixRandom")) {
        const int minProps = 1;
        const int maxProps = 3;
        for (const char* table : { "magicprefix", "magicsuffix" }) {
            TableView         view(tableSet.tables[table]);
            const ColumnsDesc desc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3);
            fillProps(
                view, desc, [&miscItemsLevels](const TableView::RowView& row) {
                    return row["spawnable"] == "1" ? row["level"].toInt() : 0; // utilize maxLevel ?
                },
                commonTypeAll,
                affixTypes,
                minProps,
                maxProps,
                itemFitPercent,
                perfectRoll,
                true);
        }
        result << "magicprefix"
               << "magicsuffix";
    }
    {
        TableView view(tableSet.tables["sets"]);

        fillProps(view, ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2), commonSetReq, commonTypeAll, setsTypes, 1, 5, itemFitPercent, true, false);
        fillProps(view, ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2), commonSetReq, commonTypeAll, setsTypes, 1, 5, itemFitPercent, true, false);
        fillProps(view, ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8), commonSetReq, commonTypeAll, setsTypes, 3, 8, itemFitPercent, true, false);
    }
    return result;
}

}
