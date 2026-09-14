/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QVariant>

#include "ConfigHandler.hpp"

namespace D2ModGen {

class StorageCache;
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

class TabController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    TabController(ConfigHandler::ModuleData& module, DelayedTimer* delayTimer, QObject* parent);
    ~TabController();

    Q_INVOKABLE QVariant get(const QString& key) const;

    Q_INVOKABLE int     getInt(const QString& key) const;
    Q_INVOKABLE QString getStr(const QString& key) const;

    Q_INVOKABLE void set(const QString& key, const QVariant& val);

    Q_INVOKABLE bool getEnabled() const;
    Q_INVOKABLE void setEnabled(bool val);

    Q_INVOKABLE void activatePreset(int index);
    Q_INVOKABLE void resetToDefault();

    void sendDataChange();
    void sendEnabledChange();

    void enableChangeOnSet() { m_changeOnSet = true; }

signals:
    void dataChangedInternal();
    void dataChanged();
    void enabledChanged();

private:
    ConfigHandler::ModuleData& m_module;
    DelayedTimer* const        m_delayTimer;
    bool                       m_changeOnSet = false;
};

class UIController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString saveFolder READ getSaveFolder NOTIFY saveFolderChanged FINAL)
public:
    UIController(ConfigHandler& configHandler);
    ~UIController();

    std::map<QString, QObject*> getTabs() const { return m_tabs; }

    Q_INVOKABLE void generate();

    Q_INVOKABLE void saveConfig(QString filename);
    Q_INVOKABLE void loadConfig(QString filename);
    Q_INVOKABLE void clearConfig();

    bool             loadConfig(const Mernel::PropertyTree& data);
    Q_INVOKABLE void loadPresetConfig(const QString& filename);

    Q_INVOKABLE QString getApp(const QString& key, const QString& def) const;
    Q_INVOKABLE void    setApp(const QString& key, const QString& value);

    Q_INVOKABLE void makeUndo();
    Q_INVOKABLE void disableAutoSave();

    Q_INVOKABLE void browseToAppSettings();
    Q_INVOKABLE void newSeed();
    Q_INVOKABLE void detectPath();
    Q_INVOKABLE void setLaunch(QString arg);
    Q_INVOKABLE void makeShortCut(QString arg);

    Q_INVOKABLE void    copyModSettings();
    Q_INVOKABLE void    copySaves();
    Q_INVOKABLE QString getSaveFolder();

    void sendDataChange();

signals:
    void statusUpdate(QString status);
    void generateInternal();
    void saveFolderChanged();

private:
    void pushUndo(const Mernel::PropertyTree& data);
    void pushUndoCurrent();
    void updateUndoAction();
    void generateFinish();

private:
    bool                        m_autoSave = true;
    DelayedTimer*               m_delayTimer;
    QList<Mernel::PropertyTree> m_undo;
    ConfigHandler&              m_configHandler;
    std::map<QString, QObject*> m_tabs;
    std::vector<TabController*> m_tabsList;
};

}
