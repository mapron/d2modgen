/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

class MainConfigPage : public IConfigPage {
public:
    MainConfigPage(QWidget* parent);
    ~MainConfigPage();

    GenerationEnvironment getEnv() const;
    void                  createNewSeed();

    // IConfigPage interface
public:
    QString caption() const override;
    QString settingKey() const override;
    void    readSettings(const QJsonObject& data) override;
    void    writeSettings(QJsonObject& data) const override;
    KeySet  generate(TableSet& tableSet, QRandomGenerator& rng, const GenerationEnvironment& env) const override;

private:
    void setLaunch(QString arg);

private:
    struct Impl;
    QScopedPointer<Impl> m_impl;
};
