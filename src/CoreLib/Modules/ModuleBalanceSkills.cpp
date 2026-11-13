/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceSkills.hpp"

namespace D2ModGen {

void ModuleBalanceSkills::generate()
{
    const auto& module          = config.m_balanceSkills;
    auto        getLevelFromRow = [&module](int row) -> int {
        switch (row) {
                // clang-format off
            case 1: return module.m_lvlreqRow1;
            case 2: return module.m_lvlreqRow2;
            case 3: return module.m_lvlreqRow3;
            case 4: return module.m_lvlreqRow4;
            case 5: return module.m_lvlreqRow5;
            case 6: return module.m_lvlreqRow6;
                // clang-format on
        }
        return 0;
    };

    /// @todo: will break will randomized skill
    static const StringSet s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };

    const int globalHitShift = config.m_balanceSkills.m_globalHitShift - 2;

    for (auto& row : output.skills.rows) {
        if (row.classSpecific == Character::None)
            continue;

        if (config.m_skillRandomizer.m_enabled == false) {
            row.hitShift = std::clamp(row.hitShift.value_or(0) + globalHitShift, 0, 15);
        }
        if (auto* desc = output.skilldesc.find(row.skilldesc))
            row.lvlreq = getLevelFromRow(desc->skillRow.value_or(0));
        if (module.m_weakenTownSkillsExt || module.m_weakenTownSkills && s_exceptions.contains(row.skill))
            row.inTown = true;
        if (module.m_leftCast)
            row.leftskill = true;
    }

    for (auto& row : output.skills.rows) {
        if (row.classSpecific == Character::None)
            continue;
    }

    const int skillPointLimit = module.m_skillPointLimit;
    if (skillPointLimit != 20) {
        output.skills.applyTransform(
            &DataRow_skills::maxlvl, [skillPointLimit](std::optional<int>& data) {
                if (!data.has_value() || data.value() != 20)
                    return;
                data = skillPointLimit;
            });
    }
}

}
