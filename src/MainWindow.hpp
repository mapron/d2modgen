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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(bool autoSave);
    ~MainWindow();

    void generate();

    bool saveConfig(const QString& filename) const;
    bool loadConfig(const QString& filename);
    bool loadConfig(const QJsonObject& data);

private:
    QString                        m_defaultConfig;
    QList<IConfigPage*>            m_pages;
    QMap<IConfigPage*, QCheckBox*> m_enableButtons;
    QLabel*                        m_status;
    MainConfigPage*                m_mainPage;
    QScopedPointer<StorageCache>   m_mainStorageCache;
    bool                           m_autoSave = true;
};

}
