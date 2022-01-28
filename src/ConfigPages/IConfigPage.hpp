/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include <QWidget>

namespace D2ModGen {

class IConfigPage : public QWidget {
public:
    explicit IConfigPage(QWidget* parent)
        : QWidget(parent)
    {}

    virtual bool        canBeDisabled() const                  = 0;
    virtual QString     caption() const                        = 0;
    virtual QString     settingKey() const                     = 0;
    virtual void        readSettings(const QJsonObject& data)  = 0;
    virtual void        writeSettings(QJsonObject& data) const = 0;
    virtual JsonFileSet extraFiles() const                     = 0;
    virtual bool        isConfigEnabled() const                = 0;
    virtual void        setConfigEnabled(bool state)           = 0;

    virtual KeySet generate(GenOutput& output, QRandomGenerator& rng, const GenerationEnvironment& env) const = 0;
};

}
