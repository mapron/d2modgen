/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
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

    struct GenerateResult {
        std::string m_error;
        bool        m_success = false;
    };

    GenerateResult generate();

    GenerationEnvironment getEnv() const;

    Mernel::PropertyTree m_appConfig;

    Mernel::PropertyTree m_currentMainConfig;

    struct ModuleData {
        IModule::Ptr         m_module;
        std::string          m_key;
        std::u16string       m_key16;
        Mernel::PropertyTree m_currentConfig;
        bool                 m_enabled = true;
    };

    using ModuleMap16 = std::map<std::u16string_view, ModuleData*>;
    using ModuleMap8  = std::map<std::string_view, ModuleData*>;

    std::vector<ModuleData> m_modules;

    ModuleMap16 m_moduleIndex16;
    ModuleMap8  m_moduleIndex8;

    std::unique_ptr<StorageCache> m_mainStorageCache;

    const Mernel::std_path m_appData;
    const Mernel::std_path m_defaultPath;
};

}
