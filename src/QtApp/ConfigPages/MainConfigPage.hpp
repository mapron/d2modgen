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
    MainConfigPage(QWidget* parent);
    ~MainConfigPage();

    GenerationEnvironment getEnv() const;
    void                  createNewSeed();
    QStringList           getOtherMods() const;

    // IConfigPage interface
public:
    bool        canBeDisabled() const override;
    QString     caption() const override;
    std::string settingKey() const override;
    QString     pageHelp() const override;
    PresetList  pagePresets() const override;

    void readSettings(const PropertyTree& data) override;
    void writeSettings(PropertyTree& data) const override;

    bool isConfigEnabled() const override;
    void setConfigEnabled(bool state) override;

    const IModule& getModule() const override;

signals:
    void updateModList();

private:
    void setLaunch(QString arg);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

}
