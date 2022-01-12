/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class ConfigPageCube : public ConfigPageAbstract {
public:
    ConfigPageCube(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Horadric Cube";
    }
    QString settingKey() const override
    {
        return "cube";
    }
    KeySet generate(TableSet& tableSet, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};
