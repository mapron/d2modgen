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
    struct PropertyCommon {
        std::string_view m_name;
        std::string      getName() const { return std::string(m_name); }
    };
    template<typename Scalar, typename Derived>
    struct PropertyScalar : public PropertyCommon {
        Scalar* m_pointer = nullptr;
        Scalar  m_zero{};
        Scalar  m_recc{};

        struct Preset {
            int    m_globalIndex = 0;
            Scalar m_value;
        };
        std::vector<Preset> m_presets;

        Derived& addPreset(int globalIndex, Scalar value)
        {
            Preset preset;
            preset.m_globalIndex = globalIndex;
            preset.m_value       = value;
            m_presets.push_back(std::move(preset));
            return static_cast<Derived&>(*this);
        }
        Derived& setParent(CommonStruct* parent)
        {
            m_parent = parent;
            return static_cast<Derived&>(*this);
        }
        Derived& setZero(Scalar value)
        {
            m_zero = value;
            m_recc = value;
            return static_cast<Derived&>(*this);
        }
        Derived& setRecc(Scalar value)
        {
            m_recc = value;
            return static_cast<Derived&>(*this);
        }

        CommonStruct* m_parent = nullptr;
        // this is to send notification from BL to UX, e.g. "we reloaded configuration, please update controls"
        std::function<void()> m_sendChangeToInner = [] {};

        void notifyChangeInner()
        {
            m_sendChangeToInner();
        }
        void notifyChangeOuter()
        {
            if (m_parent)
                return m_parent->notifyChangeOuter();
        }
        // this is set in UX to subscribe for changes from BL
        void setCallbackForOuterChanges(std::function<void()>&& callback)
        {
            m_sendChangeToInner = std::move(callback);
        }

        Scalar get() const { return *m_pointer; }
        void   set(const Scalar& val)
        {
            *m_pointer = val;
        }
        void activatePreset(int globalIndex)
        {
            for (auto& preset : m_presets)
                if (preset.m_globalIndex == globalIndex)
                    set(preset.m_value);
        }
    };

    struct PropertyInt : public PropertyScalar<int64_t, PropertyInt> {
        int64_t m_min         = 0;
        int64_t m_max         = 0;
        bool    m_isPercent   = false;
        bool    m_hasLogScale = false;

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
        PropertyInt& setLogScale()
        {
            m_hasLogScale = true;
            return *this;
        }
    };
    struct PropertyBool : public PropertyScalar<bool, PropertyBool> {
    };
    struct PropertyString : public PropertyScalar<std::string, PropertyString> {
    };

