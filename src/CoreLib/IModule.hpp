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
    virtual void generate() = 0;

    virtual ~IModule() = default;
};

struct AbstractModule : public IModule {
    DataContext&                 output;
    RandomGenerator&             rng;
    const Config&                config;
    const GenerationEnvironment& env;

    AbstractModule(DataContext& output_, RandomGenerator& rng_, const Config& config_, const GenerationEnvironment& env_)
        : output(output_)
        , rng(rng_)
        , config(config_)
        , env(env_)
    {
    }

    ~AbstractModule() override = default;
};

}
