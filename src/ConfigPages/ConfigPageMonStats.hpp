/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageMonStats : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMonStats(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Monster stats");
    }
    QString settingKey() const override
    {
        return "monster_stats";
    }
    QString pageHelp() const override;
    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
