/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QMainWindow>
#include <QScopedPointer>
#include <QSet>
#include <QMap>

class QLabel;
class QCheckBox;

namespace D2ModGen {

class IConfigPage;
class StorageCache;
class MainConfigPage;
class PropertyTree;
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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(bool autoSave);
    ~MainWindow();

    void generate();

    bool saveConfig(const QString& filename) const;
    bool loadConfig(const QString& filename);
    bool loadConfig(const PropertyTree& data);

    struct AppSettings {
        QString m_langId;
        QString m_themeId;
    };

    static AppSettings getAppSettings();

private:
    void pushUndo(const PropertyTree& data);
    void pushUndoCurrent();
    void makeUndo();
    void updateUndoAction();

private:
    QString                        m_defaultConfig;
    QList<IConfigPage*>            m_pages;
    QMap<IConfigPage*, QCheckBox*> m_enableButtons;
    QLabel*                        m_status;
    MainConfigPage*                m_mainPage;
    QScopedPointer<StorageCache>   m_mainStorageCache;
    bool                           m_autoSave = true;
    DelayedTimer*                  m_delayTimer;
    QList<PropertyTree>            m_undo;
    QAction*                       m_undoAction;
};

}
