/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QWidget>

#include "PropertyTree.hpp"

namespace D2ModGen {

class IValueWidget : public QWidget {
    Q_OBJECT
public:
    explicit IValueWidget(QWidget* parent)
        : QWidget(parent)
    {}

    virtual PropertyTree getValue() const                    = 0;
    virtual void         setValue(const PropertyTree& value) = 0;

signals:
    void dataChanged();
};

}
