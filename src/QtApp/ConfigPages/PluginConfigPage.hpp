/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class QQuickWidget;

namespace D2ModGen {

class PluginConfigPage : public ConfigPageAbstract {
    Q_OBJECT
public:
    PluginConfigPage(const IModule::Ptr& module, QWidget* parent);
    ~PluginConfigPage();

    Q_INVOKABLE void qmlDataChanged();

    // IConfigPage interface
public:
    QString caption() const override;
    QString pageHelp() const override { return ""; }

    void updateUIFromSettings(const PropertyTree& data) override;
    void writeSettingsFromUI(PropertyTree& data) const override;

private:
    QQuickWidget* m_quick = nullptr;
};

}
