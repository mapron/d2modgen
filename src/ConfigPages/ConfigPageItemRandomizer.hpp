/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"
#include "AttributeHelper.hpp"

namespace D2ModGen {

class ConfigPageItemRandomizer : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageItemRandomizer(QWidget* parent);

   
    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Item Randomizer");
    }
    QString settingKey() const override
    {
        return "randomizer";
    }
    QString pageHelp() const override;
    void    generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
