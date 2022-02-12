/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageItemDrops.hpp"
#include "AttributeHelper.hpp"

#include <QLabel>

namespace D2ModGen {

namespace {

enum class TCType
{
    Equip,
    Bow,
    Junk,
    Other,
};

const QRegularExpression s_eqRegex("Act [12345]( \\([NH]\\))? Equip [ABC]");
const QRegularExpression s_bowRegex("Act [12345]( \\([NH]\\))? Bow [ABC]");
const QRegularExpression s_junkRegex("Act [12345]( \\([NH]\\))? Junk");

TCType getTC(const QString& tc)
{
    if (s_eqRegex.match(tc).hasMatch())
        return TCType::Equip;
    if (s_bowRegex.match(tc).hasMatch())
        return TCType::Bow;
    if (s_junkRegex.match(tc).hasMatch())
        return TCType::Junk;
    return TCType::Other;
}

}

ConfigPageItemDrops::ConfigPageItemDrops(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax(tr("Increase Unique Chance"), "chance_uni", 1, 50, 1, this)
               << new SliderWidgetMinMax(tr("Increase Set Chance"), "chance_set", 1, 30, 1, this)
               << new SliderWidgetMinMax(tr("Increase Rare Chance"), "chance_rare", 1, 15, 1, this)
               << addHelp(new SliderWidgetMinMax(tr("Increase Chance of High-level equipment"), "chance_highlevel", 1, 15, 1, this),
                          tr("This will reduce the reduce the chance that equipment of lower level will drop as a fallback.\n"
                             "For example, for level 85 TC there is a 98% chance that it will drop from 84 level TC, and so on."))
               << addHelp(new CheckboxWidget(tr("Increase Champion/Unique item count"), "high_elite_drops", false, this),
                          tr("This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.\n"
                             "Note that potion drops are slightly reduced.")));

    addWidget(new QLabel(tr("Adjust proportion of main drop classes"), this));
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidget(tr("NoDrop (higher=LESS drops)"), "nodrop_percent", 20, 5, 100, true, this),
                          tr("NoDrop slider provides ability to gradually reduce NoDrop picks \n"
                             "(it basically similar to increasing players count in the game)"))
               << addHelp(new SliderWidget(tr("Good TC (Runes/Gems/Jewellery)"), "good_percent", 2, 10, 100, true, this),
                          tr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often \n"
                             "compared to equipment (armor/weapons)."))
               << addHelp(new SliderWidget(tr("Equipment (armor/weapons)"), "equip_percent", 5, 10, 100, true, this),
                          tr("Relative chance of equipment dropping compared to other items (or NoDrop)."))
               << addHelp(new SliderWidget(tr("Gold (higher=MORE gold)"), "gold_percent", 20, 10, 100, true, this),
                          tr("Relative chance of gold compared to other items."))
               << addHelp(new SliderWidget(tr("Junk (keys/bolts/etc) (higher=MORE junk)"), "junk_percent", 20, 10, 100, true, this),
                          tr("Relative chance of junk items compared to other items.")));

    addEditors(QList<IValueWidget*>()
               << addHelp(new CheckboxWidget(tr("Make all Uniques have equal rarity on same base"), "equal_uniques", false, this),
                          tr("Now Uniques with equal item base will have equal chance to drop.\n"
                             "For example Tyrael's and Templar's will have equal chance. (and all rings too)"))
               << addHelp(new CheckboxWidget(tr("Always perfect rolls"), "perfect_rolls", false, this),
                          tr("That will make all properties be perfect (where it make sense), (independent from Randomizer).")));
    closeLayout();
}

QString ConfigPageItemDrops::pageHelp() const
{
    return tr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.\n"
              "Basically all options here modifying TC pick weights - so one preferred option become less rare.\n"
              "First 3 sliders are for increasing chance for Uniques/Sets/Rares. \n"
              "Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.");
}

IConfigPage::PresetList ConfigPageItemDrops::pagePresets() const
{
    return {
        { tr("I want to find Tyrael's today!"),
          QJsonObject({
              { "chance_uni", 50 },
              { "chance_set", 30 },
              { "chance_highlevel", 15 },
              { "high_elite_drops", 1 },
              { "equal_uniques", 1 },
          }) },
        { tr("Just tired of finding my first Shako"),
          QJsonObject({
              { "chance_uni", 15 },
              { "chance_set", 10 },
              { "high_elite_drops", 1 },
          }) },
        { tr("A little bit more of good stuff, not crazy"),
          QJsonObject({
              { "chance_uni", 5 },
              { "chance_set", 5 },
              { "nodrop_percent", 50 },
              { "gold_percent", 50 },
              { "junk_percent", 10 },
          }) },
    };
}

