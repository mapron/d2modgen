/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleMonRandomizer : public PluginModule {
public:
    static constexpr const std::string_view configKey{ "monRandomizer" };
    static constexpr const std::string_view moduleKey{ "appModuleMonRandomizer" };

    ModuleMonRandomizer();

    void gatherInfo(PreGenerationContext& output) const override;
    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;

private:
    struct TCTable;
    struct MonTypeTable;

private:
    void generateSpawns(DataContext& output, RandomGenerator& rng, const InputContext& input, MonTypeTable& typeTable, TCTable& tcTable) const;
};

}
