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

struct DataRow_itemtypes : public RowBase
    , public DataRow_itemtypes_codegen {
    static constexpr const TableId s_tableId{ TableId::itemtypes };

    bool operator==(const DataRow_itemtypes&) const noexcept = default;

    AttributeIdiom idiom         = AttributeIdiom::None;
    Character      classSpecific = Character::None;

    AttributeFlagSet flags;
    ItemTypeSet      nested;
    ItemTypeSet      parents;

    SlotType       determineSlot() const;
    AttributeIdiom determineIdiom() const { return idiom; }

    void serialize();
};

// =========================================================

struct DataRow_weapons : public RowBaseEquipment<DataRow_weapons_codegen> {
    static constexpr const TableId s_tableId{ TableId::weapons };

    bool operator==(const DataRow_weapons&) const noexcept = default;
    DataRow_weapons()
    {
        category = EquipmentCategory::Weapon;
    }

    Tier getTier() const override
    {
        if (!getLevel() || this->ultracode.empty())
            return Tier::None;
        if (this->ultracode == this->code)
            return Tier::Elite;
        if (this->ubercode == this->code)
            return Tier::Exceptional;
        if (this->normcode == this->code)
            return Tier::Normal;

        return Tier::None;
    }
};
struct DataRow_armor
    : public RowBaseEquipment<DataRow_armor_codegen> {
    static constexpr const TableId s_tableId{ TableId::armor };

    bool operator==(const DataRow_armor&) const noexcept = default;
    DataRow_armor()
    {
        category = EquipmentCategory::Armor;
    }

    Tier getTier() const override
    {
        if (!getLevel() || this->ultracode.empty())
            return Tier::None;
        if (this->ultracode == this->code)
            return Tier::Elite;
        if (this->ubercode == this->code)
            return Tier::Exceptional;
        if (this->normcode == this->code)
            return Tier::Normal;

        return Tier::None;
    }
};
struct DataRow_misc
    : public RowBaseEquipment<DataRow_misc_codegen> {
    static constexpr const TableId s_tableId{ TableId::misc };

    bool operator==(const DataRow_misc&) const noexcept = default;

    Tier getTier() const override
    {
        return Tier::None;
    }

    DataRow_misc()
    {
        category = EquipmentCategory::Misc;
    }
};

// =========================================================

struct DataRow_automagic : public RowBaseAffixes<DataRow_automagic_codegen> {
    static constexpr const TableId s_tableId{ TableId::automagic };

    bool m_stub{ false };

    bool operator==(const DataRow_automagic&) const noexcept = default;
};
struct DataRow_magicprefix : public RowBaseAffixes<DataRow_magicprefix_codegen> {
    static constexpr const TableId s_tableId{ TableId::magicprefix };

    bool m_stub{ false };

    bool operator==(const DataRow_magicprefix&) const noexcept = default;
};
struct DataRow_magicsuffix : public RowBaseAffixes<DataRow_magicsuffix_codegen> {
    static constexpr const TableId s_tableId{ TableId::magicsuffix };

    bool m_stub{ false };

    bool operator==(const DataRow_magicsuffix&) const noexcept = default;
};
// =========================================================

template<class CRTP>
struct RowBaseUniqueSet
    : public RowBase
    , public IAffixContainer
    , public CRTP {
    bool operator==(const RowBaseUniqueSet&) const noexcept = default;

    EquipmentType eqipmentType = nullptr;
    std::string   displayName; // mostly for debug purposes like logs/dumps

    std::string getId() const override
    {
        return displayName;
    }

    int getLevel() const override
    {
        if (this->lvl.has_value())
            return this->lvl.value();

        if (this->lvlreq.has_value())
            return this->lvlreq.value();
        return 0;
    }

    bool isValidBase() const
    {
        return this->spawnable.value_or(false) && !this->eqipmentCode.empty() && getLevel() > 0;
    }

    bool isValid() const override
    {
        return isValidBase() && eqipmentType;
    }

    ItemCodeList getItemFilter() const override
    {
        return eqipmentType ? eqipmentType->getItemFilter() : ItemCodeList{};
    }
    AttributeFlagSet getAttributes() const override
    {
        return eqipmentType ? eqipmentType->getAttributes() : AttributeFlagSet{};
    }

    void resetAppearance()
    {
        this->itemName     = {};
        this->chrtransform = {};
        this->invtransform = {};
        this->flippyfile   = {};

        this->invfile      = {};
        this->dropsound    = {};
        this->dropsfxframe = {};
        this->usesound     = {};
    }
};

struct DataRow_uniqueitems : public RowBaseUniqueSet<DataRow_uniqueitems_codegen> {
    static constexpr const TableId s_tableId{ TableId::uniqueitems };

    bool operator==(const DataRow_uniqueitems&) const noexcept = default;

    AffixListListConst   affixLists() const override { return { &affixes }; }
    AffixListListMutable affixLists() override { return { &affixes }; }
    Limits               affixLimits() const override { return { affixes_limit }; }
};

