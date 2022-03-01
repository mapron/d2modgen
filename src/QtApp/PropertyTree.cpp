/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PropertyTree.hpp"

namespace D2ModGen {

bool PropertyTree::toBool() const noexcept
{
    if (!isScalar())
        return false;
    const auto& sc = getScalar();
    return toBool(sc);
}

std::string PropertyTree::toString() const noexcept
{
    if (!isScalar())
        return {};
    const auto& sc = getScalar();
    return toString(sc);
}

int64_t PropertyTree::toInt() const noexcept
{
    if (!isScalar())
        return 0;
    const auto& sc = getScalar();
    return toInt(sc);
}

double PropertyTree::toDouble() const noexcept
{
    if (!isScalar())
        return 0.;
    const auto& sc = getScalar();
    return toDouble(sc);
}

void PropertyTree::append(PropertyTree child)
{
    convertToList();
    PropertyTreeList& l = std::get<PropertyTreeList>(m_data);
    l.push_back(std::move(child));
}

void PropertyTree::insert(const std::string& key, PropertyTree child)
{
    convertToMap();
    PropertyTreeMap& m = std::get<PropertyTreeMap>(m_data);
    m[key]             = std::move(child);
}

void PropertyTree::convertToList() noexcept(false)
{
    if (m_data.index() == 0)
        m_data = PropertyTreeList{};
    assert(isList());
}

void PropertyTree::convertToMap() noexcept(false)
{
    if (m_data.index() == 0)
        m_data = PropertyTreeMap{};
    assert(isMap());
}

bool PropertyTree::toBool(const PropertyTreeScalar& scalar) noexcept
{
    if (const auto* bval = std::get_if<bool>(&scalar); bval)
        return *bval;
    if (const auto* ival = std::get_if<int64_t>(&scalar); ival)
        return *ival;
    if (const auto* dval = std::get_if<double>(&scalar); dval)
        return *dval;
    return false;
}

std::string PropertyTree::toString(const PropertyTreeScalar& scalar) noexcept
{
    if (const auto* sval = std::get_if<std::string>(&scalar); sval)
        return *sval;
    return {};
}

int64_t PropertyTree::toInt(const PropertyTreeScalar& scalar) noexcept
{
    if (const auto* bval = std::get_if<bool>(&scalar); bval)
        return *bval;
    if (const auto* ival = std::get_if<int64_t>(&scalar); ival)
        return *ival;
    if (const auto* dval = std::get_if<double>(&scalar); dval)
        return static_cast<int64_t>(*dval);
    return 0;
}

double PropertyTree::toDouble(const PropertyTreeScalar& scalar) noexcept
{
    if (const auto* ival = std::get_if<int64_t>(&scalar); ival)
        return static_cast<double>(*ival);
    if (const auto* dval = std::get_if<double>(&scalar); dval)
        return *dval;
    return 0.;
}

}
