/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"
#include "SliderWidget.hpp"
#include "CheckboxWidget.hpp"

#include <QVBoxLayout>
#include <QLabel>

#include <functional>

namespace {

const QSet<QString> g_ignoreMinMax{ "hit-skill", "charged" };

struct DropSet {
    struct Item {
        QString tc;
        int     prob;
    };
    int         m_noDrop = 0;
    QList<Item> m_items;

    void readRow(const TableView::RowView& row)
    {
        const QString& noDrop = row["NoDrop"];
        m_noDrop              = noDrop.toInt();

        m_items.clear();
        for (int i = 1; i <= 10; ++i) {
            const QString& prob = row[QString("Prob%1").arg(i)];
            if (prob.isEmpty())
                break;
            const QString& tcName = row[QString("Item%1").arg(i)];
            m_items << Item{ tcName, prob.toInt() };
        }
    }
    void writeRow(TableView::RowView& row) const
    {
        QString& noDrop = row["NoDrop"];
        noDrop          = QString("%1").arg(m_noDrop);
        for (int i = 1; i <= 10; ++i) {
            QString& prob   = row[QString("Prob%1").arg(i)];
            QString& tcName = row[QString("Item%1").arg(i)];
            prob.clear();
            tcName.clear();
            if (i - 1 >= m_items.size())
                continue;
            auto& item = m_items[i - 1];
            prob       = QString("%1").arg(item.prob);
            tcName     = item.tc;
        }
    }

    int getDropSomethingProb() const
    {
        int totalOther = 0;
        for (const auto& item : m_items)
            totalOther += item.prob;
        return totalOther;
    }
};

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
    void addWidget(QWidget* w)
    {
        m_layout->addWidget(w);
    }
    void addEditors(QList<IValueWidget*> editors)
    {
        for (IValueWidget* w : editors) {
            m_layout->addWidget(w);
            m_editors[w->objectName()] = w;
        }
    }
    void addEditorsPlain(QList<IValueWidget*> editors)
    {
        for (IValueWidget* w : editors)
            m_editors[w->objectName()] = w;
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
                   << new SliderWidget("Monster Attack Rating, multiply by", "mon_ar", 10, 10, this)
                   << new SliderWidget("Monster Defense, multiply by", "mon_def", 10, 10, this)
                   << new SliderWidget("Monster HP, multiply by", "mon_hp", 10, 10, this)
                   << new SliderWidget("Monster Damage, multiply by", "mon_dam", 10, 10, this)
                   << new SliderWidget("Monster XP, multiply by", "mon_xp", 10, 10, this)
                   << new SliderWidgetMinMax("Increase density, times", "density", 1, 20, 1, this)
                   << new SliderWidgetMinMax("Increase Boss packs count, times", "packs", 1, 20, 1, this)
                   << new SliderWidgetMinMax("Increase monster groups population, +count<br>"
                                             "<b>Beware! This setting have the most impact on getting screen laggy!<b><br>"
                                             "<b>Having value above +3 will work well only if previous options are not maxed!</b>",
                                             "mon_groups",
                                             0,
                                             20,
                                             0,
                                             this)
                   << new CheckboxWidget("Use Hell pack count everywhere", "hellPacks", false, this)
                   << new SliderWidgetMinMax("Increase area level (Nightmare and Hell), +levels", "levelIncrease", 0, 20, 0, this));
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
        {
            const int groupIncrease = getWidgetValue("mon_groups");
            TableView view(tableSet.tables["monstats"]);
            if (groupIncrease > 0) {
                result << "monstats";
                for (auto& row : view) {
                    if (row["Level"].isEmpty())
                        continue;
                    QString& valueMin = row["MinGrp"];
                    QString& valueMax = row["MaxGrp"];
                    if (valueMin.isEmpty() || valueMax.isEmpty())
                        continue;
                    if (valueMin == valueMax && valueMax == "1")
                        continue;
                    for (QString* value : { &valueMin, &valueMax }) {
                        const int prev = value->toInt();
                        const int next = prev + groupIncrease;
                        *value         = QString("%1").arg(std::min(next, 99));
                    }
                }
            }
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
                const int next = prev * density;
                value          = QString("%1").arg(std::clamp(next, 100, 9900));
            };
            const int packs       = getWidgetValue("packs");
            auto      packsAdjust = [packs](QString& value) {
                const int prev = value.toInt();
                const int next = prev * packs;
                value          = QString("%1").arg(std::min(next, 255));
            };
            const QString nighLevelKey = view.hasColumn("MonLvlEx(N)") ? "MonLvlEx(N)" : "MonLvl2Ex";
            const QString hellLevelKey = view.hasColumn("MonLvlEx(H)") ? "MonLvlEx(H)" : "MonLvl3Ex";
            for (auto& row : view) {
                QString& normMin   = row["MonUMin"];
                QString& normMax   = row["MonUMax"];
                QString& nighMin   = row["MonUMin(N)"];
                QString& nighMax   = row["MonUMax(N)"];
                QString& hellMin   = row["MonUMin(H)"];
                QString& hellMax   = row["MonUMax(H)"];
                QString& nighLevel = row[nighLevelKey];
                QString& hellLevel = row[hellLevelKey];

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
                   << new SliderWidgetMinMax("NoDrop reduce % (higher=more drops)", "nodrop_factor", 1, 10, 1, this)
                   << new CheckboxWidget("Increase Champion/Unique item count", "high_elite_drops", false, this)
                   << new SliderWidgetMinMax("Increase Good TC (Runes/Gems/Jewellery)", "good_factor", 1, 10, 1, this)
                   << new SliderWidgetMinMax("Increase Runes chance in Good TC", "rune_factor", 1, 10, 1, this)
                   << new CheckboxWidget("Switch Ber,Jah with Cham,Zod in rarity", "highrune_switch", false, this)
                   << new SliderWidgetMinMax("Increase Rare Rune drops<br>This is increase of dropping Zod in 'Runes 17' TC<br>Rarity of other runes will change proportionally.", "zod_factor", 1, 1000, 1, this)
                   << new CheckboxWidget("Make all Uniques have equal rarity on same base (including rings)", "equal_uniques", false, this)
                   << new CheckboxWidget("Always perfect rolls (independent from Randomizer)", "perfect_rolls", false, this));
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
        if (isAllDefault({ "chance_uni", "chance_set", "chance_rare", "nodrop_factor", "high_elite_drops", "good_factor", "rune_factor", "zod_factor", "highrune_switch", "equal_uniques", "perfect_rolls" }))
            return {};
        KeySet                     result{ "treasureclassex" };
        static const QSet<int>     s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
        static const QSet<QString> s_modifyNames{
            "Cow",
            "Cow (N)",
            "Cow (H)",
        };

