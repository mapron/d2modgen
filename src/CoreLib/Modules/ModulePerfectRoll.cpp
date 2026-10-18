/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModulePerfectRoll.hpp"

#include "AttributeHelper.hpp"

#include "Logger.hpp"

namespace D2ModGen {

ModulePerfectRoll::ModulePerfectRoll()
    : PluginModule(
          Mernel::PropertyTreeMap{
              { "affix_power_l1", Mernel::PropertyTreeScalar{ 100 } },
              { "affix_power_l50", Mernel::PropertyTreeScalar{ 100 } },
              { "affixes", Mernel::PropertyTreeScalar{ true } },
              { "armor", Mernel::PropertyTreeScalar{ true } },
              { "craft", Mernel::PropertyTreeScalar{ true } },
              { "crafted", Mernel::PropertyTreeScalar{ true } },
              { "crafted_power", Mernel::PropertyTreeScalar{ 100 } },
              { "gem_power", Mernel::PropertyTreeScalar{ 100 } },
              { "runeWords", Mernel::PropertyTreeScalar{ true } },
              { "rw_power", Mernel::PropertyTreeScalar{ 100 } },
              { "setItems", Mernel::PropertyTreeScalar{ true } },
              { "set_power_l1", Mernel::PropertyTreeScalar{ 100 } },
              { "set_power_l50", Mernel::PropertyTreeScalar{ 100 } },
              { "uniques", Mernel::PropertyTreeScalar{ true } },
              { "uniques_power_l1", Mernel::PropertyTreeScalar{ 100 } },
              { "uniques_power_l50", Mernel::PropertyTreeScalar{ 100 } },
          })
{}

void ModulePerfectRoll::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    auto makePerfectMin = [](AffixPropList& affixes, bool perfectMin) {
        if (!perfectMin)
            return;

        for (AffixProp& affixProp : affixes) {
            if (!affixProp.hasRange())
                continue;
            if (isMinMaxRange(affixProp.code))
                affixProp.min = affixProp.max;
        }
    };

    if (input.getInt("craft")) {
        for (auto& row : output.cubemain.rows) {
            const bool isCrafted = row.output == "\"usetype,crf\"";
            if (!isCrafted)
                continue;

            makePerfectMin(row.affixes_a, true);
            makePerfectMin(row.affixes_b, true);
            makePerfectMin(row.affixes_c, true);
        }
    }

    auto makeEmpower = [](AffixPropList& affixes, int level1, int level50, int level) {
        if (level1 == 100 && level50 == 100)
            return;
        if (!level)
            return;

        const int diff = level50 - level1;

        const int levelDiff = diff * level / 50;
        const int powerMult = level1 + levelDiff;
        if (powerMult == 100)
            return;

        for (AffixProp& affixProp : affixes) {
            if (!affixProp.hasRange())
                continue;
            affixProp.min = powerMultiply(affixProp.code, affixProp.min.value(), powerMult);
            affixProp.max = powerMultiply(affixProp.code, affixProp.max.value(), powerMult);
        }
    };

    {
        for (auto& row : output.uniqueitems.rows) {
            makePerfectMin(row.affixes, input.getInt("uniques"));
            makeEmpower(row.affixes, input.getInt("uniques_power_l1"), input.getInt("uniques_power_l50"), row.lvl.value_or(0));
        }
    }
    {
        const int powerLevel = input.getInt("rw_power");
        for (auto& row : output.runes.rows) {
            makePerfectMin(row.affixes, input.getInt("runeWords"));
            makeEmpower(row.affixes, powerLevel, powerLevel, row.lvl);
        }
    }
    {
        for (auto& row : output.setitems.rows) {
            makePerfectMin(row.affixes_base, input.getInt("setItems"));
            makePerfectMin(row.affixes_partial1, input.getInt("setItems"));
            makePerfectMin(row.affixes_partial2, input.getInt("setItems"));
            makeEmpower(row.affixes_base, input.getInt("set_power_l1"), input.getInt("set_power_l50"), row.lvl.value_or(0));
            makeEmpower(row.affixes_partial1, input.getInt("set_power_l1"), input.getInt("set_power_l50"), row.lvl.value_or(0));
            makeEmpower(row.affixes_partial2, input.getInt("set_power_l1"), input.getInt("set_power_l50"), row.lvl.value_or(0));
        }
    }
    {
        for (auto& row : output.magicprefix.rows) {
            makePerfectMin(row.affixes, input.getInt("affixes"));
            makeEmpower(row.affixes, input.getInt("affix_power_l1"), input.getInt("affix_power_l50"), row.getLevel());
        }
        for (auto& row : output.magicsuffix.rows) {
            makePerfectMin(row.affixes, input.getInt("affixes"));
            makeEmpower(row.affixes, input.getInt("affix_power_l1"), input.getInt("affix_power_l50"), row.getLevel());
        }
        for (auto& row : output.automagic.rows) {
            makePerfectMin(row.affixes, input.getInt("affixes"));
            makeEmpower(row.affixes, input.getInt("affix_power_l1"), input.getInt("affix_power_l50"), row.getLevel());
        }
    }
    {
        const int powerLevel = input.getInt("crafted_power");
        for (auto& row : output.cubemain.rows) {
            makePerfectMin(row.affixes_a, input.getInt("crafted"));
            makePerfectMin(row.affixes_b, input.getInt("crafted"));
            makePerfectMin(row.affixes_c, input.getInt("crafted"));
            int lvl = 1;
            makeEmpower(row.affixes_a, powerLevel, powerLevel, lvl);
            makeEmpower(row.affixes_b, powerLevel, powerLevel, lvl);
            makeEmpower(row.affixes_c, powerLevel, powerLevel, lvl);
        }
    }
    {
        const int powerLevel = input.getInt("gem_power");
        for (auto& row : output.gems.rows) {
            int lvl = 1;
            makeEmpower(row.affixes_weapon, powerLevel, powerLevel, lvl);
            makeEmpower(row.affixes_helm, powerLevel, powerLevel, lvl);
            makeEmpower(row.affixes_shield, powerLevel, powerLevel, lvl);
        }
    }
    if (input.getInt("armor")) {
        for (auto& row : output.armor.rows) {
            if (row.maxac.has_value())
                row.minac = row.maxac;
        }
    }
}

}
