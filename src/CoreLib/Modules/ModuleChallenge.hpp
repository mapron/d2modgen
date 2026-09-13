/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleChallenge : public PluginModule {
    struct Item {
        StringSet   internalIds;
        std::string settingKey;
    };
    const std::vector<Item> m_items;

public:
    static constexpr const std::string_view configKey{ "challenge" };
    static constexpr const std::string_view moduleKey{ "appModuleChallenge" };

    ModuleChallenge();

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
