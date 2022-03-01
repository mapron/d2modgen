/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageAbstract.hpp"

#include <QVBoxLayout>
#include <QLabel>

namespace D2ModGen {

ConfigPageAbstract::ConfigPageAbstract(QWidget* parent)
    : IConfigPage(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
}

bool ConfigPageAbstract::canBeDisabled() const
{
    return true;
}

IConfigPage::PresetList ConfigPageAbstract::pagePresets() const
{
    return {};
}

void ConfigPageAbstract::readSettings(const PropertyTree& data)
{
    for (const std::string& key : m_editors.keys()) {
        auto* w = m_editors[key];
        if (data.contains(key))
            w->setValue(data[key]);
        else
            w->resetValue();
    }
}

void ConfigPageAbstract::writeSettings(PropertyTree& data) const
{
    for (const std::string& key : m_editors.keys()) {
        auto* w = m_editors[key];
        if (!w->isDefault())
            data.insert(key, w->getValue());
    }
}

bool ConfigPageAbstract::isConfigEnabled() const
{
    return m_enabled;
}

void ConfigPageAbstract::setConfigEnabled(bool state)
{
    m_enabled = state;
}

void ConfigPageAbstract::addWidget(QWidget* w)
{
    m_layout->addWidget(w);
}

void ConfigPageAbstract::addEditors(QList<IValueWidget*> editors)
{
    for (IValueWidget* w : editors) {
        m_layout->addWidget(w);
    }
    addEditorsPlain(editors);
}

void ConfigPageAbstract::addEditorsPlain(QList<IValueWidget*> editors)
{
    for (IValueWidget* w : editors) {
        m_editors[w->objectName().toStdString()] = w;
        connect(w, &IValueWidget::dataChanged, this, &IConfigPage::dataChanged);
    }
}

void ConfigPageAbstract::closeLayout()
{
    m_layout->addStretch();
}

void ConfigPageAbstract::initModule()
{
    m_module = createModule(settingKey());
}

}
