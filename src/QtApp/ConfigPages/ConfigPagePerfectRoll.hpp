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
    static constexpr const std::string_view key = IModule::Key::perfectRolls;

    ConfigPagePerfectRoll(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Perfect rolls");
    }
    std::string settingKey() const override
    {
        return std::string(key);
    }
    QString pageHelp() const override;

    QMap<std::string, QString> widgetTitles() const override;
};

}