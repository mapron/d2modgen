/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "MernelPlatform/PropertyTree.hpp"
#include "MernelPlatform/FsUtils.hpp"

#include "GenerationEnvironment.hpp"

#include "ModgenCoreExport.hpp"
#include "Config.hpp"

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

    bool generate(const GenerationEnvironment& env);

    Mernel::PropertyTree m_appConfig;

    Config m_config;

    const std::unique_ptr<StorageCache> m_mainStorageCache;

    const Mernel::std_path m_appData;
    const Mernel::std_path m_defaultPath;
};

}
