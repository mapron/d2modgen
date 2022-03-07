/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageAbstract.hpp"

#include <QVBoxLayout>

namespace D2ModGen {

ConfigPageAbstract::ConfigPageAbstract(const IModule::Ptr& module, QWidget* parent)
    : IConfigPage(parent)
    , m_module(module)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
}

IConfigPage::PresetList ConfigPageAbstract::pagePresets() const
{
    return {};
}

void ConfigPageAbstract::updateUIFromSettings(const PropertyTree& data)
{
    for (const std::string& key : m_editors.keys()) {
        auto* w = m_editors[key];
        assert(data.contains(key));
        if (data.contains(key))
            w->setValue(data[key]);
    }
}

void ConfigPageAbstract::writeSettingsFromUI(PropertyTree& data) const
{
    for (const std::string& key : m_editors.keys()) {
        auto* w = m_editors[key];
        data.insert(key, w->getValue());
    }
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
    using Control   = IModule::UiControlHint::Control;
    const auto defMap = getModule().defaultValues();
    const auto& def = defMap[key];
    if (!def.isScalar())
        return nullptr;

    auto defaultValue = def.getScalar();
    auto hints        = getModule().uiHints();
    auto hint         = hints[key];
    int  min          = hint.m_min;
    int  max          = hint.m_max;
    if (hint.m_control == Control::Auto)
        max = 100;

    if (hint.m_control == Control::SliderMinMax || hint.m_control == Control::Auto && defaultValue.isInt())
        return addHelp(new SliderWidgetMinMax(title, QString::fromStdString(key), min, max, hint.m_compact, this), help);
    if (hint.m_control == Control::Slider || hint.m_control == Control::Auto && defaultValue.isDouble())
        return addHelp(new SliderWidget(title, QString::fromStdString(key), hint.m_num, hint.m_denom, hint.m_compact, this), help);
    if (hint.m_control == Control::LineEdit || hint.m_control == Control::Auto && defaultValue.isString())
        return addHelp(new LineWidget(title, QString::fromStdString(key), this), help);
    if (hint.m_control == Control::CheckBox || hint.m_control == Control::Auto && defaultValue.isBool())
        return addHelp(new CheckboxWidget(title, QString::fromStdString(key), this), help);

    assert(false);
    return nullptr;
}

QList<IValueWidget*> ConfigPageAbstract::makeEditors(const StringVector& keys)
{
    QList<IValueWidget*> result;
    auto                 titles = widgetTitles();
    auto                 helps  = widgetHelps();
    for (const std::string& key : keys) {
        auto* e = makeEditor(key, titles.value(key, QString::fromStdString(key)), helps.value(key));
        if (e)
            result << e;
    }
    return result;
}

void ConfigPageAbstract::closeLayout()
{
    m_layout->addStretch();
}

}
