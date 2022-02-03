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
    SliderWidget(const QString& caption,
                 const QString& id,
                 double         denom,
                 double         mult,
                 int            defaultValue,
                 QWidget*       parent);
    SliderWidget(const QString& caption,
                 const QString& id,
                 double         denom,
                 double         mult,
                 QWidget*       parent);
    ~SliderWidget();

    void resetValue() override;
    void setValue(int value) override;
    int  getValue() const override;
    bool isDefault() const override;

    void addHelp(const QString& helpToolTip);

private:
    void sliderToSpinbox();
    void spinboxToSlider();

private:
    const double    m_denom;
    const double    m_mult;
    const double    m_min;
    const double    m_max;
    const int       m_default;
    QSpinBox*       m_valueBox;
    QSlider*        m_slider;
    HelpToolButton* m_helpButton;
};

class SliderWidgetMinMax : public IValueWidget {
public:
    SliderWidgetMinMax(const QString& caption,
                       const QString& id,
                       int            minValue,
                       int            maxValue,
                       int            defaultValue,
                       bool           compact,
                       QWidget*       parent);
    SliderWidgetMinMax(const QString& caption,
                       const QString& id,
                       int            minValue,
                       int            maxValue,
                       int            defaultValue,
                       QWidget*       parent)
        : SliderWidgetMinMax(caption,
                             id,
                             minValue,
                             maxValue,
                             defaultValue,
                             false,
                             parent)
    {}
    ~SliderWidgetMinMax();

    void resetValue() override;
    void setValue(int value) override;
    int  getValue() const override;
    bool isDefault() const override;

    void addHelp(const QString& helpToolTip);

private:
    void sliderToSpinbox();
    void spinboxToSlider();

private:
    const int       m_minValue;
    const int       m_maxValue;
    const int       m_defaultValue;
    QSpinBox*       m_valueBox;
    QSlider*        m_slider;
    HelpToolButton* m_helpButton;
};

static inline SliderWidget* addHelp(SliderWidget* slider, const QString& helpToolTip)
{
    slider->addHelp(helpToolTip);
    return slider;
}

static inline SliderWidgetMinMax* addHelp(SliderWidgetMinMax* slider, const QString& helpToolTip)
{
    slider->addHelp(helpToolTip);
    return slider;
}

}
