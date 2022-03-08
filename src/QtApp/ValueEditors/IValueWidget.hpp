/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QWidget>

#include "PropertyTree.hpp"

#include "IModule.hpp"

namespace D2ModGen {

class IValueWidget : public QWidget {
    Q_OBJECT

public:
    enum class Control
    {
        Auto,
        SliderMinMax,
        Slider,
        CheckBox,
        LineEdit,
        Label,
    };

    struct Params {
        Control m_control = Control::Auto;
        int     m_min     = 0;
        int     m_max     = 0;
        double  m_mult    = 0.;
        double  m_denom   = 0.;
        bool    m_compact = true;
        QString m_title;
        QString m_help;
    };

public:
    explicit IValueWidget(QWidget* parent)
        : QWidget(parent)
    {}

    virtual PropertyTree getValue() const                    = 0;
    virtual void         setValue(const PropertyTree& value) = 0;

signals:
    void dataChanged();
};

}
