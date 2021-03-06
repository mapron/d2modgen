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
        std::string code;
        int         count;
        std::string loc;
        std::string quality;
    };
    std::vector<Item> m_items;
    bool              m_hasQuality = false;
    void              read(const TableView::RowView& row)
    {
        m_items.clear();
        for (int i = 1; i <= 10; ++i) {
            const auto& count = row[argCompat("item%1count", i)];
            m_hasQuality      = row.hasColumn(argCompat("item%1quality", i));
            if (count.str == "0") {
                break;
            }
            const auto& code    = row[argCompat("item%1", i)];
            const auto& loc     = row[argCompat("item%1loc", i)];
            const auto  quality = m_hasQuality ? row[argCompat("item%1quality", i)].str : std::string();
            m_items.push_back(Item{ code.str, count.toInt(), loc.str, quality });
        }
    }
    void write(TableView::RowView& row) const
    {
        for (int i = 1; i <= 10; ++i) {
            auto& code  = row[argCompat("item%1", i)];
            auto& count = row[argCompat("item%1count", i)];
            auto& loc   = row[argCompat("item%1loc", i)];
            code.clear();
            count.clear();
            loc.clear();
            if (m_hasQuality) {
                auto& quality = row[argCompat("item%1quality", i)];
                quality.clear();
            }
            if (i - 1 >= m_items.size())
                continue;
            auto& item = m_items[i - 1];
            code.str   = item.code;

            count.setInt(item.count);
            loc.str = item.loc;
            if (m_hasQuality)
                row[argCompat("item%1quality", i)].str = item.quality;
        }
    }
};
}

void ModuleCharacter::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto&     tableSet  = output.tableSet;
    Table&    charTable = tableSet.tables[TableId::charstats];
    TableView charTableView(charTable, true);

    const int statPerLevel    = input.getInt("statPerLevel");
    const int skillPerLevel   = input.getInt("skillPerLevel");
    const int skillPointLimit = input.getInt("skillPointLimit");
    const int mercHPpercent   = input.getInt("mercHP");
    const int mercDampercent  = input.getInt("mercDam");

    for (auto& row : charTableView) {
        if (row["class"].str == "Expansion")
            continue;
        CharItems items;
        items.read(row);
        if (input.getInt("addCube"))
            items.m_items.push_back(CharItems::Item{ "box", 1 });
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
        row["StatPerLevel"].setInt(statPerLevel);
        if (!input.m_env.isLegacy)
            row["SkillsPerLevel"].setInt(skillPerLevel);
    }

    if (mercHPpercent != 100 || mercDampercent != 100) {
        TableView view(tableSet.tables[TableId::hireling], true);
        view.applyIntTransform(StringVector{ "HP", "HP/Lvl" }, [mercHPpercent](const int value) -> int {
            return value * mercHPpercent / 100;
        });
        view.applyIntTransform(StringVector{ "Dmg-Min", "Dmg-Max", "Dmg/Lvl" }, [mercDampercent](const int value) -> int {
            return value * mercDampercent / 100;
        });
    }

    if (!input.m_env.isLegacy && skillPointLimit != 20) {
        Table&    skillTable = tableSet.tables[TableId::skills];
        TableView skillTableView(skillTable, true);
        for (auto& row : skillTableView) {
            if (row["maxlvl"].toInt() == 20)
                row["maxlvl"].setInt(skillPointLimit);
        }
    }
}

}
