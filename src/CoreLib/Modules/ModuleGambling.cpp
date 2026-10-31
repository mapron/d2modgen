/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleGambling.hpp"

namespace D2ModGen {

void ModuleGambling::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleGambling)
        return;
    const auto& module = config.m_moduleGambling;

    const bool charmGamble = module.m_charmGamble && env.isLegacy; // disabled for D2R now; it crashes the game.
    if (charmGamble) {
        auto& table = output.gamble;
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Small"), .code = ("cm1") });
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Medium"), .code = ("cm2") });
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Large"), .code = ("cm3") });
        table.rows.push_back(DataRow_gamble{ .name = ("Jewel"), .code = ("jew") });
    }

    auto& table = output.difficultylevels;
    table.applyIntTransform(&DataRow_difficultylevels::gambleRare, module.m_ratioRare, 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleSet, module.m_ratioSet, 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUnique, module.m_ratioUnique, 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUber, module.m_ratioExc, 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUltra, module.m_ratioElite, 1);
}

}
