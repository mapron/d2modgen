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
    CheckboxWidget(const Params& params,
                   QWidget*      parent);
    ~CheckboxWidget();

    void         setValue(const Mernel::PropertyTree& value) override;
    Mernel::PropertyTree getValue() const override;

signals:
    void toggled(bool);

private:
    QCheckBox*      m_checkBox;
    HelpToolButton* m_helpButton;
};

}
