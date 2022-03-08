/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPages/IConfigPage.hpp"

namespace D2ModGen {

using ConfigPageCreatorFunc = std::function<IConfigPage*(const std::string&,const IModule::Ptr&, QWidget*)>;

IConfigPage* createConfigPage(const std::string& localeId, const IModule::Ptr& module, QWidget* parent);
void         pageRegisterCreator(const std::string& configKey, ConfigPageCreatorFunc factory);

template<class T>
bool pageRegisterHelper()
{
    pageRegisterCreator(std::string(T::key), [](const std::string& localeId, const IModule::Ptr& module, QWidget* parent) {
        return new T(localeId, module, parent);
    });
    return true;
}

}
