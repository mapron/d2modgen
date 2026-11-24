/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleBalanceItems.hpp"

namespace D2ModGen {

void ModuleBalanceItems::generate()
{
    const auto& module = config.m_balanceItems;

    const Percent reqStr = module.m_reqStr;
    const Percent reqDex = module.m_reqDex;

    const auto& uniqueSettings    = module.m_uniques.m_doOverride ? module.m_uniques : module.m_defaults;
    const auto& setsSettings      = module.m_sets.m_doOverride ? module.m_sets : module.m_defaults;
    const auto& setitemsSettings  = module.m_setitems.m_doOverride ? module.m_setitems : module.m_defaults;
    const auto& affixesSettings   = module.m_affixes.m_doOverride ? module.m_affixes : module.m_defaults;
    const auto& runewordSettings  = module.m_runewords.m_doOverride ? module.m_runewords : module.m_defaults;
    const auto& gemSettings       = module.m_gems.m_doOverride ? module.m_gems : module.m_defaults;
    const auto& craftedSettings   = module.m_crafted.m_doOverride ? module.m_crafted : module.m_defaults;
    const auto& baseEquipSettings = module.m_equipment.m_doOverride ? module.m_equipment : module.m_defaults;

    const Percent equipReqLevel  = baseEquipSettings.m_reqLevel;
    const Percent equipItemLevel = baseEquipSettings.m_itemLevel;

    auto applyMinMax = [](const Config::BalanceItems::CommonSettings& settings, std::optional<int>& min, std::optional<int>& max) {
        if (settings.m_rollMode == 0 || !min.has_value() || !max.has_value())
            return;
        // keep|min|avg|max
        auto min_ = min.value();
        auto max_ = max.value();
        if (settings.m_rollMode == 1) {
            min = max = min_;
        } else if (settings.m_rollMode == 2) {
            min = max = ((min_ + max_ + 1) / 2);
        } else if (settings.m_rollMode == 3) {
            min = max = max_;
        }
    };

    auto makeLevelModifier = [](Percent percent) {
        return [percent](std::optional<int>& data) {
            if (!data.has_value())
                return;

            if (data.value() <= 0 || percent == g_identityPct)
                return;
            const int newLevel = data.value() * percent;
            data               = std::clamp(newLevel, 1, 110);
        };
    };

    auto equipTransform = [reqStr, reqDex, &makeLevelModifier, equipItemLevel, equipReqLevel]<typename Row>(DataTable<Row>& table) {
        table.applyTransform(&Row::reqstr, [reqStr](std::optional<int>& data) {
            if (!data.has_value())
                return;
            const int newStat = data.value() * reqStr;
            data              = newStat < 10 ? 0 : newStat;
        });
        table.applyTransform(&Row::reqdex, [reqDex](std::optional<int>& data) {
            if (!data.has_value())
                return;
            const int newStat = data.value() * reqDex;
            data              = newStat < 10 ? 0 : newStat;
        });

        table.applyTransform(&Row::lvl, makeLevelModifier(equipItemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(equipReqLevel));
    };
    equipTransform(output.weapons);
    equipTransform(output.armor);

    auto affixTransform = [&makeLevelModifier, &affixesSettings]<typename Row>(DataTable<Row>& table) {
        table.applyTransform(&Row::lvl, makeLevelModifier(affixesSettings.m_itemLevel));
        table.applyTransform(&Row::maxlevel, makeLevelModifier(affixesSettings.m_itemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(affixesSettings.m_reqLevel));
    };
    affixTransform(output.automagic);
    affixTransform(output.magicprefix);
    affixTransform(output.magicsuffix);

    auto itemTransform = [&makeLevelModifier]<typename Row>(DataTable<Row>& table, const Config::BalanceItems::CommonSettings& settings) {
        table.applyTransform(&Row::lvl, makeLevelModifier(settings.m_itemLevel));
        table.applyTransform(&Row::lvlreq, makeLevelModifier(settings.m_reqLevel));
    };
    itemTransform(output.uniqueitems, uniqueSettings);
    itemTransform(output.setitems, setitemsSettings);

    //const auto& module = config.m_modulePerfectRoll;

    auto makeScaler2 = [](const Config::BalanceItems::CommonSettings& settings) {
        return [&settings](int level) -> Percent {
            if (settings.m_powerScaleLevel1 == g_identityPct && settings.m_powerScaleLevel50 == g_identityPct)
                return g_identityPct;
            const int diff = settings.m_powerScaleLevel50.raw - settings.m_powerScaleLevel1.raw;

            const int     levelDiff = diff * level / 50;
            const Percent powerMult = settings.m_powerScaleLevel1 + Percent{ levelDiff };
            return powerMult;
        };
    };
    auto makeScaler1 = [](const Config::BalanceItems::CommonSettings& settings) {
        return [&settings](int) -> Percent {
            return settings.m_powerScaleLevel50;
        };
    };

    auto empowerRow = [&applyMinMax](AffixPropList& affixList, Percent powerMult, const Config::BalanceItems::CommonSettings& settings) {
        for (AffixProp_codegen& prop : affixList) {
            if (!prop.max.has_value())
                continue;

            auto id = getAttributeId(prop.code);
            if (isMinMaxRange(id))
                applyMinMax(settings, prop.min, prop.max);

            if (powerMult != g_identityPct) {
                prop.min = powerMultiply(id, prop.min.value(), powerMult);
                prop.max = powerMultiply(id, prop.max.value(), powerMult);
            }
        }
    };

    auto empowerTable = [&empowerRow]<typename Row>(DataTable<Row>& table, auto&& scaleFromLevel, const Config::BalanceItems::CommonSettings& settings) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;

            for (AffixPropList* affixList : row.affixLists()) {
                empowerRow(*affixList, scaleFromLevel(row.getLevel()), settings);
            }
        }
    };

    empowerTable(output.uniqueitems, makeScaler2(uniqueSettings), uniqueSettings);
    empowerTable(output.runes, makeScaler1(runewordSettings), runewordSettings);
    empowerTable(output.setitems, makeScaler2(setitemsSettings), setitemsSettings);

    empowerTable(output.magicprefix, makeScaler2(affixesSettings), affixesSettings);
    empowerTable(output.magicsuffix, makeScaler2(affixesSettings), affixesSettings);
    empowerTable(output.automagic, makeScaler2(affixesSettings), affixesSettings);

    empowerTable(output.gems, makeScaler1(gemSettings), gemSettings);
    empowerTable(output.cubemain, makeScaler1(craftedSettings), craftedSettings);

    for (auto& row : output.armor.rows) {
        applyMinMax(module.m_equipment, row.minac, row.maxac);
    }
}

}
