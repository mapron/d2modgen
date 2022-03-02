/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleFactory.hpp"

#include "Modules/ModuleChallenge.hpp"
#include "Modules/ModuleCharacter.hpp"
#include "Modules/ModuleCube.hpp"
#include "Modules/ModuleDropFiltering.hpp"
#include "Modules/ModuleGambling.hpp"
#include "Modules/ModuleItemDrops.hpp"
#include "Modules/ModuleItemRandomizer.hpp"
#include "Modules/ModuleMonDensity.hpp"
#include "Modules/ModuleMonRandomizer.hpp"
#include "Modules/ModuleMonStats.hpp"
#include "Modules/ModulePerfectRoll.hpp"
#include "Modules/ModuleQol.hpp"
#include "Modules/ModuleRuneDrops.hpp"
#include "Modules/ModuleSkillRandomizer.hpp"

namespace D2ModGen {

namespace {

using FactoryMap = std::map<std::string, std::function<IModule::Ptr()>>;

FactoryMap& getFactory()
{
    static FactoryMap s_map;
    return s_map;
}

}

IModule::Ptr createModule(const std::string& configKey)
{
    return getFactory().at(configKey)();
}

void registerCreator(const std::string& configKey, std::function<IModule::Ptr()> factory)
{
    getFactory()[configKey] = std::move(factory);
}

IModule::PtrMap createAllModules()
{
    IModule::PtrMap result;
    auto&           fmap = getFactory();
    for (auto& p : fmap)
        result[p.first] = p.second();
    return result;
}

}
