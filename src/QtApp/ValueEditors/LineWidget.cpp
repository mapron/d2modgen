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

LineWidget::LineWidget(const QString& caption,
                       const QString& id,
                       const QString& def,
                       QWidget*       parent)
    : IValueWidget(parent)
    , m_default(def)
{
    setObjectName(id);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setText(m_default);
    connect(m_lineEdit, &QLineEdit::textEdited, this, &IValueWidget::dataChanged);

    m_helpButton = new HelpToolButton("", this);
    m_helpButton->hide();

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->addWidget(new QLabel(caption, this));
    mainLayout->addWidget(m_lineEdit);
    mainLayout->addWidget(m_helpButton);
    mainLayout->addStretch();
}

LineWidget::~LineWidget() = default;

void LineWidget::resetValue()
{
    m_lineEdit->setText(m_default);
}

void LineWidget::setValue(const PropertyTree& value)
{
    m_lineEdit->setText(QString::fromStdString(value.getScalar().toString()));
}

PropertyTree LineWidget::getValue() const
{
    return PropertyTreeScalar{ m_lineEdit->text().toStdString() };
}

bool LineWidget::isDefault() const
{
    return m_lineEdit->text() == m_default;
}

void LineWidget::addHelp(const QString& helpToolTip)
{
    if (helpToolTip.isEmpty())
        return;

    m_helpButton->setToolTip(helpToolTip);
    m_helpButton->show();
}

}
