/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

class ConfigPageDropFiltering : public ConfigPageAbstract {
public:
    struct Item {
        QSet<QString> internalIds;
        QString       settingKey;
        QString       title;
    };
    const QList<Item> m_items;

    ConfigPageDropFiltering(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Drops filtering";
    }
    QString settingKey() const override
    {
        return "drop_filter";
    }
    JsonFileSet extraFiles() const override;
    KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};
