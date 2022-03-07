/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "FileIOUtils.hpp"

namespace D2ModGen {

class DyLib;
class PluginModule : public IModule {
public:
    PluginModule(const std_path& jsonDeclFilepath);
    ~PluginModule();

    PropertyTree pluginInfo() const override;

    std::string           settingKey() const override { return "plugin_" + m_id; }
    PresetList            presets() const override { return {}; }
    PropertyTreeScalarMap defaultValues() const override { return {}; }
    UiControlHintMap      uiHints() const override { return {}; }

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override {}
    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;

private:
    std::unique_ptr<DyLib> m_dylib;
    std::string            m_id;
    PropertyTree           m_info;
    void*                  m_generateAddr = nullptr;
};

}
