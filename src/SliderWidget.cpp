/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "SliderWidget.hpp"

#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>

namespace {
constexpr int s_sliderAverage  = 500;
constexpr int s_spinboxAverage = 100;
}

SliderWidget::SliderWidget(const QString& caption,
                           const QString& id,
                           double         denom,
                           double         mult,
                           QWidget*       parent)
    : IValueWidget(parent)
    , m_denom(denom)
    , m_mult(mult)
    , m_min(static_cast<int>(static_cast<double>(s_spinboxAverage) / denom))
    , m_max(static_cast<int>(static_cast<double>(s_spinboxAverage) * mult))
{
    setObjectName(id);
    m_valueBox = new QSpinBox(this);
    m_valueBox->setButtonSymbols(QSpinBox::NoButtons);
    m_valueBox->setMinimum(m_min);
    m_valueBox->setMaximum(m_max);
    m_valueBox->setValue(s_spinboxAverage);
    m_valueBox->setMinimumWidth(50);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(1000);
    m_slider->setMaximumHeight(15);
    m_slider->setValue(s_sliderAverage);

    QVBoxLayout* mainLayout   = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addWidget(new QLabel(caption, this));
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addWidget(m_slider);
    bottomLayout->addWidget(m_valueBox);

    connect(m_slider, &QSlider::valueChanged, this, &SliderWidget::sliderToSpinbox);
    connect(m_valueBox, qOverload<int>(&QSpinBox::valueChanged), this, &SliderWidget::spinboxToSlider);
}

SliderWidget::~SliderWidget() = default;

void SliderWidget::resetValue()
{
    m_valueBox->setValue(s_spinboxAverage);
}

void SliderWidget::setValue(int value)
{
    m_valueBox->setValue(value);
}

int SliderWidget::getValue() const
{
    return m_valueBox->value();
}

bool SliderWidget::isDefault() const
{
    return getValue() == s_spinboxAverage;
}

void SliderWidget::sliderToSpinbox()
{
    const int sliderRange = s_sliderAverage; // both high and low;
    m_valueBox->blockSignals(true);
    const int value = m_slider->value();
    if (value == s_sliderAverage)
        m_valueBox->setValue(s_spinboxAverage);
    else if (value < s_sliderAverage) {
        const int lowRange = s_spinboxAverage - m_min;
        m_valueBox->setValue(m_min + (value * lowRange / sliderRange));
    } else {
        const int highRatio = value - s_sliderAverage;
        const int highRange = m_max - s_spinboxAverage;
        m_valueBox->setValue(s_spinboxAverage + (highRatio * highRange / sliderRange));
    }

    m_valueBox->blockSignals(false);
}

void SliderWidget::spinboxToSlider()
{
    const int sliderRange = s_sliderAverage; // both high and low;
    const int value       = m_valueBox->value();
    if (value == s_spinboxAverage)
        m_slider->setValue(s_sliderAverage);
    else if (value < s_spinboxAverage) {
        const int lowRange = s_spinboxAverage - m_min;
        m_slider->setValue((value - m_min) * sliderRange / lowRange);
    } else {
        const int highRatio = value - s_spinboxAverage;
        const int highRange = m_max - s_spinboxAverage;
        m_slider->setValue(s_sliderAverage + (highRatio * sliderRange / highRange));
    }
}

//----------------------------------------------------------

SliderWidgetMinMax::SliderWidgetMinMax(const QString& caption,
                                       const QString& id,
                                       int            minValue,
                                       int            maxValue,
                                       int            defaultValue,
                                       QWidget*       parent)
    : IValueWidget(parent)
    , m_minValue(minValue)
    , m_maxValue(maxValue)
    , m_defaultValue(defaultValue)
{
    setObjectName(id);
    m_valueBox = new QSpinBox(this);
    m_valueBox->setButtonSymbols(QSpinBox::NoButtons);
    m_valueBox->setMinimum(m_minValue);
    m_valueBox->setMaximum(m_maxValue);
    m_valueBox->setValue(m_defaultValue);
    m_valueBox->setMinimumWidth(50);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(m_minValue);
    m_slider->setMaximum(m_maxValue);
    m_slider->setMaximumHeight(15);
    m_slider->setValue(m_defaultValue);

    QVBoxLayout* mainLayout   = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addWidget(new QLabel(caption, this));
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addWidget(m_slider);
    bottomLayout->addWidget(m_valueBox);

    connect(m_slider, &QSlider::valueChanged, this, &SliderWidgetMinMax::sliderToSpinbox);
    connect(m_valueBox, qOverload<int>(&QSpinBox::valueChanged), this, &SliderWidgetMinMax::spinboxToSlider);
}

SliderWidgetMinMax::~SliderWidgetMinMax() = default;

void SliderWidgetMinMax::resetValue()
{
    m_valueBox->setValue(m_defaultValue);
}

void SliderWidgetMinMax::setValue(int value)
{
    m_valueBox->setValue(value);
}

int SliderWidgetMinMax::getValue() const
{
    return m_valueBox->value();
}

bool SliderWidgetMinMax::isDefault() const
{
    return getValue() == m_defaultValue;
}

void SliderWidgetMinMax::sliderToSpinbox()
{
    m_valueBox->blockSignals(true);
    m_valueBox->setValue(m_slider->value());
    m_valueBox->blockSignals(false);
}

void SliderWidgetMinMax::spinboxToSlider()
{
    m_slider->setValue(m_valueBox->value());
}
