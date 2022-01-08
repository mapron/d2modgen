/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"
#include "SliderWidget.hpp"
#include "CheckboxWidget.hpp"

#include <QVBoxLayout>

#include <functional>

namespace {

class AbstractPage : public IConfigPage {
public:
    AbstractPage(QWidget* parent)
        : IConfigPage(parent)
    {
        m_layout = new QVBoxLayout(this);
    }

    // IConfigPage interface
public:
    void readSettings(const QJsonObject& data) override
    {
        for (QString key : m_editors.keys()) {
            auto* w = m_editors[key];
            if (data.contains(key))
                w->setValue(data[key].toInt());
            else
                w->resetValue();
        }
    }
    void writeSettings(QJsonObject& data) const override
    {
        for (QString key : m_editors.keys()) {
            auto* w = m_editors[key];
            if (!w->isDefault())
                data[key] = w->getValue();
        }
    }
    bool isAllDefault() const
    {
        for (QString key : m_editors.keys()) {
            auto* w = m_editors[key];
            if (!w->isDefault())
                return false;
        }
        return true;
    }
    bool isAllDefault(const QStringList& keys) const
    {
        for (const QString& key : keys) {
            auto* w = m_editors[key];
            if (!w->isDefault())
                return false;
        }
        return true;
    }

protected:
    void addEditors(QList<IValueWidget*> editors)
    {
        for (IValueWidget* w : editors) {
            m_layout->addWidget(w);
            m_editors[w->objectName()] = w;
        }
    }
    void closeLayout()
    {
        m_layout->addStretch();
    }
    int getWidgetValue(const QString& id) const
    {
        return m_editors[id]->getValue();
    }
    bool isWidgetValueDefault(const QString& id) const
    {
        return m_editors[id]->isDefault();
    }

private:
    QMap<QString, IValueWidget*> m_editors;
    QVBoxLayout*                 m_layout;
};

class DifficultyPage : public AbstractPage {
public:
    DifficultyPage(QWidget* parent)
        : AbstractPage(parent)
    {
        addEditors(QList<IValueWidget*>()
                   << new SliderWidget("Monster Attack Rating", "mon_ar", 10, 10, this)
                   << new SliderWidget("Monster Defense", "mon_def", 10, 10, this)
                   << new SliderWidget("Monster HP", "mon_hp", 10, 10, this)
                   << new SliderWidget("Monster Damage", "mon_dam", 10, 10, this)
                   << new SliderWidget("Monster XP", "mon_xp", 10, 10, this)
                   << new SliderWidget("Density", "density", 2, 20, this)
                   << new SliderWidget("Boss packs", "packs", 2, 50, this)
                   << new CheckboxWidget("Use Hell pack count everywhere", "hellPacks", false, this)
                   << new SliderWidgetMinMax("Increase area level (Nightmare and Hell)", "levelIncrease", 0, 20, 0, this));
        closeLayout();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Difficulty";
    }
    QString settingKey() const override
    {
        return "difficulty";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override
    {
        KeySet result;
        if (!isAllDefault({ "mon_ar", "mon_def", "mon_hp", "mon_dam", "mon_xp" })) {
            result << "monlvl";
            TableView view(tableSet.tables["monlvl"]);
            auto      proceedMonParam = [&view, this](QString key, QStringList cols) {
                if (isWidgetValueDefault(key))
                    return;
                const int percent = getWidgetValue(key);
                for (auto& row : view) {
                    if (row["Level"] == "0")
                        continue;
                    for (auto& col : cols) {
                        QString& value = row[col];
                        value          = QString("%1").arg(value.toInt() * percent / 100);
                    }
                }
            };

            proceedMonParam("mon_ar", { "TH", "TH(N)", "TH(H)", "L-TH", "L-TH(N)", "L-TH(H)" });
            proceedMonParam("mon_def", { "AC", "AC(N)", "AC(H)", "L-AC", "L-AC(N)", "L-AC(H)" });
            proceedMonParam("mon_hp", { "HP", "HP(N)", "HP(H)", "L-HP", "L-HP(N)", "L-HP(H)" });
            proceedMonParam("mon_dam", { "DM", "DM(N)", "DM(H)", "L-DM", "L-DM(N)", "L-DM(H)" });
            proceedMonParam("mon_xp", { "XP", "XP(N)", "XP(H)", "L-XP", "L-XP(N)", "L-XP(H)" });
        }
        if (!isAllDefault({ "density", "packs", "hellPacks", "levelIncrease" })) {
            result << "levels";
            TableView view(tableSet.tables["levels"]);
            auto      isEmptyCell = [](const QString& value) {
                return value.isEmpty() || value == "0";
            };
            auto allCellsNonEmpty = [&isEmptyCell](const QStringList& values) -> bool {
                for (const QString& value : values)
                    if (isEmptyCell(value))
                        return false;
                return true;
            };
            const int density       = getWidgetValue("density");
            const int levelIncrease = getWidgetValue("levelIncrease");
            auto      densityAdjust = [density](QString& value) {
                const int prev = value.toInt();
                const int next = prev * density / 100;
                value          = QString("%1").arg(std::clamp(next, 100, 9900));
            };
            const int packs       = getWidgetValue("packs");
            auto      packsAdjust = [packs](QString& value) {
                const int prev = value.toInt();
                const int next = prev * packs / 100;
                value          = QString("%1").arg(std::min(next, 255));
            };
            for (auto& row : view) {
                QString& normMin   = row["MonUMin"];
                QString& normMax   = row["MonUMax"];
                QString& nighMin   = row["MonUMin(N)"];
                QString& nighMax   = row["MonUMax(N)"];
                QString& hellMin   = row["MonUMin(H)"];
                QString& hellMax   = row["MonUMax(H)"];
                QString& nighLevel = row["MonLvlEx(N)"];
                QString& hellLevel = row["MonLvlEx(H)"];

                if (allCellsNonEmpty({ hellMin, hellMax })) {
                    if (getWidgetValue("hellPacks")) {
                        normMin = nighMin = hellMin;
                        normMax = nighMax = hellMax;
                    }
                }

                if (allCellsNonEmpty({ normMin, normMax, nighMin, nighMax, hellMin, hellMax })) {
                    if (!isWidgetValueDefault("packs")) {
                        packsAdjust(normMin);
                        packsAdjust(normMax);
                        packsAdjust(nighMin);
                        packsAdjust(nighMax);
                        packsAdjust(hellMin);
                        packsAdjust(hellMax);
                    }
                }
                if (!isWidgetValueDefault("density")) {
                    QString& normDen = row["MonDen"];
                    QString& nighDen = row["MonDen(N)"];
                    QString& hellDen = row["MonDen(H)"];
                    densityAdjust(normDen);
                    densityAdjust(nighDen);
                    densityAdjust(hellDen);
                }
                if (levelIncrease) {
                    for (QString* lev : { &nighLevel, &hellLevel }) {
                        int level = lev->toInt();
                        if (!level || level > 85)
                            continue;
                        *lev = QString("%1").arg(std::min(85, level + levelIncrease));
                    }
                }
            }
        }
        return result;
    }
};

class DropsPage : public AbstractPage {
public:
    DropsPage(QWidget* parent)
        : AbstractPage(parent)
    {
        addEditors(QList<IValueWidget*>()
                   << new SliderWidgetMinMax("Increase Unique Chance", "chance_uni", 1, 50, 1, this)
                   << new SliderWidgetMinMax("Increase Set Chance", "chance_set", 1, 30, 1, this)
                   << new SliderWidgetMinMax("Increase Rare Chance", "chance_rare", 1, 15, 1, this)
                   << new SliderWidgetMinMax("NoDrop reduce % (higher=more drops)", "nodrop_factor", 1, 10, 1, this));
        closeLayout();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Drops";
    }
    QString settingKey() const override
    {
        return "drops";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override
    {
        if (isAllDefault({ "chance_uni", "chance_set", "chance_rare", "nodrop_factor" }))
            return {};
        static const QSet<int>     s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
        static const QSet<QString> s_modifyNames{
            "Cow",
            "Cow (N)",
            "Cow (H)",
        };

        {
            TableView view(tableSet.tables["treasureclassex"]);

            const int factorUnique = getWidgetValue("chance_uni");
            const int factorSet    = getWidgetValue("chance_set");
            const int factorRare   = getWidgetValue("chance_rare");
            const int factorNo     = getWidgetValue("nodrop_factor");
            auto      factorAdjust = [](QString& value, double factor, int maxFact) {
                const double prev           = value.toInt();
                const double probReverseOld = (1024. - prev);
                const double probReverseNew = probReverseOld / factor;
                const double probNew        = (1024. - probReverseNew);

                const int next = static_cast<int>(probNew);
                value          = QString("%1").arg(std::min(next, maxFact));
            };
            auto nodropAdjust = [factorNo](QString& value, double totalOther) {
                const double oldNoDrop      = value.toInt();
                const double oldNoDropRatio = oldNoDrop / (oldNoDrop + totalOther);
                const double newNoDropRatio = oldNoDropRatio / factorNo;
                const double newNoDrop      = totalOther * newNoDropRatio / (1. - newNoDropRatio);
                const int    next           = static_cast<int>(newNoDrop);
                value                       = QString("%1").arg(std::max(next, 1));
            };

            for (auto& row : view) {
                QString&   treasureGroup      = row["group"];
                QString&   className          = row["Treasure Class"];
                QString&   uniqueRatio        = row["Unique"];
                QString&   setRatio           = row["Set"];
                QString&   rareRatio          = row["Rare"];
                const bool allowFillNewValues = !treasureGroup.isEmpty() && s_modifyGroups.contains(treasureGroup.toInt()) || s_modifyNames.contains(className);
                const bool allowModifyValues  = !uniqueRatio.isEmpty() && uniqueRatio != "1024" && !setRatio.isEmpty();
                if (allowFillNewValues || allowModifyValues) {
                    // these limits are empyrical - to prevent 100% drop chance on 1000% MF.
                    factorAdjust(uniqueRatio, factorUnique, 1010);
                    factorAdjust(setRatio, factorSet, 990);
                    factorAdjust(rareRatio, factorRare, 960);
                }
                QString& noDrop = row["NoDrop"];
                if (!noDrop.isEmpty() && noDrop != "0") {
                    int totalOther = 0;
                    for (int i = 1; i <= 10; ++i) {
                        QString& prob = row[QString("Prob%1").arg(i)];
                        if (prob.isEmpty())
                            break;
                        totalOther += prob.toInt();
                    }
                    nodropAdjust(noDrop, totalOther);
                }
            }
        }
        return { "treasureclassex" };
    }
};

class RandomizerPage : public AbstractPage {
public:
    RandomizerPage(QWidget* parent)
        : AbstractPage(parent)
    {
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget("Enable Item Randomizer", "enable", false, this)
                   << new SliderWidgetMinMax("Balance level (lower = more balance)", "balance", 5, 99, 99, this)
                   << new SliderWidgetMinMax("Minimum Unique properties", "min_uniq_props", 1, 12, 3, this)
                   << new SliderWidgetMinMax("Maximum Unique properties", "max_uniq_props", 1, 12, 12, this)
                   << new SliderWidgetMinMax("Minimum RW properties", "min_rw_props", 1, 7, 3, this)
                   << new SliderWidgetMinMax("Maximum RW properties", "max_rw_props", 1, 7, 7, this)
                   << new SliderWidgetMinMax("Minimum Set items properties", "min_set_props", 1, 9, 3, this)
                   << new SliderWidgetMinMax("Maximum Set items properties", "max_set_props", 1, 9, 9, this)
                   << new CheckboxWidget("Always perfect rolls", "perfectRoll", false, this)
                   << new CheckboxWidget("Randomize magix/rare affixes", "affixRandom", false, this));
        closeLayout();
    }

