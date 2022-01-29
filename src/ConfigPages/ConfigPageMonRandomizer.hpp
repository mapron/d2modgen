/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageMonRandomizer : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMonRandomizer(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Monster Randomizer");
    }
    QString settingKey() const override
    {
        return "monRandomizer";
    }
    QString pageHelp() const override;
    JsonFileSet extraFiles() const override;
    KeySet      generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
