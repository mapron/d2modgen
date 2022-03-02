/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"
#include "PropertyTree.hpp"
#include "IModule.hpp"

#include <QWidget>

namespace D2ModGen {

class IConfigPage : public QWidget {
    Q_OBJECT
public:
    using PresetList = QStringList;

public:
    explicit IConfigPage(QWidget* parent)
        : QWidget(parent)
    {}

    virtual QString    caption() const     = 0;
    virtual QString    pageHelp() const    = 0;
    virtual PresetList pagePresets() const = 0;

    virtual void updateUIFromSettings(const PropertyTree& data) = 0;
    virtual void writeSettingsFromUI(PropertyTree& data) const  = 0;

    virtual void updateModList(const QStringList& mods) = 0;

    virtual const IModule& getModule() const = 0;

signals:
    void dataChanged();
};

}
