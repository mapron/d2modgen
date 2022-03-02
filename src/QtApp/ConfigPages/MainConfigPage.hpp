/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IConfigPage.hpp"

namespace D2ModGen {

class MainConfigPage : public IConfigPage {
    Q_OBJECT
public:
    MainConfigPage(const IModule::Ptr& module, QWidget* parent);
    ~MainConfigPage();

    void        createNewSeed();
    QStringList getOtherMods() const;

    // IConfigPage interface
public:
    QString    caption() const override;
    QString    pageHelp() const override;
    PresetList pagePresets() const override;

    void updateUIFromSettings(const PropertyTree& data) override;
    void writeSettingsFromUI(PropertyTree& data) const override;

    void updateModList(const QStringList& mods) override {}

    void updateUIFromSettingsMain(const PropertyTree& data);
    void writeSettingsFromUIMain(PropertyTree& data) const;

    const IModule& getModule() const override;

signals:
    void needUpdateModList();

private:
    void setLaunch(QString arg);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

}
