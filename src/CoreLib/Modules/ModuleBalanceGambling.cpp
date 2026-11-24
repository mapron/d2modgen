/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceGambling.hpp"

namespace D2ModGen {

void ModuleBalanceGambling::generate()
{
    const auto& module = config.m_balanceGambling;

    const bool charmGamble = module.m_charmGamble && env.isLegacy;
    if (charmGamble) {
        auto& table = output.gamble;
        table.extra.push_back(DataRow_gamble_codegen{ .name = ("Charm Small"), .code = ("cm1") });
        table.extra.push_back(DataRow_gamble_codegen{ .name = ("Charm Medium"), .code = ("cm2") });
        table.extra.push_back(DataRow_gamble_codegen{ .name = ("Charm Large"), .code = ("cm3") });
        table.extra.push_back(DataRow_gamble_codegen{ .name = ("Jewel"), .code = ("jew") });
        table.commitExtraRows();
    }

    auto& table = output.difficultylevels;
    table.applyIntTransform(&DataRow_difficultylevels::gambleRare, Percent{ module.m_ratioRare * 100 });
    table.applyIntTransform(&DataRow_difficultylevels::gambleSet, Percent{ module.m_ratioSet * 100 });
    table.applyIntTransform(&DataRow_difficultylevels::gambleUnique, Percent{ module.m_ratioUnique * 100 });
    table.applyIntTransform(&DataRow_difficultylevels::gambleUber, Percent{ module.m_ratioExc * 100 });
    table.applyIntTransform(&DataRow_difficultylevels::gambleUltra, Percent{ module.m_ratioElite * 100 });
}

}
