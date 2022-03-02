/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModuleAbstract.hpp"

namespace D2ModGen {

class ModuleMonDensity : public ModuleAbstract {
public:
    static constexpr const std::string_view key = Key::monsterDensity;

    // IModule interface
public:
    std::string settingKey() const override
    {
        return std::string(key);
    }
    PropertyTreeScalarMap defaultValues() const override;
    UiControlHintMap      uiHints() const override;

    void generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const override;
};

}