/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IValueWidget.hpp"

class QSpinBox;
class QSlider;

namespace D2ModGen {

class HelpToolButton;

class SliderWidget : public IValueWidget {
public:
    SliderWidget(const Params& params,
                 QWidget*      parent);
    ~SliderWidget();

    void         setValue(const Mernel::PropertyTree& value) override;
    Mernel::PropertyTree getValue() const override;

private:
    void sliderToSpinbox();
    void spinboxToSlider();

private:
    const double    m_denom;
    const double    m_mult;
    const double    m_min;
    const double    m_max;
    QSpinBox*       m_valueBox;
    QSlider*        m_slider;
    HelpToolButton* m_helpButton;
    bool            m_settingValue = false;
};

class SliderWidgetMinMax : public IValueWidget {
public:
    SliderWidgetMinMax(const Params& params,
                       QWidget*      parent);
    ~SliderWidgetMinMax();

    void         setValue(const Mernel::PropertyTree& value) override;
    Mernel::PropertyTree getValue() const override;

private:
    void sliderToSpinbox();
    void spinboxToSlider();

private:
    const int       m_minValue;
    const int       m_maxValue;
    QSpinBox*       m_valueBox;
    QSlider*        m_slider;
    HelpToolButton* m_helpButton;
    bool            m_settingValue = false;
};

}
