/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleItemRandomizer : public PluginModule {
public:
    static constexpr const std::string_view key = Key::itemRandomizer;

    ModuleItemRandomizer(Mernel::PropertyTree moduleMetadata, std::string id)
        : PluginModule(std::move(moduleMetadata), std::move(id))
    {}

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override;
    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
