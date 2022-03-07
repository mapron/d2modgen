/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "PluginConfigPage.hpp"

namespace D2ModGen {

PluginConfigPage::PluginConfigPage(const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(module, parent)
{
    closeLayout();
}

PluginConfigPage::~PluginConfigPage()
{
}

QString PluginConfigPage::caption() const
{
    return QString::fromStdString(getModule().settingKey()).mid(7);
}

}
