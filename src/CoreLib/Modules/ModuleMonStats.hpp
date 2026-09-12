/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleMonStats : public PluginModule {
public:
    static constexpr const std::string_view key = Key::monsterStats;

    ModuleMonStats();

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
