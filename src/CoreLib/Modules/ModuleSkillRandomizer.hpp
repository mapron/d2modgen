/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once
#include "IModule.hpp"

namespace D2ModGen {

struct ModuleSkillRandomizer : public IModule {
    void generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env) override;
};

}
