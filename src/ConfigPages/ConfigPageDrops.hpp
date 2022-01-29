/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageDrops : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageDrops(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Increase Item/Rune drops");
    }
    QString settingKey() const override
    {
        return "drops";
    }
    QString pageHelp() const override;
    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
