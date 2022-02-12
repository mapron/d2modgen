/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QWidget>

namespace D2ModGen {

class IValueWidget : public QWidget {
    Q_OBJECT
public:
    explicit IValueWidget(QWidget* parent)
        : QWidget(parent)
    {}

    virtual QVariant getValue() const         = 0;
    virtual void     setValue(QVariant value) = 0;
    virtual void     resetValue()             = 0;
    virtual bool     isDefault() const        = 0;

signals:
    void dataChanged();
};

}
