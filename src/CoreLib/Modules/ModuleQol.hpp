/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleQol : public PluginModule {
public:
    static constexpr const std::string_view key = Key::qualityOfLife;

    ModuleQol(Mernel::PropertyTree moduleMetadata, std::string id)
        : PluginModule(std::move(moduleMetadata), std::move(id))
    {}

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
