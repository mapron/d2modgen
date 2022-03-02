/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPages/IConfigPage.hpp"

namespace D2ModGen {

using ConfigPageCreatorFunc = std::function<IConfigPage*(const IModule::Ptr&, QWidget*)>;

IConfigPage*        createConfigPage(const IModule::Ptr& module, QWidget* parent);
void                pageRegisterCreator(const std::string& configKey, ConfigPageCreatorFunc factory);

template<class T>
bool pageRegisterHelper()
{
    pageRegisterCreator(std::string(T::key), [](const IModule::Ptr& module, QWidget* parent) { return new T(module, parent); });
    return true;
}

}
