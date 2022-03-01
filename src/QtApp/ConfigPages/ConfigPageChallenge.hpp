/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageChallenge : public ConfigPageAbstract {
    Q_OBJECT

    struct Item {
        std::string settingKey;
        QString     title;
    };
    const QList<Item> m_items;

public:
    static constexpr const std::string_view key = IModule::Key::challenge;

    ConfigPageChallenge(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Challenge");
    }
    std::string settingKey() const override
    {
        return std::string(key);
    }
    QString pageHelp() const override;
};

}
