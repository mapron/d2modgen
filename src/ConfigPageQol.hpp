/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

class ConfigPageQol : public ConfigPageAbstract {
public:
    ConfigPageQol(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Quality of life";
    }
    QString settingKey() const override
    {
        return "qol";
    }
    KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};
