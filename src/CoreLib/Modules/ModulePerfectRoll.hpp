/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "PluginModule.hpp"

namespace D2ModGen {

class ModulePerfectRoll : public PluginModule {
public:
    static constexpr const std::string_view configKey{ "perfect_rolls" };
    static constexpr const std::string_view moduleKey{ "appModulePerfectRoll" };

    ModulePerfectRoll();

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
