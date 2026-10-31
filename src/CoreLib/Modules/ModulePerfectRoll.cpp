/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModulePerfectRoll.hpp"

#include "AttributeHelper.hpp"

namespace D2ModGen {

void ModulePerfectRoll::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_modulePerfectRoll)
        return;
    const auto& module = config.m_modulePerfectRoll;

    auto makeScaler2 = [](int scale1, int scale50) {
        return [scale1, scale50](int level) {
            if (scale1 == 100 && scale50 == 100)
                return 100;
            const int diff = scale50 - scale1;

            const int levelDiff = diff * level / 50;
            const int powerMult = scale1 + levelDiff;
            return powerMult;
        };
    };
    auto makeScaler1 = [](int scale) {
        return [scale](int level) {
            return scale;
        };
    };

    auto empowerRow = [](AffixPropList& affixList, int powerMult, bool perfectMin, bool scalePower = true) {
        for (AffixProp& prop : affixList) {
            if (!prop.hasRange())
                continue;

            auto id = getAttributeId(prop.code);
            if (perfectMin && isMinMaxRange(id))
                prop.min = prop.max;

            if (!scalePower)
                continue;

            prop.min = powerMultiply(id, prop.min.value(), powerMult);
            prop.max = powerMultiply(id, prop.max.value(), powerMult);
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

    empowerTable(output.uniqueitems, makeScaler2(module.m_uniques_power_l1, module.m_uniques_power_l50), module.m_uniques);
    empowerTable(output.runes, makeScaler1(module.m_rw_power), module.m_runeWords);
    empowerTable(output.setitems, makeScaler2(module.m_set_power_l1, module.m_set_power_l50), module.m_setItems);

    empowerTable(output.magicprefix, makeScaler2(module.m_affix_power_l1, module.m_affix_power_l50), module.m_affixes);
    empowerTable(output.magicsuffix, makeScaler2(module.m_affix_power_l1, module.m_affix_power_l50), module.m_affixes);
    empowerTable(output.automagic, makeScaler2(module.m_affix_power_l1, module.m_affix_power_l50), module.m_affixes);

    empowerTable(output.runes, makeScaler1(module.m_crafted_power), module.m_crafted);
    empowerTable(output.gems, makeScaler1(module.m_gem_power), false);

    if (module.m_craft) {
        for (auto& row : output.cubemain.rows) {
            const bool isCrafted = row.isCrafted();
            if (!isCrafted)
                continue;

            for (AffixPropList* affixList : row.affixLists())
                empowerRow(*affixList, 100, true, false);
        }
    }

    if (module.m_armor) {
        for (auto& row : output.armor.rows) {
            if (row.maxac.has_value())
                row.minac = row.maxac;
        }
    }
}

}
