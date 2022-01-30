/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageCharacter : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageCharacter(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Character");
    }
    QString settingKey() const override
    {
        return "character";
    }
    QString pageHelp() const override;
    void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
