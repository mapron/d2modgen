/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "CheckboxWidget.hpp"

#include <QCheckBox>
#include <QBoxLayout>

CheckboxWidget::CheckboxWidget(const QString& caption,
                               const QString& id,
                               bool           def,
                               QWidget*       parent)
    : IValueWidget(parent)
    , m_default(def)
{
    setObjectName(id);
    m_checkBox = new QCheckBox(caption, this);
    m_checkBox->setChecked(m_default);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->addWidget(m_checkBox);
}

CheckboxWidget::~CheckboxWidget() = default;

void CheckboxWidget::resetValue()
{
    m_checkBox->setChecked(m_default);
}

void CheckboxWidget::setValue(int value)
{
    m_checkBox->setChecked(!!value);
}

int CheckboxWidget::getValue() const
{
    return int(m_checkBox->isChecked());
}

bool CheckboxWidget::isDefault() const
{
    return m_checkBox->isChecked() == m_default;
}
