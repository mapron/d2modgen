/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>

namespace {
QFile*  g_logFile = nullptr;
QString stripDir(QString s)
{
    s = s.mid(s.lastIndexOf('\\') + 1);
    s = s.mid(s.lastIndexOf('/') + 1);
    return s;
}
}

void customMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString typeStr;
    switch (type) {
        case QtDebugMsg:
            typeStr = "Debug";
            break;
        case QtInfoMsg:
            typeStr = "Info";
            break;
        case QtWarningMsg:
            typeStr = "Warning";
            break;
        case QtCriticalMsg:
            typeStr = "Critical";
            break;
        case QtFatalMsg:
            typeStr = "Fatal";
    }
    auto       time      = QTime::currentTime().toString("HH:mm:ss.zzz");
    QString    formatted = QString("[%1] %2: %3 (%4:%5)\n").arg(time, typeStr, msg, stripDir(context.file)).arg(context.line);
    QByteArray localMsg  = formatted.toUtf8();
    fprintf(stderr, "%s", localMsg.constData());
    if (g_logFile) {
        g_logFile->write(localMsg);
        g_logFile->flush();
    }

    switch (type) {
        case QtFatalMsg:
            abort();
        default:
            break;
    }
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(customMessageOutput);
    QApplication::setApplicationName("D2R mod generator");

    qDebug() << "application started";
    QApplication app(argc, argv);
    QFile        file(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/applog.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    g_logFile = &file;
    
    auto args = app.arguments();
    if (args.value(1) == "--generate") {
        D2ModGen::MainWindow w(false);
        QString file = args.value(2);
        if (!file.isEmpty())
            w.loadConfig(file);
        w.generate();
        return 0;
    }

    D2ModGen::MainWindow w(true);
    qDebug() << "main window created";
    w.show();
    auto res = app.exec();
    qDebug() << "closing app";
    g_logFile = nullptr;
    return res;
}
