/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "PluginModule.hpp"

namespace D2ModGen {

PluginModule::PluginModule(Mernel::PropertyTreeMap defaults)
    : m_defaults(std::move(defaults))
{
}

}
