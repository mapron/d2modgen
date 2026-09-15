/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "UIController.hpp"

#include "FileIOUtils.hpp"

#include "ConfigHandler.hpp"
#include "PropertyUtil.hpp"

#include "MernelPlatform/FileFormatJson.hpp"
#include "MernelPlatform/ShellUtils.hpp"

#include <QDateTime>
#include <QCoreApplication>
#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

namespace D2ModGen {

namespace {

QString ensureTrailingSlash(QString value)
{
    auto str = QString::fromStdString(::D2ModGen::ensureTrailingSlash(value.toStdString()));
    str.replace('\\', '/');
    return str;
}

#ifdef _WIN32
QString getInstallLocationFromRegistry(bool resurrected)
{
    static const QString base("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II");
    QSettings            set(base + (resurrected ? " Resurrected" : ""),
                             QSettings::Registry32Format);
    return ensureTrailingSlash(set.value("InstallLocation").toString());
}
#else
QString getInstallLocationFromRegistry(bool resurrected)
{
    return {};
}
#endif

QString getUserHome()
{
    return ensureTrailingSlash(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0));
}

QString getSaveRoot()
{
    return getUserHome() + "Saved Games/Diablo II Resurrected/";
}

QString getBattleNetConfig()
{
    const QString config = getUserHome() + "AppData/Roaming/Battle.net/Battle.net.config";
    return QFileInfo::exists(config) ? config : "";
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

    for (auto& module : configHandler.m_modules) {
        auto* uimod                                    = new TabController(module, m_delayTimer, this);
        m_tabs[QString::fromStdString(module.m_uiKey)] = uimod;

        m_tabsList.push_back(uimod);
    }
    m_tabsList[0]->enableChangeOnSet();

    m_configHandler.loadConfig(m_configHandler.m_defaultPath);

    pushUndoCurrent();
}

UIController::~UIController()
{
    m_configHandler.saveAppConfig();
    if (m_autoSave)
        m_configHandler.saveConfig(m_configHandler.m_defaultPath);
}

void UIController::generate()
{
    emit statusUpdate(tr("Start..."));
    QTimer::singleShot(30, this, &UIController::generateFinish);
}

void UIController::saveConfig(QString filename)
{
    filename.replace("file:///", "");
    if (m_configHandler.saveConfig(filename.toStdString())) {
        emit statusUpdate(tr("Saved."));
    } else {
        emit statusUpdate(tr("Error when saving!"));
    }
}

void UIController::loadConfig(QString filename)
{
    filename.replace("file:///", "");
    const auto result = m_configHandler.loadConfig(filename.toStdString());

    if (result) {
        sendDataChange();
        emit statusUpdate(tr("Loaded."));
    } else {
        emit statusUpdate(tr("Error when loading!"));
    }
}

void UIController::clearConfig()
{
    m_configHandler.loadConfig(Mernel::PropertyTree{}, false);

    sendDataChange();
    emit statusUpdate(tr("Config is cleared."));
}

bool UIController::loadConfig(const Mernel::PropertyTree& data)
{
    const auto result = m_configHandler.loadConfig(data);

    sendDataChange();
    return result;
}

void UIController::loadPresetConfig(const QString& filename)
{
    if (m_configHandler.loadConfig(QString("presets/%1.json").arg(filename).toStdString(), false))
        sendDataChange();
}

QString UIController::getApp(const QString& key, const QString& def) const
{
    return QString::fromStdString(m_configHandler.m_appConfig.value(key.toStdString(), Mernel::PropertyTreeScalar(def.toStdString())).toString());
}

void UIController::setApp(const QString& key, const QString& value)
{
    m_configHandler.m_appConfig.getMap()[key.toStdString()] = Mernel::PropertyTreeScalar{ value.toStdString() };
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
    loadConfig(m_undo.last());
    updateUndoAction();
}

void UIController::disableAutoSave()
{
    m_autoSave = false;
}

void UIController::browseToAppSettings()
{
    QProcess::startDetached("explorer.exe", QStringList() << QDir::toNativeSeparators(QString::fromStdString(path2string(m_configHandler.m_appData))));
}

void UIController::newSeed()
{
    m_configHandler.m_modules[0].m_currentConfig["seed"] = Mernel::PropertyTreeScalar{};
    m_configHandler.updateSeed(false);
    m_tabsList[0]->sendDataChange(); // to refresh seed
}

void UIController::detectPath()
{
    auto& cfg         = m_configHandler.m_modules[0].m_currentConfig;
    bool  resurrected = cfg.value("versionMajor", Mernel::PropertyTreeScalar{ 1 }).toInt() == 1;
    cfg["inputPath"]  = Mernel::PropertyTreeScalar{ getInstallLocationFromRegistry(resurrected).toStdString() };
    m_tabsList[0]->sendDataChange(); // to refresh paths
}

void UIController::sendDataChange()
{
    for (auto* w : m_tabsList) {
        w->sendDataChange();
        w->sendEnabledChange();
    }
}

void UIController::updateUndoAction()
{
    //
}

void UIController::generateFinish()
{
    auto result = m_configHandler.generate();
    if (!result.m_success) {
        emit statusUpdate(tr("Error: %1").arg(QString::fromStdString(result.m_error)));
        return;
    }

    std::string err;
    emit        statusUpdate(tr("Mod '%1' is updated.")
                                 .arg(QString::fromStdString(m_configHandler.getEnv(err).modName)));

    m_tabsList[0]->sendDataChange(); // to refresh seed
}

void UIController::setLaunch(QString arg)
{
    const QString config = getBattleNetConfig();
    if (config.isEmpty()) {
        emit statusUpdate(tr("Failed to locate Battle.net.config"));
        return;
    }
    const auto           configPath = string2path(config.toStdString());
    Mernel::PropertyTree doc;
    std::string          buffer;
    if (!Mernel::readFileIntoBufferNoexcept(configPath, buffer) || !Mernel::readJsonFromBufferNoexcept(buffer, doc)) {
        emit statusUpdate(tr("Failed to read data from Battle.net.config"));
        return;
    }

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
    std::string err;
    auto        env = m_configHandler.getEnv(err);
    if (!err.empty())
        return;

    const bool        legacy = env.isLegacy;
    auto              d2path = env.inputPath;
    auto              desk   = string2path(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).toStdString());
    const std::string title  = "Diablo II - " + env.modName + " Mod";

