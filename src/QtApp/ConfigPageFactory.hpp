/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPages/IConfigPage.hpp"

namespace D2ModGen {

using ConfigPageCreatorFunc = std::function<IConfigPage*(QWidget*)>;

IConfigPage*        createConfigPage(const std::string& configKey, QWidget* parent);
QList<IConfigPage*> createConfigPages(const std::vector<std::string_view>& keys, QWidget* parent);
void                pageRegisterCreator(const std::string& configKey, ConfigPageCreatorFunc factory);

template<class T>
bool pageRegisterHelper()
{
    pageRegisterCreator(std::string(T::key), [](QWidget* parent) { return new T(parent); });
    return true;
}

}
