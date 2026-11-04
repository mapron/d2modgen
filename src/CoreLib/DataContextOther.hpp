/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContextBase.hpp"
#include "Storage/IStorage.hpp"

namespace D2ModGen {

struct DataRow_charstats : public RowBase {
    static constexpr const TableId s_tableId{ TableId::charstats };

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
    std::optional<int> hpPerLevel;   // in 1/4
    std::optional<int> manaPerLevel; // in 1/4
    std::optional<int> hpPotEfficacy;
    std::optional<int> manaPotEfficacy;

    std::vector<std::string> getColumns() const;
    void                     serialize();

private:
    static const size_t maxItems  = 10;
    static const size_t maxSkills = 10;
};
struct DataRow_difficultylevels : public RowBase {
    static constexpr const TableId s_tableId{ TableId::difficultylevels };

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
    static constexpr const TableId s_tableId{ TableId::experience };

    std::optional<int> expRatio;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_gamble : public RowBase {
    static constexpr const TableId s_tableId{ TableId::gamble };

    std::string name;
    std::string code;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_hireling : public RowBase {
    static constexpr const TableId s_tableId{ TableId::hireling };

    std::optional<int> hp;
    std::optional<int> hp_lvl;
    std::optional<int> dmg_min;
    std::optional<int> dmg_max;
    std::optional<int> dmg_lvl;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_itemstatcost : public RowBase {
    static constexpr const TableId s_tableId{ TableId::itemstatcost };

    std::optional<int> multiply;
    std::optional<int> add;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_levels : public RowBase {
    static constexpr const TableId s_tableId{ TableId::levels };

    int id = -1;

    std::array<std::optional<int>, 3> monUMin, monUMax, monDen;
    std::array<std::optional<int>, 3> monLvlEx;
    std::optional<int>                numMon;
    std::vector<std::string>          mon, nmon, umon;

    std::string name;

    bool useForRando = false;

    bool hasSpawns() const
    {
        return monUMax[0].has_value() || monUMax[1].has_value() || monUMax[2].has_value();
    }

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();

private:
    static const size_t maxTypes = 25;
};

struct DataRow_monlvl : public RowBase {
    static constexpr const TableId s_tableId{ TableId::monlvl };

    std::array<int, 6> TH; // to hit, attack rating
    std::array<int, 6> AC; // armor class, defence
    std::array<int, 6> HP;
    std::array<int, 6> DM;
    std::array<int, 6> XP;
    int                level = 0;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

enum class AnimationMode
{
    Undefined = -1,
    DT,
    NU,
    WL,
    GH,
    A1,
    A2,
    BL,
    SC,
    S1,
    S2,
    S3,
    S4,
    DD,
    KB,
    SQ,
    RN,

    count__,
};

inline auto getAnimationModes()
{
    using enum AnimationMode;
    return std::array{ DT, NU, WL, GH, A1, A2, BL, SC, S1, S2, S3, S4, DD, KB, SQ, RN };
}

AnimationMode animationModeFromString(const std::string& mode);
std::string   animationModeToString(AnimationMode mode);

struct DataRow_monseq : public RowBase {
    static constexpr const TableId s_tableId{ TableId::monseq };

    std::string        sequence;
    AnimationMode      mode = AnimationMode::Undefined; // The specific frame mode token (e.g., "A1", "S1")
    std::optional<int> frame;
    std::optional<int> dir;
    std::optional<int> event;
    std::optional<int> eol;

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();
};

struct DataRow_monstats : public RowBase {
    static constexpr const TableId s_tableId{ TableId::monstats };

    std::array<std::optional<int>, 3> resFi, resLi, resCo, resPo, resMa, resDm;

    std::optional<bool> enabled, isspawn, killable, npc, boss;
    std::optional<bool> rangedtype, melee, flying;

    std::optional<int> level, minGrp, maxGrp;

    std::string                           id;
    std::string                           baseId;
    std::string                           monStatsEx;
    std::string                           AI;
    std::array<std::optional<int>, 8 * 3> aipar;

    struct Skill {
        std::string        name;
        std::string        mode;
        std::optional<int> level;
    };

    std::vector<Skill> skills;

    int hcIdx = 0;

    std::array<std::array<std::string, 3>, 4> tc;

    struct Stats {
        std::optional<int> minHP;
        std::optional<int> maxHP;

        std::optional<int> AC; // defence
        std::optional<int> exp;

        std::optional<int> A1minD;
        std::optional<int> A1maxD;
        std::optional<int> A1TH; // attack rating

        std::optional<int> A2minD;
        std::optional<int> A2maxD;
        std::optional<int> A2TH;

        std::optional<int> S1minD;
        std::optional<int> S1maxD;
        std::optional<int> S1TH;

        std::optional<int> el1Pct;
        std::optional<int> el1MinD;
        std::optional<int> el1MaxD;
        std::optional<int> el1Dur;
    };
    std::array<Stats, 3> battleStats;

    std::optional<int> walkSpeed;
    std::optional<int> runSpeed;
    std::optional<int> toBlock;
    std::string        el1Mode;
    std::string        el1Type;

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

    std::array<std::string, 3> minionIds;

    bool isRegular       = false;
    bool useForRando     = false;
    bool spawnedAsNormal = false;
    bool spawnedAsElite  = false;
    bool hasMinions      = false;
    bool usedAsGuest     = false;

    size_t                     donorIdx = size_t(-1);
    std::string                donorId;
    std::optional<std::string> chimeraId;

    bool isEnabled() const { return enabled.value_or(false); }
    bool isSpawn() const { return isspawn.value_or(false); }
    bool isKillable() const { return killable.value_or(false); }
    bool isNpc() const { return npc.value_or(false); }
    bool isBoss() const { return boss.value_or(false); }

    bool isRanged() const { return rangedtype.value_or(false); }
    bool isMelee() const { return melee.value_or(false); }
    bool isFlying() const { return flying.value_or(false); }

    std::vector<std::string> getColumns() const;
    void                     serialize();

    std::map<std::string, std::string> getColumnAliases() const;

private:
    static const size_t tcCount     = 4 + 9;
    static const size_t skCount     = 8;
    static const size_t minionCount = 3;
    static const size_t aiParCount  = 8;
};

struct DataRow_monstatsEx : public RowBase {
    static constexpr const TableId s_tableId{ TableId::monstats2 };

    std::string id;
    std::string hitClass;

    int hcIdx = 0;

    std::array<std::optional<bool>, 16> modes;

    bool          hasMode(AnimationMode mode) const;
    AnimationMode getSafeSupportedMode() const;

    std::vector<std::string> getColumns() const;
    void                     serialize();

    std::map<std::string, std::string> getColumnAliases() const;
};

struct DataRow_skilldesc : public RowBase {
    static constexpr const TableId s_tableId{ TableId::skilldesc };

    std::string        skilldesc;
    std::optional<int> skillPage;
    std::optional<int> skillRow;
    std::optional<int> skillColumn;
    std::optional<int> listRow;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_skills : public RowBase {
    static constexpr const TableId s_tableId{ TableId::skills };

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
    Character                classSpecific = Character::None;

    bool canBeOskill = true;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
using DataRow_skills_ptr = const DataRow_skills*;
struct DataRow_treasureclassex : public RowBase {
    static constexpr const TableId s_tableId{ TableId::treasureclassex };

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
