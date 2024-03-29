/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class PluginConfigPage : public ConfigPageAbstract {
    Q_OBJECT
public:
    PluginConfigPage(const std::string& localeId, const IModule::Ptr& module, QWidget* parent);
    ~PluginConfigPage();

    // IConfigPage interface
public:
    void updateUIFromSettings(const Mernel::PropertyTree& data) override;
    void writeSettingsFromUI(Mernel::PropertyTree& data) const override;
};

}
