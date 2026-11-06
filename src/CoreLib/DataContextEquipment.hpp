/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContextBase.hpp"
#include "Storage/IStorage.hpp"

namespace D2ModGen {

struct DataRow_itemtypes : public RowBase {
    static constexpr const TableId s_tableId{ TableId::itemtypes };

    bool operator==(const DataRow_itemtypes&) const noexcept = default;

    std::string clas;
    std::string code;
    std::string itemType;
    std::string equiv1;
    std::string equiv2;
    std::string bodyLoc1;

    AttributeIdiom idiom         = AttributeIdiom::None;
    Character      classSpecific = Character::None;

    std::optional<bool>               throwable;
    std::optional<bool>               repair;
    std::string                       shoots;
    std::array<std::optional<int>, 3> sockets;

    AttributeFlagSet flags;
    ItemTypeSet      nested;
    ItemTypeSet      parents;

    SlotType       determineSlot() const;
    AttributeIdiom determineIdiom() const { return idiom; }

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();
};

// =========================================================

struct DataRow_weapons : public RowBaseEquipment {
    static constexpr const TableId s_tableId{ TableId::weapons };

    std::optional<int> mindam;
    std::optional<int> maxdam;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_armor : public RowBaseEquipment {
    static constexpr const TableId s_tableId{ TableId::armor };

    std::optional<int> minac;
    std::optional<int> maxac;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_misc : public RowBaseEquipment {
    static constexpr const TableId s_tableId{ TableId::misc };

    std::optional<int> maxstack;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

// =========================================================

struct DataRow_automagic : public RowBaseAffixes {
    static constexpr const TableId s_tableId{ TableId::automagic };
};
struct DataRow_magicprefix : public RowBaseAffixes {
    static constexpr const TableId s_tableId{ TableId::magicprefix };
};
struct DataRow_magicsuffix : public RowBaseAffixes {
    static constexpr const TableId s_tableId{ TableId::magicsuffix };
};
// =========================================================

struct RowBaseUniqueSet
    : public RowBase
    , public IAffixContainer {
    std::string   index;
    std::string   eqipmentCode;
    EquipmentType eqipmentType = nullptr;

    std::optional<int>  rarity;
    std::optional<int>  lvl; // "level" or "lvl"
    std::optional<int>  lvlreq;
    std::optional<bool> spawnable; // or "enabled"
    std::optional<bool> disableChronicle;

    std::optional<int> cost_mul = 5;
    std::optional<int> cost_add = 5000;

    struct UX {
        std::optional<int> id;
        std::string        itemName;
        std::string        chrtransform;
        std::string        invtransform;
        std::string        flippyfile;

        std::string invfile;
        std::string dropsound;
        std::string dropsfxframe;
        std::string usesound;
        int         eol = 0;
    } ux;

    std::string displayName; // mostly for debug purposes like logs/dumps

    std::string getId() const override
    {
        return displayName;
    }

    int getLevel() const override
    {
        if (lvl.has_value())
            return lvl.value();

        if (lvlreq.has_value())
            return lvlreq.value();
        return 0;
    }

    bool isValid() const override
    {
        return spawnable.value_or(false) && !eqipmentCode.empty() && getLevel() > 0;
    }

    ItemCodeList getItemFilter() const override
    {
        return eqipmentType ? eqipmentType->getItemFilter() : ItemCodeList{};
    }
    AttributeFlagSet getAttributes() const override
    {
        return eqipmentType ? eqipmentType->getAttributes() : AttributeFlagSet{};
    }

    std::map<std::string, std::string> getColumnAliases() const;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

struct DataRow_uniqueitems : public RowBaseUniqueSet {
    static constexpr const TableId s_tableId{ TableId::uniqueitems };

    std::optional<int> carry1;
    std::optional<int> version = 100;

    AffixPropList affixes{ 12 };

    AffixListListConst   affixLists() const { return { &affixes }; }
    AffixListListMutable affixLists() { return { &affixes }; }
    RollTypes            rollTypesGrab() const { return { RollType::Rare }; }
    RollTypes            rollTypesGen() const { return { RollType::Epic }; }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

struct DataRow_sets;
struct DataRow_setitems : public RowBaseUniqueSet {
    static constexpr const TableId s_tableId{ TableId::setitems };

    std::optional<int> addfunc;
    std::string        setIndex;

    AffixPropList affixes_base{ 9 };
    AffixPropList affixes_partial1{ 5 };
    AffixPropList affixes_partial2{ 5 };

    AffixListListConst   affixLists() const { return { &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    AffixListListMutable affixLists() { return { &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    RollTypes            rollTypesGrab() const { return { RollType::Rare, RollType::Rare, RollType::Rare }; }
    RollTypes            rollTypesGen() const { return { RollType::Epic, RollType::Epic, RollType::Epic }; }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

// =========================================================

struct DataRow_sets : public RowBase
    , public IAffixContainer {
    static constexpr const TableId s_tableId{ TableId::sets };

    bool operator==(const DataRow_sets&) const noexcept = default;

    int level = 0;

    std::optional<int> version = 100;
    std::string        index;
    std::string        name;
    std::string        uiClass;
    ItemCodeList       filter;

    AffixPropList affixes_full{ 8 };
    AffixPropList affixes_partial1{ 4 };
    AffixPropList affixes_partial2{ 4 };

    std::string getId() const override
    {
        return index;
    }

    AffixListListConst   affixLists() const override { return { &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    AffixListListMutable affixLists() override { return { &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    RollTypes            rollTypesGrab() const override { return { RollType::Epic, RollType::Rare, RollType::Rare }; }
    RollTypes            rollTypesGen() const override { return rollTypesGrab(); }

    bool             isValid() const override { return !name.empty(); }
    int              getLevel() const override { return level; }
    ItemCodeList     getItemFilter() const override { return filter; }
    AttributeFlagSet getAttributes() const override
    {
        return AttributeFlagSet{};
    }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_runes : public RowBase
    , public IAffixContainer {
    static constexpr const TableId s_tableId{ TableId::runes };

    bool operator==(const DataRow_runes&) const noexcept = default;

    int                      lvl = 1; // calculated.
    std::vector<std::string> runeCodes;
    AffixPropList            affixes{ 7 };
    std::optional<bool>      complete;
    std::vector<std::string> itype;
    std::vector<std::string> etype;
    ItemCodeList             filter;

    std::string name;

    std::string getId() const override
    {
        return name;
    }

    AffixListListConst   affixLists() const override { return { &affixes }; }
    AffixListListMutable affixLists() override { return { &affixes }; }
    RollTypes            rollTypesGrab() const override { return { RollType::Epic }; }
    RollTypes            rollTypesGen() const override { return rollTypesGrab(); }

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();

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

private:
    static const size_t maxRunes = 6;
    static const size_t maxItype = 6;
    static const size_t maxEtype = 3;
};

struct DataRow_gems : public RowBase
    , public IAffixContainer {
    static constexpr const TableId s_tableId{ TableId::gems };

    bool operator==(const DataRow_gems&) const noexcept = default;

    AffixPropList affixes_weapon{ 3 };
    AffixPropList affixes_helm{ 3 };
    AffixPropList affixes_shield{ 3 };

    EquipmentType eqipmentType = nullptr;
    std::string   eqipmentCode;

    AffixListListConst   affixLists() const override { return { &affixes_weapon, &affixes_helm, &affixes_shield }; }
    AffixListListMutable affixLists() override { return { &affixes_weapon, &affixes_helm, &affixes_shield }; }
    RollTypes            rollTypesGrab() const override { return { RollType::Common, RollType::Common, RollType::Common }; }
    RollTypes            rollTypesGen() const override { return { RollType::Rare, RollType::Rare, RollType::Rare }; }

    std::string getId() const override
    {
        return eqipmentType->getId();
    }

    bool isValid() const override
    {
        return !eqipmentCode.empty();
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

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

struct DataRow_cubemain : public RowBase
    , public IAffixContainer {
    static constexpr const TableId s_tableId{ TableId::cubemain };

    bool operator==(const DataRow_cubemain&) const noexcept = default;

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

    EquipmentType eqipmentType = nullptr;
    ItemType      itemType     = nullptr;

    std::string getId() const override
    {
        return description;
    }

    AffixListListConst   affixLists() const override { return { &affixes_a, &affixes_b, &affixes_c }; }
    AffixListListMutable affixLists() override { return { &affixes_a, &affixes_b, &affixes_c }; }
    RollTypes            rollTypesGrab() const override { return { RollType::Common, RollType::Common, RollType::Common }; }
    RollTypes            rollTypesGen() const override { return { RollType::Epic, RollType::Epic, RollType::Epic }; }

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

    std::vector<std::string> getColumns() const;
    void                     serialize();

private:
    static const size_t maxInputs = 7;
};

}
