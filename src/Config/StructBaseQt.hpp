/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "StructBaseCpp.hpp"

#include <QObject>

namespace D2ModGen {

class BaseStruct_Qt : public QObject {
    Q_OBJECT
public:
    BaseStruct_Qt(CommonStruct& data)
        : QObject(nullptr)
        , m_data(data)
    {
        m_data.setCallbackForOuterChanges([this] { emit dataChanged(); });
    }

    QString   get(const std::string& prop) const { return QString::fromStdString(prop); }
    qlonglong get(int64_t prop) const { return static_cast<qlonglong>(prop); }
    bool      get(bool prop) const { return (prop); }

    void set(std::string& prop, const QString& valq)
    {
        auto val = valq.toStdString();
        if (prop == val)
            return;
        prop = val;

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }
    void set(int64_t& prop, qlonglong val)
    {
        if (prop == val)
            return;
        prop = val;

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }
    void set(bool& prop, bool val)
    {
        if (prop == val)
            return;
        prop = val;

        emit dataChanged(); // to sync other readers of this value in UI

        m_data.notifyChangeOuter(); // to notify BL
    }

signals:
    void dataChanged();

private:
    CommonStruct& m_data;
};

}
