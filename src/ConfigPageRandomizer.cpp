/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageRandomizer.hpp"

#include <QBoxLayout>
#include <QLabel>

namespace {
constexpr const int s_maxBalanceLevel = 99;
constexpr const int s_maxIngameLevel  = 110;
}

void ConfigPageRandomizer::MagicPropBucket::postProcess(bool replaceSkills, bool replaceCharges)
{
    if (!replaceSkills && !replaceCharges)
        return;

    for (MagicPropBundle& bundle : bundles) {
        for (MagicProp& prop : bundle.props) {
            if (prop.code == "skill") {
                if (prop.par == "132")
                    continue;
                prop.code = "oskill";
            }
            if (prop.code == "charged") {
                if (prop.par == "132")
                    continue;
                prop.code = "oskill";
                if (prop.max.startsWith("-")) {
                    prop.min = prop.max = prop.max.mid(1);
                }
            }
        }
    }
}

void ConfigPageRandomizer::MagicPropBucket::sortByLevel()
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

std::pair<int, int> ConfigPageRandomizer::MagicPropBucket::getBounds(int level, int balance, int minRange) const
{
    if (balance >= s_maxBalanceLevel)
        return { 0, bundles.size() };

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
    return { 0, bundles.size() };
}

const ConfigPageRandomizer::MagicPropBundle& ConfigPageRandomizer::MagicPropBucket::getRandomBundle(QRandomGenerator& rng, int level, int balance) const
{
    const auto [lowerBound, upperBound] = getBounds(level, balance, 10);
    const int index                     = rng.bounded(lowerBound, upperBound);
    return bundles[index];
}

void ConfigPageRandomizer::MagicPropBucket::addParsedBundle(MagicPropBundle inBundle)
{
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
        auto            prop = inBundle.props.takeFirst();
        MagicPropBundle bundle;
        bundle.props << prop;
        bundle.level = prop.level;
        if (!isBundled(prop.code)) {
            bundles << bundle;
            return true;
        }
        for (int i = inBundle.props.size() - 1; i >= 0; --i) {
            const auto& prop = inBundle.props[i];
            if (isBundled(prop.code)) {
                bundle.props << prop;
                inBundle.props.removeAt(i);
            }
        }
        bundles << bundle;

        return true;
    };
    while (consume()) {
        ;
    }
}

ConfigPageRandomizer::ConfigPageRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Enable Item Randomizer", "enable", false, this)
               << new SliderWidgetMinMax("Balance level (lower = more balance)", "balance", 5, s_maxBalanceLevel, s_maxBalanceLevel, this)
               << new SliderWidgetMinMax("Number of versions of each unique", "repeat_uniques", 1, 20, 10, this));

    auto addMinimax = [this](int minValue, int maxValue, int midValue, const QString& minKey, const QString& maxKey, const QString& overallTitle) {
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
    };
    addMinimax(1, 12, 3, "min_uniq_props", "max_uniq_props", "Unique properties");
    addMinimax(1, 7, 3, "min_rw_props", "max_rw_props", "RW properties");
    addMinimax(1, 9, 3, "min_set_props", "max_set_props", "Set items properties");

    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Always perfect rolls", "perfectRoll", false, this)
               << new CheckboxWidget("Randomize magix/rare affixes", "affixRandom", false, this)
               << new CheckboxWidget("Randomize gem and runes properties", "gemsRandom", false, this)
               << new CheckboxWidget("Replace skills with oskills", "replaceSkills", false, this)
               << new CheckboxWidget("Replace charges with oskills", "replaceCharges", false, this));
    closeLayout();
}

