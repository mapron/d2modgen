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
    connect(m_checkBox, &QCheckBox::toggled, this, &CheckboxWidget::toggled);
    connect(m_checkBox, &QCheckBox::clicked, this, &IValueWidget::dataChanged);

    m_helpButton = new HelpToolButton("", this);
    m_helpButton->hide();

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->addWidget(m_checkBox);
    mainLayout->addWidget(m_helpButton);
    mainLayout->addStretch();
}

CheckboxWidget::~CheckboxWidget() = default;

void CheckboxWidget::resetValue()
{
    m_checkBox->setChecked(m_default);
}

void CheckboxWidget::setValue(const PropertyTree& value)
{
    m_checkBox->setChecked(value.getScalar().toBool());
}

PropertyTree CheckboxWidget::getValue() const
{
    return PropertyTreeScalar(m_checkBox->isChecked());
}

bool CheckboxWidget::isDefault() const
{
    return m_checkBox->isChecked() == m_default;
}

void CheckboxWidget::addHelp(const QString& helpToolTip)
{
    if (helpToolTip.isEmpty())
        return;

    m_helpButton->setToolTip(helpToolTip);
    m_helpButton->show();
}

}
