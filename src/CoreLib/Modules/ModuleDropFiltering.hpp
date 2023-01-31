/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

#include <set>

namespace D2ModGen {

class ModuleDropFiltering : public PluginModule {
    struct Item {
        StringSet   internalIds;
        std::string settingKey;
    };
    const std::vector<Item> m_items;

public:
    static constexpr const std::string_view key = Key::dropFiltering;

    ModuleDropFiltering(Mernel::PropertyTree moduleMetadata, std::string id);

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override;
    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
