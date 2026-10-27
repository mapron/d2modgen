/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextBase.hpp"
#include "DataContextEquipment.hpp"

namespace D2ModGen {

const std::string RowBase::s_empty("\0\0", 2);

void RowBase::ser(int& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty() || str == s_empty) {
            data = 0;
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        str = std::to_string(data);
    }
}

void RowBase::ser(std::optional<int>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty() || str == s_empty) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string(data.value());
        else
            str = {};
    }
}

void RowBase::ser(std::optional<bool>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty() || str == s_empty) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string((int) data.value());
        else
            str = {};
    }
}

void RowBase::ser(std::optional<bool>& data, bool valueIfNoColumn)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data.reset();
        } else if (str == s_empty) {
            data = valueIfNoColumn;
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string((int) data.value());
        else
            str = {};
    }
}

void RowBase::ser(std::string& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        data = str == s_empty ? "" : str;
    } else {
        str = data;
    }
}

AttributeFlagSet RowBaseEquipment::getAttributes() const
{
    return itemType ? itemType->flags : AttributeFlagSet{};
}

Character ItemCodeList::determineClass() const
{
    for (ItemType type : *this)
        if (type->classSpecific != Character::None)
            return type->classSpecific;

    return Character::None;
}

SlotType ItemCodeList::determineSlot() const
{
    for (ItemType type : *this) {
        auto slot = type->determineSlot();
        if (slot != SlotType::None)
            return slot;
    }

    return SlotType::None;
}

AttributeIdiom ItemCodeList::determineIdiom() const
{
    for (ItemType type : *this) {
        auto slot = type->determineIdiom();
        if (slot != AttributeIdiom::None)
            return slot;
    }

    return AttributeIdiom::None;
}

std::string ItemCodeList::toString() const
{
    std::string res = "";
    for (ItemType type : *this) {
        res += type->code + ",";
    }
    return res;
}

std::vector<std::string> RowBaseAffixes::getColumns() const
{
    std::vector<std::string> result{ "name", "level", "maxlevel", "levelreq", "spawnable" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("mod" + istr + "code");
        result.push_back("mod" + istr + "param");
        result.push_back("mod" + istr + "min");
        result.push_back("mod" + istr + "max");
    }
    for (size_t i = 0; i < 7; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("itype" + istr);
    }
    for (size_t i = 0; i < 5; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("etype" + istr);
    }

    return result;
}

void RowBaseAffixes::serialize()
{
    ser(name);
    ser(lvl);
    ser(maxlevel);
    ser(lvlreq);
    ser(spawnable);
    ser(affixes);
    ser(itype, 7);
    ser(etype, 5);
}

}
