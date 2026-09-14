/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "UIController.hpp"

#include "MernelPlatform/Logger_details.hpp"

#include "ConfigHandler.hpp"
#include "Logger.hpp"
#include "FileIOUtils.hpp"

#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QTranslator>
#include <QTextStream>
#include <QResource>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

namespace {

class RAIITranslator {
    std::unique_ptr<QTranslator> m_tr;

public:
    RAIITranslator(const QString& localeId)
    {
        if (localeId == "en_US")
            return;
        m_tr = std::make_unique<QTranslator>();

        auto res = QString(":/i18n/modgen_%1.qm").arg(localeId);
        if (!m_tr->load(res))
            qWarning() << "Failed to load " << res;

        if (!QApplication::installTranslator(m_tr.get()))
            qWarning() << "Failed to install translator for " << localeId;
    }
    ~RAIITranslator()
    {
        if (m_tr)
            QApplication::removeTranslator(m_tr.get());
    }
};

}

void customMessageOutput(QtMsgType type, const QMessageLogContext& input, const QString& msg)
{
    using namespace D2ModGen;
    Logger::LogLevel level = Logger::Debug;
    switch (type) {
        case QtDebugMsg:
            level = Logger::Debug;
            break;
        case QtInfoMsg:
            level = Logger::Info;
            break;
        case QtWarningMsg:
            level = Logger::Warning;
            break;
        case QtCriticalMsg:
            level = Logger::Err;
            break;
        case QtFatalMsg:
            level = Logger::Emerg;
            break;
    }
    Logger(level) << "QT:" << msg.toUtf8().constData();

    switch (type) {
        case QtFatalMsg:
            abort();
        default:
            break;
    }
}

int main(int argc, char* argv[])
{
    using namespace D2ModGen;

    qInstallMessageHandler(customMessageOutput);
    QApplication::setApplicationName("D2R mod generator");

    Logger() << "application started";
    QApplication      app(argc, argv);
    const auto        appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const std::string logDir  = (appData + "/applog").toStdString();

    {
        Logger::SetLoggerBackend(std::make_unique<Mernel::LoggerBackendFiles>(
            7,
            true,  /*duplicateInStderr*/
            true,  /*outputLoglevel   */
            true,  /*outputTimestamp  */
            false, /*outputTimeoffsets*/
            10,
            5000,
            string2path(logDir)));
        Logger() << "Started log redirection to:" << logDir;
    }
    ConfigHandler configHandler;
    configHandler.loadAppConfig();

    UIController   appui(configHandler);
    RAIITranslator trans(appui.getApp("langId", "en_US"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appui", &appui);
    for (auto& [key, value] : appui.getTabs())
        engine.rootContext()->setContextProperty(key, value);

    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));

    auto res = app.exec();
    Logger() << "closing app";
    return res;
}
