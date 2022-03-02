/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include "ConfigHandler.hpp"

#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QTranslator>

namespace {
QFile*  g_logFile = nullptr;
QString stripDir(QString s)
{
    s = s.mid(s.lastIndexOf('\\') + 1);
    s = s.mid(s.lastIndexOf('/') + 1);
    return s;
}

class RAIITranslator {
    std::unique_ptr<QTranslator> m_tr;

public:
    RAIITranslator(const QString& localeId)
        : m_tr(std::make_unique<QTranslator>())
    {
        m_tr->load(QString(":/Translations/modgen_%1.qm").arg(localeId));
        QApplication::installTranslator(m_tr.get());
    }
    ~RAIITranslator()
    {
        QApplication::removeTranslator(m_tr.get());
    }
};

}

void customMessageOutput(QtMsgType type, const QMessageLogContext& input, const QString& msg)
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
    QString    formatted = QString("[%1] %2: %3\n").arg(time, typeStr, msg);
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

    D2ModGen::ConfigHandler configHandler;

    auto args = app.arguments();
    if (args.value(1) == "--generate") {
        QString file = args.value(2);
        if (!file.isEmpty())
            configHandler.loadConfig(file);
        configHandler.generate();
        return 0;
    }

    auto [langId, themeId] = D2ModGen::MainWindow::getAppSettings();
    qDebug() << "langId=" << langId;
    qDebug() << "themeId=" << themeId;

    {
        Q_INIT_RESOURCE(breeze);
        QFile file(QString(":/%1/stylesheet.qss").arg(themeId));
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        app.setStyleSheet(stream.readAll());
    }

    Q_INIT_RESOURCE(Translations);
    RAIITranslator trans(langId);

    D2ModGen::MainWindow w(configHandler);
    qDebug() << "main window created";
    w.show();
    auto res = app.exec();
    qDebug() << "closing app";
    g_logFile = nullptr;
    return res;
}