    struct MagicProp {
        QString code;
        QString par;
        QString min;
        QString max;
        int     level = 0;
    };

    struct MagicPropBundle {
        QList<MagicProp> props;
        int              level = 0;
    };

    struct MagicPropBucket {
        QList<MagicPropBundle> bundles;
        QMap<int, int>         lowerLevelBounds;

        void sortByLevel()
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
            for (; lastLevel <= 110; ++lastLevel)
                lowerLevelBounds[lastLevel] = bundles.size();
        }

        std::pair<int, int> getBounds(int level, int balance, int minRange) const
        {
            int lowerIndex  = lowerLevelBounds.value(std::clamp(level - balance, 0, 110));
            int higherIndex = lowerLevelBounds.value(std::clamp(level + (balance / 2), 0, 110));
            if (higherIndex - lowerIndex >= minRange)
                return { lowerIndex, higherIndex };
            lowerIndex = lowerLevelBounds.value(std::clamp(level - balance * 2, 0, 110));
            if (higherIndex - lowerIndex >= minRange)
                return { lowerIndex, higherIndex };
            lowerIndex = 0;
            if (higherIndex - lowerIndex >= minRange)
                return { lowerIndex, higherIndex };
            return { 0, bundles.size() };
        }

        const MagicPropBundle& getRandomBundle(QRandomGenerator& rng, int level, int balance) const
        {
            const auto [lowerBound, upperBound] = getBounds(level, balance, 10);
            const int index                     = rng.bounded(lowerBound, upperBound);
            return bundles[index];
        }

