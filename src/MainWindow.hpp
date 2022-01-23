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
class IConfigPage;
struct GenOutput;
struct GenerationEnvironment;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    void generate(const GenerationEnvironment& env);

    bool saveConfig(const QString& filename) const;
    bool loadConfig(const QString& filename);
    bool loadConfig(const QJsonObject& data);

private:
    QString                        m_defaultConfig;
    QList<IConfigPage*>            m_pages;
    QMap<IConfigPage*, QCheckBox*> m_enableButtons;
    QLabel*                        m_status;
    QScopedPointer<GenOutput>      m_outputCache;
    QSet<QString>                  m_cachedFilenames;
};
