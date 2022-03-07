/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PluginModule.hpp"

#include "FileIOUtils.hpp"
#include "DyLib.hpp"
#include "Formats/FileFormatJson.hpp"

#include "CApi.h"

namespace D2ModGen {

PluginModule::PluginModule(const std_path& jsonDeclFilepath)
{
    std::string buffer;
    if (!readFileIntoBuffer(jsonDeclFilepath, buffer) || !readJsonFromBuffer(buffer, m_info)) {
        throw std::runtime_error("Failed to read json file for plugin:" + path2string(jsonDeclFilepath));
    }
    m_id = m_info.value("id", "").toString();
    if (m_id.empty())
        throw std::runtime_error("Plugin id should not be empty");
    const bool hasDylib = m_info.value("hasDylib", false).toBool();
    if (hasDylib) {
        m_dylib        = std::make_unique<DyLib>(jsonDeclFilepath.parent_path(), m_id);
        m_generateAddr = m_dylib->getProc("generate");
    }
    m_info["root"] = PropertyTreeScalar(path2string(jsonDeclFilepath.parent_path()));
}

PropertyTree PluginModule::pluginInfo() const
{
    return m_info;
}

PluginModule::~PluginModule() = default;

void PluginModule::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (!m_dylib)
        return;

    GenerateCFuncPtr generateInPlugin = reinterpret_cast<GenerateCFuncPtr>(m_generateAddr);
    CDataContext     ccontext{ &output };
    generateInPlugin(ccontext);
}

}