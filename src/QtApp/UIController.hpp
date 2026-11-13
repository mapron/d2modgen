/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QUrl>
#include <QVariant>
#include <QValidator>

#include "ConfigHandler.hpp"

namespace D2ModGen {

class DelayedTimer : public QObject {
    Q_OBJECT
public:
    DelayedTimer(int thresholdMs, std::function<void()> onShot, QObject* parent);

    void start();
    void stop();

    void timerEvent(QTimerEvent* event);

private:
    const int             m_delay;
    int                   m_timerId = -1;
    std::function<void()> m_onShot;
};

class UIController : public QObject {
    Q_OBJECT
public:
    UIController(ConfigHandler& configHandler);
    ~UIController();

    Q_INVOKABLE void generate();

    Q_INVOKABLE void saveConfig(QString filename);
    Q_INVOKABLE void loadConfig(QString filename);
    Q_INVOKABLE void clearConfig();
    Q_INVOKABLE void resetToRecommended();

    Q_INVOKABLE void makeUndo();
    Q_INVOKABLE void disableAutoSave();

    Q_INVOKABLE void    browseToAppSettings();
    Q_INVOKABLE void    refreshAllSeeds();
    Q_INVOKABLE QString detectPath();
    Q_INVOKABLE void    setLaunch(QString arg);
    Q_INVOKABLE void    makeShortCut(QString arg);
    Q_INVOKABLE QString runnerPreset(QString userHome, bool legacy);

    Q_INVOKABLE void copyModSettings();
    Q_INVOKABLE void copySaves();

    Q_INVOKABLE QString urlToPath(const QUrl& url) const;
    Q_INVOKABLE QUrl    pathToUrl(const QString& path) const;

    Q_INVOKABLE qlonglong makeSeed();

    Q_INVOKABLE QString getSaveRoot(bool modSubfolder);
    QString             getBattleNetConfig();

signals:
    void statusUpdate(QString status);
    void generateInternal();

private:
    void pushUndo(const Mernel::PropertyTree& data);
    void pushUndoCurrent();
    void updateUndoAction();
    void generateFinish();
    void makeLinuxShortCut(const Mernel::std_path& exe, const QString& args);

private:
    bool                        m_autoSave = true;
    DelayedTimer*               m_delayTimer;
    QList<Mernel::PropertyTree> m_undo;
    ConfigHandler&              m_configHandler;
};

class BasicPathValidator : public QValidator {
    Q_OBJECT
    Q_PROPERTY(bool enabled MEMBER m_enabled)
public:
    explicit BasicPathValidator(QObject* parent = nullptr)
        : QValidator(parent)
    {}

    State validate(QString& input, int& pos) const override
    {
        if (!m_enabled)
            return Acceptable;

        if (input.length() < 5) {
            return Intermediate;
        }
        return Acceptable;
    }

private:
    bool m_enabled = true;
};

}
