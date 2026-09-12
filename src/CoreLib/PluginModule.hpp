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
    const Mernel::PropertyTreeMap& defaultValues() const override { return m_defaults; }

    void               gatherInfo(PreGenerationContext& output, const InputContext& input) const override {}
    const std::string& settingKey() const override { return m_id; }

private:
    const Mernel::PropertyTreeMap m_defaults;
    const std::string             m_id;
};

}
