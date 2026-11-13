/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleGlobal.hpp"

#include "ModuleRandoItems.hpp"
#include "ModuleRandoSkills.hpp"
#include "ModuleRandoMonsters.hpp"

#include "ModuleBalanceCharacter.hpp"
#include "ModuleBalanceGambling.hpp"
#include "ModuleBalanceItems.hpp"
#include "ModuleBalanceMaps.hpp"
#include "ModuleBalanceMonsters.hpp"
#include "ModuleBalanceSkills.hpp"

#include "ModuleDroprate.hpp"

#include "ModuleOtherExpandedInventory.hpp"
#include "ModuleOtherHoradricCube.hpp"
#include "ModuleOtherQualityOfLife.hpp"

#include "Logger.hpp"

namespace D2ModGen {

ModuleGlobal::ModuleGlobal(DataContext& output_, const Config& config_, const GenerationEnvironment& env_)
    : AbstractModule(output_, config_, env_)
{
    reset();
}

void ModuleGlobal::reset()
{
    // light modules
    create<ModuleBalanceCharacter>("BalanceCharacter");
    create<ModuleBalanceGambling>("BalanceGambling");
    create<ModuleBalanceMonsters>("BalanceMonsters");
    create<ModuleBalanceMaps>("BalanceMaps");

    create<ModuleDroprate>("Droprate");
    create<ModuleOtherQualityOfLife>("OtherQualityOfLife");
    create<ModuleOtherExpandedInventory>("OtherExpandedInventory");

    // modules that can add more rows, thus in the end
    create<ModuleOtherHoradricCube>("OtherHoradricCube");
    create<ModuleRandoItems>("RandoItems");
    create<ModuleRandoMonsters>("RandoMonsters");
    create<ModuleRandoSkills>("RandoSkills");

    // item balance affect possible input (item stat ranges) for randomizer, that's why it is in the end.
    create<ModuleBalanceItems>("BalanceItems");
    // same for this coming after rando.
    create<ModuleBalanceSkills>("BalanceSkills");
}

void ModuleGlobal::generate()
{
    for (auto& module : m_modules) {
        Logger() << "start module: " << module.m_id;
        try {
            module.m_gen->generate();
        }
        catch (const std::exception& ex) {
            std::string msg("module '" + module.m_id + "': " + std::string(ex.what()));

            throw std::runtime_error(msg);
        }
    }

    auto replaceKey = [](Mernel::PropertyTreeMap& jsonObject, const std::string& sourceId, const std::string& newId) -> bool {
        if (!jsonObject.contains(sourceId))
            return false;
        const std::string modelName = jsonObject[sourceId].getScalar().toString();
        assert(!modelName.empty());
        jsonObject[newId] = Mernel::PropertyTreeScalar{ modelName };
        return true;
    };
    auto underscoreBeforeLast = [](std::string s) {
        char last = s.back();
        s.pop_back();
        s += "_";
        s += last;
        return s;
    };
    auto underscored = [](std::string s) {
        std::replace(s.begin(), s.end(), ' ', '_');
        std::replace(s.begin(), s.end(), '-', '_');
        return s;
    };

    auto doReplace = [&replaceKey, &underscoreBeforeLast, &underscored](const DataContext::IdCloneList& cloneList, Mernel::PropertyTree& jsonDoc, std::string title) {
        Mernel::PropertyTreeMap& jsonObject = jsonDoc.getMap();
        Logger() << "Start updating " << title << ", existing keys: " << jsonObject.size() << ", clone list size:" << cloneList.size();
        for (const auto& [sourceId, newId] : cloneList) {
            if (replaceKey(jsonObject, sourceId, newId))
                continue;

            auto safeId   = underscored(sourceId);
            auto safeDest = underscored(newId);
            if (replaceKey(jsonObject, safeId, safeDest))
                continue;

            safeId   = underscoreBeforeLast(safeId);
            safeDest = underscoreBeforeLast(safeDest);
            if (replaceKey(jsonObject, safeId, safeDest)) {
                continue;
            }
            Logger(Logger::Err) << "Not found replacement for=" << sourceId << " / " << safeId;
        }
        Logger() << "Finish  updating " << title << ", new size: " << jsonObject.size();
    };

    if (output.jsonFiles.contains(JsonId::Monsters)) {
        auto& jsonDoc = output.jsonFiles[JsonId::Monsters];
        doReplace(output.idClones.monstats, jsonDoc, "monster.json");
    }

    if (output.jsonFiles.contains(JsonId::Missiles)) {
        auto& jsonDoc = output.jsonFiles[JsonId::Missiles];
        doReplace(output.idClones.missiles, jsonDoc, "missiles.json");
    }
}

template<class T>
void ModuleGlobal::create(const std::string& id)
{
    m_modules.push_back(ModuleRecord{ id, std::make_unique<T>(output, config, env) });
}

}
