/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageDropFiltering : public ConfigPageAbstract {
    Q_OBJECT
public:
    static constexpr const std::string_view key = IModule::Key::dropFiltering;

    struct Item {
        std::string settingKey;
        QString     title;
    };
    const QList<Item> m_items;

    ConfigPageDropFiltering(const IModule::Ptr& module, QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Drops filtering");
    }
    QString pageHelp() const override;
};

}
