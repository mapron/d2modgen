/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include "ConfigHandler.hpp"
#include "Logger_details.hpp"
#include "PlatformPathUtils.hpp"

#include <QApplication>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QTranslator>
#include <QTextStream>
#include <QResource>

namespace {

bool setEnvVariable(const std::string& varName, const std::string& value)
{
#ifdef _WIN32
    return _putenv_s(varName.c_str(), value.c_str()) != 0;
#else
    return setenv(varName.c_str(), value.c_str(), 1) != 0;
#endif
}

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
    const auto        appData = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    const std::string logFile = (appData + "/applog.txt").toStdString();

    if (createDirectoriesForFile(logFile)) {
        Logger::SetLoggerBackend(std::make_unique<LoggerBackendFiles>(
            7,
            true,  /*duplicateInStderr*/
            true,  /*outputLoglevel   */
            true,  /*outputTimestamp  */
            false, /*outputTimeoffsets*/
            string2path(logFile)));
        Logger() << "Started log redirection to:" << logFile;
    }
    //qWarning() << "test qwarn";
    auto          exeRoot = getExecutableRootFolder();
    ConfigHandler configHandler(exeRoot + "/plugins");

    auto args = app.arguments();
    if (args.value(1) == "--generate") {
        QString file = args.value(2);
        if (!file.isEmpty())
            configHandler.loadConfig(file.toStdString());
        configHandler.generate();
        return 0;
    }

    auto [langId, themeId] = D2ModGen::MainWindow::getAppSettings();
    Logger() << "langId=" << langId.toStdString();
    Logger() << "themeId=" << themeId.toStdString();

    const bool isDark = themeId == "dark";
    setEnvVariable("QT_QUICK_CONTROLS_CONF", exeRoot + "/theme/" + (isDark ? "dark.conf" : "light.conf"));

    {
        QStringList qrc{ "Translations", "breeze" };
        QString     binDir = QApplication::applicationDirPath();
        for (QString qrcName : qrc) {
            [[maybe_unused]] bool registered = QResource::registerResource(QString("%1/assetsCompiled/%2.rcc").arg(binDir).arg(qrcName));
            assert(registered);
        }

        //Q_INIT_RESOURCE(breeze);
        QFile file(QString(":/%1/stylesheet.qss").arg(themeId));
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        app.setStyleSheet(stream.readAll());
    }

    //Q_INIT_RESOURCE(Translations);
    RAIITranslator trans(langId);

    D2ModGen::MainWindow w(configHandler);
    Logger() << "main window created";
    w.show();
    auto res = app.exec();
    Logger() << "closing app";
    return res;
}