        {
            TableView view(tableSet.tables["treasureclassex"]);

            const int  factorUnique    = getWidgetValue("chance_uni");
            const int  factorSet       = getWidgetValue("chance_set");
            const int  factorRare      = getWidgetValue("chance_rare");
            const int  factorNo        = getWidgetValue("nodrop_factor");
            const bool highDropsCount  = getWidgetValue("high_elite_drops");
            const int  factorGoodTC    = getWidgetValue("good_factor");
            const int  factorRune      = getWidgetValue("rune_factor");
            const int  factorZod       = getWidgetValue("zod_factor");
            const bool switchHighRunes = getWidgetValue("highrune_switch");
            auto       factorAdjust    = [](QString& value, double factor, int maxFact) {
                const double prev           = value.toInt();
                const double probReverseOld = (1024. - prev);
                const double probReverseNew = probReverseOld / factor;
                const double probNew        = (1024. - probReverseNew);

                const int next = static_cast<int>(probNew);
                value          = QString("%1").arg(std::min(next, maxFact));
            };
            auto nodropAdjust = [factorNo](int value, double totalOther) -> int {
                const double oldNoDrop      = value;
                const double oldNoDropRatio = oldNoDrop / (oldNoDrop + totalOther);
                const double newNoDropRatio = oldNoDropRatio / factorNo;
                const double newNoDrop      = totalOther * newNoDropRatio / (1. - newNoDropRatio);
                const int    next           = static_cast<int>(newNoDrop);
                return std::max(next, 1);
            };

            QMap<QString, QString> highRuneReplacement{
                { "r33", "r31" },
                { "r32", "r30" },
                { "r31", "r33" },
                { "r30", "r32" },
            };

            QMap<QString, double> runesReplaceFactor;
            if (factorZod > 1) {
                double       startFactor = 1.4;
                const double iterFactor  = 1.02;
                if (factorZod > 10)
                    startFactor = 1.3;
                if (factorZod > 100)
                    startFactor = 1.2;
                double mult = factorZod;
                for (int i = 16; i >= 3; --i) {
                    runesReplaceFactor[QString("Runes %1").arg(i)] = mult;
                    mult /= startFactor;
                    startFactor *= iterFactor;
                }
            }

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
                DropSet dropSet;
                dropSet.readRow(row);

                const bool needToAdjustNodrop = dropSet.m_noDrop;
                if (needToAdjustNodrop)
                    dropSet.m_noDrop = nodropAdjust(dropSet.m_noDrop, dropSet.getDropSomethingProb());

                if (highDropsCount) {
                    if (treasureGroup == "15") { // Uniques, insead of 1 item + 2*2 potion, make 4 items + 1*2 potion
                        dropSet.m_items[0].prob = 4;
                        dropSet.m_items[1].prob = 1;
                        row["Picks"]            = "-5";
                    }
                    if (treasureGroup == "13") { // Champions, insead of 1 item + 1-2*2 potion, make 2 items + 1*2 potion
                        dropSet.m_items[0].prob = 2;
                        dropSet.m_items[1].prob = 1;
                        row["Picks"]            = "-3";
                    }
                }
                if (factorGoodTC > 1) {
                    for (auto& item : dropSet.m_items) {
                        if (!item.tc.endsWith(" Good"))
                            continue;
                        item.prob *= factorGoodTC;
                        break;
                    }
                }
                if (factorRune > 1 && className.endsWith(" Good")) {
                    for (auto& item : dropSet.m_items) {
                        if (!item.tc.startsWith("Runes "))
                            continue;
                        item.prob *= factorRune;
                        break;
                    }
                }
                if (factorZod > 1 && className.startsWith("Runes ")) {
                    for (auto& item : dropSet.m_items) {
                        QString& tcName = item.tc;
                        if (!tcName.startsWith("Runes ") || !runesReplaceFactor.contains(tcName))
                            continue;
                        const int newProb = static_cast<int>(item.prob / runesReplaceFactor[tcName]);
                        item.prob         = std::max(newProb, 5);
                        break;
                    }
                }
                if (switchHighRunes && className.startsWith("Runes ")) {
                    for (auto& item : dropSet.m_items) {
                        QString& tcName = item.tc;
                        tcName          = highRuneReplacement.value(tcName, tcName);
                    }
                }
                dropSet.writeRow(row);
            }
        }

        const bool equalRarity = getWidgetValue("equal_uniques");
        if (equalRarity) {
            result << "uniqueitems";
            TableView view(tableSet.tables["uniqueitems"]);
            for (auto& row : view) {
                QString& rarity = row["rarity"];
                if (!rarity.isEmpty())
                    rarity = "1";
            }
        }
        const bool perfectRolls = getWidgetValue("perfect_rolls");
        if (perfectRolls) {
            result << "uniqueitems"
                   << "runes"
                   << "setitems"
                   << "magicprefix"
                   << "magicsuffix";
            auto updateMinParam = [](TableView&         view,
                                     const ColumnsDesc& columns) {
                for (auto& row : view) {
                    for (const auto& col : columns.m_cols) {
                        auto& min = row[col.min];
                        if (min.isEmpty())
                            break;
                        if (!g_ignoreMinMax.contains(row[col.code]))
                            min = row[col.max];
                    }
                }
            };

            {
                auto&     table = tableSet.tables["uniqueitems"];
                TableView view(table);
                updateMinParam(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 12));
            }
            {
                TableView view(tableSet.tables["runes"]);
                updateMinParam(view, ColumnsDesc("T1Code%1", "T1Param%1", "T1Min%1", "T1Max%1", 7));
            }
            {
                TableView view(tableSet.tables["setitems"]);
                updateMinParam(view, ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9));
            }
            {
                for (const char* table : { "magicprefix", "magicsuffix" }) {
                    TableView view(tableSet.tables[table]);
                    updateMinParam(view, ColumnsDesc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3));
                }
            }
        }

        return result;
    }
};

