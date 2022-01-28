/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IValueWidget.hpp"

class QCheckBox;

namespace D2ModGen {

class CheckboxWidget : public IValueWidget {
    Q_OBJECT
public:
    CheckboxWidget(const QString& caption,
                   const QString& id,
                   bool           def,
                   QWidget*       parent);
    ~CheckboxWidget();

    void resetValue() override;
    void setValue(int value) override;
    int  getValue() const override;
    bool isDefault() const override;

signals:
    void toggled(bool);

private:
    const bool m_default;
    QCheckBox* m_checkBox;
};

}
