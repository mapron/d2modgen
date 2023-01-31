/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "ModgenCoreExport.hpp"

#include <functional>

namespace D2ModGen {

MODGENCORE_EXPORT IModule::Ptr createModule(Mernel::PropertyTree moduleMetaData, std::string id);

using ModuleCreatorFunction = std::function<IModule::Ptr(Mernel::PropertyTree moduleMetaData, std::string id)>;

void registerCreator(const std::string& configKey, ModuleCreatorFunction factory);

template<class T>
bool registerHelper()
{
    registerCreator(std::string(T::key), [](Mernel::PropertyTree moduleMetaData, std::string id) {
        assert(T::key == id);
        return std::make_shared<T>(std::move(moduleMetaData), std::move(id));
    });
    return true;
}

}
