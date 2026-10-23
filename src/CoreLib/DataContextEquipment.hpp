/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContextBase.hpp"

namespace D2ModGen {

// =========================================================

struct DataRow_itemtypes : public RowBase {
    std::string clas;
    std::string code;
    std::string itemType;
    std::string equiv1;
    std::string equiv2;
    std::string bodyLoc1;

    AttributeIdiom idiom         = AttributeIdiom::Any;
    Character      classSpecific = Character::none;

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
    std::optional<int> mindam;
    std::optional<int> maxdam;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_armor : public RowBaseEquipment {
    std::optional<int> minac;
    std::optional<int> maxac;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_misc : public RowBaseEquipment {
    std::optional<int> maxstack;

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

// =========================================================

struct DataRow_automagic : public RowBaseAffixes {
};
struct DataRow_magicprefix : public RowBaseAffixes {
};
struct DataRow_magicsuffix : public RowBaseAffixes {
};
// =========================================================

struct DataRow_uniqueitems : public RowBaseUniqueSet {
    AffixPropList affixes{ 12 };

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

struct DataRow_sets;
struct DataRow_setitems : public RowBaseUniqueSet {
    std::optional<int> addfunc;
    std::string        setIndex;

    AffixPropList affixes_base{ 9 };
    AffixPropList affixes_partial1{ 5 };
    AffixPropList affixes_partial2{ 5 };

    auto affixLists() const { return std::array{ &affixes_base, &affixes_partial1, &affixes_partial2 }; }
    auto affixLists() { return std::array{ &affixes_base, &affixes_partial1, &affixes_partial2 }; }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};

// =========================================================

struct DataRow_sets : public RowBase {
    int level = 0;

    std::optional<int> version = 100;
    std::string        index;
    std::string        name;
    std::string        uiClass;
    ItemCodeList       filter;

    AffixPropList affixes_full{ 8 };
    AffixPropList affixes_partial1{ 4 };
    AffixPropList affixes_partial2{ 4 };

    std::string getId() const
    {
        return index;
    }

    auto affixLists() const { return std::array{ &affixes_full, &affixes_partial1, &affixes_partial2 }; }
    auto affixLists() { return std::array{ &affixes_full, &affixes_partial1, &affixes_partial2 }; }

    bool             isValid() const { return !name.empty(); }
    int              getLevel() const { return level; }
    ItemCodeList     getItemFilter() const { return filter; }
    AttributeFlagSet getAttributes() const
    {
        return AttributeFlagSet{};
    }

    std::vector<std::string> getColumns() const;
    void                     serialize();
};
struct DataRow_runes : public RowBase {
    int                      lvl = 1; // calculated.
    std::vector<std::string> runeCodes;
    AffixPropList            affixes{ 7 };
    std::optional<bool>      complete;
    std::vector<std::string> itype;
    std::vector<std::string> etype;
    ItemCodeList             filter;

    std::string name;

    std::string getId() const
    {
        return name;
    }

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    std::vector<std::string>           getColumns() const;
    std::map<std::string, std::string> getColumnAliases() const;
    void                               serialize();

    int getLevel() const
    {
        return isValid() ? lvl : 0;
    }

    ItemCodeList getItemFilter() const
    {
        return filter;
    }
    AttributeFlagSet getAttributes() const
    {
        return AttributeFlagSet{ AttributeFlag::Durability };
    }

    bool isValid() const
    {
        return complete.value_or(false);
    }

private:
    static const size_t maxRunes = 6;
    static const size_t maxItype = 6;
    static const size_t maxEtype = 3;
};

struct DataRow_gems : public RowBase {
    AffixPropList affixes_weapon{ 3 };
    AffixPropList affixes_helm{ 3 };
    AffixPropList affixes_shield{ 3 };

    EquipmentType eqipmentType = nullptr;
    std::string   eqipmentCode;

    auto affixLists() const { return std::array{ &affixes_weapon, &affixes_helm, &affixes_shield }; }
    auto affixLists() { return std::array{ &affixes_weapon, &affixes_helm, &affixes_shield }; }

    std::string getId() const
    {
        return eqipmentType->getId();
    }

    bool isValid() const
    {
        return !eqipmentCode.empty();
    }

    int getLevel() const
    {
        return isValid() ? eqipmentType->getLevel() : 0;
    }

    ItemCodeList getItemFilter() const
    {
        return eqipmentType->getItemFilter();
    }
    AttributeFlagSet getAttributes() const
    {
        return eqipmentType->getAttributes();
    }

    std::vector<std::string> getColumns() const;
    void                     serialize();
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
    void                     serialize();

private:
    static const size_t maxInputs = 7;
};

// =========================================================

}
