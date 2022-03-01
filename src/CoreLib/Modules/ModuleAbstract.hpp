/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "IModule.hpp"

#include "ModuleFactory.hpp"

namespace D2ModGen {

class ModuleAbstract : public IModule {
    // IModule interface
public:
    PresetList            presets() const override { return {}; }
    PropertyTreeScalarMap defaultValues() const override { return {}; }
    UiControlHintMap      uiHints() const override { return {}; }

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override {}
    void generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const override {}
};

}
