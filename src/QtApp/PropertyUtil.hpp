/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "MernelPlatform/PropertyTree.hpp"

#include <QVariant>

namespace D2ModGen {

inline Mernel::PropertyTree qvariantToProperty(const QVariant& value);
inline Mernel::PropertyTree qvariantToProperty(const QVariantMap& value)
{
    Mernel::PropertyTree result;
    result.convertToMap();
    for (auto it = value.constBegin(); it != value.constEnd(); ++it)
        result.insert(it.key().toStdString(), qvariantToProperty(it.value()));

    return result;
}

inline Mernel::PropertyTree qvariantToProperty(const QVariantList& value)
{
    Mernel::PropertyTree result;
    result.convertToList();
    for (const QVariant& iter : value)
        result.append(qvariantToProperty(iter));

    return result;
}

inline Mernel::PropertyTree qvariantToProperty(const QVariant& value)
{
    if (value.isNull())
        return Mernel::PropertyTree{};
    if (value.type() == QVariant::Bool)
        return Mernel::PropertyTreeScalar(value.toBool());
    if (value.type() == QVariant::Int)
        return Mernel::PropertyTreeScalar(value.toInt());
    if (value.type() == QVariant::Double)
        return Mernel::PropertyTreeScalar(value.toDouble());
    if (value.type() == QVariant::String)
        return Mernel::PropertyTreeScalar(value.toString().toStdString());
    if (value.type() == QVariant::List)
        return qvariantToProperty(value.toList());
    if (value.type() == QVariant::Map)
        return qvariantToProperty(value.toMap());
    qWarning() << "unhandled type:" << value;
    return {};
}

inline QVariant propertyToQVariant(const Mernel::PropertyTree& value)
{
    if (value.isNull())
        return QVariant();
    if (value.isScalar()) {
        const auto& sc = value.getScalar();
        if (sc.isBool())
            return QVariant(sc.toBool());
        if (sc.isInt())
            return qlonglong(sc.toInt());
        if (sc.isDouble())
            return sc.toDouble();
        if (sc.isString())
            return QString::fromStdString(sc.toString());
    }
    if (value.isList()) {
        QList<QVariant> arr;
        for (const auto& child : value.getList())
            arr << propertyToQVariant(child);
        return arr;
    }
    if (value.isMap()) {
        QMap<QString, QVariant> obj;
        for (const auto& child : value.getMap())
            obj[child.first.c_str()] = propertyToQVariant(child.second);
        return obj;
    }
    return {};
}
}
