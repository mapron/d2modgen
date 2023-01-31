/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "CheckboxWidget.hpp"

#include "HelpToolButton.hpp"

#include <QCheckBox>
#include <QBoxLayout>

namespace D2ModGen {

CheckboxWidget::CheckboxWidget(const Params& params,
                               QWidget*      parent)
    : IValueWidget(parent)
{
    m_checkBox = new QCheckBox(params.m_title, this);
    connect(m_checkBox, &QCheckBox::toggled, this, &CheckboxWidget::toggled);
    connect(m_checkBox, &QCheckBox::clicked, this, &IValueWidget::dataChanged);

    m_helpButton = new HelpToolButton(params.m_help, this);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->addWidget(m_checkBox);
    mainLayout->addWidget(m_helpButton);
    mainLayout->addStretch();
}

CheckboxWidget::~CheckboxWidget() = default;

void CheckboxWidget::setValue(const Mernel::PropertyTree& value)
{
    m_checkBox->setChecked(value.getScalar().toBool());
}

Mernel::PropertyTree CheckboxWidget::getValue() const
{
    return Mernel::PropertyTreeScalar(m_checkBox->isChecked());
}

}
