/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"
#include "MernelPlatform/PropertyTree.hpp"
#include "MernelPlatform/FsUtils.hpp"

#include "ModgenCoreExport.hpp"

namespace D2ModGen {
class StorageCache;
class MODGENCORE_EXPORT ConfigHandler {
public:
    explicit ConfigHandler();
    ConfigHandler(const ConfigHandler&) = delete;
    ~ConfigHandler();

    bool loadAppConfig();
    bool saveAppConfig() const;

    bool loadConfig(const Mernel::std_path& filename, bool resetMain = true);
    bool saveConfig(const Mernel::std_path& filename) const;

    bool loadConfig(const Mernel::PropertyTree& data, bool resetMain = true);
    bool saveConfig(Mernel::PropertyTree& data) const;

    void updateSeed(bool onGenerate = false);

    struct GenerateResult {
        std::string m_error;
        bool        m_success = false;
    };

    GenerateResult generate();

    GenerationEnvironment getEnv(std::string& err) const;

    Mernel::PropertyTree m_appConfig;

    struct ModuleData {
        IModule::Ptr         m_module;
        std::string          m_uiKey;
        std::string          m_key;
        Mernel::PropertyTree m_currentConfig;
        bool                 m_enabled = true;
    };
    std::vector<ModuleData> m_modules;

    std::unique_ptr<StorageCache> m_mainStorageCache;

    const Mernel::std_path m_appData;
    const Mernel::std_path m_defaultPath;
};

}
