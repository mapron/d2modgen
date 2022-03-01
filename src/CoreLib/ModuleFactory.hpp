/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

namespace D2ModGen {

IModule::Ptr createModule(const std::string& configKey);
void         registerCreator(const std::string& configKey, std::function<IModule::Ptr()> factory);

template<class T>
bool registerHelper()
{
    registerCreator(std::string(T::key), [] { return std::make_unique<T>(); });
    return true;
}
}
