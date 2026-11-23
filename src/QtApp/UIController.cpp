/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "UIController.hpp"

#include "FileIOUtils.hpp"
#include "GameDetection.hpp"
#include "Shortcut.hpp"

#include "ConfigHandler.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"
#include "MernelPlatform/ShellUtils.hpp"

#include <QDateTime>
#include <QCoreApplication>
#include <QTimer>
#include <QProcess>
#include <QDir>
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

QStringList s_noticeLogs;

QString ensureTrailingSlash(QString value)
{
    auto str = QString::fromStdString(::D2ModGen::ensureTrailingSlash(value.toStdString()));
    str.replace('\\', '/');
    return str;
}

QString getUserHomeDefault()
{
    QString home = ensureTrailingSlash(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0));
    if (!s_isWindows) {
        // 2536520 = app id for D2R
        home += ".steam/steam/steamapps/compatdata/2536520/pfx/drive_c/users/steamuser/";
    }
    return home;
}

bool sameDir(const Mernel::std_path& l, const Mernel::std_path& r)
{
    return (l / "").lexically_normal() == (r / "").lexically_normal();
}

void setGamePaths(AppConfig& cfg, const GameMatch& match)
{
    cfg.m_userHome  = path2string(match.userHome);
    cfg.m_inputPath = ::D2ModGen::ensureTrailingSlash(path2string(match.installPath));
}

std::vector<GameMatch> findGamesFor(const AppConfig& cfg)
{
    const auto kind = cfg.m_versionMajor == 1 ? GameKind::Resurrected : GameKind::Legacy;
    const auto home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
    return findGames(kind, string2path(home), qEnvironmentVariable("WINEPREFIX").toStdString());
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

    cfg.m_installLocationD2R    = ensureTrailingSlash(QString::fromStdString(getInstallLocationFromRegistry(GameKind::Resurrected))).toStdString();
    cfg.m_installLocationLegacy = ensureTrailingSlash(QString::fromStdString(getInstallLocationFromRegistry(GameKind::Legacy))).toStdString();

    // out contract is to always have at least 1 record in m_undo. We can undo when we have 2 and more.
    pushUndoCurrent();
}

UIController::~UIController()
{
    if (m_autoSave) {
        m_configHandler.saveAppConfig();
        m_configHandler.saveConfig(m_configHandler.m_defaultPath);
    }
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

void UIController::logNotice(const QString& message)
{
    s_noticeLogs << message;
}

void UIController::generate()
{
    s_noticeLogs.clear();

    GenerationEnvironment env;

    std::string crc;
    bool        result = env.init(m_configHandler.m_appConfig) && m_configHandler.generate(env, crc);

    if (!result) {
        emit statusUpdate(tr("Error: %1").arg(QString::fromStdString("generation failure")));
        emit showErrorDialog(s_noticeLogs.last(), s_noticeLogs.join("\n"));
        return;
    }

    emit statusUpdate(tr("Mod '%1' is updated. CRC: %2")
                          .arg(QString::fromStdString(m_configHandler.m_appConfig.m_modname))
                          .arg(QString::fromStdString(crc)));
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
    if (s_isWindows)
        return QString::fromStdString(resurrected ? cfg.m_installLocationD2R : cfg.m_installLocationLegacy);

    m_matches = findGamesFor(cfg);
    if (m_matches.empty()) {
        emit statusUpdate(tr("No Wine prefix with Diablo II found. See the log for where it looked."));
        return {};
    }
    if (m_matches.size() == 1) {
        applyMatch(0);
        return {};
    }

    int          preselected = 0;
    QVariantList matches;
    for (size_t i = 0; i < m_matches.size(); ++i) {
        const auto& match = m_matches[i];
        matches << QVariantMap{
            { "prefix", QString::fromStdString(path2string(match.prefix)) },
            { "installPath", QString::fromStdString(path2string(match.installPath)) },
            { "hasSavedGames", match.hasSavedGames },
            { "hasBattleNetConfig", match.hasBattleNetConfig },
        };
        if (sameDir(match.userHome, string2path(cfg.m_userHome)))
            preselected = static_cast<int>(i);
    }
    emit prefixChoice(matches, preselected);
    return {};
}

void UIController::applyMatch(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= m_matches.size())
        return;

    const auto& match = m_matches[index];
    auto&       cfg   = m_configHandler.m_appConfig;
    setGamePaths(cfg, match);
    cfg.notifyChangeInner();
    emit statusUpdate(tr("Found Diablo II in %1").arg(QString::fromStdString(path2string(match.prefix))));
}

