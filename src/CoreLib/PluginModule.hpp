/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "FileIOUtils.hpp"

#include "ModuleFactory.hpp"

namespace D2ModGen {

class DyLib;
class PluginModule : public IModule {
public:
    PluginModule(Mernel::PropertyTree moduleMetadata, std::string id, std::string idPrefix = "");
    ~PluginModule();

    const Mernel::PropertyTree& pluginInfo() const override { return m_info; }
    const Mernel::PropertyTree& defaultValues() const override { return m_defaults; }

    std::string settingKey() const override { return m_idPrefix + m_id; }

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override {}
    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;

private:
    const Mernel::PropertyTree m_info;
    const Mernel::PropertyTree m_defaults;
    const std::string          m_id;
    const std::string          m_idPrefix;

    std::unique_ptr<DyLib> m_dylib;
    void*                  m_generateAddr = nullptr;
};

}
