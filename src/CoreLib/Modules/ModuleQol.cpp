/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleQol.hpp"

#include "Colors.hpp"

namespace D2ModGen {

void ModuleQol::generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env)
{
    const auto& module = config.m_qualityOfLife;

    const int  tomeSize            = module.m_tomeSize;
    const int  keySize             = module.m_keySize;
    const int  quiverSize          = module.m_quiverSize;
    const bool uniqueCharmLimit    = module.m_uniqueCharmLimit;
    const bool weakenTownSkills    = module.m_weakenTownSkills;
    const bool weakenTownSkillsExt = module.m_weakenTownSkillsExt;
    const bool showItemLevel       = module.m_showItemLevel;
    const bool disableExpPenalty   = module.m_disableExpPenalty;
    const int  reduceCost          = module.m_reduceCost;

    auto updateStack = [](std::optional<int>& value, int configValue) {
        if (!value.has_value())
            return;
        int prev = value.value();
        if (prev == 0)
            return;
        value = std::max(prev, configValue);
    };

    if (keySize || quiverSize) {
        for (auto& row : output.misc.rows) {
            auto& code = row.code;
            if (row.code == "ibk" || row.code == "tbk")
                updateStack(row.maxstack, tomeSize);
            if (keySize && code == "key")
                updateStack(row.maxstack, keySize);
            if (quiverSize && (code == "aqv" || code == "cqv"))
                updateStack(row.maxstack, quiverSize);
        }
    }
    if (uniqueCharmLimit) {
        for (auto& row : output.uniqueitems.rows) {
            row.carry1.reset();
        }
    }
    if (weakenTownSkills || weakenTownSkillsExt) {
        static const StringSet s_exceptions{ "Teleport", "Battle Orders", "Battle Command" };
        for (auto& row : output.skills.rows) {
            if (s_exceptions.contains(row.skill) || weakenTownSkillsExt)
                row.inTown = true;
        }
    }
    if (reduceCost != 100) {
        for (auto& row : output.skills.rows) {
            auto& mult = row.costMult;
            auto& add  = row.costAdd;
            if (!mult.has_value())
                continue;
            mult = (mult.value() * reduceCost / 100);
            add  = (add.value_or(0) * reduceCost / 100);
        }

        for (auto& row : output.itemstatcost.rows) {
            auto& mult = row.multiply;
            auto& add  = row.add;
            if (!mult.has_value())
                continue;
            mult = (mult.value() * reduceCost / 100);
            add  = (add.value_or(0) * reduceCost / 100);
        }
    }
    if (showItemLevel) {
        for (auto& row : output.armor.rows)
            row.showLevel = true;
        for (auto& row : output.weapons.rows)
            row.showLevel = true;
    }
    if (disableExpPenalty) {
        for (auto& row : output.experience.rows)
            row.expRatio = 1024;
    }

    auto replaceJson = [&output, &env](const std::map<std::string, std::string>& replacements, const JsonId& name) {
        if (replacements.empty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto& jsonArray = jsonDoc.getList();
        for (Mernel::PropertyTree& lang : jsonArray) {
            const std::string key = lang["Key"].getScalar().toString();
            if (replacements.contains(key)) {
                auto val              = replacements.at(key);
                lang[env.d2rLocaleId] = Mernel::PropertyTreeScalar{ val };
            }
        }
    };
    std::map<std::string, std::string> replacementsItems;
    std::map<std::string, std::string> replacementsAffix;
    auto                               twoColor = [](const Color& color, const std::string& coloredText, const std::string& grayText) {
        return getColorDesc(color).m_fullBinaryCode + coloredText + getColorDesc(Color::grey).m_fullBinaryCode + grayText;
    };
    if (module.m_compactPots) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "mp1", twoColor(Color::blue, "!", "MP1") },
            { "mp2", twoColor(Color::blue, "!", "MP2") },
            { "mp3", twoColor(Color::blue, "!", "MP3") },
            { "mp4", twoColor(Color::blue, "!", "MP4") },
            { "mp5", twoColor(Color::blue, "!", "MP5") },
            { "hp1", twoColor(Color::red, "!", "HP1") },
            { "hp2", twoColor(Color::red, "!", "HP2") },
            { "hp3", twoColor(Color::red, "!", "HP3") },
            { "hp4", twoColor(Color::red, "!", "HP4") },
            { "hp5", twoColor(Color::red, "!", "HP5") },
            { "rvs", twoColor(Color::purple, "Rej", "") },
            { "rvl", twoColor(Color::purple, "Full", "") },
        });
    }
    if (module.m_compactScrolls) {
        replacementsItems.merge(std::map<std::string, std::string>{
            { "tsc", twoColor(Color::blue, "•", "TP") },
            { "isc", twoColor(Color::red, "•", "ID") },
        });
    }

    replaceJson(replacementsItems, JsonId::ItemNames);
    replaceJson(replacementsAffix, JsonId::ItemNameAffixes);
}

}
