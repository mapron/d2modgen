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
    static constexpr const std::string_view key = IModule::Key::monsterDensity;

    ConfigPageMonDensity(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Monster density");
    }
    std::string settingKey() const override
    {
        return std::string(key);
    }
    QString pageHelp() const override;
};

}
