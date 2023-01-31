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
    ConfigPageAbstract(const std::string& localeId, const IModule::Ptr& module, QWidget* parent);

    // IConfigPage interface
public:
    PresetList pagePresets() const override;
    QString    caption() const override;
    QString    pageHelp() const override;

    void updateUIFromSettings(const Mernel::PropertyTree& data) override;
    void writeSettingsFromUI(Mernel::PropertyTree& data) const override;

    const IModule& getModule() const override { return *m_module; }

    void updateModList(const QStringList& mods) override {}

protected:
    void                 addWidget(QWidget* w);

    void makeAllEditors();

    void closeLayout();

private:
    IValueWidget* makeEditor(const IValueWidget::Params& params);
    void          makeWidget(const std::string& id, const IValueWidget::Params& params);

private:
    const std::string                    m_localeId;
    std::map<std::string, IValueWidget*> m_editors;
    QVBoxLayout*                         m_layout;
    const IModule::Ptr                   m_module;
    IConfigPage::PresetList              m_presets;

    std::vector<std::pair<std::string, IValueWidget::Params>> m_uiHints;

    QString m_title;
    QString m_help;
};

}
