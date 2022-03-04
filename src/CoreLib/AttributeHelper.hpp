/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

enum class AttributeFlag
{
    ANY,
    Damage,
    Attack,
    Defense,
    DamageReduction,
    Resistance,
    Stats,
    Speed,
    Skills,
    PerLevel,
    Durability,
    Quantity,
    Missile,
    Sockets,
    Leech,
    OP,
    NoMinMax,
    PD2Map,
};

using AttributeFlagSet = std::set<AttributeFlag>;

enum class AttributeItemReq
{
    Weapon,
    Armor,
    Shield,
    Chest,
    Helm,
    Throwing,
    Bows,
};

using AttributeItemReqSet = std::set<AttributeItemReq>;

struct AttributeDesc {
    std::string         code;
    AttributeFlagSet    flags;
    AttributeItemReqSet items;
};

enum class AttributeConsume
{
    Known,
    Skip,
    Keep,
};

AttributeConsume getAttributeConsume(const std::string& code);
bool             isMinMaxRange(const std::string& code);

const AttributeDesc& getAttributeDesc(const std::string& code);

class UniqueAttributeChecker {
    StringSet m_data;

public:
    void add(const std::string& attr);
    void add(const StringSet& attrs);

    bool contains(const std::string& attr) const { return m_data.contains(attr); }
};

}
