/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "CoreExport.hpp"

#include <functional>

namespace D2ModGen {

CORE_EXPORT IModule::Ptr createModule(const std::string& configKey);
CORE_EXPORT IModule::PtrMap createAllModules();

void registerCreator(const std::string& configKey, std::function<IModule::Ptr()> factory);

template<class T>
bool registerHelper()
{
    registerCreator(std::string(T::key), [] { return std::make_shared<T>(); });
    return true;
}
}
