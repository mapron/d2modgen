/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ModuleAbstract.hpp"

#include <set>

namespace D2ModGen {

class ModuleChallenge : public ModuleAbstract {
    struct Item {
        StringSet internalIds;
        std::string           settingKey;
    };
    const std::vector<Item> m_items;

public:
    ModuleChallenge();

    static constexpr const std::string_view key = Key::challenge;

    // IModule interface
public:
    std::string settingKey() const override
    {
        return std::string(key);
    }
    PropertyTreeScalarMap defaultValues() const override;
    UiControlHintMap      uiHints() const override;

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;
};

}
