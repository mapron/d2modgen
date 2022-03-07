/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IValueWidget.hpp"

class QLineEdit;

namespace D2ModGen {

class HelpToolButton;
class LineWidget : public IValueWidget {
    Q_OBJECT
public:
    LineWidget(const QString& caption,
               const QString& id,
               QWidget*       parent);
    ~LineWidget();

    void         setValue(const PropertyTree& value) override;
    PropertyTree getValue() const override;

    void addHelp(const QString& helpToolTip);

private:
    QLineEdit*      m_lineEdit;
    HelpToolButton* m_helpButton;
};

static inline LineWidget* addHelp(LineWidget* cb, const QString& helpToolTip)
{
    cb->addHelp(helpToolTip);
    return cb;
}

}
