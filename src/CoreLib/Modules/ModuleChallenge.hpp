/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "PluginModule.hpp"

#include <set>

namespace D2ModGen {

class ModuleChallenge : public PluginModule {
    struct Item {
        StringSet   internalIds;
        std::string settingKey;
    };
    const std::vector<Item> m_items;

public:
    static constexpr const std::string_view key = Key::challenge;

    ModuleChallenge(Mernel::PropertyTree moduleMetadata, std::string id);

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
