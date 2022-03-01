/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleCharacter.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleCharacter>();

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

PropertyTreeScalarMap ModuleCharacter::defaultValues() const
{
    return {
        { "addCube", false },
        { "idTome", false },
        { "manaPots", false },
        { "statPerLevel", 5 },
        { "skillPerLevel", 1 },
        { "statLower", 100 },
        { "mercHP", 100 },
        { "mercDam", 100 },
    };
}

IModule::UiControlHintMap ModuleCharacter::uiHints() const
{
    return {
        { "statPerLevel", UiControlHintSliderMinMax(1, 25) },
        { "skillPerLevel", UiControlHintSliderMinMax(1, 5) },
        { "statLower", UiControlHintSliderMinMax(10, 100) },
        { "mercHP", UiControlHintSlider(5, 10) },
        { "mercDam", UiControlHintSlider(5, 10) },
    };
}

void ModuleCharacter::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto&     tableSet  = output.tableSet;
    Table&    charTable = tableSet.tables["charstats"];
    TableView charTableView(charTable, true);

    const int statPerLevel   = input.getInt("statPerLevel");
    const int skillPerLevel  = input.getInt("skillPerLevel");
    const int statLower      = input.getInt("statLower");
    const int mercHPpercent  = input.getInt("mercHP");
    const int mercDampercent = input.getInt("mercDam");

    for (auto& row : charTableView) {
        if (row["class"] == "Expansion")
            continue;
        CharItems items;
        items.read(row);
        if (input.getInt("addCube"))
            items.m_items << CharItems::Item{ "box", 1 };
        if (input.getInt("idTome"))
            for (auto& item : items.m_items) {
                if (item.code == "isc")
                    item.code = "ibk";
            }
        if (input.getInt("manaPots"))
            for (auto& item : items.m_items) {
                if (item.code == "hp1")
                    item.code = "mp1";
            }
        items.write(row);
        row["StatPerLevel"] = QString("%1").arg(statPerLevel);
        if (!input.m_env.isLegacy)
            row["SkillsPerLevel"] = QString("%1").arg(skillPerLevel);
    }

    if (statLower != 100) {
        auto trans = [statLower](const int value) {
            const int newStat = value * statLower / 100;
            return newStat < 10 ? 0 : newStat;
        };
        for (const char* tableName : { "armor", "weapons" }) {
            Table&    table = tableSet.tables[tableName];
            TableView view(table, true);
            view.applyIntTransform(QStringList{ "reqstr", "reqdex" }, trans);
        }
    }
    if (mercHPpercent != 100 || mercDampercent != 100) {
        TableView view(tableSet.tables["hireling"], true);
        view.applyIntTransform(QStringList{ "HP", "HP/Lvl" }, [mercHPpercent](const int value) -> int {
            return value * mercHPpercent / 100;
        });
        view.applyIntTransform(QStringList{ "Dmg-Min", "Dmg-Max", "Dmg/Lvl" }, [mercDampercent](const int value) -> int {
            return value * mercDampercent / 100;
        });
    }
}

}
