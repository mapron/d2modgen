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

SliderWidget::SliderWidget(const QString& caption,
                           const QString& id,
                           double         denom,
                           double         mult,
                           int            defaultValue,
                           bool           compact,
                           QWidget*       parent)
    : IValueWidget(parent)
    , m_denom(denom)
    , m_mult(mult)
    , m_min(static_cast<int>(static_cast<double>(s_spinboxAverage) / denom))
    , m_max(static_cast<int>(static_cast<double>(s_spinboxAverage) * mult))
    , m_defaultValue(defaultValue)
{
    setObjectName(id);
    m_valueBox = new QSpinBox(this);
    m_valueBox->setButtonSymbols(QSpinBox::NoButtons);
    m_valueBox->setMinimum(m_min);
    m_valueBox->setMaximum(m_max);
    m_valueBox->setValue(m_defaultValue);
    m_valueBox->setMinimumWidth(50);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(1000);
    m_slider->setMaximumHeight(15);
    m_slider->setValue(s_sliderAverage);

    m_helpButton = new HelpToolButton("", this);
    m_helpButton->hide();

    QBoxLayout* mainLayout;
    if (compact)
        mainLayout = new QHBoxLayout(this);
    else
        mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* captionLayout = new QHBoxLayout();
    captionLayout->setMargin(0);
    captionLayout->addWidget(new QLabel(caption, this));
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

SliderWidget::SliderWidget(const QString& caption, const QString& id, double denom, double mult, QWidget* parent)
    : SliderWidget(caption, id, denom, mult, s_spinboxAverage, false, parent)
{
}

void SliderWidget::resetValue()
{
    m_settingValue = true;
    m_valueBox->setValue(m_defaultValue);
    m_settingValue = false;
}

void SliderWidget::setValue(const PropertyTree& value)
{
    m_settingValue = true;
    m_valueBox->setValue(value.toInt());
    m_settingValue = false;
}

PropertyTree SliderWidget::getValue() const
{
    return PropertyTreeScalar{ static_cast<int64_t>(m_valueBox->value()) };
}

bool SliderWidget::isDefault() const
{
    return getValue().getScalar() == PropertyTreeScalar{ m_defaultValue };
}

void SliderWidget::addHelp(const QString& helpToolTip)
{
    if (helpToolTip.isEmpty())
        return;
    m_helpButton->setToolTip(helpToolTip);
    m_helpButton->show();
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

SliderWidgetMinMax::SliderWidgetMinMax(const QString& caption,
                                       const QString& id,
                                       int            minValue,
                                       int            maxValue,
                                       int            defaultValue,
                                       bool           compact,
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

    m_helpButton = new HelpToolButton("", this);
    m_helpButton->hide();

    QBoxLayout* mainLayout;
    if (compact)
        mainLayout = new QHBoxLayout(this);
    else
        mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    QHBoxLayout* captionLayout = new QHBoxLayout();
    captionLayout->setMargin(0);
    captionLayout->addWidget(new QLabel(caption, this));
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

void SliderWidgetMinMax::resetValue()
{
    m_settingValue = true;
    m_valueBox->setValue(m_defaultValue);
    m_settingValue = false;
}

void SliderWidgetMinMax::setValue(const PropertyTree& value)
{
    m_settingValue = true;
    m_valueBox->setValue(value.toInt());
    m_settingValue = false;
}

PropertyTree SliderWidgetMinMax::getValue() const
{
    return PropertyTreeScalar{ static_cast<int64_t>(m_valueBox->value()) };
}

bool SliderWidgetMinMax::isDefault() const
{
    return getValue().getScalar() == PropertyTreeScalar{ m_defaultValue };
}

void SliderWidgetMinMax::addHelp(const QString& helpToolTip)
{
    if (helpToolTip.isEmpty())
        return;
    m_helpButton->setToolTip(helpToolTip);
    m_helpButton->show();
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
