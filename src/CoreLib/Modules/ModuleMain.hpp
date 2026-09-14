/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleMain : public PluginModule {
public:
    static constexpr const std::string_view configKey{ "main" };
    static constexpr const std::string_view moduleKey{ "appModuleMain" };
    ModuleMain();

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
