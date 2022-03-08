/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageFactory.hpp"

#include "ConfigPages/PluginConfigPage.hpp"

namespace D2ModGen {

namespace {

using FactoryMap = std::map<std::string, ConfigPageCreatorFunc>;

FactoryMap& getFactory()
{
    static FactoryMap s_map;
    return s_map;
}

}

IConfigPage* createConfigPage(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
{
    const std::string configKey = module->settingKey();
    const auto&       factory   = getFactory();
    if (!factory.contains(configKey)) {
        IConfigPage* commonPage = new PluginConfigPage(localeId, module, parent);
        return commonPage;
    }
    auto& creator = factory.at(configKey);
    return creator(localeId, module, parent);
}

void pageRegisterCreator(const std::string& configKey, ConfigPageCreatorFunc factory)
{
    getFactory()[configKey] = std::move(factory);
}

}
