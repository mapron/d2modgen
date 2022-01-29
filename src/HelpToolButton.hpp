/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QToolButton>
#include <QHelpEvent>
#include <QApplication>

namespace D2ModGen {

class HelpToolButton : public QToolButton {
public:
    HelpToolButton(const QString& toolTip, QWidget* parent)
        : QToolButton(parent)
    {
        setObjectName("helpTooltipButton");
        setToolTip(toolTip);
        setCursor(Qt::WhatsThisCursor);
        connect(this, &QToolButton::clicked, this, [this] {
            QHelpEvent* event = new QHelpEvent(QEvent::ToolTip,
                                               QPoint(this->pos().x(), this->pos().y()),
                                               QPoint(QCursor::pos().x(), QCursor::pos().y()));

            QApplication::postEvent(this, event);
        });
    }
};
}
