/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "AttributeHelper.hpp"
#include "AttributeValues.hpp"

#include <unordered_map>
#include <optional>

namespace D2ModGen {

class DataRow_itemtypes;
class RowBaseEquipment;
using EquipmentType = const RowBaseEquipment*;
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

    std::vector<std::string> requestedColumns;
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

    bool parseTable(const std::string& buffer);

    bool serializeTable(std::string& buffer, bool full) const;
};

struct AffixProp {
    std::string        code;
    std::string        par;
    std::optional<int> min;
    std::optional<int> max;
    std::optional<int> chance;

    bool moveParToMinMax()
    {
        int val = parAsInt();
        if (!val)
            return false;
        min = max = val;
        par.clear();
        return true;
    }
    bool hasPar() const { return !(par.empty() || par == "0" || par == " "); }
    int  parAsInt() const { return hasPar() ? std::atoi(par.c_str()) : 0; }
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
    bool                       _ser_read = false;
    std::optional<std::string> _ser_key;
    size_t                     _ser_cellIndex = 0;
    std::vector<std::string>   _ser_unusedCells;
    std::vector<std::string>   _ser_usedCells;

    static const std::string s_empty;

    std::map<std::string, std::string> getColumnAliases() const { return {}; }

    void ser(int& data);
    void ser(std::optional<int>& data);
    void ser(std::optional<bool>& data);
    void ser(std::optional<bool>& data, bool valueIfNoColumn);
    void ser(std::string& data);

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

    void ser(AffixPropList& data, bool useChance = false)
    {
        serArray(data, data.getLimit(), [this, useChance](AffixProp& item) {
            ser(item.code);
            ser(item.par);
            ser(item.min);
            ser(item.max);
            if (useChance)
                ser(item.chance); }, [](AffixProp& item) { return !item.code.empty(); });
    }

    void ser(std::vector<std::string>& data, size_t size)
    {
        serArray(data, size, [this](std::string& item) { ser(item); }, [](std::string& item) { return !item.empty(); });
    }
    void ser(std::vector<int>& data, size_t size)
    {
        serArray(data, size, [this](int& item) { ser(item); }, [](int& item) { return item > 0; });
    }
    template<class T, size_t size>
    void ser(std::array<T, size>& data)
    {
        for (size_t i = 0; i < size; ++i)
            ser(data[i]);
    }
};

struct RowBaseAffixes : public RowBase {
    std::optional<int>  lvl;
    std::optional<int>  maxlevel;
    std::optional<int>  lvlreq;
    std::optional<bool> spawnable;
    AffixPropList       affixes{ 3 };

    std::vector<std::string> itype; //7
    std::vector<std::string> etype; // 5
    ItemCodeList             filter;

    std::string name;

    std::string getId() const
    {
        return name;
    }

    auto affixLists() const { return std::array{ &affixes }; }
    auto affixLists() { return std::array{ &affixes }; }

    int getLevel() const { return spawnable.value_or(false) ? lvl.value_or(0) : 0; }

    bool isValid() const
    {
        return getLevel() > 0;
    }

    ItemCodeList getItemFilter() const
    {
        return filter;
    }
    AttributeFlagSet getAttributes() const
    {
        return AttributeFlagSet{};
    }

    std::vector<std::string> getColumns() const;
    void                     serialize();
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

struct RowBaseEquipment : public RowBase {
    EquipmentCategory   category = EquipmentCategory::Misc;
    std::optional<int>  lvl;
    std::optional<int>  lvlreq;
    std::optional<int>  reqstr;
    std::optional<int>  reqdex;
    std::optional<bool> showLevel;

    std::string name;
    std::string code;
    std::string itemTypeCode;
    ItemType    itemType = nullptr;

    std::string normcode;
    std::string ubercode;
    std::string ultracode;

    Tier getTier() const
    {
        if (!getLevel() || ultracode.empty())
            return Tier::None;
        if (ultracode == code)
            return Tier::Elite;
        if (ubercode == code)
            return Tier::Exceptional;
        if (normcode == code)
            return Tier::Normal;

        return Tier::None;
    }

    std::string getId() const
    {
        return code;
    }

    int getLevel() const
    {
        if (lvl.has_value() && lvl.value() > 0)
            return lvl.value();

        if (lvlreq.has_value())
            return lvlreq.value();
        return 0;
    }

    bool isValid() const
    {
        return !itemTypeCode.empty() && (getLevel() > 0 || category == EquipmentCategory::Misc);
    }

    ItemCodeList getItemFilter() const
    {
        ItemCodeList result;
        if (itemType)
            result.push_back(itemType);
        return result;
    }

    AttributeFlagSet getAttributes() const;
};

}
