/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "UIController.hpp"

#include "MernelPlatform/Logger_details.hpp"
#include "MernelPlatform/AppLocations.hpp"

#include "ConfigHandler.hpp"
#include "Logger.hpp"
#include "FileIOUtils.hpp"
#include "VersionChecker.hpp"

#include "ConfigQt.hpp"

#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QTranslator>
#include <QTextStream>
#include <QResource>
#include <QTimer>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QValidator>

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
    QApplication app(argc, argv);
    const auto   appData = Mernel::AppLocations("D2R mod generator").getAppdataDir();
    const auto   logDir  = appData / "applog";

    {
        Logger::SetLoggerBackend(std::make_unique<Mernel::LoggerBackendFiles>(
            7,
            true,  /*duplicateInStderr*/
            true,  /*outputLoglevel   */
            true,  /*outputTimestamp  */
            false, /*outputTimeoffsets*/
            10,
            5000,
            logDir));
        Logger() << "Started log redirection to:" << path2string(logDir);
    }
    ConfigHandler configHandler;
    configHandler.loadAppConfig();

    UIController   appui(configHandler);
    RAIITranslator trans(appui.getApp("langId", "en_US"));

    VersionChecker versionChecker;

    qmlRegisterType<D2ModGen::BasicPathValidator>("CustomValidators", 1, 0, "BasicPathValidator");

    Config configRoot;
    configRoot.resetToDefaults();
    assert(configRoot.m_one.m_flag == true);
    assert(configRoot.m_two.m_weight == 42);
    Mernel::PropertyTree configTree;
    configTree["one"]["flag"]   = Mernel::PropertyTreeScalar(false);
    configTree["two"]["weight"] = Mernel::PropertyTreeScalar(110);
    configRoot.readFromProperty(configTree);

    assert(configRoot.m_one.m_flag == false);
    assert(configRoot.m_two.m_weight == 100);

    Config_Qt rootQt(configRoot);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("versionChecker", &versionChecker);
    engine.rootContext()->setContextProperty("appui", &appui);
    engine.rootContext()->setContextProperty("config", &rootQt);
    for (auto& [key, value] : appui.getTabs())
        engine.rootContext()->setContextProperty(key, value);

    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));

    QTimer::singleShot(5000, [&configRoot] {
        configRoot.m_one.m_leaf.m_mode = "5k";
        configRoot.m_two.m_weight      = 11;
        configRoot.notifyChangeInner();
    });

    configRoot.setCallbackForInnerChanges([] {
        qWarning() << "Config changed!";
    });

    auto res = app.exec();
    Logger() << "closing app";
    return res;
}
