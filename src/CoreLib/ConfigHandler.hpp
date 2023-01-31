/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"
#include "PropertyTree.hpp"

#include "ModgenCoreExport.hpp"

namespace D2ModGen {
class StorageCache;
class MODGENCORE_EXPORT ConfigHandler {
public:
    explicit ConfigHandler(const std::string& pluginsRoot);
    ConfigHandler(const ConfigHandler&) = delete;
    ~ConfigHandler();

    bool loadConfig(const std::string& filename);
    bool saveConfig(const std::string& filename) const;

    bool loadConfig(const PropertyTree& data);
    bool saveConfig(PropertyTree& data) const;

    bool isConfigEnabled(const std::string& key) const;
    void setConfigEnabled(const std::string& key, bool value);

    IModule::Ptr getModule(std::string_view key) const;

    struct GenerateResult {
        std::string m_error;
        bool        m_success = false;
    };

    GenerateResult generate();

    GenerationEnvironment getEnv() const;

    PropertyTree m_currentMainConfig;
    struct ModuleData {
        IModule::Ptr m_module;
        PropertyTree m_currentConfig;
        bool         m_enabled = true;
        int64_t      m_order   = 1000;
    };

    std::map<std::string, ModuleData> m_modules;
    std::vector<std::string>          m_pluginIds;

    std::unique_ptr<StorageCache> m_mainStorageCache;
};

}
