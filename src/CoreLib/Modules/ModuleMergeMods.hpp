/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModuleAbstract.hpp"

namespace D2ModGen {

class ModuleMergeMods : public ModuleAbstract {
public:
    // IModule interface
public:
    PropertyTreeScalarMap defaultValues() const override;

protected:
    void gatherInfoInternal(ExtraDependencies& output, const InputContext& input) const;
};

class ModuleMergeModsPreload : public ModuleMergeMods {
public:
    static constexpr const std::string_view key = Key::mergePregen;
    // IModule interface
public:
    std::string settingKey() const override
    {
        return std::string(key);
    }
    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override
    {
        gatherInfoInternal(output.m_preGen, input);
    }
};

class ModuleMergeModsPostGen : public ModuleMergeMods {
public:
    static constexpr const std::string_view key = Key::mergePostgen;
    // IModule interface
public:
    std::string settingKey() const override
    {
        return std::string(key);
    }
    void gatherInfo(PreGenerationContext& output, const InputContext& input) const override
    {
        gatherInfoInternal(output.m_postGen, input);
    }
};

}
