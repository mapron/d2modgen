/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageCharacter.hpp"

namespace D2ModGen {

namespace {
struct CharItems {
    struct Item {
        QString code;
        int     count;
        QString loc;
        QString quality;
    };
    QList<Item> m_items;
    bool        m_hasQuality = false;
    void        read(const TableView::RowView& row)
    {
        m_items.clear();
        for (int i = 1; i <= 10; ++i) {
            const QString& count = row[QString("item%1count").arg(i)];
            m_hasQuality         = row.hasColumn(QString("item%1quality").arg(i));
            if (count == "0") {
                break;
            }
            const QString& code    = row[QString("item%1").arg(i)];
            const QString& loc     = row[QString("item%1loc").arg(i)];
            const QString  quality = m_hasQuality ? row[QString("item%1quality").arg(i)] : QString();
            m_items << Item{ code, count.toInt(), loc, quality };
        }
    }
    void write(TableView::RowView& row) const
    {
        for (int i = 1; i <= 10; ++i) {
            QString& code  = row[QString("item%1").arg(i)];
            QString& count = row[QString("item%1count").arg(i)];
            QString& loc   = row[QString("item%1loc").arg(i)];
            code.clear();
            count.clear();
            loc.clear();
            if (m_hasQuality) {
                QString& quality = row[QString("item%1quality").arg(i)];
                quality.clear();
            }
            if (i - 1 >= m_items.size())
                continue;
            auto& item = m_items[i - 1];
            code       = item.code;

            count = QString("%1").arg(item.count);
            loc   = item.loc;
            if (m_hasQuality)
                row[QString("item%1quality").arg(i)] = item.quality;
        }
    }
};
}

ConfigPageCharacter::ConfigPageCharacter(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Add Horadric Cube as starting item"), "addCube", false, this)
               << new CheckboxWidget(tr("Replace ID scroll with ID tome"), "idTome", false, this)
               << new CheckboxWidget(tr("Replace starting Health pots with Mana"), "manaPots", false, this)
               << new SliderWidgetMinMax(tr("Stat points gain per level"), "statPerLevel", 1, 25, 5, this)
               << new SliderWidgetMinMax(tr("Skill points gain per level (D2R only!)"), "skillPerLevel", 1, 5, 1, this)
               << new SliderWidgetMinMax(tr("Lower Strength/Dexterity requirements on items, %"), "statLower", 10, 100, 100, this));
    closeLayout();
}

QString ConfigPageCharacter::pageHelp() const
{
    return tr("First three options allow you to change starting items for characters: \n"
              "add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. \n"
              "Next two sliders allow you to change how many skill and stat poits you get on levelup. \n "
              "Last slider allow you to lower Strength and Dexterity requirements on all items.  ");
}

KeySet ConfigPageCharacter::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault())
        return {};
    KeySet result;
    result << "charstats";
    auto&     tableSet  = output.tableSet;
    Table&    charTable = tableSet.tables["charstats"];
    TableView charTableView(charTable);

    const int statPerLevel  = getWidgetValue("statPerLevel");
    const int skillPerLevel = getWidgetValue("skillPerLevel");
    const int statLower     = getWidgetValue("statLower");

    for (auto& row : charTableView) {
        if (row["class"] == "Expansion")
            continue;
        CharItems items;
        items.read(row);
        if (getWidgetValue("addCube"))
            items.m_items << CharItems::Item{ "box", 1 };
        if (getWidgetValue("idTome"))
            for (auto& item : items.m_items) {
                if (item.code == "isc")
                    item.code = "ibk";
            }
        if (getWidgetValue("manaPots"))
            for (auto& item : items.m_items) {
                if (item.code == "hp1")
                    item.code = "mp1";
            }
        items.write(row);
        row["StatPerLevel"] = QString("%1").arg(statPerLevel);
        if (!env.isLegacy)
            row["SkillsPerLevel"] = QString("%1").arg(skillPerLevel);
    }
    if (statLower != 100) {
        result << "armor"
               << "weapons";
        for (const char* tableName : { "armor", "weapons" }) {
            Table&    table = tableSet.tables[tableName];
            TableView tableView(table);
            for (auto& row : tableView) {
                for (const char* col : { "reqstr", "reqdex" }) {
                    if (!tableView.hasColumn(col))
                        continue;
                    QString& value = row[col];
                    if (value.isEmpty())
                        continue;
                    const int newStat = value.toInt() * statLower / 100;
                    value             = QString("%1").arg(newStat < 10 ? 0 : newStat);
                }
            }
        }
    }
    return result;
}

}
