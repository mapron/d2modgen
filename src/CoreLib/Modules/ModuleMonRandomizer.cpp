/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonRandomizer.hpp"
#include "TableUtils.hpp"

#include "Logger.hpp"

#include <algorithm>

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleMonRandomizer>();

constexpr const int s_maxIngameLevel = 110;
const std::string   s_monstersJson   = "data\\hd\\character\\monsters.json";
}

struct ModuleMonRandomizer::MonTypeTable {
    struct MonRecord {
        std::string id;
        int         level;
    };
    struct MonType {
        std::string            baseId;
        std::vector<MonRecord> children;
        int                    count = 0;
    };
    std::map<std::string, MonType>     types;
    std::map<std::string, std::string> id2base;
    std::map<std::string, int>         id2index;
    struct MonCopy {
        int         sourceIndex;
        std::string sourceId;
        std::string newId;
        int         oldLevel;
        int         newLevel;
    };
    using MonCopyList = std::vector<MonCopy>;
    MonCopyList newCopies;

    void add(const TableCell& id, const TableCell& baseId, int level, int index)
    {
        MonType& mt = types[baseId.str];
        mt.baseId   = baseId.str;
        MonRecord r{ id.str, level };
        mt.children.push_back(r);
        mt.count++;
        id2base[id.str]  = baseId.str;
        id2index[id.str] = index;
    }

    std::string pickRandomId(RandomGenerator& rng, const std::string& baseId) const
    {
        const MonType&   mt = types.at(baseId);
        const MonRecord& mr = mt.children[rng(mt.count)];
        return mr.id;
    }

    std::string pickRandomId(RandomGenerator& rng, const std::string& baseId, int targetLevel, int maxDistance = 3)
    {
        assert(types.contains(baseId));
        MonType& mt = types[baseId];
        assert(mt.count > 0);
        std::vector<int> okIndexes;
        for (int i = 0; i < mt.children.size(); ++i) {
            const MonRecord& mr = mt.children[i];
            if (std::abs(mr.level - targetLevel) <= maxDistance)
                okIndexes.push_back(i);
        }
        if (okIndexes.empty()) {
            okIndexes.push_back(mt.children.size());
            auto             i      = rng(mt.count);
            const MonRecord& source = mt.children[i];

            std::string newId = source.id + "_" + std::to_string(targetLevel);
            newCopies.push_back(MonCopy{ id2index[source.id], source.id, newId, source.level, targetLevel });
            mt.children.push_back(MonRecord{ newId, targetLevel });
        }
        const int        index = okIndexes[rng((int) okIndexes.size())];
        const MonRecord& mr    = mt.children[index];
        return mr.id;
    }

    void dump() const
    {
        for (auto& p : types) {
            auto&       type = p.second;
            std::string s    = type.baseId + ": ";
            for (auto& c : type.children)
                s += c.id + "(" + std::to_string(c.level) + "), ";
            Logger() << s;
        }
    }
};

struct ModuleMonRandomizer::TCTable {
    struct TCGroup {
        struct TCRecord {
            std::string id;
            int         level;
        };
        std::vector<TCRecord> records;
    };
    std::map<int, TCGroup>     groups;
    std::map<std::string, int> tc2group;

    void add(const TableCell& id, const TableCell& group, const TableCell& level)
    {
        if (group.isEmpty() || level.isEmpty())
            return;
        TCGroup& g = groups[group.toInt()];
        g.records.push_back(TCGroup::TCRecord{ id.str, level.toInt() });
        tc2group[id.str] = group.toInt();
    }
    std::string promote(const TableCell& tc, const int oldLevel, const int newLevel) const
    {
        if (!tc2group.contains(tc.str) || oldLevel == newLevel)
            return tc.str;
        const TCGroup& g = groups.at(tc2group.at(tc.str));

        std::string res = tc.str;
        for (auto& rec : g.records) {
            if (rec.level > newLevel)
                break;
            res = rec.id;
        }
        return res;
    };
};

struct MonResist {
    enum class Elem
    {
        Physical,
        Magic,
        Fire,
        Light,
        Cold,
        Poison,

    };
    std::map<Elem, int> m_resist;

    void readFromRow(const TableView::RowView& row, const std::string& difficultySuffix)
    {
        m_resist[Elem::Physical] = row["ResDm" + difficultySuffix].toInt();
        m_resist[Elem::Magic]    = row["ResMa" + difficultySuffix].toInt();
        m_resist[Elem::Fire]     = row["ResFi" + difficultySuffix].toInt();
        m_resist[Elem::Light]    = row["ResLi" + difficultySuffix].toInt();
        m_resist[Elem::Cold]     = row["ResCo" + difficultySuffix].toInt();
        m_resist[Elem::Poison]   = row["ResPo" + difficultySuffix].toInt();
    }

