/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"
#include "MernelPlatform/PropertyTree.hpp"

#include <memory>

namespace D2ModGen {

struct DataContext;
class IModule {
public:
    using Ptr    = std::shared_ptr<const IModule>;
    using PtrMap = std::map<std::string, Ptr>;

    struct PreGenerationContext {
        StringSet m_extraJson;
    };

    struct InputContext {
        Mernel::PropertyTree  m_settings;
        Mernel::PropertyTree  m_mergedSettings;
        GenerationEnvironment m_env;

        bool isAllDefault() const noexcept
        {
            if (!m_settings.isMap())
                return true;
            auto& m = m_settings.getMap();
            return m.empty();
        }
        bool isAllDefault(const StringVector& keys) const noexcept
        {
            if (!m_settings.isMap())
                return true;

            for (const std::string& key : keys) {
                if (m_settings.contains(key))
                    return false;
            }
            return true;
        }
        bool isDefault(const std::string& key) const noexcept
        {
            return !m_settings.contains(key);
        }

        const Mernel::PropertyTreeScalar& getScalar(const std::string& key) const noexcept(false)
        {
            auto it = m_mergedSettings.getMap().find(key);
            if (it == m_mergedSettings.getMap().cend())
                throw std::runtime_error("no key'" + key + "'");
            return it->second.getScalar();
        }

        int getInt(const std::string& key) const noexcept(false)
        {
            return getScalar(key).toInt();
        }
    };

public:
    virtual const Mernel::PropertyTreeMap&  defaultValues() const = 0;
    virtual const Mernel::PropertyTreeList& presets() const       = 0;

    virtual void gatherInfo(PreGenerationContext& output) const                                       = 0;
    virtual void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const = 0;
};

}
