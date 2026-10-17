/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "Storage/IStorage.hpp"
#include "MernelPlatform/PropertyTree.hpp"
#include "TableId.hpp"
#include "AttributeHelper.hpp"

#include <optional>
#include <unordered_map>

namespace D2ModGen {

template<class Row>
struct DataTable {
    std::vector<Row> rows;

    std::vector<std::string> columnsOriginal;
    std::vector<std::string> columnsUsed;

    std::vector<size_t> indicesUsed;
    std::vector<size_t> indicesUnused;

    std::unordered_map<std::string, const Row*> index;

    bool valid = false;

    template<class F, class FieldType, class Row1>
    void applyTransform(FieldType Row1::* m, F&& t)
    {
        for (Row& row : rows)
            t(row.*m);
    }
    void applyIntTransform(std::optional<int> Row::* m, int mult, int den = 100)
    {
        applyTransform(m, [mult, den](std::optional<int>& data) {
            if (!data.has_value())
                return;
            data = data.value() * mult / den;
        });
    }
    void makeIndex(std::string Row::* m)
    {
        index.clear();
        for (auto& row : rows)
            index[row.*m] = &row;
    }
};

struct AffixProp {
    std::string        code;
    std::string        par;
    std::optional<int> min;
    std::optional<int> max;
    std::optional<int> chance;

    bool hasRange() const { return min.has_value() && max.has_value(); }
};
class AffixPropList : public std::vector<AffixProp> {
    size_t m_limit = 0;

public:
    AffixPropList() = default;
    AffixPropList(size_t limit)
        : m_limit(limit)
    {}

    size_t getLimit() const { return m_limit; };
};

struct RowBase {
    bool                     _ser_read      = false;
    size_t                   _ser_cellIndex = 0;
    std::vector<std::string> _ser_unusedCells;
    std::vector<std::string> _ser_usedCells;

    void serf(int& data);
    void serf(std::optional<int>& data);
    void serf(std::optional<bool>& data);
    void serf(std::string& data);

    template<class Item, class F1, class F2>
    void serArray(std::vector<Item>& data, size_t size, F1&& itemCallback, F2&& isValidCheck)
    {
        if (_ser_read) {
            data.clear();
            data.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                Item item;
                itemCallback(item);
                if (isValidCheck(item))
                    data.push_back(std::move(item));
            }
        } else {
            data.resize(size);
            for (size_t i = 0; i < size; ++i) {
                itemCallback(data[i]);
            }
        }
    }

    void serf(AffixPropList& data, bool useChance = false)
    {
        serArray(data, data.getLimit(), [this, useChance](AffixProp& item) {
            serf(item.code);
            serf(item.par);
            serf(item.min);
            serf(item.max);
            if (useChance)
                serf(item.chance); }, [](AffixProp& item) { return !item.code.empty(); });
    }

    void serf(std::vector<std::string>& data, size_t size)
    {
        serArray(data, size, [this](std::string& item) { serf(item); }, [](std::string& item) { return !item.empty(); });
    }
    void serf(std::vector<int>& data, size_t size)
    {
        serArray(data, size, [this](int& item) { serf(item); }, [](int& item) { return item > 0; });
    }
};

struct RowBaseAffixes : public RowBase {
    std::optional<int>  lvl;
    std::optional<int>  maxlevel;
    std::optional<int>  lvlreq;
    std::optional<bool> spawnable;
    AffixPropList       affixes;

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    int getSpawnableLevel() const { return spawnable.value_or(false) ? lvl.value_or(0) : 0; }

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxAffixes = 3;
};

struct DataRow_weapons : public RowBase {
    std::optional<int>  lvl;
    std::optional<int>  lvlreq;
    std::optional<int>  reqstr;
    std::optional<int>  reqdex;
    std::optional<bool> showLevel;

    std::optional<int> mindam;
    std::optional<int> maxdam;

    std::string code;
    std::string type;

    std::vector<std::string> getColumns() const;

    void ser();
};
struct DataRow_armor : public RowBase {
    std::optional<int>  lvl;
    std::optional<int>  lvlreq;
    std::optional<int>  reqstr;
    std::optional<int>  reqdex;
    std::optional<bool> showLevel;

    std::optional<int> minac;
    std::optional<int> maxac;

    std::string code;
    std::string type;

    std::vector<std::string> getColumns() const;

    void ser();
};
struct DataRow_automagic : public RowBaseAffixes {
};
struct DataRow_charstats : public RowBase {
    std::string clas;

    struct Item {
        std::string        code;
        std::optional<int> count;
        std::string        loc;
        std::string        quality;
    };
    std::vector<Item> items;

