/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "TableUtils.hpp"

#include "../ValueEditors/SliderWidget.hpp"
#include "../ValueEditors/CheckboxWidget.hpp"
#include "../ValueEditors/LineWidget.hpp"

#include "IConfigPage.hpp"
#include "ConfigPageFactory.hpp"
#include "ModuleFactory.hpp"

#include <QMap>

#include <functional>

class QVBoxLayout;

namespace D2ModGen {

class ConfigPageAbstract : public IConfigPage {
public:
    ConfigPageAbstract(const IModule::Ptr& module, QWidget* parent);

    // IConfigPage interface
public:
    PresetList pagePresets() const override;

    void updateUIFromSettings(const PropertyTree& data) override;
    void writeSettingsFromUI(PropertyTree& data) const override;

    const IModule& getModule() const override { return *m_module; }

    void updateModList(const QStringList& mods) override{}

    virtual QMap<std::string, QString> widgetTitles() const { return {}; };
    virtual QMap<std::string, QString> widgetHelps() const { return {}; };

protected:
    void                 addWidget(QWidget* w);
    void                 addEditors(QList<IValueWidget*> editors);
    void                 addEditorsPlain(QList<IValueWidget*> editors);
    IValueWidget*        makeEditor(const std::string& key, const QString& title, const QString& help = QString());
    QList<IValueWidget*> makeEditors(const StringVector& keys);
    void                 closeLayout();

private:
    QMap<std::string, IValueWidget*> m_editors;
    QVBoxLayout*                     m_layout;
    const IModule::Ptr               m_module;
};

}
