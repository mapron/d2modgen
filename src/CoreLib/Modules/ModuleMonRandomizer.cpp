/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMonRandomizer.hpp"

#include "Logger.hpp"

#include <algorithm>

namespace D2ModGen {

namespace {
constexpr const int s_maxIngameLevel = 110;
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
    MonCopyList allCopies;

    void add(const std::string& id, const std::string& baseId, int level, int index)
    {
        MonType& mt = types[baseId];
        mt.baseId   = baseId;
        MonRecord r{ id, level };
        mt.children.push_back(r);
        mt.count++;
        id2base[id]  = baseId;
        id2index[id] = index;
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

    void add(const std::string& id, int group, int level)
    {
        if (!group || !level)
            return;
        TCGroup& g = groups[group];
        g.records.push_back(TCGroup::TCRecord{ id, level });
        tc2group[id] = group;
    }
    std::string promote(const std::string& tc, const int oldLevel, const int newLevel) const
    {
        if (!tc2group.contains(tc) || oldLevel == newLevel)
            return tc;
        const TCGroup& g = groups.at(tc2group.at(tc));

        std::string res = tc;
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

    void readFromRow(const DataRow_monstats& row, size_t i)
    {
        m_resist[Elem::Physical] = row.resDm[i].value_or(0);
        m_resist[Elem::Magic]    = row.resMa[i].value_or(0);
        m_resist[Elem::Fire]     = row.resFi[i].value_or(0);
        m_resist[Elem::Light]    = row.resLi[i].value_or(0);
        m_resist[Elem::Cold]     = row.resCo[i].value_or(0);
        m_resist[Elem::Poison]   = row.resPo[i].value_or(0);
    }

    void writeToRow(DataRow_monstats& row, size_t i) const
    {
        row.resDm[i] = m_resist.at(Elem::Physical);
        row.resMa[i] = m_resist.at(Elem::Magic);
        row.resFi[i] = m_resist.at(Elem::Fire);
        row.resLi[i] = m_resist.at(Elem::Light);
        row.resCo[i] = m_resist.at(Elem::Cold);
        row.resPo[i] = m_resist.at(Elem::Poison);
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

ModuleMonRandomizer::ModuleMonRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "hellResistances", Mernel::PropertyTreeScalar{ false } },
          { "maxTypes", Mernel::PropertyTreeScalar{ 25 } },
          { "randomizeResistances", Mernel::PropertyTreeScalar{ false } },
          { "randomizeSpawns", Mernel::PropertyTreeScalar{ true } },
          { "spawnedCount", Mernel::PropertyTreeScalar{ 5 } },
      })
{}

void ModuleMonRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    MonTypeTable typeTable;
    TCTable      tcTable;

    const bool randomizeSpawns      = input.getInt("randomizeSpawns");
    const bool randomizeResistances = input.getInt("randomizeResistances");
    const bool hellResistances      = input.getInt("hellResistances");
    if (randomizeSpawns)
        generateSpawns(output, rng, input, typeTable, tcTable);