KeySet ConfigPageRandomizer::generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (!getWidgetValue("enable"))
        return {};

    auto&  tableSet = output.tableSet;
    KeySet result;
    result << "uniqueitems"
           << "runes"
           << "setitems"
           << "sets";

    QMap<QString, QString> code2type;
    for (const auto* tableName : { "armor", "weapons", "misc" }) {
        TableView view(tableSet.tables[tableName]);
        for (auto& row : view) {
            QString& code = row["code"];
            QString& type = row["type"];
            if (!code.isEmpty() && !type.isEmpty())
                code2type[code] = type;
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

    MagicPropSet all;
    {
        using LevelCallback = std::function<int(const TableView::RowView& row)>;
        using TypeCallback  = std::function<QString(const TableView::RowView& row)>;
        auto grabProps      = [&all, &code2type](TableView&           view,
                                            const ColumnsDesc&   columns,
                                            const LevelCallback& levelCb) {
            static const QSet<QString> s_ignoredCodes{ "ac/time" }; // unused and buggy.
            for (auto& row : view) {
                const int level = levelCb(row);

                MagicPropBundle bundle;

                for (const auto& col : columns.m_cols) {
                    MagicProp mp;
                    mp.code  = row[col.code];
                    mp.par   = row[col.par];
                    mp.min   = row[col.min];
                    mp.max   = row[col.max];
                    mp.level = level;
                    if (mp.code.isEmpty())
                        break;
                    if (s_ignoredCodes.contains(mp.code))
                        continue;
                    bundle.props << mp;
                }
                if (!bundle.props.isEmpty())
                    all.bucketByType[MagicPropSet::s_all].addParsedBundle(bundle);
            }
        };
        auto commonLvlReq = [](const TableView::RowView& row) { return row["lvl"].toInt(); };
        auto commonRWreq  = [&determineRWlevel](const TableView::RowView& row) {
            return determineRWlevel({ row["Rune1"], row["Rune2"], row["Rune3"], row["Rune4"], row["Rune5"], row["Rune6"] });
        };
        auto commonSetReq = [&setLevels](const TableView::RowView& row) {
            return setLevels.value(row["name"]);
        };
        {
            auto&     table = tableSet.tables["uniqueitems"];
            TableView view(table);
            grabProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12), commonLvlReq);
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
            grabProps(view, ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7), commonRWreq);
        }
        {
            TableView view(tableSet.tables["setitems"]);
            grabProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9), commonLvlReq);
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
                    });
            }
        }
        {
            for (const char* table : { "magicprefix", "magicsuffix" }) {
                TableView         view(tableSet.tables[table]);
                const ColumnsDesc desc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3);
                grabProps(
                    view, desc, [](const TableView::RowView& row) {
                        return row["level"].toInt(); // utilize maxLevel ?
                    });
            }
        }
        {
            TableView view(tableSet.tables["sets"]);

            grabProps(view, ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2), commonSetReq);
            grabProps(view, ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2), commonSetReq);
            grabProps(view, ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8), commonSetReq);
        }
        all.bucketByType[MagicPropSet::s_all].postProcess(getWidgetValue("replaceSkills"), getWidgetValue("replaceCharges"));
        all.bucketByType[MagicPropSet::s_all].sortByLevel();

        const int  balance     = getWidgetValue("balance");
        const bool perfectRoll = getWidgetValue("perfectRoll");
        auto       fillProps   = [&all, &code2type, &rng, balance](TableView&           view,
                                                           const ColumnsDesc&   columns,
                                                           const LevelCallback& levelCb,
                                                           const int            minProps,
                                                           const int            maxProps,
                                                           const bool           isPerfect) {
            const MagicPropBucket& bucket = all.bucketByType[MagicPropSet::s_all];
            for (auto& row : view) {
                QString& firstPar = row[columns.m_cols[0].code];
                if (firstPar.isEmpty())
                    continue;

                const int level = levelCb(row);
                if (level <= 0) {
                    continue;
                }

                const int newCnt = rng.bounded(minProps, maxProps + 1);

                int col = 0;
                for (int i = 1; i <= newCnt; ++i) {
                    const auto& bundle = bucket.getRandomBundle(rng, level, balance);
                    for (int j = 0; j < bundle.props.size(); ++j) {
                        if (col >= columns.m_cols.size())
                            break;
                        const auto& colDesc = columns.m_cols[col];
                        const auto& prop    = bundle.props[j];
                        auto&       code    = row[colDesc.code];
                        auto&       par     = row[colDesc.par];
                        auto&       min     = row[colDesc.min];
                        auto&       max     = row[colDesc.max];
                        code                = prop.code;
                        par                 = prop.par;
                        min                 = isPerfect && !s_ignoreMinMax.contains(code) ? prop.max : prop.min;
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
            fillProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12), commonLvlReq, minProps, maxProps, perfectRoll);
        }
        {
            const int minProps = getWidgetValue("min_rw_props");
            const int maxProps = std::max(minProps, getWidgetValue("max_rw_props"));
            TableView view(tableSet.tables["runes"]);
            fillProps(view, ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7), commonRWreq, minProps, maxProps, perfectRoll);
        }
        {
            const int minProps = getWidgetValue("min_set_props");
            const int maxProps = std::max(minProps, getWidgetValue("max_set_props"));
            TableView view(tableSet.tables["setitems"]);
            fillProps(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9), commonLvlReq, minProps, maxProps, perfectRoll);
        }
        if (getWidgetValue("gemsRandom")) {
            const int minProps = 1;
            const int maxProps = 3;
            TableView view(tableSet.tables["gems"]);
            for (QString prefix : QStringList{ "weaponMod", "helmMod", "shieldMod" }) {
                const ColumnsDesc desc(prefix + "%1Code", prefix + "%1Param", prefix + "%1Min", prefix + "%1Max", 3);
                fillProps(
                    view, desc, [&miscItemsLevels](const TableView::RowView& row) {
                        return miscItemsLevels.value(row["code"]);
                    },
                    minProps,
                    maxProps,
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
                        return row["level"].toInt(); // utilize maxLevel ?
                    },
                    minProps,
                    maxProps,
                    perfectRoll);
            }
            result << "magicprefix"
                   << "magicsuffix";
        }
        {
            TableView view(tableSet.tables["sets"]);

            fillProps(view, ColumnsDesc("PCode%1a", "PParam%1a", "PMin%1a", "PMax%1a", 5, 2), commonSetReq, 1, 5, true);
            fillProps(view, ColumnsDesc("PCode%1b", "PParam%1b", "PMin%1b", "PMax%1b", 5, 2), commonSetReq, 1, 5, true);
            fillProps(view, ColumnsDesc("FCode%1", "FParam%1", "FMin%1", "FMax%1", 8), commonSetReq, 3, 8, true);
        }
        return result;
    }
}

const QString ConfigPageRandomizer::MagicPropSet::s_all = "all";