    std::vector<std::string> skills;

    std::optional<int> statPerLevel;
    std::optional<int> skillsPerLevel;

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxItems  = 10;
    static const size_t maxSkills = 10;
};
struct DataRow_cubemain : public RowBase {
    AffixPropList            affixes_a{ 5 };
    AffixPropList            affixes_b{ 5 };
    AffixPropList            affixes_c{ 5 };
    std::vector<std::string> inputs;
    std::string              output;

    std::string         description;
    std::optional<bool> enabled;
    std::optional<int>  version;
    std::optional<int>  op;
    std::optional<int>  numinputs;
    int                 eol = 0;

    std::optional<int> ilvl;
    std::optional<int> plvl;
    std::optional<int> lvl;

    auto affixLists() const { return std::array{ &affixes_a, &affixes_b, &affixes_c }; }
    auto affixLists() { return std::array{ &affixes_a, &affixes_b, &affixes_c }; }

    bool isCrafted() const { return output == "\"usetype,crf\""; }
    bool hasLvls() const { return ilvl.has_value() || plvl.has_value() || lvl.has_value(); }

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxInputs = 7;
};
struct DataRow_difficultylevels : public RowBase {
    std::string        name;
    std::optional<int> resistPenalty;
    std::optional<int> gambleRare;
    std::optional<int> gambleSet;
    std::optional<int> gambleUnique;
    std::optional<int> gambleUber;
    std::optional<int> gambleUltra;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_experience : public RowBase {
    std::optional<int> expRatio;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_gamble : public RowBase {
    std::string name;
    std::string code;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_gems : public RowBase {
    AffixPropList affixes_weapon{ 3 };
    AffixPropList affixes_helm{ 3 };
    AffixPropList affixes_shield{ 3 };

    auto affixLists() const { return std::array{ &affixes_weapon, &affixes_helm, &affixes_shield }; }
    auto affixLists() { return std::array{ &affixes_weapon, &affixes_helm, &affixes_shield }; }

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_hireling : public RowBase {
    std::optional<int> hp;
    std::optional<int> hp_lvl;
    std::optional<int> dmg_min;
    std::optional<int> dmg_max;
    std::optional<int> dmg_lvl;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_itemstatcost : public RowBase {
    std::optional<int> multiply;
    std::optional<int> add;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_itemtypes : public RowBase {
    std::string code;
    std::string itemType;
    std::string equiv1;
    std::string equiv2;

    std::optional<bool>               throwable;
    std::optional<bool>               repair;
    std::optional<bool>               shoots;
    std::array<std::optional<int>, 3> sockets;

    std::set<AttributeFlag> flags;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_levels : public RowBase {
    std::array<std::optional<int>, 3> monUMin, monUMax, monDen;
    std::array<std::optional<int>, 3> monLvlEx;
    std::optional<int>                numMon;
    std::vector<std::string>          mon, nmon, umon;

    std::string name;

    bool hasSpawns() const
    {
        return monUMin[0].has_value() && monUMin[1].has_value() && monUMin[2].has_value();
    }

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxTypes = 25;
};
struct DataRow_magicprefix : public RowBaseAffixes {
};
struct DataRow_magicsuffix : public RowBaseAffixes {
};
struct DataRow_misc : public RowBase {
    std::string        code;
    std::string        type;
    std::optional<int> maxstack;
    std::optional<int> lvlreq;
    std::optional<int> lvl;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_monlvl : public RowBase {
    std::array<int, 6> TH; // to hit, attack rating
    std::array<int, 6> AC; // armor class, defence
    std::array<int, 6> HP;
    std::array<int, 6> DM;
    std::array<int, 6> XP;
    int                level = 0;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_monstats : public RowBase {
    std::array<std::optional<int>, 3> resFi, resLi, resCo, resPo, resMa, resDm;

    std::optional<bool> spawn, killable, npc, boss;

    std::optional<int> level, minGrp, maxGrp;

    std::string id;
    std::string baseId;
    int         hcIdx = 0;

    std::vector<std::string> tc;
    std::vector<int>         skLevels;
    std::vector<std::string> minionIds;

    bool isSpawn() const { return spawn.has_value() && spawn.value(); }
    bool isKillable() const { return killable.has_value() && killable.value(); }
    bool isNpc() const { return npc.has_value() && npc.value(); }
    bool isBoss() const { return boss.has_value() && boss.value(); }

    bool isRegularMob() const
    {
        return isSpawn() && isKillable() && !isNpc() && !isBoss() && level.has_value();
    }

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t tcCount     = 4 + 9;
    static const size_t skCount     = 8;
    static const size_t minionCount = 3;
};

struct DataRow_runes : public RowBase {
    int                      lvl = 1; // calculated.
    std::vector<std::string> runeCodes;
    AffixPropList            affixes{ 7 };
    std::optional<bool>      complete;

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxRunes = 6;
};
struct DataRow_setitems : public RowBase {
    std::optional<int> lvl;
    std::optional<int> lvlreq;

    AffixPropList affixes_base{ 9 };
    AffixPropList affixes_partial1{ 5 };
    AffixPropList affixes_partial2{ 5 };

    auto affixLists() const { return std::array{ &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    auto affixLists() { return std::array{ &affixes_base, &affixes_partial1, &affixes_partial2 }; }

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_sets : public RowBase {
    std::optional<int> lvl;

    AffixPropList affixes_full{ 8 };
    AffixPropList affixes_partial1{ 4 };
    AffixPropList affixes_partial2{ 4 };

    auto affixLists() const { return std::array{ &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    auto affixLists() { return std::array{ &affixes_full, &affixes_partial1, &affixes_partial2 }; }

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_skilldesc : public RowBase {
    std::string        skilldesc;
    std::optional<int> skillPage;
    std::optional<int> skillRow;
    std::optional<int> skillColumn;
    std::optional<int> listRow;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_skills : public RowBase {
    std::string         skill;
    std::optional<int>  maxlvl;
    std::optional<int>  lvlreq;
    std::optional<bool> inTown;
    std::optional<int>  costMult;
    std::optional<int>  costAdd;

    std::optional<int> eLen;
    std::string        eType;
    std::optional<int> hitShift;

    std::vector<std::string> reqskills;
    std::string              charclass;

    std::vector<std::string> getColumns() const;
    void                     ser();
};
struct DataRow_treasureclassex : public RowBase {
    struct Item {
        std::string        tc;
        std::optional<int> prob;
    };
    std::vector<Item> items;

    std::string        name;
    std::optional<int> k_unique, k_set, k_rare, k_magic;
    std::optional<int> noDrop;
    std::optional<int> group;
    std::optional<int> picks;
    std::optional<int> level;

    int getDropSomethingProb() const;

    std::vector<std::string> getColumns() const;
    void                     ser();

private:
    static const size_t maxItems = 10;
};
struct DataRow_uniqueitems : public RowBase {
    std::optional<int> rarity;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::optional<int> carry1;

    AffixPropList affixes{ 12 };

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    std::vector<std::string> getColumns() const;
    void                     ser();
};

struct DataContext {
    DataTable<DataRow_weapons>          weapons;
    DataTable<DataRow_armor>            armor;
    DataTable<DataRow_automagic>        automagic;
    DataTable<DataRow_charstats>        charstats;
    DataTable<DataRow_cubemain>         cubemain;
    DataTable<DataRow_difficultylevels> difficultylevels;
    DataTable<DataRow_experience>       experience;
    DataTable<DataRow_gamble>           gamble;
    DataTable<DataRow_gems>             gems;
    DataTable<DataRow_hireling>         hireling;
    DataTable<DataRow_itemstatcost>     itemstatcost;
    DataTable<DataRow_itemtypes>        itemtypes;
    DataTable<DataRow_levels>           levels;
    DataTable<DataRow_magicprefix>      magicprefix;
    DataTable<DataRow_magicsuffix>      magicsuffix;
    DataTable<DataRow_misc>             misc;
    DataTable<DataRow_monlvl>           monlvl;
    DataTable<DataRow_monstats>         monstats;
    DataTable<DataRow_runes>            runes;
    DataTable<DataRow_setitems>         setitems;
    DataTable<DataRow_sets>             sets;
    DataTable<DataRow_skilldesc>        skilldesc;
    DataTable<DataRow_skills>           skills;
    DataTable<DataRow_treasureclassex>  treasureclassex;
    DataTable<DataRow_uniqueitems>      uniqueitems;

    std::map<JsonId, Mernel::PropertyTree> jsonFiles;

    int64_t dataVersion = 0;

    DataContext();
    ~DataContext();

    bool parseData(const IStorage::StoredData& data);
    bool serializeData(IStorage::StoredData& data) const;

private:
    template<class Row>
    bool parseTable(DataTable<Row>& table, TableId tableId, const IStorage::StoredData& data);

    template<class Row>
    bool serializeTable(const DataTable<Row>& table, TableId tableId, IStorage::StoredData& data) const;
};

}
