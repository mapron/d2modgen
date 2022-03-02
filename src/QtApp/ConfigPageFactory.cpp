/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageFactory.hpp"

#include "ConfigPages/ConfigPageChallenge.hpp"
#include "ConfigPages/ConfigPageCharacter.hpp"
#include "ConfigPages/ConfigPageCube.hpp"
#include "ConfigPages/ConfigPageDropFiltering.hpp"
#include "ConfigPages/ConfigPageGambling.hpp"
#include "ConfigPages/ConfigPageItemDrops.hpp"
#include "ConfigPages/ConfigPageItemRandomizer.hpp"
#include "ConfigPages/ConfigPageMonDensity.hpp"
#include "ConfigPages/ConfigPageMonRandomizer.hpp"
#include "ConfigPages/ConfigPageMonStats.hpp"
#include "ConfigPages/ConfigPagePerfectRoll.hpp"
#include "ConfigPages/ConfigPageQol.hpp"
#include "ConfigPages/ConfigPageRuneDrops.hpp"
#include "ConfigPages/ConfigPageSkillRandomizer.hpp"

namespace D2ModGen {

namespace {

using FactoryMap = std::map<std::string, ConfigPageCreatorFunc>;

FactoryMap& getFactory()
{
    static FactoryMap s_map;
    return s_map;
}

}

IConfigPage* createConfigPage(const IModule::Ptr& module, QWidget* parent)
{
    const std::string configKey = module->settingKey();
    return getFactory().at(configKey)(module, parent);
}

void pageRegisterCreator(const std::string& configKey, ConfigPageCreatorFunc factory)
{
    getFactory()[configKey] = std::move(factory);
}

}
