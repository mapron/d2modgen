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

IValueWidget* ConfigPageAbstract::makeEditor(const std::string& key, const QString& title, const QString& help)
{
    using Control            = IModule::UiControlHint::Control;
    auto        defaultValue = getModule().defaultValues()[key];
    auto        hints        = getModule().uiHints();
    auto        hint         = hints[key];
    int         min          = hint.m_min;
    int         max          = hint.m_max;
    const auto* ival         = std::get_if<int64_t>(&defaultValue);
    const auto* bval         = std::get_if<bool>(&defaultValue);
    if (hint.m_control == Control::Auto)
        max = 100;

    if (hint.m_control == Control::SliderMinMax || hint.m_control == Control::Auto && ival)
        return addHelp(new SliderWidgetMinMax(title, QString::fromStdString(key), min, max, PropertyTree::toInt(defaultValue), hint.m_compact, this), help);
    if (hint.m_control == Control::Slider)
        return addHelp(new SliderWidget(title, QString::fromStdString(key), hint.m_num, hint.m_denom, PropertyTree::toInt(defaultValue), hint.m_compact, this), help);
    if (hint.m_control == Control::LineEdit)
        return addHelp(new LineWidget(title, QString::fromStdString(key), QString::fromStdString(PropertyTree::toString(defaultValue)), this), help);
    if (hint.m_control == Control::CheckBox || hint.m_control == Control::Auto && bval)
        return addHelp(new CheckboxWidget(title, QString::fromStdString(key), PropertyTree::toBool(defaultValue), this), help);

    assert(false);
    return nullptr;
}

QList<IValueWidget*> ConfigPageAbstract::makeEditors(const StringVector& keys)
{
    QList<IValueWidget*> result;
    auto                 titles = widgetTitles();
    auto                 helps  = widgetHelps();
    for (const std::string& key : keys)
        result << makeEditor(key, titles.value(key, QString::fromStdString(key)), helps.value(key));
    return result;
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