    void writeToRow(TableView::RowView& row, const std::string& difficultySuffix) const
    {
        row["ResDm" + difficultySuffix].setInt(m_resist.at(Elem::Physical));
        row["ResMa" + difficultySuffix].setInt(m_resist.at(Elem::Magic));
        row["ResFi" + difficultySuffix].setInt(m_resist.at(Elem::Fire));
        row["ResLi" + difficultySuffix].setInt(m_resist.at(Elem::Light));
        row["ResCo" + difficultySuffix].setInt(m_resist.at(Elem::Cold));
        row["ResPo" + difficultySuffix].setInt(m_resist.at(Elem::Poison));
    }

    void shuffle(RandomGenerator& rng)
    {
        std::map<Elem, Elem>    permutation;
        const std::vector<Elem> commonElems{ Elem::Fire, Elem::Light, Elem::Cold, Elem::Poison };
        std::set<Elem>          remaining{ Elem::Physical, Elem::Magic, Elem::Fire, Elem::Light, Elem::Cold, Elem::Poison };
        // first, Physical and Magic have 20% chance to move to elemental, 20% to swap or 60% to stay the same.
        if (rng(5) == 0) {
            permutation[Elem::Physical] = Elem::Magic;
            permutation[Elem::Magic]    = Elem::Physical;
            remaining.erase(Elem::Physical);
            remaining.erase(Elem::Magic);
        } else {
            auto remainingCopy = remaining;
            remainingCopy.erase(Elem::Physical);
            remainingCopy.erase(Elem::Magic);
            for (Elem elem : { Elem::Physical, Elem::Magic }) {
                if (rng(5) == 0) {
                    auto it = remainingCopy.begin();
                    if (remainingCopy.size() > 1)
                        std::advance(it, rng(remainingCopy.size()));
                    Elem dest         = *it;
                    permutation[elem] = dest;
                    remainingCopy.erase(dest);
                    remaining.erase(dest);
                } else {
                    permutation[elem] = elem;
                    remaining.erase(elem);
                }
            }
        }
        // second, common element permute with the rest
        for (Elem elem : commonElems) {
            auto it = remaining.begin();
            if (remaining.size() > 1)
                std::advance(it, rng(remaining.size()));
            Elem dest         = *it;
            permutation[elem] = dest;
            remaining.erase(dest);
        }
        assert(remaining.size() == 0);
        assert(permutation.size() == 6);
        assert(permutation.count(Elem::Physical));
        assert(permutation.count(Elem::Magic));
        assert(permutation.count(Elem::Fire));
        assert(permutation.count(Elem::Light));
        assert(permutation.count(Elem::Cold));
        assert(permutation.count(Elem::Poison));
        std::map<Elem, int> newResist;
        for (const auto& [src, dest] : permutation)
            newResist[dest] = m_resist[src];

        m_resist = newResist;
    }
};

void ModuleMonRandomizer::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy)
        return;

    output.m_extraJson.insert(s_monstersJson);
}

void ModuleMonRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    auto& tableSet = output.tableSet;

    MonTypeTable typeTable;
    TCTable      tcTable;

    const bool randomizeSpawns      = input.getInt("randomizeSpawns");
    const bool randomizeResistances = input.getInt("randomizeResistances");
    const bool hellResistances      = input.getInt("hellResistances");
    if (randomizeSpawns)
        generateSpawns(output, rng, input, typeTable, tcTable);

    if (randomizeResistances || hellResistances) {
        Table&    table = tableSet.tables[TableId::monstats];
        TableView tableView(table, true);
        for (auto& row : tableView) {
            MonResist norm, night, hell;
            norm.readFromRow(row, "");
            night.readFromRow(row, "(N)");
            hell.readFromRow(row, "(H)");

            if (hellResistances) {
                norm  = hell;
                night = hell;
            }
            if (randomizeResistances) {
                norm.shuffle(rng);
                night.shuffle(rng);
                hell.shuffle(rng);
            }

            norm.writeToRow(row, "");
            night.writeToRow(row, "(N)");
            hell.writeToRow(row, "(H)");
        }
    }
    if (randomizeSpawns && output.jsonFiles.contains(s_monstersJson)) {
        auto& jsonDoc    = output.jsonFiles[s_monstersJson];
        auto& jsonObject = jsonDoc.getMap();
        for (const auto& copy : typeTable.newCopies) {
            const std::string sourceId  = copy.sourceId;
            const std::string newId     = copy.newId;
            const std::string modelName = jsonObject[sourceId].getScalar().toString();
            assert(!modelName.empty());
            jsonObject[newId] = PropertyTreeScalar{ modelName };
        }
    }
}

