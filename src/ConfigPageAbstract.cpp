/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageAbstract.hpp"

#include <QVBoxLayout>
#include <QLabel>

void DropSet::readRow(const TableView::RowView& row)
{
    const QString& noDrop = row["NoDrop"];
    m_noDrop              = noDrop.toInt();

    m_items.clear();
    for (int i = 1; i <= 10; ++i) {
        const QString& prob = row[QString("Prob%1").arg(i)];
        if (prob.isEmpty())
            break;
        const QString& tcName = row[QString("Item%1").arg(i)];
        m_items << Item{ tcName, prob.toInt() };
    }
}

void DropSet::writeRow(TableView::RowView& row) const
{
    QString& noDrop = row["NoDrop"];
    noDrop          = QString("%1").arg(m_noDrop);
    for (int i = 1; i <= 10; ++i) {
        QString& prob   = row[QString("Prob%1").arg(i)];
        QString& tcName = row[QString("Item%1").arg(i)];
        prob.clear();
        tcName.clear();
        if (i - 1 >= m_items.size())
            continue;
        auto& item = m_items[i - 1];
        prob       = QString("%1").arg(item.prob);
        tcName     = item.tc;
    }
}

int DropSet::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : m_items)
        totalOther += item.prob;
    return totalOther;
}

ConfigPageAbstract::ConfigPageAbstract(QWidget* parent)
    : IConfigPage(parent)
{
    m_layout = new QVBoxLayout(this);
}

void ConfigPageAbstract::readSettings(const QJsonObject& data)
{
    for (QString key : m_editors.keys()) {
        auto* w = m_editors[key];
        if (data.contains(key))
            w->setValue(data[key].toInt());
        else
            w->resetValue();
    }
}

void ConfigPageAbstract::writeSettings(QJsonObject& data) const
{
    for (QString key : m_editors.keys()) {
        auto* w = m_editors[key];
        if (!w->isDefault())
            data[key] = w->getValue();
    }
}

JsonFileSet ConfigPageAbstract::extraFiles() const
{
    return {};
}

bool ConfigPageAbstract::isAllDefault() const
{
    for (QString key : m_editors.keys()) {
        auto* w = m_editors[key];
        if (!w->isDefault())
            return false;
    }
    return true;
}

bool ConfigPageAbstract::isAllDefault(const QStringList& keys) const
{
    for (const QString& key : keys) {
        auto* w = m_editors[key];
        if (!w->isDefault())
            return false;
    }
    return true;
}

void ConfigPageAbstract::addWidget(QWidget* w)
{
    m_layout->addWidget(w);
}

void ConfigPageAbstract::addEditors(QList<IValueWidget*> editors)
{
    for (IValueWidget* w : editors) {
        m_layout->addWidget(w);
        m_editors[w->objectName()] = w;
    }
}

void ConfigPageAbstract::addEditorsPlain(QList<IValueWidget*> editors)
{
    for (IValueWidget* w : editors)
        m_editors[w->objectName()] = w;
}

void ConfigPageAbstract::closeLayout()
{
    m_layout->addStretch();
}

int ConfigPageAbstract::getWidgetValue(const QString& id) const
{
    return m_editors[id]->getValue();
}

bool ConfigPageAbstract::isWidgetValueDefault(const QString& id) const
{
    return m_editors[id]->isDefault();
}
const QSet<QString> ConfigPageAbstract::s_ignoreMinMax{ "hit-skill", "charged" };
