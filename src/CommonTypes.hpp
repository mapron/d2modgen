/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QStringList>
#include <QMap>
#include <QSet>
#include <QList>
#include <QWidget>
#include <QJsonObject>
#include <QRandomGenerator>

#include <vector>

struct TableRow {
    QStringList data;
};

struct Table {
    QString         id;
    QList<TableRow> rows;
    QStringList     columns;
};

struct TableSet {
    QMap<QString, Table> tables;
};

using KeySet = QSet<QString>;

class IConfigPage : public QWidget {
public:
    explicit IConfigPage(QWidget* parent)
        : QWidget(parent)
    {}

    virtual QString caption() const                                           = 0;
    virtual QString settingKey() const                                        = 0;
    virtual void    readSettings(const QJsonObject& data)                     = 0;
    virtual void    writeSettings(QJsonObject& data) const                    = 0;
    virtual KeySet  generate(TableSet& tableSet, QRandomGenerator& rng) const = 0;
};

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

class TableView {
public:
    TableView(Table& table)
        : m_table(table)
    {
        for (int i = 0; i < m_table.columns.size(); ++i)
            m_columnIndex[m_table.columns[i]] = i;
        for (int i = 0; i < m_table.rows.size(); ++i)
            m_rows.emplace_back(i, *this);
    }

    class RowView {
    public:
        RowView(int index, TableView& parent)
            : i(index)
            , parent(parent)
        {}

        QString& operator[](int index) { return parent.m_table.rows[i].data[index]; }
        QString& operator[](QString index) { return parent.m_table.rows[i].data[ind(index)]; }

        const QString& operator[](int index) const { return parent.m_table.rows[i].data[index]; }
        const QString& operator[](QString index) const { return parent.m_table.rows[i].data[ind(index)]; }

    private:
        int        ind(const QString& index) const { return parent.m_columnIndex.value(index, -1); }
        int        i;
        TableView& parent;
    };

    auto begin() { return m_rows.begin(); }
    auto end() { return m_rows.end(); }
    auto cbegin() const { return m_rows.begin(); }
    auto cend() const { return m_rows.end(); }
    auto begin() const { return m_rows.begin(); }
    auto end() const { return m_rows.end(); }

    bool hasColumn(const QString& name) const { return m_columnIndex.contains(name); }

private:
    Table&               m_table;
    std::vector<RowView> m_rows;
    QHash<QString, int>  m_columnIndex;
};

struct GenerationEnvironment {
    QString  modName;
    QString  d2rPath;
    QString  appData;
    QString  outPath;
    bool     exportAllTables;
    bool     isLegacy;
    uint32_t seed;
};
