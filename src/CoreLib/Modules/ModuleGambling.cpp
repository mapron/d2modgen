/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleGambling.hpp"

namespace D2ModGen {

ModuleGambling::ModuleGambling()
    : PluginModule(Mernel::PropertyTreeMap{
          { "charmGamble", Mernel::PropertyTreeScalar{ false } },
          { "ratioElite", Mernel::PropertyTreeScalar{ 1 } },
          { "ratioExc", Mernel::PropertyTreeScalar{ 1 } },
          { "ratioRare", Mernel::PropertyTreeScalar{ 1 } },
          { "ratioSet", Mernel::PropertyTreeScalar{ 1 } },
          { "ratioUnique", Mernel::PropertyTreeScalar{ 1 } },
      })
{}

void ModuleGambling::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool charmGamble = input.getInt("charmGamble") && input.m_env.isLegacy; // disabled for D2R now; it crashes the game.
    if (charmGamble) {
        auto& table = output.gamble;
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Small"), .code = ("cm1") });
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Medium"), .code = ("cm2") });
        table.rows.push_back(DataRow_gamble{ .name = ("Charm Large"), .code = ("cm3") });
        table.rows.push_back(DataRow_gamble{ .name = ("Jewel"), .code = ("jew") });
    }

    auto& table = output.difficultylevels;
    table.applyIntTransform(&DataRow_difficultylevels::gambleRare, input.getInt("ratioRare"), 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleSet, input.getInt("ratioSet"), 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUnique, input.getInt("ratioUnique"), 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUber, input.getInt("ratioExc"), 1);
    table.applyIntTransform(&DataRow_difficultylevels::gambleUltra, input.getInt("ratioElite"), 1);
}

}
