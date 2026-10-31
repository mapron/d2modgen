/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"
#include "DataContext.hpp"
#include "Config.hpp"

namespace D2ModGen {

struct IModule {
    virtual void generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env) = 0;

    virtual ~IModule() = default;
};

}
