/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageGambling : public ConfigPageAbstract {
    Q_OBJECT
public:
    static constexpr const std::string_view key = IModule::Key::gambling;

    ConfigPageGambling(const IModule::Ptr& module, QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Gambling");
    }
    QString                    pageHelp() const override;
    QMap<std::string, QString> widgetTitles() const override;
};

}
