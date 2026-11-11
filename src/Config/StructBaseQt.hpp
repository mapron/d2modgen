/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "StructBaseCpp.hpp"

#include <QObject>

namespace D2ModGen {

class CommonStruct_Qt : public QObject {
    Q_OBJECT

public:
    CommonStruct_Qt(CommonStruct& data)
        : m_data(data)
    {}

    Q_INVOKABLE void resetToZero() { m_data.resetToZero(true); }
    Q_INVOKABLE void resetToRecommended() { m_data.resetToRecommended(true); }
    Q_INVOKABLE void activatePreset(QString index)
    {
        int presetIndex = CommonStruct::getPresetIndex(index.toStdString());
        if (presetIndex < 0)
            return;

        m_data.resetToRecommended(false);
        m_data.activatePreset(presetIndex, true);
    }

private:
    CommonStruct& m_data;
};

class PropertyBool_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool value READ getValue WRITE setValue NOTIFY dataChanged)

public:
    PropertyBool_Qt(CommonStruct::PropertyBool& data)
        : m_data(data)
    {
        m_data.setCallbackForOuterChanges([this] { emit dataChanged(); });
    }

    bool getValue() const { return m_data.get(); }

    void setValue(bool val)
    {
        if (m_data.get() == val)
            return;
        m_data.set(val);

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }

signals:
    void dataChanged();

private:
    CommonStruct::PropertyBool& m_data;
};

class PropertyInt_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(qlonglong value READ getValue WRITE setValue NOTIFY dataChanged)
    Q_PROPERTY(qlonglong min READ getMin CONSTANT)
    Q_PROPERTY(qlonglong max READ getMax CONSTANT)
    Q_PROPERTY(bool isPercent READ isPercent CONSTANT)
    Q_PROPERTY(bool hasLogScale READ hasLogScale CONSTANT)

public:
    PropertyInt_Qt(CommonStruct::PropertyInt& data)
        : m_data(data)
    {
        m_data.setCallbackForOuterChanges([this] { emit dataChanged(); });
    }

    qlonglong getValue() const { return m_data.get(); }

    void setValue(qlonglong val)
    {
        if (m_data.get() == val)
            return;
        m_data.set(val);

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }

    qlonglong getMin() const { return m_data.m_min; }
    qlonglong getMax() const { return m_data.m_max; }
    bool      isPercent() const { return m_data.m_isPercent; }
    bool      hasLogScale() const { return m_data.m_hasLogScale; }

signals:
    void dataChanged();

private:
    CommonStruct::PropertyInt& m_data;
};

class PropertyString_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY dataChanged)

public:
    PropertyString_Qt(CommonStruct::PropertyString& data)
        : m_data(data)
    {
        m_data.setCallbackForOuterChanges([this] { emit dataChanged(); });
    }

    QString getValue() const { return QString::fromStdString(m_data.get()); }

    void setValue(const QString& val)
    {
        if (getValue() == val)
            return;
        m_data.set(val.toStdString());

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }

signals:
    void dataChanged();

private:
    CommonStruct::PropertyString& m_data;
};

class ConstantPropertyString_Qt : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString value READ getValue CONSTANT)

public:
    ConstantPropertyString_Qt(CommonStruct::PropertyString& data)
        : m_data(data)
    {
    }

    QString getValue() const { return QString::fromStdString(m_data.get()); }

private:
    CommonStruct::PropertyString& m_data;
};

}