protected:
    template<typename Property, typename Scalar>
    struct PropertyContainer {
        Property& add(std::string_view name, Scalar& val)
        {
            Property prop;
            prop.m_name    = name;
            prop.m_pointer = &val;
            m_list.push_back(std::move(prop));
            return m_list.back();
        }
        void resetToZero()
        {
            for (auto& prop : m_list)
                *prop.m_pointer = prop.m_zero;
        }
        void resetToRecommended()
        {
            for (auto& prop : m_list)
                *prop.m_pointer = prop.m_recc;
        }
        void activatePreset(int globalIndex)
        {
            for (auto& prop : m_list)
                prop.activatePreset(globalIndex);
        }
        void readFromProperty(const Mernel::PropertyTree& propertyTree)
        {
            for (auto& prop : m_list)
                propertyTree.value(prop.getName(), Mernel::PropertyTreeScalar(prop.m_zero)).convertTo(*prop.m_pointer);
        }

        void writeToProperty(Mernel::PropertyTree& propertyTree) const
        {
            for (auto& prop : m_list) {
                if (*prop.m_pointer == prop.m_zero)
                    continue;

                propertyTree[prop.getName()] = Mernel::PropertyTreeScalar(*prop.m_pointer);
            }
        }
        void notifyChangeInner()
        {
            for (auto& prop : m_list)
                prop.notifyChangeInner();
        }

        void assign(PropertyContainer& other)
        {
            for (size_t i = 0; i < m_list.size() && i < other.m_list.size(); ++i)
                *(m_list[i].m_pointer) = *(other.m_list[i].m_pointer);
        }

        std::vector<Property> m_list;
    };

    PropertyContainer<PropertyInt, int64_t>        m_int;
    PropertyContainer<PropertyString, std::string> m_string;
    PropertyContainer<PropertyBool, bool>          m_bool;

    PropertyInt& addScalar(std::string_view name, int64_t& val)
    {
        return m_int.add(name, val).setParent(this);
    }
    PropertyBool& addScalar(std::string_view name, bool& val)
    {
        return m_bool.add(name, val).setParent(this);
    }
    PropertyString& addScalar(std::string_view name, std::string& val)
    {
        return m_string.add(name, val).setParent(this);
    }

    struct Child : public PropertyCommon {
        CommonStruct* m_record;
    };

    std::vector<Child> m_children;

    CommonStruct* m_parent = nullptr;

    // this is to send notification from UX to BL, e.g. "user changed slider, please make undo frame"
    std::function<void()> m_sendChangeToOuter = [] {};

    Child& addStruct(std::string_view name, CommonStruct& s)
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

        m_int.notifyChangeInner();
        m_bool.notifyChangeInner();
        m_string.notifyChangeInner();
    }

    // this called in UX to send notification to BL
    void notifyChangeOuter()
    {
        if (m_parent)
            return m_parent->notifyChangeOuter();

        m_sendChangeToOuter();
    }

    // this is set in BL to subscribe for changes from UX
    void setCallbackForInnerChanges(std::function<void()>&& callback)
    {
        m_sendChangeToOuter = std::move(callback);
    }

    void notifyInnerAndOuter(bool notifyTwoWay)
    {
        if (!notifyTwoWay)
            return;

        notifyChangeInner();
        notifyChangeOuter();
    }

    void resetToZero(bool notifyTwoWay = false)
    {
        m_int.resetToZero();
        m_bool.resetToZero();
        m_string.resetToZero();
        for (auto& child : m_children)
            child.m_record->resetToZero(false);

        notifyInnerAndOuter(notifyTwoWay);
    }
    void resetToRecommended(bool notifyTwoWay = false)
    {
        m_int.resetToRecommended();
        m_bool.resetToRecommended();
        m_string.resetToRecommended();
        for (auto& child : m_children)
            child.m_record->resetToRecommended();

        notifyInnerAndOuter(notifyTwoWay);
    }

    void assign(CommonStruct& another, bool notifyTwoWay = false)
    {
        m_int.assign(another.m_int);
        m_bool.assign(another.m_bool);
        m_string.assign(another.m_string);
        for (size_t i = 0, cnt = std::min(m_children.size(), another.m_children.size()); i < cnt; ++i)
            m_children[i].m_record->assign(*(another.m_children[i].m_record));

        notifyInnerAndOuter(notifyTwoWay);
    }

    void activatePreset(int globalIndex, bool notifyTwoWay = false)
    {
        m_int.activatePreset(globalIndex);
        m_bool.activatePreset(globalIndex);
        m_string.activatePreset(globalIndex);
        for (auto& child : m_children)
            child.m_record->activatePreset(globalIndex);

        notifyInnerAndOuter(notifyTwoWay);
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
        resetToZero();
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

    PropertyInt& getPropertyInt(size_t index)
    {
        return m_int.m_list[index];
    }
    PropertyBool& getPropertyBool(size_t index)
    {
        return m_bool.m_list[index];
    }
    PropertyString& getPropertyString(size_t index)
    {
        return m_string.m_list[index];
    }

    static inline std::vector<std::string> s_presetNames{};

    static bool setPresetNames(std::vector<std::string> names)
    {
        s_presetNames = std::move(names);
        return true;
    }
    static int getPresetIndex(const std::string& name)
    {
        auto it = std::find(s_presetNames.cbegin(), s_presetNames.cend(), name);
        if (it == s_presetNames.cend())
            return -1;
        return static_cast<int>(std::distance(s_presetNames.cbegin(), it));
    }
};

}