        void addParsedBundle(MagicPropBundle inBundle)
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
    };

    struct MagicPropSet {
        // @todo: do we really need to handle item-specific properties (knockback, replinish quantity)?
        static const QString           s_all;
        QMap<QString, MagicPropBucket> bucketByType;
    };

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Item Randomizer";
    }
    QString settingKey() const override
    {
        return "randomizer";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override
    {
        if (!getWidgetValue("enable"))
            return {};
        KeySet result;
        result << "uniqueitems"
               << "runes"
               << "gems"
               << "setitems";

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
                if (!code.isEmpty())
                    miscItemsLevels[code] = levelreq.toInt();
            }
        }
        auto determineRWlevel = [&miscItemsLevels](const QStringList& runes) {
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
                                                const QString&       codeTpl,
                                                const QString&       parTpl,
                                                const QString&       minTpl,
                                                const QString&       maxTpl,
                                                const LevelCallback& levelCb,
                                                const int            columns) {
                for (auto& row : view) {
                    const int level = levelCb(row);

                    MagicPropBundle bundle;

                    for (int i = 1; i <= columns; ++i) {
                        MagicProp mp;
                        mp.code  = row[codeTpl.arg(i)];
                        mp.par   = row[parTpl.arg(i)];
                        mp.min   = row[minTpl.arg(i)];
                        mp.max   = row[maxTpl.arg(i)];
                        mp.level = level;
                        if (mp.code.isEmpty())
                            break;
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
            {
                TableView view(tableSet.tables["uniqueitems"]);
                grabProps(view, "prop%1", "par%1", "min%1", "max%1", commonLvlReq, 12);
            }
            {
                TableView view(tableSet.tables["runes"]);
                grabProps(view, "T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", commonRWreq, 7);
            }
            {
                TableView view(tableSet.tables["setitems"]);
                grabProps(view, "prop%1", "par%1", "min%1", "max%1", commonLvlReq, 9);
            }
            {
                TableView view(tableSet.tables["gems"]);
                for (QString prefix : QStringList{ "weaponMod", "helmMod", "shieldMod" }) {
                    grabProps(
                        view, prefix + "%1Code", prefix + "%1Param", prefix + "%1Min", prefix + "%1Max", [&miscItemsLevels](const TableView::RowView& row) {
                            return miscItemsLevels.value(row["code"]);
                        },
                        3);
                }
            }
            {
                for (const char* table : { "magicprefix", "magicsuffix" }) {
                    TableView view(tableSet.tables[table]);
                    grabProps(
                        view, "mod%1code", "mod%1param", "mod%1min", "mod%1max", [&miscItemsLevels](const TableView::RowView& row) {
                            return row["level"].toInt(); // utilize maxLevel ?
                        },
                        3);
                }
            }
            all.bucketByType[MagicPropSet::s_all].sortByLevel();

            const int  balance     = getWidgetValue("balance");
            const bool perfectRoll = getWidgetValue("perfectRoll");
            auto       fillProps   = [&all, &code2type, &rng, balance](TableView& view,

                                                               const QString&       codeTpl,
                                                               const QString&       parTpl,
                                                               const QString&       minTpl,
                                                               const QString&       maxTpl,
                                                               const LevelCallback& levelCb,
                                                               const int            columns,
                                                               const int            minProps,
                                                               const int            maxProps,
                                                               const bool           isPerfect) {
                const MagicPropBucket& bucket = all.bucketByType[MagicPropSet::s_all];
                for (auto& row : view) {
                    QString& firstPar = row[codeTpl.arg(1)];
                    if (firstPar.isEmpty())
                        continue;

                    const int level = levelCb(row);

                    const int newCnt = rng.bounded(minProps, maxProps + 1);

                    int col = 1;
                    for (int i = 1; i <= newCnt; ++i) {
                        const auto& bundle = bucket.getRandomBundle(rng, level, balance);
                        for (int j = 0; j < bundle.props.size(); ++j) {
                            if (col > columns)
                                break;
                            const auto& prop = bundle.props[j];
                            auto&       code = row[codeTpl.arg(col)];
                            auto&       par  = row[parTpl.arg(col)];
                            auto&       min  = row[minTpl.arg(col)];
                            auto&       max  = row[maxTpl.arg(col)];
                            code             = prop.code;
                            par              = prop.par;
                            min              = isPerfect ? prop.max : prop.min;
                            max              = prop.max;
                            col++;
                        }
                    }
                    for (; col <= columns; ++col) {
                        row[codeTpl.arg(col)] = "";
                        row[parTpl.arg(col)]  = "";
                        row[minTpl.arg(col)]  = "";
                        row[maxTpl.arg(col)]  = "";
                    }
                }
            };

            {
                const int minProps = getWidgetValue("min_uniq_props");
                const int maxProps = std::max(minProps, getWidgetValue("max_uniq_props"));
                TableView view(tableSet.tables["uniqueitems"]);
                fillProps(view, "prop%1", "par%1", "min%1", "max%1", commonLvlReq, 12, minProps, maxProps, perfectRoll);
            }
            {
                const int minProps = getWidgetValue("min_rw_props");
                const int maxProps = std::max(minProps, getWidgetValue("max_rw_props"));
                TableView view(tableSet.tables["runes"]);
                fillProps(view, "T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", commonRWreq, 7, minProps, maxProps, perfectRoll);
            }
            {
                const int minProps = getWidgetValue("min_set_props");
                const int maxProps = std::max(minProps, getWidgetValue("max_set_props"));
                TableView view(tableSet.tables["setitems"]);
                fillProps(view, "prop%1", "par%1", "min%1", "max%1", commonLvlReq, 9, minProps, maxProps, perfectRoll);
            }
            {
                const int minProps = 1;
                const int maxProps = 3;
                TableView view(tableSet.tables["gems"]);
                for (QString prefix : QStringList{ "weaponMod", "helmMod", "shieldMod" }) {
                    fillProps(
                        view, prefix + "%1Code", prefix + "%1Param", prefix + "%1Min", prefix + "%1Max", [&miscItemsLevels](const TableView::RowView& row) {
                            return miscItemsLevels.value(row["code"]);
                        },
                        3,
                        minProps,
                        maxProps,
                        true);
                }
            }
            if (getWidgetValue("affixRandom")) {
                const int minProps = 1;
                const int maxProps = 3;
                for (const char* table : { "magicprefix", "magicsuffix" }) {
                    TableView view(tableSet.tables[table]);
                    fillProps(
                        view, "mod%1code", "mod%1param", "mod%1min", "mod%1max", [&miscItemsLevels](const TableView::RowView& row) {
                            return row["level"].toInt(); // utilize maxLevel ?
                        },
                        3,
                        minProps,
                        maxProps,
                        perfectRoll);
                }
                result << "magicprefix"
                       << "magicsuffix";
            }
            return result;
        }
    }
};

const QString RandomizerPage::MagicPropSet::s_all = "all";

}

QList<IConfigPage*> CreateConfigPages(QWidget* parent)
{
    return QList<IConfigPage*>{
        new DifficultyPage(parent),
        new DropsPage(parent),
        new RandomizerPage(parent),
    };
}