    if (!Mernel::createShortCut(desk, title, d2path, (legacy ? "Diablo II.exe" : "D2R.exe"), arg.toStdString())) {
        emit statusUpdate(tr("Failed to write shortcut"));
        return;
    }

    emit statusUpdate(tr("Shortcut created!"));
}

void UIController::copyModSettings()
{
    std::string   err;
    auto          env   = m_configHandler.getEnv(err);
    const QString saves = ensureTrailingSlash(getSaveRoot());
    if (saves.isEmpty() || !QFileInfo::exists(saves))
        return;
    const QString modSaves = saves + "mods/" + QString::fromStdString(env.modName) + "/";
    if (!QFileInfo::exists(modSaves))
        QDir().mkpath(modSaves);
    const QString name = "Settings.json";
    QFile::remove(modSaves + name);
    QFile::copy(saves + name, modSaves + name);
    emit statusUpdate(tr("Settings updated!"));
}

void UIController::copySaves()
{
    std::string   err;
    auto          env   = m_configHandler.getEnv(err);
    const QString saves = ensureTrailingSlash(getSaveRoot());
    if (saves.isEmpty() || !QFileInfo::exists(saves))
        return;
    const QString modSaves = saves + "mods/" + QString::fromStdString(env.modName) + "/";
    if (!QFileInfo::exists(modSaves))
        QDir().mkpath(modSaves);

    auto files = QDir(saves).entryInfoList(QStringList() << "*.d2s");
    for (auto& file : files) {
        QFile::copy(saves + file.fileName(), modSaves + file.fileName());
    }

    emit statusUpdate(tr("Save files copied (%1)").arg(files.size()));
}

QString UIController::getSaveFolder()
{
    return getSaveRoot() + "mods/";
}

TabController::TabController(ConfigHandler::ModuleData& module, DelayedTimer* delayTimer, QObject* parent)
    : QObject(parent)
    , m_module(module)
    , m_delayTimer(delayTimer)
{
    connect(this, &TabController::dataChangedInternal, this, &TabController::dataChanged, Qt::QueuedConnection);
}

TabController::~TabController() = default;

QVariant TabController::get(const QString& key) const
{
    auto&      cfg    = m_module;
    const auto keyStd = key.toStdString();

    auto it = m_module.m_currentConfig.getMap().find(keyStd);
    if (it == m_module.m_currentConfig.getMap().cend())
        return propertyToQVariant(cfg.m_module->defaultValues().at(keyStd));

    return propertyToQVariant(it->second);
}

int TabController::getInt(const QString& key) const
{
    return get(key).toInt();
}

QString TabController::getStr(const QString& key) const
{
    return get(key).toString();
}

void TabController::set(const QString& key, const QVariant& val)
{
    auto& old     = m_module.m_currentConfig.getMap()[key.toStdString()];
    auto  newVal  = qvariantToProperty(val);
    bool  changed = old != newVal;
    if (!changed)
        return;
    old = std::move(newVal);

    m_delayTimer->start();

    if (m_changeOnSet)
        sendDataChange();
}

bool TabController::getEnabled() const
{
    return m_module.m_enabled;
}

void TabController::setEnabled(bool val)
{
    if (m_module.m_enabled == val)
        return;

    m_module.m_enabled = val;
    m_delayTimer->start();
    emit enabledChanged();
}

void TabController::activatePreset(int index)
{
    if (index < 1)
        return;
    m_module.m_currentConfig = m_module.m_module->presets().at(index - 1);

    m_delayTimer->start();
    sendDataChange();
}

void TabController::resetToDefault()
{
    m_module.m_currentConfig = {};
    m_module.m_currentConfig.convertToMap();
    sendDataChange();
}

void TabController::sendDataChange()
{
    emit dataChangedInternal();
}

void TabController::sendEnabledChange()
{
    emit enabledChanged();
}

}
