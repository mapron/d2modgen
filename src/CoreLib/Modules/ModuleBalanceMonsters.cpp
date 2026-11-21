/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceMonsters.hpp"

#include "MonsterUtils.hpp"
#include "Logger.hpp"

#include <algorithm>

namespace D2ModGen {

namespace {

enum ChangeMode
{
    None,
    HellOnly,
    NmAndHell,
    All,
};

}

void ModuleBalanceMonsters::generate()
{
    const auto& module = config.m_balanceMonsters;

    m_verbose = env.extraTrace.contains("monster_balance");

    auto proceedMonParam = [](Percent percent, int& val) {
        val = val * percent;
    };
    for (auto& row : output.monlvl.rows) {
        if (row.level <= 0)
            continue;
        for (size_t i = 0; i < 6; ++i) {
            proceedMonParam(module.m_monsterAttack, row.TH[i]);
            proceedMonParam(module.m_monsterDefense, row.AC[i]);
            proceedMonParam(module.m_monsterHealth, row.HP[i]);
            proceedMonParam(module.m_monsterDamage, row.DM[i]);
            proceedMonParam(module.m_monsterExperience, row.XP[i]);
        }
    }

    auto applySingeResClamp = [](int* res, Percent min, Percent max) {
        *res = std::clamp(*res, (int) min.raw, (int) max.raw);
    };

    auto applyResClamp = [&module, &applySingeResClamp](MonResist& monResist) {
        applySingeResClamp(monResist.m_sorted[0], module.m_resMin1, module.m_resMax1);
        applySingeResClamp(monResist.m_sorted[1], module.m_resMin2, module.m_resMax2);
        applySingeResClamp(monResist.m_sorted[2], module.m_resMin3, module.m_resMax3);
    };

    for (auto& row : output.monstats.rows) {
        if (!row.isRegular)
            continue;
        const size_t nm   = 1;
        const size_t hell = 2;

        std::array<MonResist, 3> r;

        for (size_t i = 0; i < 3; ++i) {
            r[i].readFromRow(row, i); // stable_sort inside for m_sorted
        }
        Logger(m_verbose) << "Resistances '" << row.id
                          << "', before: \n    NOR =" << r[0].toString()
                          << "\n    NM  =" << r[1].toString()
                          << "\n    HELL=" << r[2].toString();

        if (module.m_resChangeMode != ChangeMode::None) {
            for (size_t i = 0; i < 3; ++i) {
                const bool applyOnHell   = module.m_resChangeMode == ChangeMode::HellOnly && i == hell;
                const bool applyOnNmPlus = module.m_resChangeMode == ChangeMode::NmAndHell && i >= nm;
                const bool applyAll      = module.m_resChangeMode == ChangeMode::All;
                if (applyOnHell || applyOnNmPlus || applyAll)
                    applyResClamp(r[i]);
            }
        }

        // separate feature "apply Hell res to all 3 difficulties".
        if (module.m_hellResistances) {
            for (size_t i = 0; i < 3; ++i)
                r[hell].writeToRow(row, i);
        } else {
            for (size_t i = 0; i < 3; ++i)
                r[i].writeToRow(row, i);
        }

        Logger(m_verbose) << "Resistances '" << row.id
                          << "',  after:\n    NOR =" << r[0].toString()
                          << "\n    NM  =" << r[1].toString()
                          << "\n    HELL=" << r[2].toString();
    }
}

}