void ConfigPageItemDrops::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    static const QSet<int>     s_modifyGroups{ 6, 7, 8, 9, 10, 16, 17 }; // groups with empty item ratio weights.
    static const QSet<QString> s_modifyNames{
        "Cow",
        "Cow (N)",
        "Cow (H)",
    };
    auto& tableSet = output.tableSet;
    {
        TableView view(tableSet.tables["treasureclassex"], true);

        const int  factorUnique    = getWidgetValue("chance_uni");
        const int  factorSet       = getWidgetValue("chance_set");
        const int  factorRare      = getWidgetValue("chance_rare");
        const int  factorHighlevel = getWidgetValue("chance_highlevel");
        const int  percentNoDrop   = getWidgetValue("nodrop_percent");
        const int  percentGoodTC   = getWidgetValue("good_percent");
        const int  percentGold     = getWidgetValue("gold_percent");
        const int  percentEquip    = getWidgetValue("equip_percent");
        const int  percentJunk     = getWidgetValue("junk_percent");
        const bool highDropsCount  = getWidgetValue("high_elite_drops");

        auto factorAdjust = [](QString& value, double factor, int maxFact) {
            const double prev           = value.toInt();
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value          = QString("%1").arg(std::min(next, maxFact));
        };
        auto adjustPick = [](int value, int num, int denum) -> int {
            if (!value)
                return 0;
            if (num == denum)
                return value;
            value = value * num / denum;
            return std::max(1, value);
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
            DropSet dropSet;
            dropSet.readRow(row);

            dropSet.m_noDrop = adjustPick(dropSet.m_noDrop, percentNoDrop, 100);

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

            const auto rowTC = getTC(className);
            if (rowTC == TCType::Other) {
                for (auto& item : dropSet.m_items) {
                    int        percent   = 100;
                    const auto subItemTC = getTC(item.tc);
                    if (item.tc.endsWith(" Good"))
                        percent = percentGoodTC;
                    else if (item.tc.startsWith("gld") || item.tc.startsWith("\"gld"))
                        percent = percentGold;
                    else if (subItemTC == TCType::Junk)
                        percent = percentJunk;
                    else if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        percent = percentEquip;

                    item.prob = adjustPick(item.prob, percent, 100);
                }
            }
            if (rowTC == TCType::Equip || rowTC == TCType::Bow) {
                for (auto& item : dropSet.m_items) {
                    const auto subItemTC = getTC(item.tc);
                    if (subItemTC == TCType::Equip || subItemTC == TCType::Bow)
                        item.prob = adjustPick(item.prob, 1, factorHighlevel);
                }
            }

            dropSet.writeRow(row);
        }
    }

    const bool equalRarity = getWidgetValue("equal_uniques");
    if (equalRarity) {
        TableView view(tableSet.tables["uniqueitems"], true);
        for (auto& row : view) {
            QString& rarity = row["rarity"];
            if (!rarity.isEmpty())
                rarity = "1";
        }
    }
    const bool perfectRolls = getWidgetValue("perfect_rolls");
    if (perfectRolls) {
        auto updateMinParam = [](TableView&         view,
                                 const ColumnsDesc& columns) {
            view.markModified();
            for (auto& row : view) {
                for (const auto& col : columns.m_cols) {
                    auto& min = row[col.min];
                    if (min.isEmpty())
                        continue;
                    if (isMinMaxRange(row[col.code]))
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
            const ColumnsDescList s_descSetItems{
                ColumnsDesc("prop%1", "par%1", "min%1", "max%1", 9),
                ColumnsDesc("aprop%1a", "apar%1a", "amin%1a", "amax%1a", 5),
                ColumnsDesc("aprop%1b", "apar%1b", "amin%1b", "amax%1b", 5),
            };
            TableView view(tableSet.tables["setitems"]);
            for (const auto& desc : s_descSetItems)
                updateMinParam(view, desc);
        }
        {
            for (const char* table : { "magicprefix", "magicsuffix", "automagic" }) {
                if (!tableSet.tables.contains(table))
                    continue;
                TableView view(tableSet.tables[table], true);
                updateMinParam(view, ColumnsDesc("mod%1code", "mod%1param", "mod%1min", "mod%1max", 3));
            }
        }
    }
}

}
