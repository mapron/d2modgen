/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PropertyTree.hpp"

namespace D2ModGen {

bool PropertyTreeScalar::toBool() const noexcept
{
    if (const auto* bval = std::get_if<bool>(&m_data); bval)
        return *bval;
    if (const auto* ival = std::get_if<int64_t>(&m_data); ival)
        return *ival;
    if (const auto* dval = std::get_if<double>(&m_data); dval)
        return *dval;
    return false;
}

std::string PropertyTreeScalar::toString() const noexcept
{
    if (const auto* sval = std::get_if<std::string>(&m_data); sval)
        return *sval;
    return {};
}

const char* PropertyTreeScalar::toCString() const noexcept
{
    if (const auto* sval = std::get_if<std::string>(&m_data); sval)
        return sval->c_str();
    return nullptr;
}

int64_t PropertyTreeScalar::toInt() const noexcept
{
    if (const auto* bval = std::get_if<bool>(&m_data); bval)
        return *bval;
    if (const auto* ival = std::get_if<int64_t>(&m_data); ival)
        return *ival;
    if (const auto* dval = std::get_if<double>(&m_data); dval)
        return static_cast<int64_t>(*dval);
    return 0;
}

double PropertyTreeScalar::toDouble() const noexcept
{
    if (const auto* ival = std::get_if<int64_t>(&m_data); ival)
        return static_cast<double>(*ival);
    if (const auto* dval = std::get_if<double>(&m_data); dval)
        return *dval;
    return 0.;
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

void PropertyTree::merge(const PropertyTree& another) noexcept(false)
{
    convertToMap();
    if (!another.isMap())
        return;
    PropertyTreeMap& m = std::get<PropertyTreeMap>(m_data);
    for (const auto& p : another.getMap())
        m[p.first] = p.second;
}

void PropertyTree::removeEqualValues(const PropertyTree& another) noexcept(false)
{
    convertToMap();
    if (!another.isMap())
        return;
    PropertyTreeMap& m = std::get<PropertyTreeMap>(m_data);
    for (const auto& p : another.getMap()) {
        if (m.contains(p.first) && m[p.first] == p.second)
            m.erase(p.first);
    }
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

}
