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
    LineWidget(const Params& params,
               QWidget*      parent);
    ~LineWidget();

    void         setValue(const PropertyTree& value) override;
    PropertyTree getValue() const override;

private:
    QLineEdit*      m_lineEdit;
    HelpToolButton* m_helpButton;
};

}
