/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "LineWidget.hpp"

#include "HelpToolButton.hpp"

#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>

namespace D2ModGen {

LineWidget::LineWidget(const Params& params,
                       QWidget*      parent)
    : IValueWidget(parent)
{
    m_lineEdit = new QLineEdit(this);
    connect(m_lineEdit, &QLineEdit::textEdited, this, &IValueWidget::dataChanged);

    m_helpButton = new HelpToolButton(params.m_help, this);
    m_helpButton->hide();

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->addWidget(new QLabel(params.m_title, this));
    mainLayout->addWidget(m_lineEdit);
    mainLayout->addWidget(m_helpButton);
    mainLayout->addStretch();
}

LineWidget::~LineWidget() = default;

void LineWidget::setValue(const Mernel::PropertyTree& value)
{
    m_lineEdit->setText(QString::fromStdString(value.getScalar().toString()));
}

Mernel::PropertyTree LineWidget::getValue() const
{
    return Mernel::PropertyTreeScalar{ m_lineEdit->text().toStdString() };
}

}
