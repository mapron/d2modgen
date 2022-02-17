/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPagePerfectRoll : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPagePerfectRoll(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Perfect rolls");
    }
    QString settingKey() const override
    {
        return "perfect_rolls";
    }
    QString pageHelp() const override;

    void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