void ModuleMonRandomizer::generateSpawns(DataContext& output, RandomGenerator& rng, const InputContext& input, MonTypeTable& typeTable, TCTable& tcTable) const
{
    auto& tableSet = output.tableSet;
    {
        Table&    table = tableSet.tables[TableId::monstats];
        TableView tableView(table);
        for (auto& row : tableView) {
            const int normalLevel = row["Level"].toInt();
            if (normalLevel <= 0)
                continue;
            typeTable.add(row["Id"], row["BaseId"], normalLevel, row.index());
        }
    }
    {
        Table&    table = tableSet.tables[TableId::treasureclassex];
        TableView tableView(table);
        for (auto& row : tableView)
            tcTable.add(row["Treasure Class"], row["group"], row["level"]);
    }
    {
        Table&    table = tableSet.tables[TableId::levels];
        TableView tableView(table, true);
        StringSet baseIds, ubaseIds, nonUniqueBaseIds;
        int       cols = 25;
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
                auto base = mapValue(typeTable.id2base, id.str);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (int i = 1; i <= cols; ++i) {
                auto& id = row[argCompat("nmon%1", i)];
                if (id.isEmpty())
                    break;
                auto base = mapValue(typeTable.id2base, id.str);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (int i = 1; i <= cols; ++i) {
                auto& id = row[argCompat("umon%1", i)];
                if (id.isEmpty())
                    break;
                auto base = mapValue(typeTable.id2base, id.str);
                if (!base.empty())
                    ubaseIds.insert(base);
            }
        }
        nonUniqueBaseIds = baseIds;
        {
            StringSet resultData;
            std::set_difference(nonUniqueBaseIds.cbegin(), nonUniqueBaseIds.cend(), ubaseIds.cbegin(), ubaseIds.cend(), std::inserter(resultData, resultData.end()));
            std::swap(nonUniqueBaseIds, resultData);
        }

        StringVector idsList(baseIds.cbegin(), baseIds.cend());
        for (auto& row : tableView) {
            if (row["mon1"].isEmpty())
                continue;
            if (row["Name"] == "Act 5 - Mountain Top")
                continue;
            const int normalLevel = row.hasColumn("MonLvlEx") ? row["MonLvlEx"].toInt() : row["MonLvl1Ex"].toInt();
            StringSet normalSet;
            for (int i = 0; i < maxTypes * 2; ++i) {
                normalSet.insert(idsList[rng(idsList.size())]);
                if (normalSet.size() >= maxTypes)
                    break;
            }

            StringVector normalSetList;
            StringVector uniques;
            for (auto baseId : normalSet) {
                auto id = typeTable.pickRandomId(rng, baseId, normalLevel);
                normalSetList << id;
                if (!nonUniqueBaseIds.contains(baseId))
                    uniques << id;
            }

            for (int i = 1; i <= cols; ++i) {
                row[argCompat("mon%1", i)]  = normalSetList.size() >= i ? normalSetList[i - 1] : "";
                row[argCompat("nmon%1", i)] = normalSetList.size() >= i ? normalSetList[i - 1] : "";
                row[argCompat("umon%1", i)] = uniques.size() >= i ? uniques[i - 1] : "";
            }
            row["NumMon"] = argCompat("%1", spawnedCount);
        }
    }
    {
        Table& table   = tableSet.tables[TableId::monstats];
        table.modified = true;
        MonTypeTable::MonCopyList newCopies;
        auto                      insertNewRows = [&table, &rng, &newCopies, &typeTable, &tcTable]() {
            MonTypeTable::MonCopyList tmpList = std::move(typeTable.newCopies);
            assert(!typeTable.types.empty());
            newCopies.insert(newCopies.end(), tmpList.cbegin(), tmpList.cend());
            int              idCol    = 0;
            int              uidCol   = 1;
            int              levelCol = table.indexOf("Level");
            std::vector<int> minionCols;
            std::vector<int> skillCols;
            std::vector<int> tcCols;
            for (const char* col : { "spawn", "minion1", "minion2" }) {
                int colIndex = table.indexOf(col);
                assert(colIndex > -1);
                minionCols.push_back(colIndex);
            }
            for (int i = 1; i <= 8; ++i) {
                int colIndex = table.indexOf(argCompat("Sk%1lvl", i));
                assert(colIndex > -1);
                skillCols.push_back(colIndex);
            }
            for (int i = 1; i <= 4; ++i) {
                int colIndex = table.indexOf(argCompat("TreasureClass%1", i));
                assert(colIndex > -1);
                tcCols.push_back(colIndex);
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
                    const auto minionBase = mapValue(typeTable.id2base, minion.str);
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
        assert(!typeTable.types.empty());
        insertNewRows();
        insertNewRows();
        insertNewRows();
        typeTable.newCopies = std::move(newCopies);
    }
}

}
