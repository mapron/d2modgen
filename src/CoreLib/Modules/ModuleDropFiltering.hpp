/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModuleAbstract.hpp"

namespace D2ModGen {

class ModuleDropFiltering : public ModuleAbstract {
public:
    static constexpr const std::string_view key = Key::dropFiltering;

    struct Item {
        std::set<std::string> internalIds;
        std::string           settingKey;
    };
    const std::vector<Item> m_items;

    ModuleDropFiltering();

    // IModule interface
public:
    std::string settingKey() const override
    {
        return std::string(key);
    }
    PropertyTreeScalarMap defaultValues() const override;

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override;
    void generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const override;
};

}
