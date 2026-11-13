/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "UIController.hpp"

#include "FileIOUtils.hpp"
#include "LinuxShortcut.hpp"

#include "ConfigHandler.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"
#include "MernelPlatform/ShellUtils.hpp"

#include <QDateTime>
#include <QCoreApplication>
#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QUrl>

#include <random>

namespace D2ModGen {

constexpr const bool s_isWindows =
#ifdef _WIN32
    true;
#else
    false;
#endif

namespace {

QString ensureTrailingSlash(QString value)
{
    auto str = QString::fromStdString(::D2ModGen::ensureTrailingSlash(value.toStdString()));
    str.replace('\\', '/');
    return str;
}

#ifdef _WIN32
std::string getInstallLocationFromRegistry(bool resurrected)
{
    static const QString base("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II");
    QSettings            set(base + (resurrected ? " Resurrected" : ""),
                             QSettings::Registry32Format);
    return ensureTrailingSlash(set.value("InstallLocation").toString()).toStdString();
}
#else
std::string getInstallLocationFromRegistry(bool resurrected)
{
    return {};
}
#endif

QString getUserHomeDefault()
{
    QString home = ensureTrailingSlash(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0));
    if (!s_isWindows) {
        // 2536520 = app id for D2R
        home += ".steam/steam/steamapps/compatdata/2536520/pfx/drive_c/users/steamuser/";
    }
    return home;
}

}

DelayedTimer::DelayedTimer(int thresholdMs, std::function<void()> onShot, QObject* parent)
    : m_delay(thresholdMs)
    , m_onShot(onShot)
{
}

void DelayedTimer::start()
{
    if (m_timerId != -1)
        killTimer(m_timerId);
    m_timerId = startTimer(m_delay);
}

void DelayedTimer::stop()
{
    if (m_timerId != -1)
        killTimer(m_timerId);
    m_timerId = -1;
}

void DelayedTimer::timerEvent(QTimerEvent*)
{
    m_onShot();
    killTimer(m_timerId);
    m_timerId = -1;
}

UIController::UIController(ConfigHandler& configHandler)
    : QObject(nullptr)
    , m_configHandler(configHandler)
{
    m_delayTimer = new DelayedTimer(1000, [this] { pushUndoCurrent(); }, this);

    m_configHandler.loadConfig(m_configHandler.m_defaultPath);

    m_configHandler.m_config.setCallbackForInnerChanges([this] {
        m_delayTimer->start();
    });

    auto& cfg = m_configHandler.m_appConfig;

    if (s_isWindows || cfg.m_userHome.empty())
        cfg.m_userHome = getUserHomeDefault().toStdString();

    cfg.m_installLocationD2R    = getInstallLocationFromRegistry(true);
    cfg.m_installLocationLegacy = getInstallLocationFromRegistry(false);

    pushUndoCurrent();
}

UIController::~UIController()
{
    m_configHandler.saveAppConfig();
    if (m_autoSave)
        m_configHandler.saveConfig(m_configHandler.m_defaultPath);
}

QString UIController::getSaveRoot(bool modSubfolder)
{
    auto& cfg  = m_configHandler.m_appConfig;
    auto  root = ensureTrailingSlash(cfg.m_userHome) + cfg.m_saveSubfolder;
    if (modSubfolder)
        root += "mods/" + cfg.m_modname + "/";
    return QString::fromStdString(root);
}

QString UIController::getBattleNetConfig()
{
    auto&         cfg    = m_configHandler.m_appConfig;
    const QString config = QString::fromStdString(ensureTrailingSlash(cfg.m_userHome) + cfg.m_bnetConfig);
    return QFileInfo::exists(config) ? config : "";
}

void UIController::generate()
{
    emit statusUpdate(tr("Start..."));
    QTimer::singleShot(30, this, &UIController::generateFinish); // todo: move this bs to another thread.
}

void UIController::saveConfig(QString filename)
{
    filename = QUrl(filename).toLocalFile();
    if (m_configHandler.saveConfig(filename.toStdString())) {
        emit statusUpdate(tr("Saved."));
    } else {
        emit statusUpdate(tr("Error when saving!"));
    }
}

