/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once
#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageChallenge : public ConfigPageAbstract {
    struct Item {
        QSet<QString> internalIds;
        QString       settingKey;
        QString       title;
    };
    const QList<Item> m_items;

public:
    ConfigPageChallenge(QWidget* parent);

    // IConfigPage interface
public:
    QString caption() const override
    {
        return "Challenge";
    }
    QString settingKey() const override
    {
        return "challenge";
    }
    KeySet generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
