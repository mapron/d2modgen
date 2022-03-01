/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"
#include "PropertyTree.hpp"

#include <QWidget>

namespace D2ModGen {

class IConfigPage : public QWidget {
    Q_OBJECT
public:
    struct Preset {
        QString     title;
        QJsonObject data;
    };
    using PresetList = QList<Preset>;

public:
    explicit IConfigPage(QWidget* parent)
        : QWidget(parent)
    {}

    virtual bool        canBeDisabled() const = 0;
    virtual QString     caption() const       = 0;
    virtual std::string settingKey() const    = 0;
    virtual QString     pageHelp() const      = 0;
    virtual PresetList  pagePresets() const   = 0;

    virtual void readSettings(const PropertyTree& data)  = 0;
    virtual void writeSettings(PropertyTree& data) const = 0;

    virtual bool isConfigEnabled() const      = 0;
    virtual void setConfigEnabled(bool state) = 0;

    virtual void gatherInfo(PreGenerationContext& output, const GenerationEnvironment& env) const             = 0;
    virtual void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const = 0;

signals:
    void dataChanged();
};

}
