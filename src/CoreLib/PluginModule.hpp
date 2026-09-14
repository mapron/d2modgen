/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

namespace D2ModGen {

class PluginModule : public IModule {
public:
    PluginModule(Mernel::PropertyTreeMap defaults);
    const Mernel::PropertyTreeMap&  defaultValues() const override { return m_defaults; }
    const Mernel::PropertyTreeList& presets() const override { return m_presets; }

    void gatherInfo(PreGenerationContext& output) const override {}

protected:
    void setPresets(Mernel::PropertyTreeList presets)
    {
        m_presets = std::move(presets);
    }

private:
    const Mernel::PropertyTreeMap m_defaults;
    Mernel::PropertyTreeList      m_presets;
};

}
