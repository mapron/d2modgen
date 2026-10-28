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

struct PropertyList {
protected:
    struct PropertyCommon {
        std::string_view m_name;
    };

    struct PropertyInt : public PropertyCommon {
        int64_t* m_pointer = nullptr;
        int64_t  m_default = 0;
        int64_t  m_min     = 0;
        int64_t  m_max     = 0;
    };
    struct PropertyBool : public PropertyCommon {
        bool* m_pointer = nullptr;
        bool  m_default = false;
    };
    struct PropertyString : public PropertyCommon {
        std::string* m_pointer = nullptr;
    };

    std::vector<PropertyInt>    m_int;
    std::vector<PropertyString> m_string;
    std::vector<PropertyBool>   m_bool;

    void add(std::string_view name, int64_t& val, int64_t d, int64_t min, int64_t max)
    {
        PropertyInt prop;
        prop.m_name    = name;
        prop.m_pointer = &val;
        prop.m_default = d;
        prop.m_min     = min;
        prop.m_max     = max;
        m_int.push_back(std::move(prop));
    }
    void add(std::string_view name, bool& val, bool d)
    {
        PropertyBool prop;
        prop.m_name    = name;
        prop.m_pointer = &val;
        prop.m_default = d;
        m_bool.push_back(std::move(prop));
    }
    void add(std::string_view name, std::string& val)
    {
        PropertyString prop;
        prop.m_name    = name;
        prop.m_pointer = &val;
        m_string.push_back(std::move(prop));
    }

    void resetToDefaults()
    {
        for (auto& prop : m_int)
            *prop.m_pointer = prop.m_default;
        for (auto& prop : m_bool)
            *prop.m_pointer = prop.m_default;
        for (auto& prop : m_string)
            *prop.m_pointer = {};
    }
    void validateAndClamp()
    {
        for (auto& prop : m_int)
            *prop.m_pointer = std::clamp(*prop.m_pointer, prop.m_min, prop.m_max);
    }
};

struct CommonStruct : protected PropertyList {
protected:
    std::vector<std::pair<std::string_view, CommonStruct*>> m_children;

    CommonStruct* m_parent = nullptr;

    // this is to send notification from UX to BL, e.g. "user changed slider, please make undo frame"
    std::function<void()> m_sendChangeToOuter = [] {};

    // this is to send notification from BL to UX, e.g. "we reloaded configuration, please update controls"
    std::function<void()> m_sendChangeToInner = [] {};

    using PropertyList::add;
    void add(std::string_view name, CommonStruct& s)
    {
        s.m_parent = this;
        m_children.push_back({ name, &s });
    }

public:
    // this called in BL to send notification to UX
    void notifyChangeInner()
    {
        for (auto& [name, child] : m_children) {
            child->notifyChangeInner();
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
        PropertyList::resetToDefaults();
        for (auto& [name, child] : m_children) {
            child->resetToDefaults();
        }
    }
    void validateAndClamp()
    {
        PropertyList::validateAndClamp();
        for (auto& [name, child] : m_children) {
            child->validateAndClamp();
        }
    }

    void readFromProperty(const Mernel::PropertyTree& propertyTree)
    {
        resetToDefaults();
        for (auto& prop : m_int)
            *prop.m_pointer = propertyTree.value(std::string(prop.m_name), Mernel::PropertyTreeScalar(prop.m_default)).toInt();
        for (auto& prop : m_bool)
            *prop.m_pointer = propertyTree.value(std::string(prop.m_name), Mernel::PropertyTreeScalar(prop.m_default)).toBool();
        for (auto& prop : m_string)
            *prop.m_pointer = propertyTree.value(std::string(prop.m_name), Mernel::PropertyTreeScalar("")).toString();

        validateAndClamp();
        for (auto& [name, child] : m_children) {
            std::string key(name);
            if (!propertyTree.contains(key)) {
                continue;
            }
            child->readFromProperty(propertyTree[key]);
        }
    }

    void writeToProperty(Mernel::PropertyTree& propertyTree)
    {
        propertyTree = {};
        for (auto& prop : m_int) {
            if (*prop.m_pointer == prop.m_default)
                continue;

            propertyTree[std::string(prop.m_name)] = Mernel::PropertyTreeScalar(*prop.m_pointer);
        }
        for (auto& prop : m_bool) {
            if (*prop.m_pointer == prop.m_default)
                continue;

            propertyTree[std::string(prop.m_name)] = Mernel::PropertyTreeScalar(*prop.m_pointer);
        }
        for (auto& prop : m_string) {
            if (*prop.m_pointer == std::string{})
                continue;

            propertyTree[std::string(prop.m_name)] = Mernel::PropertyTreeScalar(*prop.m_pointer);
        }
        for (auto& [name, child] : m_children) {
            Mernel::PropertyTree childData;
            child->writeToProperty(childData);
            if (childData.isNull())
                continue;

            propertyTree[std::string(name)] = std::move(childData);
            std::string key(name);
        }
    }
};

}
