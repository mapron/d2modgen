/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "AttributeValues.hpp"
#include "MernelPlatform/StringUtils.hpp"
#include "Scale.hpp"
#include "Tables.hpp"

#include <unordered_map>
#include <optional>
#include <set>

namespace D2ModGen {

class DataRow_itemtypes;
struct IBaseEquipment;
using EquipmentType = const IBaseEquipment*;
using ItemType      = const DataRow_itemtypes*;
using ItemTypeSet   = std::set<ItemType>;

struct ItemCodeList : public std::vector<ItemType> {
    ItemCodeList() = default;
    ItemCodeList(const ItemTypeSet& codes)
        : std::vector<ItemType>(codes.cbegin(), codes.cend())
    {}

    Character      determineClass() const;
    SlotType       determineSlot() const;
    AttributeIdiom determineIdiom() const;
    std::string    toString() const;
};

template<class Row>
struct DataTable {
    std::vector<Row> rows;
    std::vector<Row> extra;

    std::vector<std::string> requestedColumns;
    std::vector<std::string> columnsOriginal;
    std::vector<std::string> columnsUsed;

    // Requested column index -> original CSV column index.
    std::vector<size_t> indicesUsed;

    // Original CSV columns not consumed by the generated schema.
    std::vector<size_t> indicesUnused;

    std::unordered_map<std::string, int> indexN;

    std::string Row::* indexField = nullptr;

    bool valid = false;

    template<class F, class FieldType, class Row1>
    void applyTransform(FieldType Row1::* m, F&& t)
    {
        for (Row& row : rows)
            t(row.*m);
    }

    void applyIntTransform(std::optional<int> Row::* m, Percent mult)
    {
        applyTransform(m, [mult](std::optional<int>& data) {
            if (!data.has_value())
                return;

            data = data.value() * mult;
        });
    }

    void makeIndex(std::string Row::* m)
    {
        indexField = m;
        updateIndex();
    }

    void updateIndex()
    {
        if (!indexField)
            return;

        indexN.clear();

        for (int i = 0; auto& row : rows) {
            auto key    = Mernel::strToLower(row.*indexField);
            indexN[key] = i++;
        }
    }

    bool parseTable(const std::string& buffer);

    bool serializeTable(std::string& buffer, bool full) const;

    const Row* find(const std::string& code) const
    {
        const auto idx = findN(code);
        return idx == size_t(-1) ? nullptr : &rows[idx];
    }

    Row* find(const std::string& code)
    {
        const auto idx = findN(code);
        return idx == size_t(-1) ? nullptr : &rows[idx];
    }

    size_t findN(const std::string& code) const
    {
        const auto it = indexN.find(code);
        return it == indexN.cend() ? size_t(-1) : it->second;
    }

    void commitExtraRows()
    {
        rows.insert(rows.end(), extra.begin(), extra.end());
        extra.clear();
        updateIndex();
    }

private:
    void resolveColumns();
};

using AffixPropList = std::vector<AffixProp_codegen>;

struct IItem {
    virtual ~IItem() = default;

    virtual std::string getId() const    = 0;
    virtual int         getLevel() const = 0;

    virtual bool isValid() const = 0;

    virtual ItemCodeList     getItemFilter() const = 0;
    virtual AttributeFlagSet getAttributes() const = 0;

    bool operator==(const IItem&) const noexcept { return true; }
};

struct IAffixContainer : public IItem {
    using AffixListListConst   = std::vector<const AffixPropList*>;
    using AffixListListMutable = std::vector<AffixPropList*>;
    using Limits               = std::vector<size_t>;

    virtual AffixListListConst   affixLists() const  = 0;
    virtual AffixListListMutable affixLists()        = 0;
    virtual Limits               affixLimits() const = 0;

    bool operator==(const IAffixContainer&) const noexcept { return true; }
};

struct RowBase {
    // Cells from unrecognized CSV columns, preserved for full serialization.
    std::vector<std::string> _ser_unusedCells;

    // Optional key used by partial/diff serialization.
    std::optional<std::string> _ser_key;

    bool operator==(const RowBase&) const noexcept
    {
        return true;
    }
};

template<class CRTP>
struct RowBaseAffixes
    : public RowBase
    , public IAffixContainer
    , public CRTP {
    bool operator==(const RowBaseAffixes&) const noexcept = default;

    ItemCodeList filter;

    std::string getId() const override
    {
        return this->name;
    }

    AffixListListConst   affixLists() const override { return { &this->affixes }; }
    AffixListListMutable affixLists() override { return { &this->affixes }; }
    Limits               affixLimits() const override { return { this->affixes_limit }; }

    int getLevel() const override { return this->spawnable.value_or(false) ? this->lvl.value_or(0) : 0; }

    bool isValid() const override
    {
        return getLevel() > 0;
    }

    ItemCodeList getItemFilter() const override
    {
        return this->filter;
    }
    AttributeFlagSet getAttributes() const override
    {
        return AttributeFlagSet{};
    }
};

enum class Tier
{
    None,
    Normal,
    Exceptional,
    Elite,
};

enum class EquipmentCategory
{
    Misc,
    Weapon,
    Armor,
};

struct IBaseEquipment : public IItem {
    virtual Tier              getTier() const     = 0;
    virtual EquipmentCategory getCategory() const = 0;
    virtual std::string       getName() const     = 0;
    virtual std::string       getCode() const     = 0;
};

template<class CRTP>
struct RowBaseEquipment
    : public RowBase
    , public IBaseEquipment
    , public CRTP {
    bool operator==(const RowBaseEquipment&) const noexcept = default;

    EquipmentCategory category = EquipmentCategory::Misc;
    ItemType          itemType = nullptr;

    EquipmentCategory getCategory() const override
    {
        return category;
    }
    std::string getName() const override { return this->name; }
    std::string getCode() const override { return this->code; }

    std::string getId() const override
    {
        return this->code;
    }

    int getLevel() const override
    {
        if (this->lvl.has_value() && this->lvl.value() > 0)
            return this->lvl.value();

        if (this->lvlreq.has_value())
            return this->lvlreq.value();
        return 0;
    }

    bool isValid() const override
    {
        return !this->itemTypeCode.empty() && (getLevel() > 0 || category == EquipmentCategory::Misc);
    }

    ItemCodeList getItemFilter() const override
    {
        ItemCodeList result;
        if (this->itemType)
            result.push_back(this->itemType);
        return result;
    }

    AttributeFlagSet getAttributes() const override;
};

}
