/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace D2ModGen {

class VersionChecker : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString versionText READ getText NOTIFY versionTxtChanged)

public:
    explicit VersionChecker(QObject* parent = nullptr);

    QString getText() const { return m_versionTxt; }

    Q_INVOKABLE void checkForUpdates();

signals:
    void versionTxtChanged();

private:
    void onNetworkReply(QNetworkReply* reply, QNetworkReply::NetworkError error);

private:
    void updateTxt();
    void onError(QString err);

private:
    enum class State
    {
        AppStart,
        StartedCheck,
        Error,
        VersionDetected,
    };

    State m_state = State::AppStart;

    QNetworkAccessManager m_networkManager;

    QString m_latestVersion;
    QString m_currentVersion;
    QString m_error;

    QString m_versionTxt;
};

}
