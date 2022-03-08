/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#define _USE_MATH_DEFINES
#include "SliderWidget.hpp"
#include "HelpToolButton.hpp"

#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>

namespace D2ModGen {

namespace {
constexpr int s_sliderAverage  = 500;
constexpr int s_spinboxAverage = 100;

// map [0..1] to [0..1] but with exponential curve
double expGrowth(double val)
{
    double ex = std::exp(val * 2);
    ex -= 1.;
    ex /= (M_E * M_E - 1);
    return ex;
}

double expGrowthRev(double val)
{
    val *= (M_E * M_E - 1);
    val += 1.;
    const double lg = std::log(val);
    return lg / 2;
}

}

SliderWidget::SliderWidget(const Params& params,
                           QWidget*      parent)
    : IValueWidget(parent)
    , m_denom(params.m_denom)
    , m_mult(params.m_mult)
    , m_min(static_cast<int>(static_cast<double>(s_spinboxAverage) / params.m_denom))
    , m_max(static_cast<int>(static_cast<double>(s_spinboxAverage) * params.m_mult))
{
    m_valueBox = new QSpinBox(this);
    m_valueBox->setButtonSymbols(QSpinBox::NoButtons);
    m_valueBox->setMinimum(m_min);
    m_valueBox->setMaximum(m_max);
    m_valueBox->setMinimumWidth(50);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(1000);
    m_slider->setMaximumHeight(15);
    m_slider->setValue(s_sliderAverage);

    m_helpButton = new HelpToolButton(params.m_help, this);

    QBoxLayout* mainLayout;
    if (params.m_compact)
        mainLayout = new QHBoxLayout(this);
    else
        mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* captionLayout = new QHBoxLayout();
    captionLayout->setMargin(0);
    captionLayout->addWidget(new QLabel(params.m_title, this));
    captionLayout->addWidget(m_helpButton);
    captionLayout->addStretch(0);
    mainLayout->addLayout(captionLayout);
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addWidget(m_slider);
    bottomLayout->addWidget(m_valueBox);
    bottomLayout->addWidget(new QLabel("%", this));

    connect(m_slider, &QSlider::valueChanged, this, &SliderWidget::sliderToSpinbox);
    connect(m_valueBox, qOverload<int>(&QSpinBox::valueChanged), this, &SliderWidget::spinboxToSlider);
}

SliderWidget::~SliderWidget() = default;

void SliderWidget::setValue(const PropertyTree& value)
{
    m_settingValue = true;
    m_valueBox->setValue(value.getScalar().toInt());
    m_settingValue = false;
}

PropertyTree SliderWidget::getValue() const
{
    return PropertyTreeScalar{ static_cast<int64_t>(m_valueBox->value()) };
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
        const int    highRatio = value - s_sliderAverage;
        const int    highRange = m_max - s_spinboxAverage;
        const double ratio     = double(highRatio) / sliderRange;
        const double ratioExp  = expGrowth(ratio);
        m_valueBox->setValue(s_spinboxAverage + static_cast<int>(ratioExp * highRange));
    }

    m_valueBox->blockSignals(false);
    if (!m_settingValue)
        emit dataChanged();
}

void SliderWidget::spinboxToSlider()
{
    m_slider->blockSignals(true);
    const int sliderRange = s_sliderAverage; // both high and low;
    const int value       = m_valueBox->value();
    if (value == s_spinboxAverage)
        m_slider->setValue(s_sliderAverage);
    else if (value < s_spinboxAverage) {
        const int lowRange = s_spinboxAverage - m_min;
        m_slider->setValue((value - m_min) * sliderRange / lowRange);
    } else {
        const int    highRatio = value - s_spinboxAverage;
        const int    highRange = m_max - s_spinboxAverage;
        const double ratio     = double(highRatio) / highRange;
        const double ratioLog  = expGrowthRev(ratio);
        m_slider->setValue(s_sliderAverage + (ratioLog * sliderRange));
    }
    m_slider->blockSignals(false);
    if (!m_settingValue)
        emit dataChanged();
}

//----------------------------------------------------------

SliderWidgetMinMax::SliderWidgetMinMax(const Params& params,
                                       QWidget*      parent)
    : IValueWidget(parent)
    , m_minValue(params.m_min)
    , m_maxValue(params.m_max)
{
    m_valueBox = new QSpinBox(this);
    m_valueBox->setButtonSymbols(QSpinBox::NoButtons);
    m_valueBox->setMinimum(m_minValue);
    m_valueBox->setMaximum(m_maxValue);
    m_valueBox->setMinimumWidth(50);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(m_minValue);
    m_slider->setMaximum(m_maxValue);
    m_slider->setMaximumHeight(15);

    m_helpButton = new HelpToolButton(params.m_help, this);

    QBoxLayout* mainLayout;
    if (params.m_compact)
        mainLayout = new QHBoxLayout(this);
    else
        mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* captionLayout = new QHBoxLayout();
    captionLayout->setMargin(0);
    captionLayout->addWidget(new QLabel(params.m_title, this));
    captionLayout->addWidget(m_helpButton);
    captionLayout->addStretch(0);
    mainLayout->addLayout(captionLayout);
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addLayout(bottomLayout);
    bottomLayout->addWidget(m_slider);
    bottomLayout->addWidget(m_valueBox);

    connect(m_slider, &QSlider::valueChanged, this, &SliderWidgetMinMax::sliderToSpinbox);
    connect(m_valueBox, qOverload<int>(&QSpinBox::valueChanged), this, &SliderWidgetMinMax::spinboxToSlider);
}

SliderWidgetMinMax::~SliderWidgetMinMax() = default;

void SliderWidgetMinMax::setValue(const PropertyTree& value)
{
    m_settingValue = true;
    m_valueBox->setValue(value.getScalar().toInt());
    m_settingValue = false;
}

PropertyTree SliderWidgetMinMax::getValue() const
{
    return PropertyTreeScalar{ static_cast<int64_t>(m_valueBox->value()) };
}

void SliderWidgetMinMax::sliderToSpinbox()
{
    m_valueBox->blockSignals(true);
    m_valueBox->setValue(m_slider->value());
    m_valueBox->blockSignals(false);
    if (!m_settingValue)
        emit dataChanged();
}

void SliderWidgetMinMax::spinboxToSlider()
{
    m_slider->blockSignals(true);
    m_slider->setValue(m_valueBox->value());
    m_slider->blockSignals(false);
    if (!m_settingValue)
        emit dataChanged();
}

}
