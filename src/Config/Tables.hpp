#pragma once

// Do not edit. Generated from Tables.yaml using tables_update.py.

#include "TablesBaseCpp.hpp"

namespace D2ModGen {
// clang-format off

struct AffixProp_codegen {
    bool operator==(const AffixProp_codegen&) const noexcept = default;
    std::string code;
    std::string par;
    std::optional<int> min;
    std::optional<int> max;
    std::optional<int> chance;
};

struct DataRow_itemtypes_codegen {
    bool operator==(const DataRow_itemtypes_codegen&) const noexcept = default;

    std::string clas;
    std::string code;
    std::string itemType;
    std::string equiv1;
    std::string equiv2;
    std::string bodyLoc1;
    std::optional<bool> throwable;
    std::optional<bool> repair;
    std::string shoots;
    std::array<std::optional<int>, 3> sockets;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_charstats_codegen {
    bool operator==(const DataRow_charstats_codegen&) const noexcept = default;

    std::string clas;
    std::optional<int> statPerLevel;
    std::optional<int> skillsPerLevel;
    std::optional<int> hpPerLevel;
    std::optional<int> manaPerLevel;
    std::optional<int> hpPotEfficacy;
    std::optional<int> manaPotEfficacy;
    std::string startSkill;
    struct Item {
        bool operator==(const Item&) const noexcept = default;
    std::optional<int> count;
    std::string code;
    std::string loc;
    };
    std::vector<Item> items;
    std::vector<std::string> skills;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_difficultylevels_codegen {
    bool operator==(const DataRow_difficultylevels_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> resistPenalty;
    std::optional<int> gambleRare;
    std::optional<int> gambleSet;
    std::optional<int> gambleUnique;
    std::optional<int> gambleUber;
    std::optional<int> gambleUltra;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_experience_codegen {
    bool operator==(const DataRow_experience_codegen&) const noexcept = default;

    std::optional<int> expRatio;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_gamble_codegen {
    bool operator==(const DataRow_gamble_codegen&) const noexcept = default;

    std::string name;
    std::string code;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_hireling_codegen {
    bool operator==(const DataRow_hireling_codegen&) const noexcept = default;

    std::optional<int> hp;
    std::optional<int> hp_lvl;
    std::optional<int> dmg_min;
    std::optional<int> dmg_max;
    std::optional<int> dmg_lvl;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_itemstatcost_codegen {
    bool operator==(const DataRow_itemstatcost_codegen&) const noexcept = default;

    std::string stat;
    std::optional<int> multiply;
    std::optional<int> add;
    std::optional<int> sendParamBits;
    std::optional<int> saveParamBits;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_levels_codegen {
    bool operator==(const DataRow_levels_codegen&) const noexcept = default;

    std::optional<int> id;
    std::string name;
    std::optional<int> numMon;
    std::array<std::optional<int>, 3> monUMin;
    std::array<std::optional<int>, 3> monUMax;
    std::array<std::optional<int>, 3> monDen;
    std::array<std::optional<int>, 3> monLvlEx;
    std::vector<std::string> mon;
    std::vector<std::string> nmon;
    std::vector<std::string> umon;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_monlvl_codegen {
    bool operator==(const DataRow_monlvl_codegen&) const noexcept = default;

    std::array<std::optional<int>, 6> TH;
    std::array<std::optional<int>, 6> AC;
    std::array<std::optional<int>, 6> HP;
    std::array<std::optional<int>, 6> DM;
    std::array<std::optional<int>, 6> XP;
    std::optional<int> level;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_monseq_codegen {
    bool operator==(const DataRow_monseq_codegen&) const noexcept = default;

    std::string mode;
    std::string sequence;
    std::optional<int> frame;
    std::optional<int> dir;
    std::optional<int> event;
    std::optional<int> eol;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_monstats_codegen {
    bool operator==(const DataRow_monstats_codegen&) const noexcept = default;

    std::array<std::optional<int>, 3> resFi;
    std::array<std::optional<int>, 3> resLi;
    std::array<std::optional<int>, 3> resCo;
    std::array<std::optional<int>, 3> resPo;
    std::array<std::optional<int>, 3> resMa;
    std::array<std::optional<int>, 3> resDm;
    std::array<std::array<std::string, 3>, 4> tc;
    struct Skill {
        bool operator==(const Skill&) const noexcept = default;
    std::string name;
    std::string mode;
    std::optional<int> level;
    };
    std::vector<Skill> skills;
    std::array<std::optional<int>, 24> aipar;
    struct Stats {
        bool operator==(const Stats&) const noexcept = default;
    std::optional<int> minHP;
    std::optional<int> maxHP;
    std::optional<int> AC;
    std::optional<int> exp;
    std::optional<int> A1minD;
    std::optional<int> A1maxD;
    std::optional<int> A1TH;
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
    std::string el1Mode;
    std::string el1Type;
    std::array<std::string, 3> minionIds;
    std::optional<bool> enabled;
    std::optional<bool> isspawn;
    std::optional<bool> killable;
    std::optional<bool> npc;
    std::optional<bool> boss;
    std::optional<bool> rangedtype;
    std::optional<bool> melee;
    std::optional<bool> flying;
    std::optional<int> level;
    std::optional<int> minGrp;
    std::optional<int> maxGrp;
    std::string id;
    std::string baseId;
    std::optional<int> hcIdx;
    std::string monStatsEx;
    std::string AI;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_monstats2_codegen {
    bool operator==(const DataRow_monstats2_codegen&) const noexcept = default;

    std::string id;
    std::string hitClass;
    std::optional<int> hcIdx;
    std::array<std::optional<bool>, 16> modes;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_skilldesc_codegen {
    bool operator==(const DataRow_skilldesc_codegen&) const noexcept = default;

    std::string skilldesc;
    std::optional<int> skillPage;
    std::optional<int> skillRow;
    std::optional<int> skillColumn;
    std::optional<int> listRow;
    std::string strName;
    std::optional<int> iconCel;
    std::array<std::string, 3> missile;
    std::array<std::string, 36> calc;
    std::array<std::string, 36> desc;
    std::array<std::optional<int>, 18> descLine;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_skills_codegen {
    bool operator==(const DataRow_skills_codegen&) const noexcept = default;

    std::string skill;
    std::optional<int> id;
    std::string skilldesc;
    std::optional<int> maxlvl;
    std::optional<int> lvlreq;
    std::optional<bool> inTown;
    std::optional<bool> leftskill;
    std::optional<int> costMult;
    std::optional<int> costAdd;
    std::optional<int> eLen;
    std::optional<int> hitShift;
    std::optional<int> manaShift;
    std::string eType;
    std::string classSpecific;
    std::string itypea1;
    std::string anim;
    std::string seqtrans;
    std::vector<std::string> reqskills;
    std::array<std::string, 10> missile;
    std::array<std::string, 41> calc;
    std::vector<std::string> aurastatcalc;
    std::vector<std::string> aurastat;
    std::vector<std::string> passivestat;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_treasureclassex_codegen {
    bool operator==(const DataRow_treasureclassex_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> k_unique;
    std::optional<int> k_set;
    std::optional<int> k_rare;
    std::optional<int> k_magic;
    std::optional<int> noDrop;
    std::optional<int> group;
    std::optional<int> picks;
    std::optional<int> level;
    struct Item {
        bool operator==(const Item&) const noexcept = default;
    std::optional<int> prob;
    std::string tc;
    };
    std::vector<Item> items;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_missiles_codegen {
    bool operator==(const DataRow_missiles_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> id;
    std::string eType;
    struct Color {
        bool operator==(const Color&) const noexcept = default;
    std::optional<int> red;
    std::optional<int> green;
    std::optional<int> blue;
    };
    Color color;
    std::string skill;
    std::optional<int> missileSkill;
    std::string EDmgSymPerCalc;
    std::array<std::string, 15> childmissile;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_inventory_codegen {
    bool operator==(const DataRow_inventory_codegen&) const noexcept = default;

    std::string charclass;
    std::optional<int> gridX;
    std::optional<int> gridY;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_lvlmaze_codegen {
    bool operator==(const DataRow_lvlmaze_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> levelId;
    std::array<std::optional<int>, 3> rooms;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_superuniques_codegen {
    bool operator==(const DataRow_superuniques_codegen&) const noexcept = default;

    std::string monsterClass;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_weapons_codegen {
    bool operator==(const DataRow_weapons_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::string code;
    std::string itemTypeCode;
    std::optional<int> reqstr;
    std::optional<int> reqdex;
    std::optional<bool> showLevel;
    std::string normcode;
    std::string ubercode;
    std::string ultracode;
    std::optional<int> mindam;
    std::optional<int> maxdam;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_armor_codegen {
    bool operator==(const DataRow_armor_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::string code;
    std::string itemTypeCode;
    std::optional<int> reqstr;
    std::optional<int> reqdex;
    std::optional<bool> showLevel;
    std::string normcode;
    std::string ubercode;
    std::string ultracode;
    std::optional<int> minac;
    std::optional<int> maxac;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_misc_codegen {
    bool operator==(const DataRow_misc_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::string code;
    std::string itemTypeCode;
    std::optional<int> maxstack;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_automagic_codegen {
    bool operator==(const DataRow_automagic_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> maxlevel;
    std::optional<int> lvlreq;
    std::optional<bool> spawnable;
    std::vector<AffixProp_codegen> affixes;
    std::size_t affixes_limit = 3;
    std::vector<std::string> itype;
    std::vector<std::string> etype;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_magicprefix_codegen {
    bool operator==(const DataRow_magicprefix_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> maxlevel;
    std::optional<int> lvlreq;
    std::optional<bool> spawnable;
    std::vector<AffixProp_codegen> affixes;
    std::size_t affixes_limit = 3;
    std::vector<std::string> itype;
    std::vector<std::string> etype;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_magicsuffix_codegen {
    bool operator==(const DataRow_magicsuffix_codegen&) const noexcept = default;

    std::string name;
    std::optional<int> lvl;
    std::optional<int> maxlevel;
    std::optional<int> lvlreq;
    std::optional<bool> spawnable;
    std::vector<AffixProp_codegen> affixes;
    std::size_t affixes_limit = 3;
    std::vector<std::string> itype;
    std::vector<std::string> etype;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_uniqueitems_codegen {
    bool operator==(const DataRow_uniqueitems_codegen&) const noexcept = default;

    std::string index;
    std::string eqipmentCode;
    std::optional<int> rarity;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::optional<bool> spawnable;
    std::optional<bool> disableChronicle;
    std::optional<int> cost_mul;
    std::optional<int> cost_add;
    std::optional<int> id;
    std::string itemName;
    std::string chrtransform;
    std::string invtransform;
    std::string flippyfile;
    std::string invfile;
    std::string dropsound;
    std::string dropsfxframe;
    std::string usesound;
    std::optional<int> eol;
    std::optional<int> carry1;
    std::optional<int> version;
    std::vector<AffixProp_codegen> affixes;
    std::size_t affixes_limit = 12;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_setitems_codegen {
    bool operator==(const DataRow_setitems_codegen&) const noexcept = default;

    std::string index;
    std::string eqipmentCode;
    std::optional<int> rarity;
    std::optional<int> lvl;
    std::optional<int> lvlreq;
    std::optional<bool> spawnable;
    std::optional<bool> disableChronicle;
    std::optional<int> cost_mul;
    std::optional<int> cost_add;
    std::optional<int> id;
    std::string itemName;
    std::string chrtransform;
    std::string invtransform;
    std::string flippyfile;
    std::string invfile;
    std::string dropsound;
    std::string dropsfxframe;
    std::string usesound;
    std::optional<int> eol;
    std::optional<int> addfunc;
    std::string setIndex;
    std::vector<AffixProp_codegen> affixes_base;
    std::size_t affixes_base_limit = 9;
    std::vector<AffixProp_codegen> affixes_partial1;
    std::size_t affixes_partial1_limit = 5;
    std::vector<AffixProp_codegen> affixes_partial2;
    std::size_t affixes_partial2_limit = 5;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_sets_codegen {
    bool operator==(const DataRow_sets_codegen&) const noexcept = default;

    std::string index;
    std::string name;
    std::string uiClass;
    std::optional<int> version;
    std::vector<AffixProp_codegen> affixes_full;
    std::size_t affixes_full_limit = 8;
    std::vector<AffixProp_codegen> affixes_partial1;
    std::size_t affixes_partial1_limit = 4;
    std::vector<AffixProp_codegen> affixes_partial2;
    std::size_t affixes_partial2_limit = 4;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_runes_codegen {
    bool operator==(const DataRow_runes_codegen&) const noexcept = default;

    std::optional<bool> complete;
    std::string name;
    std::vector<AffixProp_codegen> affixes;
    std::size_t affixes_limit = 7;
    std::vector<std::string> runeCodes;
    std::vector<std::string> itype;
    std::vector<std::string> etype;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_gems_codegen {
    bool operator==(const DataRow_gems_codegen&) const noexcept = default;

    std::string eqipmentCode;
    std::vector<AffixProp_codegen> affixes_weapon;
    std::size_t affixes_weapon_limit = 3;
    std::vector<AffixProp_codegen> affixes_helm;
    std::size_t affixes_helm_limit = 3;
    std::vector<AffixProp_codegen> affixes_shield;
    std::size_t affixes_shield_limit = 3;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

struct DataRow_cubemain_codegen {
    bool operator==(const DataRow_cubemain_codegen&) const noexcept = default;

    std::string description;
    std::optional<bool> enabled;
    std::optional<int> version;
    std::optional<int> op;
    std::optional<int> eol;
    std::optional<int> numinputs;
    std::string output;
    std::optional<int> ilvl;
    std::optional<int> plvl;
    std::optional<int> lvl;
    std::vector<AffixProp_codegen> affixes_a;
    std::size_t affixes_a_limit = 5;
    std::vector<AffixProp_codegen> affixes_b;
    std::size_t affixes_b_limit = 5;
    std::vector<AffixProp_codegen> affixes_c;
    std::size_t affixes_c_limit = 5;
    std::vector<std::string> inputs;

private:
    enum Column : std::size_t;

public:
    static const GeneratedTableSchema& tableSchema();

    void readData(const CsvRowReader& reader);

    void writeData(CsvRowWriter& writer) const;
};

// clang-format on
} // namespace D2ModGen
