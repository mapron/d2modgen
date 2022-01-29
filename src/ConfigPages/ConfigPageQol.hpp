/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageQol : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageQol(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Quality of life");
    }
    QString settingKey() const override
    {
        return "qol";
    }
    QString pageHelp() const override;
    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
