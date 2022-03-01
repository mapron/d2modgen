/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

namespace D2ModGen {

class ConfigPageDropFiltering : public ConfigPageAbstract {
    Q_OBJECT
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
        return tr("Drops filtering");
    }
    std::string settingKey() const override
    {
        return "drop_filter";
    }
    QString pageHelp() const override;

    void gatherInfo(PreGenerationContext& output, const GenerationEnvironment& env) const override;
    void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override;
};

}
