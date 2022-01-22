/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPages.hpp"
#include "SliderWidget.hpp"
#include "CheckboxWidget.hpp"

#include <functional>

class QVBoxLayout;

struct DropSet {
    struct Item {
        QString tc;
        int     prob;
    };
    int         m_noDrop = 0;
    QList<Item> m_items;

    void readRow(const TableView::RowView& row);
    void writeRow(TableView::RowView& row) const;

    int getDropSomethingProb() const;
};

class ConfigPageAbstract : public IConfigPage {
public:
    ConfigPageAbstract(QWidget* parent);

    // IConfigPage interface
public:
    void        readSettings(const QJsonObject& data) override;
    void        writeSettings(QJsonObject& data) const override;
    JsonFileSet extraFiles() const override;
    bool        isAllDefault() const;
    bool        isAllDefault(const QStringList& keys) const;

protected:
    void addWidget(QWidget* w);
    void addEditors(QList<IValueWidget*> editors);
    void addEditorsPlain(QList<IValueWidget*> editors);
    void closeLayout();
    int  getWidgetValue(const QString& id) const;
    bool isWidgetValueDefault(const QString& id) const;

private:
    QMap<QString, IValueWidget*> m_editors;
    QVBoxLayout*                 m_layout;
};
