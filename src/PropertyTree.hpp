/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <cassert>

namespace D2ModGen {

using PropertyTreeScalar = std::variant<std::monostate, bool, int64_t, double, std::string>;
class PropertyTree;

using PropertyTreeList = std::vector<PropertyTree>;
using PropertyTreeMap  = std::map<std::string, PropertyTree>;

class PropertyTree {
public:
    PropertyTree()                    = default;
    PropertyTree(const PropertyTree&) = default;

    PropertyTree(const PropertyTreeScalar& scalar)
        : m_data(scalar)
    {}

    // check what is inside
    bool isNull() const noexcept { return m_data.index() == 0; }
    bool isScalar() const noexcept { return m_data.index() == 1; }
    bool isList() const noexcept { return m_data.index() == 2; }
    bool isMap() const noexcept { return m_data.index() == 3; }

    // const access to scalar and containers (without checking). May throw.
    const auto& getScalar() const noexcept(false) { return std::get<PropertyTreeScalar>(m_data); }
    const auto& getList() const noexcept(false) { return std::get<PropertyTreeList>(m_data); }
    const auto& getMap() const noexcept(false) { return std::get<PropertyTreeMap>(m_data); }

    auto& getList() noexcept(false) { return std::get<PropertyTreeList>(m_data); }
    auto& getMap() noexcept(false) { return std::get<PropertyTreeMap>(m_data); }

    // convert value to standard scalar types. If conversion cannot be made, returns default value.
    bool        toBool() const noexcept;
    std::string toString() const noexcept;
    int64_t     toInt() const noexcept;
    double      toDouble() const noexcept;

    // checks if container have child object with provided key. returns false if property not a map.
    bool contains(const std::string& key) const noexcept
    {
        if (!isMap())
            return false;
        return getMap().contains(key);
    }

    // get direct access to child value without checking by key.
    const PropertyTree& operator[](const std::string& key) const noexcept(false)
    {
        return std::get<PropertyTreeMap>(m_data).at(key);
    }
    // add new key into map or modify existing one. property will automatically converted to map type.
    PropertyTree& operator[](const std::string& key) noexcept(false)
    {
        if (m_data.index() == 0)
            m_data = PropertyTreeMap{};
        return std::get<PropertyTreeMap>(m_data)[key];
    }
    PropertyTree value(const std::string& key, PropertyTreeScalar defaultValue) const noexcept(false)
    {
        if (!isMap())
            return std::move(defaultValue);
        const auto& map = std::get<PropertyTreeMap>(m_data);
        auto        it  = map.find(key);
        if (it == map.cend())
            return std::move(defaultValue);
        return it->second;
    }

    void append(PropertyTree child) noexcept(false);
    void insert(const std::string& key, PropertyTree child) noexcept(false);

    void convertToList() noexcept(false);
    void convertToMap() noexcept(false);

private:
    using Variant = std::variant<std::monostate, PropertyTreeScalar, PropertyTreeList, PropertyTreeMap>;
    Variant m_data;
};

}
