/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageDrops.hpp"
#include "AttributeHelper.hpp"

namespace D2ModGen {

ConfigPageDrops::ConfigPageDrops(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new SliderWidgetMinMax(tr("Increase Unique Chance"), "chance_uni", 1, 50, 1, this)
               << new SliderWidgetMinMax(tr("Increase Set Chance"), "chance_set", 1, 30, 1, this)
               << new SliderWidgetMinMax(tr("Increase Rare Chance"), "chance_rare", 1, 15, 1, this)
               << addHelp(new SliderWidgetMinMax(tr("NoDrop reduce % (higher=more drops)"), "nodrop_factor", 1, 10, 1, this),
                          tr("NoDrop slider provides ability to gradually reduce NoDrop picks \n"
                             "(it basically similar to increasing players count in the game)"))
               << addHelp(new CheckboxWidget(tr("Increase Champion/Unique item count"), "high_elite_drops", false, this),
                          tr("This will make Unique bosses to drop 4 items instead of just 1, and Champions drop 2 items instead of one.\n"
                             "Note that potion drops are slightly reduced."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Good TC (Runes/Gems/Jewellery)"), "good_factor", 1, 10, 1, this),
                          tr("That will make Gems/Runes/Rings/Amulets/Charms drop far more often \n"
                             "compared to equipment (armor/weapons)."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Runes chance in Good TC"), "rune_factor", 1, 10, 1, this),
                          tr("When dropping from Good TC, that will make Runes drop more often \n"
                             "compared to other jewellery (Gems/Rings/Amulets/Charms)"))
               << addHelp(new CheckboxWidget(tr("Switch (Ber,Jah) with (Cham,Zod) in rarity"), "highrune_switch", false, this),
                          tr("When game desides to drop Jah rune, it drops Zod instead. And vice versa."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Rare Rune drops"), "zod_factor", 1, 1000, 1, this),
                          tr("That will drastically improve High Rune drop rates! You probably don't need more than 50x.\n"
                             "Value here increases chance of dropping Zod in 'Runes 17' TC\n"
                             "Rarity of other runes will change proportionally \n"
                             "(so High Runes still be more rare in the same manner)."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Countess Runes TC level"), "countess_rune_upgrade", 0, 5, 0, this),
                          tr("Upgrade countess Rune drops from 'Runes 4', 'Runes 8', 'Runes 12' \n"
                             "to higher rune TC's, up to Runes 17 (Zod Rune)"))
               << addHelp(new CheckboxWidget(tr("Make all Uniques have equal rarity on same base"), "equal_uniques", false, this),
                          tr("Now Uniques with equal item base will have equal chance to drop.\n"
                             "For example Tyrael's and Templar's will have equal chance. (and all rings too)"))
               << addHelp(new CheckboxWidget(tr("Always perfect rolls"), "perfect_rolls", false, this),
                          tr("That will make all properties be perfect (where it make sense), (independent from Randomizer).")));
    closeLayout();
}

QString ConfigPageDrops::pageHelp() const
{
    return tr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.\n"
              "Basically all options here modifying TC pick weights - so one preferred option become less rare.\n"
              "First 3 sliders are for increasing chance for Uniques/Sets/Rares. \n"
              "Note that increase is accurate when your chances are low, but with high MF it can have diminishing return.");
}

void ConfigPageDrops::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
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
        const int  factorNo        = getWidgetValue("nodrop_factor");
        const bool highDropsCount  = getWidgetValue("high_elite_drops");
        const int  factorGoodTC    = getWidgetValue("good_factor");
        const int  factorRune      = getWidgetValue("rune_factor");
        const int  factorZod       = getWidgetValue("zod_factor");
        const int  countessUpg     = getWidgetValue("countess_rune_upgrade");
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
            if (countessUpg > 0 && className.startsWith("Countess Rune")) {
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    if (!tcName.startsWith("Runes "))
                        continue;
                    const int oldTC = tcName.midRef(6).toInt();
                    const int newTC = oldTC + countessUpg;
                    tcName          = QString("Runes %1").arg(newTC);
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
