/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContextBase.hpp"

namespace D2ModGen {

struct DataRow_charstats : public RowBase {
    std::string clas;

    struct Item {
        std::string        code;
        std::optional<int> count;
        std::string        loc;
    };
    std::vector<Item> items;

    std::vector<std::string> skills;

    std::optional<int> statPerLevel;
    std::optional<int> skillsPerLevel;

    std::vector<std::string> getColumns() const;
    void                     serialize();

private:
    static const size_t maxItems  = 10;
    static const size_t maxSkills = 10;
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
    void                     serialize();
};
struct DataRow_experience : public RowBase {
    std::optional<int> expRatio;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_gamble : public RowBase {
    std::string name;
    std::string code;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_hireling : public RowBase {
    std::optional<int> hp;
    std::optional<int> hp_lvl;
    std::optional<int> dmg_min;
    std::optional<int> dmg_max;
    std::optional<int> dmg_lvl;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_itemstatcost : public RowBase {
    std::optional<int> multiply;
    std::optional<int> add;

    std::vector<std::string> getColumns() const;
    void                     serialize();
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

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();

private:
    static const size_t maxTypes = 25;
};

struct DataRow_monlvl : public RowBase {
    std::array<int, 6> TH; // to hit, attack rating
    std::array<int, 6> AC; // armor class, defence
    std::array<int, 6> HP;
    std::array<int, 6> DM;
    std::array<int, 6> XP;
    int                level = 0;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_monstats : public RowBase {
    std::array<std::optional<int>, 3> resFi, resLi, resCo, resPo, resMa, resDm;

    std::optional<bool> enabled, isspawn, killable, npc, boss;

    std::optional<int> level, minGrp, maxGrp;

    std::string id;
    std::string baseId;
    int         hcIdx = 0;

    std::array<std::array<std::string, 3>, 4> tc;

    std::vector<std::string*> nonEmptyTCs()
    {
        std::vector<std::string*> result;
        for (auto& diffTC : tc) {
            for (std::string& tcs : diffTC) {
                if (!tcs.empty())
                    result.push_back(&tcs);
            }
        }
        return result;
    }

    std::vector<int>           skLevels;
    std::array<std::string, 3> minionIds;

    bool isEnabled() const { return enabled.value_or(false); }
    bool isSpawn() const { return isspawn.value_or(false); }
    bool isKillable() const { return killable.value_or(false); }
    bool isNpc() const { return npc.value_or(false); }
    bool isBoss() const { return boss.value_or(false); }

    bool isRegularMob() const
    {
        return isEnabled() && isSpawn() && isKillable() && !isNpc() && !isBoss() && level.has_value();
    }

    std::vector<std::string> getColumns() const;
    void                     serialize();

    std::map<std::string, std::string> getColumnAliases() const;

private:
    static const size_t tcCount     = 4 + 9;
    static const size_t skCount     = 8;
    static const size_t minionCount = 3;
};

struct DataRow_skilldesc : public RowBase {
    std::string        skilldesc;
    std::optional<int> skillPage;
    std::optional<int> skillRow;
    std::optional<int> skillColumn;
    std::optional<int> listRow;

    std::vector<std::string> getColumns() const;
    void                     serialize();
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
    Character                classSpecific = Character::none;

    bool canBeOskill = true;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
using DataRow_skills_ptr = const DataRow_skills*;
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
    void                     serialize();

private:
    static const size_t maxItems = 10;
};

}