    if (randomizeResistances || hellResistances) {
        for (auto& row : output.monstats.rows) {
            std::array<MonResist, 3> r;
            for (size_t i = 0; i < 3; ++i)
                r[i].readFromRow(row, i);

            if (hellResistances) {
                r[0] = r[2];
                r[1] = r[2];
            }
            if (randomizeResistances) {
                for (size_t i = 0; i < 3; ++i)
                    r[i].shuffle(rng);
            }
            for (size_t i = 0; i < 3; ++i)
                r[i].writeToRow(row, i);
        }
    }
    if (randomizeSpawns && output.jsonFiles.contains(JsonId::Monsters)) {
        auto& jsonDoc    = output.jsonFiles[JsonId::Monsters];
        auto& jsonObject = jsonDoc.getMap();
        for (const auto& copy : typeTable.allCopies) {
            const std::string sourceId  = copy.sourceId;
            const std::string newId     = copy.newId;
            const std::string modelName = jsonObject[sourceId].getScalar().toString();
            assert(!modelName.empty());
            jsonObject[newId] = Mernel::PropertyTreeScalar{ modelName };
        }
    }
}

void ModuleMonRandomizer::generateSpawns(DataContext& output, RandomGenerator& rng, const InputContext& input, MonTypeTable& typeTable, TCTable& tcTable) const
{
    {
        for (int index = -1; auto& row : output.monstats.rows) {
            index++;
            if (!row.isRegularMob())
                continue;
            typeTable.add(row.id, row.baseId, row.level.value(), index);
        }
    }
    {
        for (auto& row : output.treasureclassex.rows)
            tcTable.add(row.name, row.group.value_or(0), row.level.value_or(0));
    }
    {
        StringSet baseIds, ubaseIds, nonUniqueBaseIds;

        const int spawnedCount = input.getInt("spawnedCount");
        const int maxTypes     = input.getInt("maxTypes");
        for (auto& row : output.levels.rows) {
            if (row.name == "Act 5 - Mountain Top")
                continue;

            for (auto& id : row.mon) {
                auto base = mapValue(typeTable.id2base, id);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (auto& id : row.nmon) {
                auto base = mapValue(typeTable.id2base, id);
                if (!base.empty())
                    baseIds.insert(base);
            }
            for (auto& id : row.umon) {
                auto base = mapValue(typeTable.id2base, id);
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
        for (auto& row : output.levels.rows) {
            if (row.mon.empty())
                continue;
            if (row.name == "Act 5 - Mountain Top")
                continue;
            const int normalLevel = row.monLvlEx[0].value_or(0);
            StringSet normalSet;
            for (int i = 0; i < maxTypes * 2; ++i) {
                normalSet.insert(idsList[rng(idsList.size())]);
                if (normalSet.size() >= maxTypes)
                    break;
            }

            StringVector normalSetList;
            StringVector uniques;
            for (auto&& baseId : normalSet) {
                auto id = typeTable.pickRandomId(rng, baseId, normalLevel);
                normalSetList << id;
                if (!nonUniqueBaseIds.contains(baseId))
                    uniques << id;
            }
            row.mon  = normalSetList;
            row.nmon = normalSetList;
            row.umon = uniques;

            row.numMon = spawnedCount;
        }
    }
    for (size_t i = 0; i < 5 && generateCopies(output, rng, input, typeTable, tcTable); ++i) {
    }
}

bool ModuleMonRandomizer::generateCopies(DataContext& output, RandomGenerator& rng, const InputContext& input, MonTypeTable& typeTable, TCTable& tcTable) const
{
    auto copies = typeTable.newCopies;
    typeTable.newCopies.clear();
    for (const auto& copy : copies) {
        auto newRow  = output.monstats.rows[copy.sourceIndex];
        newRow.id    = copy.newId;
        newRow.level = copy.newLevel;
        newRow.hcIdx = int(output.monstats.rows.size());
        for (auto& minion : newRow.minionIds) {
            if (minion.empty())
                continue;
            const auto minionBase = mapValue(typeTable.id2base, minion);
            if (minionBase.empty()) {
                minion = "";
                continue;
            }
            auto id = typeTable.pickRandomId(rng, minionBase, copy.newLevel);
            minion  = id;
        }
        if (copy.oldLevel > copy.newLevel) {
            for (int& skill : newRow.skLevels) {
                const int newLevel = copy.newLevel * skill / copy.oldLevel;
                skill              = (std::max(1, newLevel));
            }
        }
        for (auto* tc : newRow.nonEmptyTCs()) {
            *tc = tcTable.promote(*tc, copy.oldLevel, copy.newLevel);
        }

        output.monstats.rows.push_back(std::move(newRow));
    }
    typeTable.allCopies.insert(typeTable.allCopies.end(), copies.cbegin(), copies.cend());
    return !typeTable.newCopies.empty(); //pickRandomId can add more.
}

}
