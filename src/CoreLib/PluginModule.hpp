/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

namespace D2ModGen {

class PluginModule : public IModule {
public:
    PluginModule(Mernel::PropertyTreeMap defaults, std::string id);
    const Mernel::PropertyTreeMap&  defaultValues() const override { return m_defaults; }
    const Mernel::PropertyTreeList& presets() const override { return m_presets; }

    void               gatherInfo(PreGenerationContext& output, const InputContext& input) const override {}
    const std::string& settingKey() const override { return m_id; }

protected:
    void setPresets(Mernel::PropertyTreeList presets)
    {
        m_presets = std::move(presets);
    }

private:
    const Mernel::PropertyTreeMap m_defaults;
    const std::string             m_id;
    Mernel::PropertyTreeList      m_presets;
};

}
