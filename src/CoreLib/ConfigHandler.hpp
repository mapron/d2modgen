/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"
#include "PropertyTree.hpp"

#include "CoreExport.hpp"

namespace D2ModGen {
class StorageCache;
class CORE_EXPORT ConfigHandler {
public:
    ConfigHandler();
    ConfigHandler(const ConfigHandler&) = delete;
    ~ConfigHandler();

    bool loadConfig(const QString& filename);
    bool saveConfig(const QString& filename) const;

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
    };

    std::map<std::string, ModuleData> m_modules;

    std::unique_ptr<StorageCache> m_mainStorageCache;
};

}
