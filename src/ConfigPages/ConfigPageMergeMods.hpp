/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class QListWidget;

namespace D2ModGen {

class ConfigPageMergeMods : public ConfigPageAbstract {
public:
    ConfigPageMergeMods(QWidget* parent);

    void setModList(QStringList mods);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Merge mods";
    }
    QString settingKey() const override
    {
        return "merging";
    }
    void   readSettings(const QJsonObject& data) override;
    void   writeSettings(QJsonObject& data) const override;
    KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;

private:
    QStringList getSelected() const;

private:
    QListWidget* m_modList;
};

}
