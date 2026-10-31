/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "MernelPlatform/PropertyTree.hpp"

#include <algorithm>
#include <functional>

namespace D2ModGen {

struct CommonStruct {
protected:
    template<typename Scalar>
    struct PropertyPreset {
        int    m_index = 0;
        Scalar m_value;
    };
    struct PropertyCommon {
        std::string_view m_name;
        std::string      getName() const { return std::string(m_name); }
    };
    template<typename Scalar, typename Derived>
    struct PropertyScalar : public PropertyCommon {
        Scalar* m_pointer = nullptr;
        Scalar  m_default{};

        using Preset = PropertyPreset<int64_t>;
        std::vector<Preset> m_presets;

        Derived& addPreset(int index, Scalar value)
        {
            Preset preset;
            preset.m_index = index;
            preset.m_value = value;
            m_presets.push_back(std::move(preset));
            return static_cast<Derived&>(*this);
        }
    };

    struct PropertyInt : public PropertyScalar<int64_t, PropertyInt> {
        int64_t m_min       = 0;
        int64_t m_max       = 0;
        bool    m_isPercent = false;

        PropertyInt& setRange(int64_t min, int64_t max)
        {
            m_min = min;
            m_max = max;
            return *this;
        }
        PropertyInt& setPercent()
        {
            m_isPercent = true;
            return *this;
        }
    };
    struct PropertyBool : public PropertyScalar<bool, PropertyBool> {
    };
    struct PropertyString : public PropertyScalar<std::string, PropertyString> {
    };

    template<typename Property, typename Scalar>
    struct PropertyContainer {
        Property& add(std::string_view name, Scalar& val, Scalar d)
        {
            Property prop;
            prop.m_name    = name;
            prop.m_pointer = &val;
            prop.m_default = d;
            m_list.push_back(std::move(prop));
            return m_list.back();
        }
        void resetToDefaults()
        {
            for (auto& prop : m_list)
                *prop.m_pointer = prop.m_default;
        }
        void readFromProperty(const Mernel::PropertyTree& propertyTree)
        {
            for (auto& prop : m_list)
                propertyTree.value(prop.getName(), Mernel::PropertyTreeScalar(prop.m_default)).convertTo(*prop.m_pointer);
        }

        void writeToProperty(Mernel::PropertyTree& propertyTree) const
        {
            for (auto& prop : m_list) {
                if (*prop.m_pointer == prop.m_default)
                    continue;

                propertyTree[prop.getName()] = Mernel::PropertyTreeScalar(*prop.m_pointer);
            }
        }

        std::vector<Property> m_list;
    };

    PropertyContainer<PropertyInt, int64_t>        m_int;
    PropertyContainer<PropertyString, std::string> m_string;
    PropertyContainer<PropertyBool, bool>          m_bool;

    PropertyInt& add(std::string_view name, int64_t& val, int64_t d)
    {
        return m_int.add(name, val, d);
    }
    PropertyBool& add(std::string_view name, bool& val, bool d)
    {
        return m_bool.add(name, val, d);
    }
    PropertyString& add(std::string_view name, std::string& val, std::string d)
    {
        return m_string.add(name, val, d);
    }

    struct Child : public PropertyCommon {
        CommonStruct* m_record;
    };

    std::vector<Child> m_children;

    CommonStruct* m_parent = nullptr;

    // this is to send notification from UX to BL, e.g. "user changed slider, please make undo frame"
    std::function<void()> m_sendChangeToOuter = [] {};

    // this is to send notification from BL to UX, e.g. "we reloaded configuration, please update controls"
    std::function<void()> m_sendChangeToInner = [] {};

    Child& add(std::string_view name, CommonStruct& s)
    {
        s.m_parent = this;
        Child child;
        child.m_name   = name;
        child.m_record = &s;
        m_children.push_back(std::move(child));
        return m_children.back();
    }

public:
    // this called in BL to send notification to UX
    void notifyChangeInner()
    {
        for (auto& child : m_children) {
            child.m_record->notifyChangeInner();
        }
        m_sendChangeToInner();
    }

    // this called in UX to send notification to BL
    void notifyChangeOuter()
    {
        if (m_parent)
            return m_parent->notifyChangeOuter();

        m_sendChangeToOuter();
    }

    // this is set in UX to subscribe for changes from BL
    void setCallbackForOuterChanges(std::function<void()>&& callback)
    {
        m_sendChangeToInner = std::move(callback);
    }
    // this is set in BL to subscribe for changes from UX
    void setCallbackForInnerChanges(std::function<void()>&& callback)
    {
        m_sendChangeToOuter = std::move(callback);
    }

    void resetToDefaults()
    {
        m_int.resetToDefaults();
        m_bool.resetToDefaults();
        m_string.resetToDefaults();
        for (auto& child : m_children) {
            child.m_record->resetToDefaults();
        }
    }
    void validateAndClamp()
    {
        for (auto& prop : m_int.m_list)
            *prop.m_pointer = std::clamp(*prop.m_pointer, prop.m_min, prop.m_max);
        for (auto& child : m_children) {
            child.m_record->validateAndClamp();
        }
    }

    void readFromPropertyImpl(const Mernel::PropertyTree& propertyTree)
    {
        m_int.readFromProperty(propertyTree);
        m_bool.readFromProperty(propertyTree);
        m_string.readFromProperty(propertyTree);

        for (auto& child : m_children) {
            const std::string key = child.getName();
            if (!propertyTree.contains(key)) {
                continue;
            }
            child.m_record->readFromPropertyImpl(propertyTree[key]);
        }
    }

    void readFromProperty(const Mernel::PropertyTree& propertyTree)
    {
        resetToDefaults();
        readFromPropertyImpl(propertyTree);
        validateAndClamp();
    }

    void writeToProperty(Mernel::PropertyTree& propertyTree) const
    {
        propertyTree = {};
        m_int.writeToProperty(propertyTree);
        m_bool.writeToProperty(propertyTree);
        m_string.writeToProperty(propertyTree);

        for (auto& child : m_children) {
            Mernel::PropertyTree childData;
            child.m_record->writeToProperty(childData);
            if (childData.isNull())
                continue;

            propertyTree[child.getName()] = std::move(childData);
        }
    }
};

}
