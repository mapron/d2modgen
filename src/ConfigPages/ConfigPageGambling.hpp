/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageGambling : public ConfigPageAbstract {
public:
    ConfigPageGambling(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Gambling";
    }
    QString settingKey() const override
    {
        return "gambling";
    }
    KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
