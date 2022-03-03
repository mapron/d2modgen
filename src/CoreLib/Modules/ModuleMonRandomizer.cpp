/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonRandomizer.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleMonRandomizer>();

constexpr const int s_maxIngameLevel = 110;
const QString       s_monstersJson   = "data\\hd\\character\\monsters.json";

struct MotTypeTable {
    struct MonRecord {
        std::string id;
        int         level;
    };
    struct MonType {
        std::string      baseId;
        QList<MonRecord> children;
        int              count = 0;
    };
    QMap<std::string, MonType>     types;
    QMap<std::string, std::string> id2base;
    QMap<std::string, int>         id2index;
    struct MonCopy {
        int         sourceIndex;
        std::string sourceId;
        std::string newId;
        int         oldLevel;
        int         newLevel;
    };
    using MonCopyList = QList<MonCopy>;
    MonCopyList newCopies;

    void add(const TableCell& id, const TableCell& baseId, int level, int index)
    {
        MonType& mt = types[baseId.str];
        mt.baseId   = baseId.str;
        MonRecord r{ id.str, level };
        mt.children << r;
        mt.count++;
        id2base[id.str]  = baseId.str;
        id2index[id.str] = index;
    }

    std::string pickRandomId(QRandomGenerator& rng, const std::string& baseId) const
    {
        const MonType&   mt = types[baseId];
        const MonRecord& mr = mt.children[rng.bounded(mt.count)];
        return mr.id;
    }

    std::string pickRandomId(QRandomGenerator& rng, const std::string& baseId, int targetLevel, int maxDistance = 3)
    {
        assert(types.contains(baseId));
        MonType& mt = types[baseId];
        assert(mt.count > 0);
        QList<int> okIndexes;
        for (int i = 0; i < mt.children.size(); ++i) {
            const MonRecord& mr = mt.children[i];
            if (std::abs(mr.level - targetLevel) <= maxDistance)
                okIndexes << i;
        }
        if (okIndexes.empty()) {
            okIndexes << mt.children.size();
            const MonRecord& source = mt.children[rng.bounded(mt.count)];

            std::string newId = source.id + "_" + std::to_string(targetLevel);
            mt.children << MonRecord{ newId, targetLevel };
            newCopies << MonCopy{ id2index[source.id], source.id, newId, source.level, targetLevel };
        }
        const int        index = okIndexes[rng.bounded(okIndexes.size())];
        const MonRecord& mr    = mt.children[index];
        return mr.id;
    }

    void dump()
    {
        for (auto& type : types) {
            std::string s = type.baseId + ": ";
            for (auto& c : type.children)
                s += c.id + "(" + std::to_string(c.level) + "), ";
            qDebug() << s.c_str();
        }
    }
};

struct TCTable {
    struct TCGroup {
        struct TCRecord {
            std::string id;
            int         level;
        };
        QList<TCRecord> records;
    };
    QMap<int, TCGroup>     groups;
    QMap<std::string, int> tc2group;

    void add(const TableCell& id, const TableCell& group, const TableCell& level)
    {
        if (group.isEmpty() || level.isEmpty())
            return;
        TCGroup& g = groups[group.toInt()];
        g.records << TCGroup::TCRecord{ id.str, level.toInt() };
        tc2group[id.str] = group.toInt();
    }
    std::string promote(const TableCell& tc, const int oldLevel, const int newLevel) const
    {
        if (!tc2group.contains(tc.str) || oldLevel == newLevel)
            return tc.str;
        const TCGroup& g = groups[tc2group[tc.str]];

        std::string res = tc.str;
        for (auto& rec : g.records) {
            if (rec.level > newLevel)
                break;
            res = rec.id;
        }
        return res;
    };
};

}

PropertyTreeScalarMap ModuleMonRandomizer::defaultValues() const
{
    return {
        { "spawnedCount", 5 },
        { "maxTypes", 25 },
    };
}

IModule::UiControlHintMap ModuleMonRandomizer::uiHints() const
{
    return {
        { "spawnedCount", UiControlHintSliderMinMax(2, 10) },
        { "maxTypes", UiControlHintSliderMinMax(3, 25) },
    };
}

void ModuleMonRandomizer::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy)
        return;

    output.m_extraJson << s_monstersJson;
}

