/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QSet>
#include <QMap>
#include <QVariant>

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

class UIController : public QObject {
    Q_OBJECT
public:
    UIController(ConfigHandler& configHandler);
    ~UIController();

    Q_INVOKABLE void generate();

    Q_INVOKABLE void saveConfig(QString filename);
    Q_INVOKABLE void loadConfig(QString filename);
    Q_INVOKABLE void clearConfig();

    bool loadConfig(const Mernel::PropertyTree& data);
    bool loadPresetConfig(const QString& filename);

    Q_INVOKABLE QVariant get(const QString& context, const QString& key) const;

    Q_INVOKABLE int getInt(const QString& context, const QString& key) const;

    Q_INVOKABLE void set(const QString& context, const QString& key, const QVariant& val);

    Q_INVOKABLE QVariant getEnabled(const QString& context) const;
    Q_INVOKABLE void     setEnabled(const QString& context, bool val);

    Q_INVOKABLE QString getApp(const QString& key, const QString& def) const;
    Q_INVOKABLE void    setApp(const QString& key, const QString& value);

    Q_INVOKABLE void activatePreset(const QString& context, int index);
    Q_INVOKABLE void resetToDefault(const QString& context);

    Q_INVOKABLE void makeUndo();
    Q_INVOKABLE void disableAutoSave();

    Q_INVOKABLE void browseToAppSettings();

signals:
    void dataChanged();
    void dataChangedInternal();
    void statusUpdate(QString status);
    void generateInternal();

private:
    void pushUndo(const Mernel::PropertyTree& data);
    void pushUndoCurrent();
    void updateUndoAction();
    void generateFinish();

private:
    QList<IConfigPage*>                   m_pages;
    QMap<IConfigPage*, QList<QCheckBox*>> m_enableButtons;
    QLabel*                               m_status;
    MainConfigPage*                       m_mainPage;
    bool                                  m_autoSave = true;
    DelayedTimer*                         m_delayTimer;
    QList<Mernel::PropertyTree>           m_undo;
    ConfigHandler&                        m_configHandler;
};

}
