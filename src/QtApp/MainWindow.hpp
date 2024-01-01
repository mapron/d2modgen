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

namespace Mernel {
class PropertyTree;
}

namespace D2ModGen {

class IConfigPage;
class StorageCache;
class MainConfigPage;
class ConfigHandler;
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
    MainWindow(ConfigHandler& configHandler);
    ~MainWindow();

    void generate();

    bool saveConfig(const QString& filename) const;
    bool loadConfig(const QString& filename);
    bool loadConfig(const Mernel::PropertyTree& data);
    bool loadPresetConfig(const QString& filename);

    struct AppSettings {
        QString m_langId;
        QString m_themeId;
    };

    static AppSettings getAppSettings();

private:
    void pushUndo(const Mernel::PropertyTree& data);
    void pushUndoCurrent();
    void makeUndo();
    void updateUndoAction();

    void updateUIFromSettings();
    void updateUIFromSettings(IConfigPage* page, const Mernel::PropertyTree& currentConfig);
    void writeSettingsFromUI(IConfigPage* page);

private:
    QString                               m_defaultConfig;
    QList<IConfigPage*>                   m_pages;
    QMap<IConfigPage*, QList<QCheckBox*>> m_enableButtons;
    QLabel*                               m_status;
    MainConfigPage*                       m_mainPage;
    bool                                  m_autoSave = true;
    DelayedTimer*                         m_delayTimer;
    QList<Mernel::PropertyTree>           m_undo;
    QAction*                              m_undoAction;
    ConfigHandler&                        m_configHandler;
};

}
