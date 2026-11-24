/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContextBase.hpp"
#include "Storage/IStorage.hpp"

#include "Tables.hpp"

namespace D2ModGen {

struct DataRow_charstats : public RowBase
    , public DataRow_charstats_codegen {
    static constexpr const TableId s_tableId{ TableId::charstats };

    bool operator==(const DataRow_charstats&) const noexcept = default;
};
struct DataRow_difficultylevels : public RowBase
    , public DataRow_difficultylevels_codegen {
    static constexpr const TableId s_tableId{ TableId::difficultylevels };

    bool operator==(const DataRow_difficultylevels&) const noexcept = default;
};
struct DataRow_experience : public RowBase
    , public DataRow_experience_codegen {
    static constexpr const TableId s_tableId{ TableId::experience };

    bool operator==(const DataRow_experience&) const noexcept = default;
};
struct DataRow_gamble : public RowBase
    , public DataRow_gamble_codegen {
    static constexpr const TableId s_tableId{ TableId::gamble };
    DataRow_gamble() = default;
    DataRow_gamble(const DataRow_gamble_codegen& val)
        : DataRow_gamble_codegen(val)
    {}

    bool operator==(const DataRow_gamble&) const noexcept = default;
};
struct DataRow_hireling : public RowBase
    , public DataRow_hireling_codegen {
    static constexpr const TableId s_tableId{ TableId::hireling };

    bool operator==(const DataRow_hireling&) const noexcept = default;
};
struct DataRow_itemstatcost
    : public RowBase
    , public DataRow_itemstatcost_codegen {
    static constexpr const TableId s_tableId{ TableId::itemstatcost };

    bool operator==(const DataRow_itemstatcost&) const noexcept = default;
};

struct DataRow_levels
    : public RowBase
    , public DataRow_levels_codegen {
    static constexpr const TableId s_tableId{ TableId::levels };

    bool operator==(const DataRow_levels&) const noexcept = default;

    std::vector<int> superUniquesIndexes;

    bool useForRando = false;
};

struct DataRow_monlvl
    : public RowBase
    , public DataRow_monlvl_codegen {
    static constexpr const TableId s_tableId{ TableId::monlvl };

    bool operator==(const DataRow_monlvl&) const noexcept = default;
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

struct DataRow_monseq
    : public RowBase
    , public DataRow_monseq_codegen {
    static constexpr const TableId s_tableId{ TableId::monseq };

    bool operator==(const DataRow_monseq&) const noexcept = default;

    AnimationMode mode = AnimationMode::Undefined; // The specific frame mode token (e.g., "A1", "S1")
};

struct DataRow_monstats
    : public RowBase
    , public DataRow_monstats_codegen {
    static constexpr const TableId s_tableId{ TableId::monstats };

    bool operator==(const DataRow_monstats&) const noexcept = default;

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

    bool isRegular       = false;
    bool useForRando     = false;
    bool spawnedAsNormal = false;
    bool spawnedAsElite  = false;
    bool usedAsGuest     = false;

    int firstAreaIndexToSpawn = -1;

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
};

struct DataRow_monstatsEx
    : public RowBase
    , public DataRow_monstats2_codegen {
    static constexpr const TableId s_tableId{ TableId::monstats2 };

    bool operator==(const DataRow_monstatsEx&) const noexcept = default;

    bool          hasMode(AnimationMode mode) const;
    AnimationMode getSafeSupportedMode() const;
};

struct DataRow_skilldesc
    : public RowBase
    , public DataRow_skilldesc_codegen {
    static constexpr const TableId s_tableId{ TableId::skilldesc };

    bool operator==(const DataRow_skilldesc&) const noexcept = default;

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

struct DataRow_skills
    : public RowBase
    , public DataRow_skills_codegen {
public:
    static constexpr const TableId s_tableId{ TableId::skills };

    bool operator==(const DataRow_skills&) const noexcept = default;

    Character getClass() const;
    void      setNewClass(Character clas);
};

using DataRow_skills_mutableptr = DataRow_skills*;
using DataRow_skills_ptr        = const DataRow_skills*;

struct ISkillGetter {
    virtual ~ISkillGetter() = default;

    virtual int resolveSkillName(const std::string& nameOrId) const = 0;

    virtual DataRow_skills_mutableptr getSkill(int idx)       = 0;
    virtual DataRow_skills_ptr        getSkill(int idx) const = 0;
};

struct DataRow_treasureclassex
    : public RowBase
    , public DataRow_treasureclassex_codegen {
    static constexpr const TableId s_tableId{ TableId::treasureclassex };

    bool operator==(const DataRow_treasureclassex&) const noexcept = default;

    int getDropSomethingProb() const;
};

struct DataRow_missiles
    : public RowBase
    , public DataRow_missiles_codegen {
    static constexpr const TableId s_tableId{ TableId::missiles };

    bool operator==(const DataRow_missiles&) const noexcept = default;
};

struct DataRow_inventory
    : public RowBase
    , public DataRow_inventory_codegen {
    static constexpr const TableId s_tableId{ TableId::inventory };

    bool operator==(const DataRow_inventory&) const noexcept = default;

    bool isPlayable = false;
};

struct DataRow_lvlmaze
    : public RowBase
    , public DataRow_lvlmaze_codegen {
    static constexpr const TableId s_tableId{ TableId::lvlmaze };

    bool operator==(const DataRow_lvlmaze&) const noexcept = default;

    bool isValid() const { return levelId.has_value(); }
};

struct DataRow_superuniques
    : public RowBase
    , public DataRow_superuniques_codegen {
    static constexpr const TableId s_tableId{ TableId::superuniques };

    bool operator==(const DataRow_superuniques&) const noexcept = default;
};

}
