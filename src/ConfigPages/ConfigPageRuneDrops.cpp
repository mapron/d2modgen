/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageRuneDrops.hpp"
#include "AttributeHelper.hpp"

namespace D2ModGen {

ConfigPageRuneDrops::ConfigPageRuneDrops(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << addHelp(new SliderWidgetMinMax(tr("Increase Rare Rune drops"), "zod_factor", 1, 1000, 1, this),
                          tr("That will drastically improve High Rune drop rates! You probably don't need more than 50x.\n"
                             "Value here increases chance of dropping Zod in 'Runes 17' TC\n"
                             "Rarity of other runes will change proportionally \n"
                             "(so High Runes still be more rare in the same manner)."))
               << addHelp(new SliderWidgetMinMax(tr("Increase Runes chance in Good TC"), "rune_factor", 1, 10, 1, this),
                          tr("When dropping from Good TC, that will make Runes drop more often \n"
                             "compared to other jewellery (Gems/Rings/Amulets/Charms)"))
               << addHelp(new CheckboxWidget(tr("Switch (Ber,Jah) with (Cham,Zod) in rarity"), "highrune_switch", false, this),
                          tr("When game desides to drop Jah rune, it drops Zod instead. And vice versa."))
               << new CheckboxWidget(tr("Make Countess Drop higher runes (up to Runes 17=Zod on Hell)"), "countess_rune_higher", false, this)
               << new CheckboxWidget(tr("Increase Countess Runes drop count to 5"), "countess_rune_more", false, this)
               << new CheckboxWidget(tr("Increase Wraiths Runes drop"), "wraith_runes", false, this));
    closeLayout();
}

QString ConfigPageRuneDrops::pageHelp() const
{
    return tr("All help and explanation on this page assume you are familiar with Treasure Class (TC) and NoDrop terminology.\n"
              "Basically all options here modifying TC pick weights - so one preferred option become less rare.");
}

IConfigPage::PresetList ConfigPageRuneDrops::pagePresets() const
{
    return {
        { tr("Gimme pile of Ber runes!"),
          QJsonObject({
              { "zod_factor", 200 },
              { "rune_factor", 10 },
              { "countess_rune_higher", 1 },
              { "countess_rune_more", 1 },
              { "wraith_runes", 1 },
          }) },
        { tr("I want to make Infinity at least once in my life"),
          QJsonObject({
              { "zod_factor", 10 },
              { "rune_factor", 3 },
          }) },
        { tr("I want high runes to be rare, but want more rune drops overall"),
          QJsonObject({
              { "countess_rune_higher", 1 },
              { "countess_rune_more", 1 },
              { "wraith_runes", 1 },
              { "rune_factor", 3 },
          }) },
    };
}

void ConfigPageRuneDrops::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return;

    auto& tableSet = output.tableSet;
    {
        TableView view(tableSet.tables["treasureclassex"], true);

        const int  factorRune      = getWidgetValue("rune_factor");
        const int  factorZod       = getWidgetValue("zod_factor");
        const bool countessMore    = getWidgetValue("countess_rune_more");
        const int  countessUpg     = getWidgetValue("countess_rune_higher") ? 5 : 0;
        const bool wraithMore      = getWidgetValue("wraith_runes");
        const bool switchHighRunes = getWidgetValue("highrune_switch");
        auto       factorAdjust    = [](QString& value, double factor, int maxFact) {
            const double prev           = value.toInt();
            const double probReverseOld = (1024. - prev);
            const double probReverseNew = probReverseOld / factor;
            const double probNew        = (1024. - probReverseNew);

            const int next = static_cast<int>(probNew);
            value          = QString("%1").arg(std::min(next, maxFact));
        };

        QMap<QString, QString> highRuneReplacement{
            { "r33", "r31" },
            { "r32", "r30" },
            { "r31", "r33" },
            { "r30", "r32" },
        };

        QMap<QString, double> runesReplaceMult;
        if (factorZod > 1) {
            const double factor   = factorZod;
            const double iterMult = std::pow(factor, 0.1);
            double       mult     = 1. / factor;

            for (int i = 16; i >= 6; --i) {
                runesReplaceMult[QString("Runes %1").arg(i)] = mult;
                mult *= iterMult;
            }
        }

        for (auto& row : view) {
            QString& className = row["Treasure Class"];

            DropSet dropSet;
            dropSet.readRow(row);

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
                    if (!tcName.startsWith("Runes ") || !runesReplaceMult.contains(tcName))
                        continue;
                    const int newProb = static_cast<int>(item.prob * runesReplaceMult[tcName]);
                    item.prob         = std::max(newProb, 5);
                    break;
                }
            }
            const bool countessRuneTC = className.startsWith("Countess Rune");
            const bool countessItemTC = className.startsWith("Countess Item");
            if (countessUpg > 0 && countessRuneTC) {
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
            if (countessMore) {
                if (countessRuneTC) {
                    row["Picks"]     = "5";
                    dropSet.m_noDrop = 1;
                }
                if (countessItemTC)
                    row["Picks"] = "1";
            }
            if (switchHighRunes && className.startsWith("Runes ")) {
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    tcName          = highRuneReplacement.value(tcName, tcName);
                }
            }
            if (wraithMore && className.contains(") Wraith ")) {
                static const QMap<QString, QString> s_wraithReplacement{
                    { "Act 1 (N)", "Runes 8" },
                    { "Act 2 (N)", "Runes 9" },
                    { "Act 3 (N)", "Runes 10" },
                    { "Act 4 (N)", "Runes 11" },
                    { "Act 5 (N)", "Runes 12" },
                    { "Act 1 (H)", "Runes 13" },
                    { "Act 2 (H)", "Runes 14" },
                    { "Act 3 (H)", "Runes 15" },
                    { "Act 4 (H)", "Runes 16" },
                    { "Act 5 (H)", "Runes 17" },
                };
                for (auto& item : dropSet.m_items) {
                    QString& tcName = item.tc;
                    if (!tcName.contains(") Magic "))
                        continue;
                    tcName = s_wraithReplacement.value(tcName.mid(0, 9), tcName);
                    break;
                }
            }
            dropSet.writeRow(row);
        }
    }
}

}