void UIController::loadConfig(QString filename)
{
    filename          = QUrl(filename).toLocalFile();
    const auto result = m_configHandler.loadConfig(filename.toStdString());

    if (result) {
        emit statusUpdate(tr("Loaded."));
    } else {
        emit statusUpdate(tr("Error when loading!"));
    }
}

void UIController::clearConfig()
{
    m_configHandler.m_config.resetToZero(true);

    emit statusUpdate(tr("Config is cleared."));
}

void UIController::resetToRecommended()
{
    m_configHandler.m_config.resetToRecommended(true);

    emit statusUpdate(tr("Preset applied."));
}

void UIController::pushUndo(const Mernel::PropertyTree& data)
{
    m_undo << data;
    while (m_undo.size() > 50)
        m_undo.removeFirst();

    updateUndoAction();
}

void UIController::pushUndoCurrent()
{
    Mernel::PropertyTree data;
    m_configHandler.saveConfig(data);
    pushUndo(data);
}

void UIController::makeUndo()
{
    m_delayTimer->stop();
    if (m_undo.size() < 2)
        return;

    m_undo.removeLast();

    m_configHandler.loadConfig(m_undo.last());

    updateUndoAction();
}

void UIController::disableAutoSave()
{
    m_autoSave = false;
}

void UIController::browseToAppSettings()
{
    Mernel::showFolderInFileManager(m_configHandler.m_appData);
}

QString UIController::urlToPath(const QUrl& url) const
{
    return ensureTrailingSlash(url.toLocalFile());
}

QUrl UIController::pathToUrl(const QString& path) const
{
    return QUrl::fromLocalFile(path);
}

void UIController::refreshAllSeeds()
{
    m_configHandler.updateSeeds(true);
}

QString UIController::detectPath()
{
    auto& cfg         = m_configHandler.m_appConfig;
    bool  resurrected = cfg.m_versionMajor == 1;
    return QString::fromStdString(resurrected ? cfg.m_installLocationD2R : cfg.m_installLocationLegacy);
}

void UIController::updateUndoAction()
{
    // @todo: change enabled state
}

void UIController::generateFinish()
{
    GenerationEnvironment env;

    bool result = env.init(m_configHandler.m_appConfig) && m_configHandler.generate(env);

    if (!result) {
        emit statusUpdate(tr("Error: %1").arg(QString::fromStdString("generation failure")));
        return;
    }

    emit statusUpdate(tr("Mod '%1' is updated.")
                          .arg(QString::fromStdString(m_configHandler.m_appConfig.m_modname)));
}

void UIController::setLaunch(QString arg)
{
    const QString bnetPath = getBattleNetConfig();
    if (bnetPath.isEmpty()) {
        emit statusUpdate(tr("Failed to locate Battle.net.config"));
        return;
    }

    const auto           configPath = string2path(bnetPath.toStdString());
    Mernel::PropertyTree doc;
    std::string          buffer;
    if (!Mernel::readFileIntoBufferNoexcept(configPath, buffer) || !Mernel::readJsonFromBufferNoexcept(buffer, doc)) {
        emit statusUpdate(tr("Failed to read data from Battle.net.config"));
        return;
    }
    Logger() << "Updating: " << configPath;

    auto& valGames                      = doc["Games"].getMap();
    auto& valOsi                        = valGames["osi"];
    valOsi["AdditionalLaunchArguments"] = Mernel::PropertyTreeScalar(arg.toStdString());

    buffer.clear();
    writeJsonToBufferNoexcept(buffer, doc);
    if (!Mernel::writeFileFromBufferNoexcept(configPath, buffer)) {
        emit statusUpdate(tr("Failed to write data to Battle.net.config"));
        return;
    }

    emit statusUpdate(tr("Battle.net launch options updated"));
}

