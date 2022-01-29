/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageCharacter : public ConfigPageAbstract {
public:
    ConfigPageCharacter(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Character";
    }
    QString settingKey() const override
    {
        return "character";
    }
    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
