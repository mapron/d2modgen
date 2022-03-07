/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IValueWidget.hpp"

class QCheckBox;

namespace D2ModGen {

class HelpToolButton;
class CheckboxWidget : public IValueWidget {
    Q_OBJECT
public:
    CheckboxWidget(const QString& caption,
                   const QString& id,
                   QWidget*       parent);
    ~CheckboxWidget();

    void         setValue(const PropertyTree& value) override;
    PropertyTree getValue() const override;

    void addHelp(const QString& helpToolTip);

signals:
    void toggled(bool);

private:
    QCheckBox*      m_checkBox;
    HelpToolButton* m_helpButton;
};

static inline CheckboxWidget* addHelp(CheckboxWidget* cb, const QString& helpToolTip)
{
    cb->addHelp(helpToolTip);
    return cb;
}

}
