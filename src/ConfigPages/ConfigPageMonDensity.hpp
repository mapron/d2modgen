/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageMonDensity : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMonDensity(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Monster density");
    }
    std::string settingKey() const override
    {
        return "density";
    }
    QString pageHelp() const override;
    void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
