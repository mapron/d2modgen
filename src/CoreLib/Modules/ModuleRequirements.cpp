/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleRequirements.hpp"

namespace D2ModGen {

void ModuleRequirements::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleRequirements)
        return;
    const auto& module = config.m_moduleRequirements;

    const int reqStr = module.m_reqStr;
    const int reqDex = module.m_reqDex;

    const int equipReqLevel   = module.m_equipReqLevel;
    const int uniquesReqLevel = module.m_uniquesReqLevel;
    const int affixReqLevel   = module.m_affixReqLevel;
    const int skillsReqLevel  = module.m_skillsReqLevel;

    const int equipItemLevel   = module.m_equipItemLevel;
    const int uniquesItemLevel = module.m_uniquesItemLevel;
    const int affixItemLevel   = module.m_affixItemLevel;

    auto makeLevelModifier = [](int percent) {
        return [percent](std::optional<int>& data) {
            if (!data.has_value())
                return;

            if (data.value() <= 0 || percent == 100)
                return;
            const int newLevel = data.value() * percent / 100;
            data               = std::clamp(newLevel, 1, 110);
        };
    };

    auto equipTransform = [reqStr, reqDex, &makeLevelModifier, equipItemLevel, equipReqLevel]<typename Row>(DataTable<Row>& table) {
        table.applyTransform(&Row::reqstr, [reqStr](std::optional<int>& data) {
            if (!data.has_value())
                return;
            const int newStat = data.value() * reqStr / 100;
            data              = newStat < 10 ? 0 : newStat;
        });
        table.applyTransform(&Row::reqdex, [reqDex](std::optional<int>& data) {
            if (!data.has_value())
                return;
            const int newStat = data.value() * reqDex / 100;
            data              = newStat < 10 ? 0 : newStat;
        });

        table.applyTransform(&Row::lvl, makeLevelModifier(equipItemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(equipReqLevel));
    };
    equipTransform(output.weapons);
    equipTransform(output.armor);

    auto affixTransform = [&makeLevelModifier, affixItemLevel, affixReqLevel]<typename Row>(DataTable<Row>& table) {
        table.applyTransform(&Row::lvl, makeLevelModifier(affixItemLevel));
        table.applyTransform(&Row::maxlevel, makeLevelModifier(affixItemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(affixReqLevel));
    };
    affixTransform(output.automagic);
    affixTransform(output.magicprefix);
    affixTransform(output.magicsuffix);

    auto itemTransform = [&makeLevelModifier, uniquesItemLevel, uniquesReqLevel]<typename Row>(DataTable<Row>& table) {
        table.applyTransform(&Row::lvl, makeLevelModifier(uniquesItemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(uniquesReqLevel));
    };
    itemTransform(output.uniqueitems);
    itemTransform(output.setitems);

    output.skills.applyTransform(&DataRow_skills::lvlreq, makeLevelModifier(skillsReqLevel));
}

}
