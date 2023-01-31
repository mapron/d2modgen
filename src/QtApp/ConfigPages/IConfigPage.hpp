/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"
#include "MernelPlatform/PropertyTree.hpp"
#include "IModule.hpp"

#include <QWidget>

namespace D2ModGen {

class IConfigPage : public QWidget {
    Q_OBJECT
public:
    struct Preset {
        QString      m_title;
        Mernel::PropertyTree m_data;
    };
    using PresetList = std::vector<Preset>;

public:
    explicit IConfigPage(QWidget* parent)
        : QWidget(parent)
    {}

    virtual QString    caption() const     = 0;
    virtual QString    pageHelp() const    = 0;
    virtual PresetList pagePresets() const = 0;

    virtual void updateUIFromSettings(const Mernel::PropertyTree& data) = 0;
    virtual void writeSettingsFromUI(Mernel::PropertyTree& data) const  = 0;

    virtual void updateModList(const QStringList& mods) = 0;

    virtual const IModule& getModule() const = 0;

signals:
    void dataChanged();
};

}
