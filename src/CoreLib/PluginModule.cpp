/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PluginModule.hpp"

namespace D2ModGen {

PluginModule::PluginModule(Mernel::PropertyTreeMap defaults, std::string id)
    : m_defaults(std::move(defaults))
    , m_id(std::move(id))
{
}

}