struct DataRow_sets;
struct DataRow_setitems : public RowBaseUniqueSet<DataRow_setitems_codegen> {
    static constexpr const TableId s_tableId{ TableId::setitems };

    bool operator==(const DataRow_setitems&) const noexcept = default;

    AffixListListConst   affixLists() const override { return { &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    AffixListListMutable affixLists() override { return { &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    Limits               affixLimits() const override { return { affixes_base_limit, affixes_partial1_limit, affixes_partial2_limit }; }
};

// =========================================================

struct DataRow_sets : public RowBase
    , public IAffixContainer
    , public DataRow_sets_codegen {
    static constexpr const TableId s_tableId{ TableId::sets };

    bool operator==(const DataRow_sets&) const noexcept = default;

    int          level = 0;
    ItemCodeList filter;

    std::string getId() const override
    {
        return index;
    }

    AffixListListConst   affixLists() const override { return { &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    AffixListListMutable affixLists() override { return { &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    Limits               affixLimits() const override { return { affixes_full_limit, affixes_partial1_limit, affixes_partial2_limit }; }

    bool             isValid() const override { return !name.empty(); }
    int              getLevel() const override { return level; }
    ItemCodeList     getItemFilter() const override { return filter; }
    AttributeFlagSet getAttributes() const override
    {
        return AttributeFlagSet{};
    }
};
struct DataRow_runes : public RowBase
    , public IAffixContainer
    , DataRow_runes_codegen {
    static constexpr const TableId s_tableId{ TableId::runes };

    bool operator==(const DataRow_runes&) const noexcept = default;

    int          lvl = 1; // calculated.
    ItemCodeList filter;

    std::string getId() const override
    {
        return name;
    }

    AffixListListConst   affixLists() const override { return { &affixes }; }
    AffixListListMutable affixLists() override { return { &affixes }; }
    Limits               affixLimits() const override { return { affixes_limit }; }

    int getLevel() const override
    {
        return isValid() ? lvl : 0;
    }

    ItemCodeList getItemFilter() const override
    {
        return filter;
    }
    AttributeFlagSet getAttributes() const override
    {
        return AttributeFlagSet{ AttributeFlag::Durability };
    }

    bool isValid() const override
    {
        return complete.value_or(false);
    }
};

struct DataRow_gems : public RowBase
    , public IAffixContainer
    , public DataRow_gems_codegen {
    static constexpr const TableId s_tableId{ TableId::gems };

    bool operator==(const DataRow_gems&) const noexcept = default;

    EquipmentType eqipmentType = nullptr;

    AffixListListConst   affixLists() const override { return { &affixes_weapon, &affixes_helm, &affixes_shield }; }
    AffixListListMutable affixLists() override { return { &affixes_weapon, &affixes_helm, &affixes_shield }; }
    Limits               affixLimits() const override { return { affixes_weapon_limit, affixes_helm_limit, affixes_shield_limit }; }

    std::string getId() const override
    {
        return eqipmentType->getId();
    }

    bool isValidBase() const
    {
        return !eqipmentCode.empty();
    }

    bool isValid() const override
    {
        return isValidBase() && eqipmentType;
    }

    int getLevel() const override
    {
        return isValid() ? eqipmentType->getLevel() : 0;
    }

    ItemCodeList getItemFilter() const override
    {
        return eqipmentType->getItemFilter();
    }
    AttributeFlagSet getAttributes() const override
    {
        return eqipmentType->getAttributes();
    }
};

struct DataRow_cubemain : public RowBase
    , public IAffixContainer
    , public DataRow_cubemain_codegen {
    static constexpr const TableId s_tableId{ TableId::cubemain };

    bool operator==(const DataRow_cubemain&) const noexcept = default;

    EquipmentType eqipmentType = nullptr;
    ItemType      itemType     = nullptr;

    std::string getId() const override
    {
        return description;
    }

    AffixListListConst   affixLists() const override { return { &affixes_a, &affixes_b, &affixes_c }; }
    AffixListListMutable affixLists() override { return { &affixes_a, &affixes_b, &affixes_c }; }
    Limits               affixLimits() const override { return { affixes_a_limit, affixes_b_limit, affixes_c_limit }; }

    bool isCrafted() const { return output == "\"usetype,crf\""; }
    bool hasLvls() const { return ilvl.has_value() || plvl.has_value() || lvl.has_value(); }

    bool isValid() const override { return itemType || eqipmentType; }

    int getLevel() const override
    {
        return isValid() ? 30 : 0;
    }

    ItemCodeList getItemFilter() const override
    {
        if (eqipmentType)
            return eqipmentType->getItemFilter();
        ItemCodeList result;
        if (itemType)
            result.push_back(itemType);
        return result;
    }

    AttributeFlagSet getAttributes() const override
    {
        if (eqipmentType)
            return eqipmentType->getAttributes();
        return itemType ? itemType->flags : AttributeFlagSet{};
    }
};

}
