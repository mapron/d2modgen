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
    auto makeScaler2 = [&input](const std::string& power1, const std::string& power50) {
        int scale1  = input.getInt(power1);
        int scale50 = input.getInt(power50);

        return [scale1, scale50](int level) {
            if (scale1 == 100 && scale50 == 100)
                return 100;
            const int diff = scale50 - scale1;

            const int levelDiff = diff * level / 50;
            const int powerMult = scale1 + levelDiff;
            return powerMult;
        };
    };
    auto makeScaler1 = [&input](const std::string& power) {
        int scale = input.getInt(power);
        return [scale](int level) {
            return scale;
        };
    };

    auto empowerRow = [](AffixPropList& affixList, int powerMult, bool perfectMin, bool scalePower = true) {
        for (AffixProp& prop : affixList) {
            if (!prop.hasRange())
                continue;

            if (perfectMin && isMinMaxRange(prop.code))
                prop.min = prop.max;

            if (!scalePower)
                continue;

            prop.min = powerMultiply(prop.code, prop.min.value(), powerMult);
            prop.max = powerMultiply(prop.code, prop.max.value(), powerMult);
        }
    };

    auto empowerTable = [&empowerRow]<typename Row>(DataTable<Row>& table, auto&& scaleFromLevel, bool perfectMin, bool scalePower = true) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;

            for (AffixPropList* affixList : row.affixLists()) {
                empowerRow(*affixList, scaleFromLevel(row.getLevel()), perfectMin, scalePower);
            }
        }
    };

    empowerTable(output.uniqueitems, makeScaler2("uniques_power_l1", "uniques_power_l50"), input.getInt("uniques"));
    empowerTable(output.runes, makeScaler1("rw_power"), input.getInt("runeWords"));
    empowerTable(output.setitems, makeScaler2("set_power_l1", "set_power_l50"), input.getInt("setItems"));

    empowerTable(output.magicprefix, makeScaler2("affix_power_l1", "affix_power_l50"), input.getInt("affixes"));
    empowerTable(output.magicsuffix, makeScaler2("affix_power_l1", "affix_power_l50"), input.getInt("affixes"));
    empowerTable(output.automagic, makeScaler2("affix_power_l1", "affix_power_l50"), input.getInt("affixes"));

    empowerTable(output.runes, makeScaler1("crafted_power"), input.getInt("crafted"));
    empowerTable(output.gems, makeScaler1("gem_power"), false);

    if (input.getInt("craft")) {
        for (auto& row : output.cubemain.rows) {
            const bool isCrafted = row.isCrafted();
            if (!isCrafted)
                continue;

            for (AffixPropList* affixList : row.affixLists())
                empowerRow(*affixList, 100, true, false);
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
