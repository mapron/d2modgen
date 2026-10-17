/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleRequirements.hpp"

namespace D2ModGen {

ModuleRequirements::ModuleRequirements()
    : PluginModule(Mernel::PropertyTreeMap{
          { "affixItemLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "affixReqLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "equipItemLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "equipReqLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "reqDex", Mernel::PropertyTreeScalar{ 100 } },
          { "reqStr", Mernel::PropertyTreeScalar{ 100 } },
          { "skillsReqLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "uniquesItemLevel", Mernel::PropertyTreeScalar{ 100 } },
          { "uniquesReqLevel", Mernel::PropertyTreeScalar{ 100 } },
      })
{}

void ModuleRequirements::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const int reqStr = input.getInt("reqStr");
    const int reqDex = input.getInt("reqDex");

    const int equipReqLevel   = input.getInt("equipReqLevel");
    const int uniquesReqLevel = input.getInt("uniquesReqLevel");
    const int affixReqLevel   = input.getInt("affixReqLevel");
    const int skillsReqLevel  = input.getInt("skillsReqLevel");

    const int equipItemLevel   = input.getInt("equipItemLevel");
    const int uniquesItemLevel = input.getInt("uniquesItemLevel");
    const int affixItemLevel   = input.getInt("affixItemLevel");

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
