/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleRuneDrops.hpp"
#include "AttributeHelper.hpp"

#include <cmath>

namespace D2ModGen {

void ModuleRuneDrops::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    if (!config.m_enabledModules.m_moduleRequirements)
        return;

    const auto& module = config.m_moduleRuneDrops;

    const int  factorRune      = module.m_rune_factor;
    const int  factorZod       = module.m_zod_factor;
    const bool countessMore    = module.m_countess_rune_more;
    const int  countessUpg     = module.m_countess_rune_higher ? 5 : 0;
    const bool wraithMore      = module.m_wraith_runes;
    const bool switchHighRunes = module.m_highrune_switch;

    std::map<std::string, std::string> highRuneReplacement{
        { "r33", "r31" },
        { "r32", "r30" },
        { "r31", "r33" },
        { "r30", "r32" },
    };

    std::map<std::string, double> runesReplaceMult;
    if (factorZod > 1) {
        const double factor   = factorZod;
        const double iterMult = std::pow(factor, 0.1);
        double       mult     = 1. / factor;

        for (int i = 16; i >= 6; --i) {
            runesReplaceMult["Runes " + std::to_string(i)] = mult;
            mult *= iterMult;
        }
    }

    for (auto& row : output.treasureclassex.rows) {
        auto& className = row.name;

        if (factorRune > 1 && className.ends_with(" Good")) {
            for (auto& item : row.items) {
                if (!item.tc.starts_with("Runes "))
                    continue;
                item.prob = item.prob.value() * factorRune;
                break;
            }
        }
        if (factorZod > 1 && className.starts_with("Runes ")) {
            for (auto& item : row.items) {
                auto& tcName = item.tc;
                if (!tcName.starts_with("Runes ") || !runesReplaceMult.contains(tcName))
                    continue;
                const int newProb = static_cast<int>(item.prob.value() * runesReplaceMult[tcName]);
                item.prob         = std::max(newProb, 5);
                break;
            }
        }
        const bool countessRuneTC = className.starts_with("Countess Rune");
        const bool countessItemTC = className.starts_with("Countess Item");
        if (countessUpg > 0 && countessRuneTC) {
            for (auto& item : row.items) {
                auto& tcName = item.tc;
                if (!tcName.starts_with("Runes "))
                    continue;
                const int oldTC = std::atoi(tcName.substr(6).c_str());
                const int newTC = oldTC + countessUpg;
                tcName          = "Runes " + std::to_string(newTC);
                break;
            }
        }
        if (countessMore) {
            if (countessRuneTC) {
                row.picks  = 5;
                row.noDrop = 1;
            }
            if (countessItemTC)
                row.picks = 1;
        }
        if (switchHighRunes && className.starts_with("Runes ")) {
            for (auto& item : row.items) {
                auto& tcName = item.tc;
                auto  it     = highRuneReplacement.find(tcName);
                if (it != highRuneReplacement.cend())
                    tcName = it->second;
            }
        }
        if (wraithMore && className.find(") Wraith ") != std::string::npos) {
            static const std::map<std::string, std::string> s_wraithReplacement{
                { "Act 1 (N)", "Runes 8" },
                { "Act 2 (N)", "Runes 9" },
                { "Act 3 (N)", "Runes 10" },
                { "Act 4 (N)", "Runes 11" },
                { "Act 5 (N)", "Runes 12" },
                { "Act 1 (H)", "Runes 13" },
                { "Act 2 (H)", "Runes 14" },
                { "Act 3 (H)", "Runes 15" },
                { "Act 4 (H)", "Runes 16" },
                { "Act 5 (H)", "Runes 17" },
            };
            for (auto& item : row.items) {
                auto& tcName = item.tc;
                if (tcName.find(") Magic ") != std::string::npos)
                    continue;
                auto it = s_wraithReplacement.find(tcName.substr(0, 9));
                if (it != s_wraithReplacement.cend())
                    tcName = it->second;
                break;
            }
        }
    }
}

}
