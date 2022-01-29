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
    QString     settingKey() const override;
    void        readSettings(const QJsonObject& data) override;
    void        writeSettings(QJsonObject& data) const override;
    JsonFileSet extraFiles() const override;
    bool        isConfigEnabled() const override;
    void        setConfigEnabled(bool state) override;

    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;

signals:
    void updateModList();

private:
    void setLaunch(QString arg);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

}
