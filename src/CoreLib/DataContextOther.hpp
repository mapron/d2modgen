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

    bool operator==(const DataRow_charstats&) const noexcept = default;

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

    bool operator==(const DataRow_difficultylevels&) const noexcept = default;

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

    bool operator==(const DataRow_experience&) const noexcept = default;

    std::optional<int> expRatio;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_gamble : public RowBase {
    static constexpr const TableId s_tableId{ TableId::gamble };

    bool operator==(const DataRow_gamble&) const noexcept = default;

    std::string name;
    std::string code;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_hireling : public RowBase {
    static constexpr const TableId s_tableId{ TableId::hireling };

    bool operator==(const DataRow_hireling&) const noexcept = default;

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

    bool operator==(const DataRow_itemstatcost&) const noexcept = default;

    std::optional<int> multiply;
    std::optional<int> add;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_levels : public RowBase {
    static constexpr const TableId s_tableId{ TableId::levels };

    bool operator==(const DataRow_levels&) const noexcept = default;

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

    bool operator==(const DataRow_monlvl&) const noexcept = default;

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

    bool operator==(const DataRow_monseq&) const noexcept = default;

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

    bool operator==(const DataRow_monstats&) const noexcept = default;

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

        bool operator==(const Skill&) const noexcept = default;
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

        bool operator==(const Stats&) const noexcept = default;
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

    bool operator==(const DataRow_monstatsEx&) const noexcept = default;

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
private:
    static const size_t maxDesc1 = 6;
    static const size_t maxDesc2 = 5; // d2 legacy has 4
    static const size_t maxDesc3 = 7;
    static const size_t maxDesc  = maxDesc1 + maxDesc2 + maxDesc3;

public:
    static constexpr const TableId s_tableId{ TableId::skilldesc };

    bool operator==(const DataRow_skilldesc&) const noexcept = default;

    std::string        skilldesc;
    std::optional<int> skillPage;
    std::optional<int> skillRow;
    std::optional<int> skillColumn;
    std::optional<int> listRow;

    std::array<std::string, maxDesc * 2>    calc;
    std::array<std::string, maxDesc * 2>    desc;
    std::array<std::optional<int>, maxDesc> descLine;

    std::array<std::string, 3> missile;

    std::string strName; // Strings like "Skillname265" for "Cloak of Shadows". Do not match exact id!

    std::vector<std::string> getColumns() const;
    void                     serialize();

    bool isValid() const
    {
        return skillPage.value_or(0) > 0 && skillRow.value_or(0) > 0 && skillColumn.value_or(0) > 0 && listRow.value_or(0) > 0;
    }
    void makeHidden()
    {
        skillPage   = 0;
        skillRow    = 0;
        skillColumn = 0;
        listRow     = 0;
    }
};
struct DataRow_skills : public RowBase {
private:
    static constexpr const size_t s_calcCount = 6 + 14 + 3 + 5 + 3 + 10;

public:
    static constexpr const TableId s_tableId{ TableId::skills };

    bool operator==(const DataRow_skills&) const noexcept = default;

    int                 id = -1;
    std::string         skill;
    std::string         skilldesc;
    std::optional<int>  maxlvl;
    std::optional<int>  lvlreq;
    std::optional<bool> inTown;
    std::optional<int>  costMult;
    std::optional<int>  costAdd;

    std::optional<int> eLen;
    std::string        eType;
    std::optional<int> hitShift;
    std::optional<int> manaShift;

    std::vector<std::string> reqskills;
    std::string              charclass;
    Character                classSpecific = Character::None;

    std::array<std::string, 10> missile;

    std::vector<std::string> passivestat;

    std::string              itypea1;
    std::vector<std::string> aurastat;
    std::vector<std::string> aurastatcalc;

    std::string anim;
    std::string seqtrans;

    bool canBeOskill = true;

    std::array<std::string, s_calcCount> calc;

    std::vector<std::string> getColumns() const;
    void                     serialize();

    std::map<std::string, std::string> getColumnAliases() const;

    void setNewClass(Character clas);
};

using DataRow_skills_ptr = const DataRow_skills*;
struct DataRow_treasureclassex : public RowBase {
    static constexpr const TableId s_tableId{ TableId::treasureclassex };

    bool operator==(const DataRow_treasureclassex&) const noexcept = default;

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

struct DataRow_missiles : public RowBase {
    static constexpr const TableId s_tableId{ TableId::missiles };

    bool operator==(const DataRow_missiles&) const noexcept = default;

    std::string name;
    int         id = -1;

    std::string        skill;
    std::optional<int> missileSkill;

    std::string eType;
    struct Color {
        int red   = 0;
        int green = 0;
        int blue  = 0;

        bool operator==(const Color&) const noexcept = default;
    };
    Color color;

    std::array<std::string, 15> childmissile;

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;

    void serialize();

private:
    static const size_t maxItems = 10;
};

}
