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

#include <functional>

class QVBoxLayout;

namespace D2ModGen {

class ConfigPageAbstract : public IConfigPage {
public:
    ConfigPageAbstract(QWidget* parent);

    // IConfigPage interface
public:
    bool       canBeDisabled() const override;
    PresetList pagePresets() const override;

    void readSettings(const PropertyTree& data) override;
    void writeSettings(PropertyTree& data) const override;

    bool isConfigEnabled() const override;
    void setConfigEnabled(bool state) override;

    const IModule& getModule() const override { return *m_module; }

    virtual QMap<std::string, QString> widgetTitles() const { return {}; };
    virtual QMap<std::string, QString> widgetHelps() const { return {}; };

protected:
    void addWidget(QWidget* w);
    void addEditors(QList<IValueWidget*> editors);
    void addEditorsPlain(QList<IValueWidget*> editors);
    void closeLayout();
    void initModule();

private:
    QMap<std::string, IValueWidget*> m_editors;
    QVBoxLayout*                     m_layout;
    bool                             m_enabled = true;
    std::unique_ptr<IModule>         m_module;
};

}