void UIController::makeShortCut(QString arg)
{
    GenerationEnvironment env;

    auto& cfg = m_configHandler.m_appConfig;
    if (!env.init(cfg))
        return;

    if (!s_isWindows) {
        makeLinuxShortCut(env.inputPath / (env.isLegacy ? "Diablo II.exe" : "D2R.exe"), arg);
        return;
    }

    const bool        legacy = env.isLegacy;
    auto              d2path = env.inputPath;
    const std::string desk   = cfg.m_userHome + "Desktop/";
    const std::string title  = "Diablo II - " + cfg.m_modname + " Mod";

    Logger() << "Short cut: " << desk << " -> " << d2path;

    if (!Mernel::createShortCut(desk, title, d2path, (legacy ? "Diablo II.exe" : "D2R.exe"), arg.toStdString())) {
        emit statusUpdate(tr("Failed to write shortcut"));
        return;
    }

    emit statusUpdate(tr("Shortcut created!"));
}

void UIController::makeLinuxShortCut(const Mernel::std_path& exe, const QString& args)
{
    auto&      cfg      = m_configHandler.m_appConfig;
    const auto location = [](QStandardPaths::StandardLocation type) {
        return string2path(QStandardPaths::writableLocation(type).toStdString());
    };

    LinuxShortcutRequest request;
    request.userHome       = string2path(cfg.m_userHome);
    request.exe            = exe;
    request.args           = args.toStdString();
    request.runnerTemplate = cfg.m_runnerTemplate;
    request.modName        = cfg.m_modname;
    request.appsDir        = location(QStandardPaths::ApplicationsLocation);
    request.desktopDir     = location(QStandardPaths::DesktopLocation);
    request.iconDir        = location(QStandardPaths::GenericDataLocation) / "d2modgen" / "icons";
    request.homeDir        = location(QStandardPaths::HomeLocation);
    request.pathEnv        = qEnvironmentVariable("PATH").toStdString();

    const LinuxShortcutResult result = makeLinuxShortcut(request);
    switch (result.status) {
        case LinuxShortcutResult::Status::NoPreset:
            emit statusUpdate(tr("No runner preset for this Wine prefix. Enter a runner template, or use \"Set launch to mod\"."));
            return;
        case LinuxShortcutResult::Status::WriteFailed:
            emit statusUpdate(tr("Failed to write shortcut"));
            return;
        case LinuxShortcutResult::Status::Written:
            break;
    }

    QStringList written;
    for (const auto& file : result.written) {
        Logger() << "Short cut: " << file;
        written << QString::fromStdString(path2string(file));
    }
    // GNOME only starts launchers on the Desktop once they are trusted.
    if (result.written.size() > 1)
        QProcess::startDetached("gio", { "set", written.last(), "metadata::trusted", "true" });

    if (result.runnerMissing)
        emit statusUpdate(tr("Shortcut created, but its runner was not found. Edit the runner template."));
    else
        emit statusUpdate(tr("Shortcut created: %1").arg(written.join(", ")));
}

QString UIController::runnerPreset(QString userHome, bool legacy)
{
    const auto home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
    return QString::fromStdString(linuxRunnerPreset(string2path(userHome.toStdString()), legacy, string2path(home)));
}

void UIController::copyModSettings()
{
    const QString saves = getSaveRoot(false);
    if (saves.isEmpty() || !QFileInfo::exists(saves))
        return;

    const QString modSaves = getSaveRoot(true);
    if (!QFileInfo::exists(modSaves))
        QDir().mkpath(modSaves);
    const QString name = "Settings.json";
    QFile::remove(modSaves + name);
    QFile::copy(saves + name, modSaves + name);
    emit statusUpdate(tr("Settings updated!"));
}

void UIController::copySaves()
{
    const QString saves = getSaveRoot(false);
    if (saves.isEmpty() || !QFileInfo::exists(saves))
        return;

    const QString modSaves = getSaveRoot(true);
    if (!QFileInfo::exists(modSaves))
        QDir().mkpath(modSaves);

    auto files = QDir(saves).entryInfoList(QStringList() << "*.d2s");
    for (auto& file : files) {
        QFile::copy(saves + file.fileName(), modSaves + file.fileName());
    }

    emit statusUpdate(tr("Save files copied (%1)").arg(files.size()));
}

qlonglong UIController::makeSeed()
{
    std::random_device dev;

    return dev();
}

}