void ModuleMonRandomizer::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto& tableSet = output.tableSet;

    MotTypeTable typeTable;
    TCTable      tcTable;
    {
        Table&    table = tableSet.tables["monstats"];
        TableView tableView(table);
        for (auto& row : tableView) {
            const int normalLevel = row["Level"].toInt();
            if (normalLevel <= 0)
                continue;
            typeTable.add(row["Id"], row["BaseId"], normalLevel, row.index());
        }
    }
    {
        Table&    table = tableSet.tables["treasureclassex"];
        TableView tableView(table);
        for (auto& row : tableView)
            tcTable.add(row["Treasure Class"], row["group"], row["level"]);
    }
    {
        Table&            table = tableSet.tables["levels"];
        TableView         tableView(table, true);
        std::set<std::string> baseIds, ubaseIds, nonUniqueBaseIds;
        int               cols = 25;
        for (const char* colTpl : { "mon%1", "nmon%1", "umon%1" }) {
            for (int i = 1; i <= cols; ++i)
                if (!tableView.hasColumn(argCompat(colTpl, i))) {
                    cols = i - 1;
                    break;
                }
        }
        const int spawnedCount = input.getInt("spawnedCount");
        const int maxTypes     = std::min(input.getInt("maxTypes"), cols);
        for (auto& row : tableView) {
            if (row["Name"] == "Act 5 - Mountain Top")
                continue;
            for (int i = 1; i <= cols; ++i) {
                auto& id = row[argCompat("mon%1", i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id.str);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (int i = 1; i <= cols; ++i) {
                auto& id = row[argCompat("nmon%1", i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id.str);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (int i = 1; i <= cols; ++i) {
                auto& id = row[argCompat("umon%1", i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id.str);
                if (!base.empty())
                    ubaseIds.insert(base);
            }
        }
        nonUniqueBaseIds = baseIds;
        
        //nonUniqueBaseIds -= ubaseIds;
        {
            std::set<std::string> resultData;
            std::set_difference(nonUniqueBaseIds.cbegin(), nonUniqueBaseIds.cend(),
                                ubaseIds.cbegin(), ubaseIds.cend(), 
                                std::inserter(resultData, resultData.end()));
            std::swap(nonUniqueBaseIds, resultData);
        }
        //qDebug() << "non uniques:" << nonUniqueBaseIds;
        //typeTable.removeUnused(baseIds);

        QList<std::string> idsList(baseIds.cbegin(), baseIds.cend());
        for (auto& row : tableView) {
            if (row["mon1"].isEmpty())
                continue;
            if (row["Name"] == "Act 5 - Mountain Top")
                continue;
            const int         normalLevel = row.hasColumn("MonLvlEx") ? row["MonLvlEx"].toInt() : row["MonLvl1Ex"].toInt();
            std::set<std::string> normalSet;
            for (int i = 0; i < maxTypes * 2; ++i) {
                normalSet.insert(idsList[rng.bounded(idsList.size())]);
                if (normalSet.size() >= maxTypes)
                    break;
            }

            QList<std::string> normalSetList;
            QList<std::string> uniques;
            for (auto baseId : normalSet) {
                auto id = typeTable.pickRandomId(rng, baseId, normalLevel);
                normalSetList << id;
                if (!nonUniqueBaseIds.contains(baseId))
                    uniques << id;
            }

            for (int i = 1; i <= cols; ++i) {
                row[argCompat("mon%1", i)]  = normalSetList.value(i - 1);
                row[argCompat("nmon%1", i)] = normalSetList.value(i - 1);
                row[argCompat("umon%1", i)] = uniques.value(i - 1);
            }
            row["NumMon"] = argCompat("%1", spawnedCount);
        }
    }
    {
        Table& table   = tableSet.tables["monstats"];
        table.modified = true;
        MotTypeTable::MonCopyList newCopies;
        auto                      insertNewRows = [&table, &rng, &newCopies, &typeTable, &tcTable]() {
            MotTypeTable::MonCopyList tmpList = std::move(typeTable.newCopies);
            assert(!typeTable.types.isEmpty());
            newCopies << tmpList;
            int        idCol    = 0;
            int        uidCol   = 1;
            int        levelCol = table.indexOf("Level");
            QList<int> minionCols;
            QList<int> skillCols;
            QList<int> tcCols;
            for (const char* col : { "spawn", "minion1", "minion2" }) {
                int colIndex = table.indexOf(col);
                assert(colIndex > -1);
                minionCols << colIndex;
            }
            for (int i = 1; i <= 8; ++i) {
                int colIndex = table.indexOf(argCompat("Sk%1lvl", i));
                assert(colIndex > -1);
                skillCols << colIndex;
            }
            for (int i = 1; i <= 4; ++i) {
                int colIndex = table.indexOf(argCompat("TreasureClass%1", i));
                assert(colIndex > -1);
                tcCols << colIndex;
            }

            for (const auto& copy : tmpList) {
                auto newRow           = table.rows[copy.sourceIndex];
                newRow.data[idCol]    = copy.newId;
                newRow.data[levelCol] = argCompat("%1", copy.newLevel);
                newRow.data[uidCol]   = argCompat("%1", table.rows.size());
                for (int minionCol : minionCols) {
                    auto& minion = newRow.data[minionCol];
                    if (minion.isEmpty())
                        continue;
                    const auto minionBase = typeTable.id2base.value(minion.str);
                    if (minionBase.empty()) {
                        minion.str = "";
                        continue;
                    }
                    auto id                = typeTable.pickRandomId(rng, minionBase, copy.newLevel);
                    newRow.data[minionCol] = id;
                }
                if (copy.oldLevel > copy.newLevel) {
                    for (int skillCol : skillCols) {
                        auto& cell = newRow.data[skillCol];
                        if (cell.isEmpty())
                            break;
                        const int newLevel = copy.newLevel * cell.toInt() / copy.oldLevel;
                        cell.setInt(std::max(1, newLevel));
                    }
                }
                for (int tcCol : tcCols) {
                    auto& cell = newRow.data[tcCol];
                    if (cell.isEmpty())
                        continue;
                    cell = tcTable.promote(cell, copy.oldLevel, copy.newLevel);
                }

                table.rows.push_back(std::move(newRow));
            }
        };
        assert(!typeTable.types.isEmpty());
        insertNewRows();
        insertNewRows();
        insertNewRows();
        typeTable.newCopies = std::move(newCopies);
    }
    if (output.jsonFiles.contains(s_monstersJson)) {
        auto& jsonDoc    = output.jsonFiles[s_monstersJson];
        auto& jsonObject = jsonDoc.getMap();
        for (const auto& copy : typeTable.newCopies) {
            const std::string sourceId  = copy.sourceId;
            const std::string newId     = copy.newId;
            const std::string modelName = jsonObject[sourceId].toString();
            assert(!modelName.empty());
            jsonObject[newId] = PropertyTreeScalar{ modelName };
        }
    }
}

}