class DropFiltering : public AbstractPage {
public:
    struct Item {
        QSet<QString> internalIds;
        QString       settingKey;
        QString       title;
    };
    const QList<Item> m_items;

    DropFiltering(QWidget* parent)
        : AbstractPage(parent)
        , m_items{
            { { "isc" }, "isc", "ID scroll" },
            { { "tsc" }, "tsc", "TP scroll" },
            { { "hp1", "hp2", "hp3" }, "hps", "Health pots 1-3" },
            { { "hp1", "hp2", "hp3", "hp4", "hp5" }, "hpsa", "All Health pots" },
            { { "mp1", "mp2", "mp3" }, "mps", "Mana pots 1-3" },
            { { "mp1", "mp2", "mp3", "mp4", "mp5" }, "mpsa", "All Mana pots" },
            { { "rvs" }, "rvs", "Rejuv pots" },
            { { "rvl" }, "rvl", "Full Rejuv pots" },
            { { "Ammo" }, "ammo", "Bolts/Arrows" },
            { { "vps", "yps", "wms" }, "ammo", "Stamina/Antidote/Thawing" },
            { { "Misc 0", "Misc 1", "Misc 2" }, "junks", "Keys/Fire/Poison pots" },
        }

    {
        addWidget(new QLabel("<b>Warning</b>: please note, there is no real way to filter drops in D2R;<br>"
                             "Instead of filtering loot, we just make it to not drop at all instead.<br>"
                             "In practice, it's the same for user, but you have no way to pick an item if you changed you mind.<br>"
                             "Other approach - is to change item tag to something really short - that solution require <br>"
                             "localization edits, which is possible, but not yet supported.<br>"
                             "All options below just <b>replace drops with NoDrop</b>, so no real affect on probabilities of other items."

                             ,
                             this));
        for (auto& item : m_items)
            addEditors(QList<IValueWidget*>()
                       << new CheckboxWidget("Disable " + item.title, "nodrop_" + item.settingKey, false, this));
        closeLayout();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Drops filtering";
    }
    QString settingKey() const override
    {
        return "drop_filter";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override
    {
        if (isAllDefault())
            return {};
        KeySet result;
        result << "treasureclassex";

        TableView view(tableSet.tables["treasureclassex"]);

        QSet<QString> disabledIds;
        for (auto& item : m_items) {
            const bool disable = getWidgetValue("nodrop_" + item.settingKey);
            if (disable)
                disabledIds += item.internalIds;
        }
        if (!disabledIds.empty()) {
            for (auto& row : view) {
                DropSet dropSet;
                dropSet.readRow(row);
                for (int i = dropSet.m_items.size() - 1; i >= 0; i--) {
                    auto&    dropItem = dropSet.m_items[i];
                    QString& tcName   = dropItem.tc;
                    if (disabledIds.contains(tcName)) {
                        dropSet.m_noDrop += dropItem.prob;
                        dropSet.m_items.removeAt(i);
                    }
                }
                dropSet.writeRow(row);
            }
        }

        return result;
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
                   << new SliderWidgetMinMax("Create several versions of each Unique item", "repeat_uniques", 1, 20, 10, this));

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

        void postProcess(bool replaceSkills, bool replaceCharges)
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
                if (!code.isEmpty())
                    miscItemsLevels[code] = levelreq.toInt();
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
                            min                 = isPerfect && !g_ignoreMinMax.contains(code) ? prop.max : prop.min;
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
};

const QString RandomizerPage::MagicPropSet::s_all = "all";

class CubePage : public AbstractPage {
public:
    CubePage(QWidget* parent)
        : AbstractPage(parent)
    {
        addEditors(QList<IValueWidget*>()
                   << new CheckboxWidget("Remove gems from Rune upgrades", "noGemUpgrade", false, this)
                   << new CheckboxWidget("Add quick portal access recipes:\n"
                                         "1. TP book + Id scroll = Cow Portal\n"
                                         "2. TP book + Id scroll x2 = Minor Uber Portal\n"
                                         "3. TP book + Id scroll x3 = Uber Tristram Portal\n",
                                         "quickPortals",
                                         false,
                                         this)
                   << new CheckboxWidget("Add socketing recipes:\n"
                                         "1. Normal item + TP scroll x1 + Id scroll x1 = Add 3 sockets\n"
                                         "2. Normal item + TP scroll x1 + Id scroll x2 = Add 4 sockets\n"
                                         "3. Normal item + TP scroll x2 + Id scroll x1 = Add 5 sockets\n"
                                         "4. Normal item + TP scroll x2 + Id scroll x2 = Add 6 sockets\n"
                                         "5. Unique item + TP scroll x1 + Id scroll x1 = Add 1 socket\n",
                                         "socketing",
                                         false,
                                         this));
        closeLayout();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Horadric Cube";
    }
    QString settingKey() const override
    {
        return "cube";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng) const override
    {
        if (isAllDefault({ "noGemUpgrade", "quickPortals", "socketing" }))
            return {};
        KeySet result;
        result << "cubemain";

        const bool noGemUpgrade = getWidgetValue("noGemUpgrade");
        const bool quickPortals = getWidgetValue("quickPortals");
        const bool newSocketing = getWidgetValue("socketing");

        TableView view(tableSet.tables["cubemain"]);
        for (auto& row : view) {
            QString& description = row["description"];
            QString& numinputs   = row["numinputs"];
            QString& input1      = row["input 1"];
            QString& input2      = row["input 2"];
            QString& input3      = row["input 3"];

            if (noGemUpgrade && description.contains("->") && description.endsWith(" Rune") && !input2.isEmpty()) {
                numinputs = QString("%1").arg(numinputs.toInt() - 1);
                input2    = "";
            }
        }
        if (quickPortals) {
            using StringMap = QMap<QString, QString>;
            const StringMap base{
                { "description", "Quick Portal" },
                { "enabled", "1" },
                { "version", "0" },
                { "op", "28" },
                { "*eol", "0" },
                { "input 1", "tbk" },

            };
            StringMap cows    = base;
            cows["numinputs"] = "2";
            cows["input 2"]   = "isc";
            cows["output"]    = "Cow Portal";

            StringMap uber1    = base;
            uber1["numinputs"] = "3";
            uber1["input 2"]   = "\"isc,qty=2\"";
            uber1["output"]    = "Pandemonium Portal";

            StringMap uber2    = base;
            uber2["numinputs"] = "4";
            uber2["input 2"]   = "\"isc,qty=3\"";
            uber2["output"]    = "Pandemonium Finale Portal";

            view.appendRow(cows);
            view.appendRow(uber1);
            view.appendRow(uber2);
        }
        if (newSocketing) {
            using StringMap = QMap<QString, QString>;
            const StringMap base{
                { "description", "Quick Sockets" },
                { "enabled", "1" },
                { "version", "0" },
                { "*eol", "0" },
                { "input 1", "\"any,nor,nos\"" },

            };
            StringMap socket3    = base;
            socket3["numinputs"] = "3";
            socket3["input 2"]   = "tsc";
            socket3["input 3"]   = "isc";
            socket3["output"]    = "\"useitem,sock=3\"";

            StringMap socket4    = base;
            socket4["numinputs"] = "4";
            socket4["input 2"]   = "\"tsc,qty=2\"";
            socket4["input 3"]   = "isc";
            socket4["output"]    = "\"useitem,sock=4\"";

            StringMap socket5    = base;
            socket5["numinputs"] = "4";
            socket5["input 2"]   = "\"isc,qty=2\"";
            socket5["input 3"]   = "tsc";
            socket5["output"]    = "\"useitem,sock=5\"";

            StringMap socket6    = base;
            socket6["numinputs"] = "5";
            socket6["input 2"]   = "\"isc,qty=2\"";
            socket6["input 3"]   = "\"tsc,qty=2\"";
            socket6["output"]    = "\"useitem,sock=6\"";

            StringMap socket1    = base;
            socket1["numinputs"] = "3";
            socket1["input 1"]   = "\"any,uni,nos\"";
            socket1["input 2"]   = "tsc";
            socket1["input 3"]   = "isc";
            socket1["output"]    = "\"useitem,sock=1\"";

            view.appendRow(socket3);
            view.appendRow(socket4);
            view.appendRow(socket5);
            view.appendRow(socket6);
            view.appendRow(socket1);
        }

        return result;
    }
};
}

QList<IConfigPage*> CreateConfigPages(QWidget* parent)
{
    return QList<IConfigPage*>{
        new DifficultyPage(parent),
        new DropsPage(parent),
        new DropFiltering(parent),
        new RandomizerPage(parent),
        new CubePage(parent),
    };
}
