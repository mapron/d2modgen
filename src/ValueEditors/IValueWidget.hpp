/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QWidget>

namespace D2ModGen {

class IValueWidget : public QWidget {
public:
    explicit IValueWidget(QWidget* parent)
        : QWidget(parent)
    {}

    virtual int  getValue() const    = 0;
    virtual void setValue(int value) = 0;
    virtual void resetValue()        = 0;
    virtual bool isDefault() const   = 0;
};

}