void UIController::updateUndoAction()
{
    emit undoEnabled(m_undo.size() >= 2);
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
        const auto location = [](QStandardPaths::StandardLocation type) {
            return string2path(QStandardPaths::writableLocation(type).toStdString());
        };

        ShortcutRequest request;
        request.userHome       = string2path(cfg.m_userHome);
        request.exe            = env.inputPath / (env.isLegacy ? "Diablo II.exe" : "D2R.exe");
        request.args           = arg.toStdString();
        request.runnerTemplate = cfg.m_runnerTemplate;
        request.modName        = cfg.m_modname;
        request.appsDir        = location(QStandardPaths::ApplicationsLocation);
        request.desktopDir     = location(QStandardPaths::DesktopLocation);
        request.iconPath       = location(QStandardPaths::GenericDataLocation) / "d2modgen" / "icons" / "logo.png";
        request.homeDir        = location(QStandardPaths::HomeLocation);
        request.findExecutable = [](const std::string& program) {
            return string2path(QStandardPaths::findExecutable(QString::fromStdString(program)).toStdString());
        };

        // The launcher icon is the app logo from the resources; Icon= needs it as a file on disk.
        const QString iconFile = QString::fromStdString(path2string(request.iconPath));
        QDir().mkpath(QFileInfo(iconFile).path());
        QFile::remove(iconFile);
        if (!QFile::copy(":/assets/logo.png", iconFile))
            request.iconPath.clear();

        const ShortcutResult result = makeShortcut(request);
        switch (result.status) {
            case ShortcutResult::Status::NoPreset:
                emit statusUpdate(tr("No runner preset for this Wine prefix. Enter a runner template, or use \"Set launch to mod\"."));
                return;
            case ShortcutResult::Status::RunnerNotFound:
                emit statusUpdate(tr("%1 was not found. Install it or edit the runner template.").arg(QString::fromStdString(result.runner)));
                return;
            case ShortcutResult::Status::WriteFailed:
                emit statusUpdate(tr("Failed to write shortcut"));
                return;
            case ShortcutResult::Status::Written:
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

        emit statusUpdate(tr("Shortcut created: %1").arg(written.join(", ")));
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

QString UIController::runnerPreset(QString userHome, bool legacy)
{
    const auto home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).toStdString();
    return QString::fromStdString(D2ModGen::runnerPreset(string2path(userHome.toStdString()), legacy, string2path(home)));
}

void UIController::copyModSettings()
{
    const QString saves = getSaveRoot(false);
    if (saves.isEmpty() || !QFileInfo::exists(saves))
        return;

    const QString modSaves = getSaveRoot(true);
    if (!QDir().mkpath(modSaves)) {
        emit statusUpdate(tr("Failed to create mod settings folder."));
        return;
    }

    int copied = 0;
    for (const QString& name : QStringList{ "Settings.json", "Custom.key" }) {
        const QString source      = saves + name;
        const QString destination = modSaves + name;
        if (!QFileInfo::exists(source))
            continue;

        if ((QFileInfo::exists(destination) && !QFile::remove(destination))
            || !QFile::copy(source, destination)) {
            emit statusUpdate(tr("Failed to copy %1.").arg(name));
            return;
        }
        copied++;
    }
    emit statusUpdate(copied ? tr("Settings copied (%1 files).").arg(copied) : tr("No settings files found."));
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
