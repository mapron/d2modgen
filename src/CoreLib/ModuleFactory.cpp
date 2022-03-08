/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleFactory.hpp"

#include "PluginModule.hpp"

namespace D2ModGen {

namespace {

using FactoryMap = std::map<std::string, ModuleCreatorFunction>;

FactoryMap& getFactory()
{
    static FactoryMap s_map;
    return s_map;
}

}

IModule::Ptr createModule(PropertyTree moduleMetaData, std::string id)
{
    const auto& factory = getFactory();
    if (!factory.contains(id)) {
        auto pluginModule = std::make_shared<const PluginModule>(std::move(moduleMetaData), std::move(id), "plugin_");
        return pluginModule;
    }
    auto& creator = factory.at(id);
    return creator(std::move(moduleMetaData), std::move(id));
}

void registerCreator(const std::string& configKey, ModuleCreatorFunction factory)
{
    getFactory()[configKey] = std::move(factory);
}

}
