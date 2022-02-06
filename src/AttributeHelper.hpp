/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QString>
#include <QSet>

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
};
inline uint qHash(AttributeFlag key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

using AttributeFlagSet = QSet<AttributeFlag>;

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
inline uint qHash(AttributeItemReq key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

using AttributeItemReqSet = QSet<AttributeItemReq>;

struct AttributeDesc {
    QString             code;
    AttributeFlagSet    flags;
    AttributeItemReqSet items;
};

bool isUnusedAttribute(const QString& code);
bool isMinMaxRange(const QString& code);

const AttributeDesc& getAttributeDesc(const QString& code);

class UniqueAttributeChecker {
    QSet<QString> m_data;

public:
    void add(const QString& attr);
    void add(const QSet<QString>& attrs);

    bool contains(const QString& attr) const { return m_data.contains(attr); }
};

}
