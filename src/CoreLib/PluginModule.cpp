/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PluginModule.hpp"

#include "FileIOUtils.hpp"
#include "DyLib.hpp"
#include "Formats/FileFormatJson.hpp"
#include "Logger.hpp"

#include "Bindings/C/ModgenCApi.h"

namespace D2ModGen {

PluginModule::PluginModule(PropertyTree moduleMetadata, std::string id, std::string idPrefix)
    : m_info(std::move(moduleMetadata))
    , m_defaults(m_info.contains("defaults") ? m_info["defaults"] : PropertyTree{ PropertyTreeScalarMap{} })
    , m_id(std::move(id))
    , m_idPrefix(std::move(idPrefix))
{
    const bool        hasDylib = m_info.value("hasDylib", false).toBool();
    const std::string root     = m_info.value("root", "").toString();
    if (hasDylib) {
        m_dylib        = std::make_unique<DyLib>(string2path(root), m_id);
        m_generateAddr = m_dylib->getProc("generate");
    }
}

PluginModule::~PluginModule() = default;

void PluginModule::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (!m_dylib)
        return;

    GenerateCFuncPtr generateInPlugin = reinterpret_cast<GenerateCFuncPtr>(m_generateAddr);
    CDataContext     ccontext{ &output };
    CPropertyTree    cdata{ &input.m_mergedSettings };
    generateInPlugin(ccontext, cdata);
}

}
