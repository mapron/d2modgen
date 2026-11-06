/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleGlobal.hpp"

#include "ModuleChallenge.hpp"
#include "ModuleCharacter.hpp"
#include "ModuleCube.hpp"
#include "ModuleGambling.hpp"
#include "ModuleTreasureClasses.hpp"
#include "ModuleItemRandomizer.hpp"
#include "ModuleMonRandomizer.hpp"
#include "ModuleMonStats.hpp"
#include "ModuleItemBalance.hpp"
#include "ModuleQol.hpp"
#include "ModuleSkillRandomizer.hpp"

#include "Logger.hpp"

namespace D2ModGen {

ModuleGlobal::ModuleGlobal(DataContext& output_, RandomGenerator& rng_, const Config& config_, const GenerationEnvironment& env_)
    : AbstractModule(output_, rng_, config_, env_)
{
    reset();
}

void ModuleGlobal::reset()
{
    // light modules
    create<ModuleChallenge>("Challenge");
    create<ModuleCharacter>("Character");
    create<ModuleGambling>("Gambling");
    create<ModuleTreasureClasses>("TreasureClasses");
    create<ModuleMonStats>("MonsterStats");
    create<ModuleQol>("QualityOfLife");

    // modules that can add more rows, thus in the end
    create<ModuleCube>("HoradricCube");
    create<ModuleMonRandomizer>("MonsterRandomizer");
    create<ModuleItemRandomizer>("ItemRandomizer");
    create<ModuleSkillRandomizer>("SkillRandomizer");

    // item balance affect input for randomizer, that's why it is in the end.
    create<ModuleItemBalance>("ItemBalance");
}

void ModuleGlobal::generate()
{
    for (auto& module : m_modules) {
        Logger() << "start module: " << module.m_id;
        try {
            module.m_gen->generate();
        }
        catch (const std::exception& ex) {
            m_err = std::string("module '" + module.m_id + "': " + std::string(ex.what()));

            return;
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
        return s;
    };

    auto doReplace = [&replaceKey, &underscoreBeforeLast, &underscored](const DataContext::IdCloneList& cloneList, Mernel::PropertyTree& jsonDoc) {
        Mernel::PropertyTreeMap& jsonObject = jsonDoc.getMap();
        for (const auto& [sourceId, newId] : cloneList) {
            if (replaceKey(jsonObject, sourceId, newId))
                continue;

            auto safeId   = underscored(sourceId);
            auto safeDest = underscored(newId);
            if (replaceKey(jsonObject, safeId, safeDest))
                continue;

            safeId   = underscoreBeforeLast(safeId);
            safeDest = underscoreBeforeLast(safeDest);
            if (jsonObject.contains(safeId)) {
                replaceKey(jsonObject, safeId, safeDest);
                continue;
            }
            Logger(Logger::Err) << "Not found replacement for=" << sourceId << " / " << safeId;
        }
    };

    if (output.jsonFiles.contains(JsonId::Monsters)) {
        Logger() << "Update monster.json";
        Mernel::PropertyTree& jsonDoc = output.jsonFiles[JsonId::Monsters];
        doReplace(output.idClones.monstats, jsonDoc);
    }

    if (output.jsonFiles.contains(JsonId::Missiles)) {
        Logger() << "Update missiles.json";
        auto& jsonDoc = output.jsonFiles[JsonId::Missiles];
        doReplace(output.idClones.missiles, jsonDoc);
    }
}

template<class T>
void ModuleGlobal::create(const std::string& id)
{
    m_modules.push_back(ModuleRecord{ id, std::make_unique<T>(output, rng, config, env) });
}

}
