/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

namespace D2ModGen {

class ModuleMergeMods : public PluginModule {
public:
    ModuleMergeMods(Mernel::PropertyTree moduleMetadata, std::string id)
        : PluginModule(std::move(moduleMetadata), std::move(id))
    {}

protected:
    void gatherInfoInternal(ExtraDependencies& output, const InputContext& input) const;
};

class ModuleMergeModsPreload : public ModuleMergeMods {
public:
    static constexpr const std::string_view key = Key::mergePregen;

    ModuleMergeModsPreload(Mernel::PropertyTree moduleMetadata, std::string id)
        : ModuleMergeMods(std::move(moduleMetadata), std::move(id))
    {}
    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override
    {
        gatherInfoInternal(output.m_preGen, input);
    }
};

class ModuleMergeModsPostGen : public ModuleMergeMods {
public:
    static constexpr const std::string_view key = Key::mergePostgen;

    ModuleMergeModsPostGen(Mernel::PropertyTree moduleMetadata, std::string id)
        : ModuleMergeMods(std::move(moduleMetadata), std::move(id))
    {}

    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override
    {
        gatherInfoInternal(output.m_postGen, input);
    }
};

}
