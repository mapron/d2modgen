/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainConfigPage.hpp"
#include "FileIOUtils.hpp"
#include "PlatformPathUtils.hpp"
#include "TableUtils.hpp"
#include "ModuleFactory.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"

#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QSettings>
#include <QStandardPaths>
#include <QRegularExpressionValidator>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include <random>

namespace D2ModGen {

namespace {

QString ensureTrailingSlash(QString value)
{
    return QString::fromStdString(::D2ModGen::ensureTrailingSlash(value.toStdString()));
}

QString getInstallLocationFromRegistry(bool resurrected)
{
    static const QString base("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II");
    QSettings            set(base + (resurrected ? " Resurrected" : ""),
                  QSettings::Registry32Format);
    return ensureTrailingSlash(set.value("InstallLocation").toString());
}

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

struct MainConfigPage::Impl {
    QLineEdit* modName;
    QCheckBox* d2legacyMode;
    QCheckBox* d2rUseROTW;
    QLineEdit* d2rPath;
    QLineEdit* d2legacyPath;
    QLineEdit* d2rSaves;
    QLineEdit* d2rArgs;
    QLineEdit* seed;
    QLineEdit* outPath;
    QCheckBox* addKeys;
    QCheckBox* exportAll;

    std::function<void(bool)> setMode;

    IModule::Ptr module;
};

MainConfigPage::MainConfigPage(const IModule::Ptr& module, QWidget* parent)
    : IConfigPage(parent)
    , m_impl(new Impl())
{
    m_impl->module = module;
    assert(module);

    m_impl->modName       = new QLineEdit(this);
    QValidator* validator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_]+"), this);
    m_impl->modName->setValidator(validator);

    m_impl->d2legacyMode = new QCheckBox(tr("Use Diablo II legacy installation"), this);
    m_impl->d2rUseROTW   = new QCheckBox(tr("Target for D2R Reign of the Warlock"), this);

    m_impl->d2rPath      = new QLineEdit(this);
    m_impl->d2legacyPath = new QLineEdit(this);

    m_impl->d2rSaves = new QLineEdit(this);
    m_impl->d2rSaves->setText(getSaveRoot());

    m_impl->d2rArgs = new QLineEdit(this);
    m_impl->d2rArgs->setReadOnly(true);

    m_impl->seed              = new QLineEdit(this);
    QValidator* validatorSeed = new QRegularExpressionValidator(QRegularExpression("[0-9]{1,10}"), this);
    m_impl->seed->setValidator(validatorSeed);
    m_impl->seed->setMaximumWidth(80);

    m_impl->outPath = new QLineEdit(this);

    m_impl->addKeys = new QCheckBox(tr("Add key to new char inventory (Basic mod test)"), this);

    m_impl->exportAll = new QCheckBox(tr("Export all *.txt (for further manual edit)"), this);
    m_impl->exportAll->setChecked(false);

    QPushButton* copySettings = new QPushButton(tr("Copy Settings.json"), this);

    QPushButton* launchArgs      = new QPushButton(tr("Set launch to mod"), this);
    QPushButton* launchArgsClear = new QPushButton(tr("Reset launch to unmodded"), this);
    QPushButton* makeShortcut    = new QPushButton(tr("Make shortcut on Desktop"), this);

    QList<QWidget*> d2resurrectedWidgets;
    QList<QWidget*> d2legacyWidgets;

    // layouts

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setMargin(0);

    {
        QHBoxLayout* rowLayout = new QHBoxLayout();
        rowLayout->setSpacing(15);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel(tr("Mod id:"), this));
        rowLayout->addWidget(m_impl->modName);
    }
    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(m_impl->d2legacyMode);
    }
    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(m_impl->d2rUseROTW);
    }
    {
        QWidget*     rowWidget = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setSpacing(5);
        rowLayout->setMargin(0);
        mainLayout->addWidget(rowWidget);
        rowLayout->addWidget(new QLabel(tr("D2R path:"), this));
        rowLayout->addWidget(m_impl->d2rPath);
        d2resurrectedWidgets << rowWidget;
    }

    {
        QWidget*     rowWidget = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setSpacing(5);
        rowLayout->setMargin(0);
        mainLayout->addWidget(rowWidget);
        rowLayout->addWidget(new QLabel(tr("D2 legacy path:"), this));
        rowLayout->addWidget(m_impl->d2legacyPath);
        d2legacyWidgets << rowWidget;
    }

    {
        QWidget*     rowWidget = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setSpacing(5);
        rowLayout->setMargin(0);
        mainLayout->addWidget(rowWidget);
        rowLayout->addWidget(new QLabel(tr("D2R save and user settings root:"), this));
        rowLayout->addWidget(m_impl->d2rSaves);
        QHBoxLayout* rowLayoutButtons = new QHBoxLayout();
        rowLayout->addLayout(rowLayoutButtons);
        rowLayoutButtons->addWidget(copySettings);
        rowLayoutButtons->addWidget(new QLabel(tr("(this will copy default settings to mod folder)"), this));
        rowLayoutButtons->addStretch();
        d2resurrectedWidgets << rowWidget;
    }

    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel(tr("D2R command arguments (read-only):"), this));
        rowLayout->addWidget(m_impl->d2rArgs);
        QHBoxLayout* rowLayoutButtonShortcut = new QHBoxLayout();
        rowLayout->addLayout(rowLayoutButtonShortcut);
        rowLayoutButtonShortcut->addWidget(makeShortcut);
        rowLayoutButtonShortcut->addStretch();
        QWidget*     rowWidget        = new QWidget(this);
        QHBoxLayout* rowLayoutButtons = new QHBoxLayout(rowWidget);
        rowLayout->addWidget(rowWidget);
        rowLayoutButtons->setMargin(0);
        rowLayoutButtons->addWidget(launchArgsClear);
        rowLayoutButtons->addWidget(launchArgs);
        rowLayoutButtons->addWidget(new QLabel(tr("(<b>Close Battlenet launcher before actions!</b>)"), this));
        rowLayoutButtons->addStretch();
        d2resurrectedWidgets << rowWidget;
    }

    {
        QHBoxLayout* rowLayout = new QHBoxLayout();
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel("Random seed:", this));
        rowLayout->addWidget(m_impl->seed);
        rowLayout->addStretch();
    }

    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel(tr("Output directory (leave empty to output to D2R path):"), this));
        rowLayout->addWidget(m_impl->outPath);
    }

    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(m_impl->addKeys);
    }
    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(m_impl->exportAll);
    }

    mainLayout->addStretch();

    for (QWidget* w : d2legacyWidgets)
        w->setVisible(false);

    // connects
    auto updateArgs = [this]() {
        auto modname = m_impl->modName->text();
        auto args    = QString("-mod %1 -txt").arg(modname);
        if (m_impl->d2legacyMode->isChecked())
            args = "-direct -txt";
        m_impl->d2rArgs->setText(args);
        emit needUpdateModList();
    };

    connect(m_impl->d2rPath, &QLineEdit::textChanged, this, &MainConfigPage::needUpdateModList);
    connect(m_impl->modName, &QLineEdit::textChanged, this, updateArgs);
    connect(m_impl->d2rPath, &QLineEdit::textEdited, this, &IConfigPage::dataChanged);
    connect(m_impl->d2legacyPath, &QLineEdit::textEdited, this, &IConfigPage::dataChanged);
    connect(m_impl->modName, &QLineEdit::textEdited, this, &IConfigPage::dataChanged);
    connect(m_impl->addKeys, &QCheckBox::clicked, this, &IConfigPage::dataChanged);
    connect(copySettings, &QPushButton::clicked, this, [this] {
        const QString saves = ensureTrailingSlash(m_impl->d2rSaves->text());
        if (saves.isEmpty() || !QFileInfo::exists(saves))
            return;
        const QString modSaves = saves + "mods/" + m_impl->modName->text() + "/";
        if (!QFileInfo::exists(modSaves))
            QDir().mkpath(modSaves);
        const QString name = "Settings.json";
        QFile::remove(modSaves + name);
        QFile::copy(saves + name, modSaves + name);
    });
    connect(launchArgsClear, &QPushButton::clicked, this, [this] {
        setLaunch("");
    });
    connect(launchArgs, &QPushButton::clicked, this, [this] {
        setLaunch(m_impl->d2rArgs->text());
    });
    connect(makeShortcut, &QPushButton::clicked, this, [this] {
        const bool legacy  = m_impl->d2legacyMode->isChecked();
        auto       d2rpath = ensureTrailingSlash(legacy ? m_impl->d2legacyPath->text() : m_impl->d2rPath->text());
        auto       desk    = ensureTrailingSlash(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        createShortCut((desk + "Diablo II - " + m_impl->modName->text() + " Mod").toStdString(),
                       (d2rpath + (legacy ? "Diablo II.exe" : "D2R.exe")).toStdString(),
                       m_impl->d2rArgs->text().toStdString());
    });
    connect(m_impl->d2legacyMode, &QCheckBox::stateChanged, this, [this, d2resurrectedWidgets, d2legacyWidgets, updateArgs](int) {
        const bool legacy = m_impl->d2legacyMode->isChecked();
        for (QWidget* w : d2resurrectedWidgets)
            w->setVisible(!legacy);
        for (QWidget* w : d2legacyWidgets)
            w->setVisible(legacy);
        updateArgs();
    });
    connect(m_impl->d2legacyMode, &QCheckBox::clicked, this, &IConfigPage::dataChanged);
    connect(m_impl->d2rUseROTW, &QCheckBox::clicked, this, &IConfigPage::dataChanged);
    connect(m_impl->exportAll, &QCheckBox::clicked, this, &IConfigPage::dataChanged);

    m_impl->modName->setText("rando");
}

MainConfigPage::~MainConfigPage() = default;

void MainConfigPage::createNewSeed()
{
    std::random_device rd;
    auto               seed = rd();
    m_impl->seed->setText(QString("%1").arg(seed));
    emit dataChanged();
}

QStringList MainConfigPage::getOtherMods() const
{
    QStringList   result;
    const QString modName = m_impl->modName->text();
    for (auto d : QDir(m_impl->d2rPath->text() + "/mods").entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (d.fileName() != modName)
            result << d.fileName();
    }
    return result;
}

QString MainConfigPage::caption() const
{
    return tr("Main");
}

QString MainConfigPage::pageHelp() const
{
    return tr("This page allow you to select most important options:\n"
              "-Where input and output data is (D2 installation - Resurrected or Legacy);\n"
              "-Create new D2R save folder with global settings;\n"
              "-Create desktop shortcut to launch mod.\n"
              "However, there are some less important abilities: \n"
              "-Seed number (max 2^32) determines how random generation behaves. \n"
              "If seed is unchaged, then all generation will be the same every time \n"
              "You can edit seed manually or press F5 to generate new one.\n"
              "-Add key to character inventory for fresh character - now you are sure you started modded version.");
}

IConfigPage::PresetList MainConfigPage::pagePresets() const
{
    return {};
}

void MainConfigPage::updateUIFromSettings(const Mernel::PropertyTree& data)
{
    m_impl->addKeys->setChecked(data.value("addKeys", Mernel::PropertyTreeScalar(true)).toBool());
}

void MainConfigPage::writeSettingsFromUI(Mernel::PropertyTree& data) const
{
    data["addKeys"] = Mernel::PropertyTreeScalar{ m_impl->addKeys->isChecked() };
}

void MainConfigPage::updateUIFromSettingsMain(const Mernel::PropertyTree& data)
{
    if (data.contains("modname"))
        m_impl->modName->setText(QString::fromStdString(data["modname"].getScalar().toString()));
    else
        m_impl->modName->setText("rando");

    if (data.contains("seed"))
        m_impl->seed->setText(QString("%1").arg(data["seed"].getScalar().toInt()));
    else
        createNewSeed();

    if (data.contains("d2rPath"))
        m_impl->d2rPath->setText(QString::fromStdString(data["d2rPath"].getScalar().toString()));
    else
        m_impl->d2rPath->setText(getInstallLocationFromRegistry(true));

    if (data.contains("d2legacyPath"))
        m_impl->d2legacyPath->setText(QString::fromStdString(data["d2legacyPath"].getScalar().toString()));
    else
        m_impl->d2legacyPath->setText(getInstallLocationFromRegistry(false));

    m_impl->d2legacyMode->setChecked(data.value("isLegacy", Mernel::PropertyTreeScalar(false)).toBool());
    m_impl->d2rUseROTW->setChecked(data.value("d2rUseROTW", Mernel::PropertyTreeScalar(true)).toBool());
    m_impl->exportAll->setChecked(data.value("exportAllTables", Mernel::PropertyTreeScalar(false)).toBool());
    m_impl->outPath->setText(QString::fromStdString(data.value("outPath", Mernel::PropertyTreeScalar("")).toString()));
}

void MainConfigPage::writeSettingsFromUIMain(Mernel::PropertyTree& data) const
{
    data["modname"]         = Mernel::PropertyTreeScalar{ m_impl->modName->text().toStdString() };
    data["seed"]            = Mernel::PropertyTreeScalar{ static_cast<int64_t>(m_impl->seed->text().toUInt()) };
    data["d2rPath"]         = Mernel::PropertyTreeScalar{ m_impl->d2rPath->text().toStdString() };
    data["d2legacyPath"]    = Mernel::PropertyTreeScalar{ m_impl->d2legacyPath->text().toStdString() };
    data["isLegacy"]        = Mernel::PropertyTreeScalar{ m_impl->d2legacyMode->isChecked() };
    data["d2rUseROTW"]      = Mernel::PropertyTreeScalar{ m_impl->d2rUseROTW->isChecked() };
    data["exportAllTables"] = Mernel::PropertyTreeScalar{ m_impl->exportAll->isChecked() };
    data["outPath"]         = Mernel::PropertyTreeScalar{ m_impl->outPath->text().toStdString() };

    Logger() << "seed after write:" << static_cast<uint32_t>(data.value("seed", Mernel::PropertyTreeScalar(0)).toInt());
}

const IModule& MainConfigPage::getModule() const
{
    return *(m_impl->module);
}

void MainConfigPage::setLaunch(QString arg)
{
    const QString config = getBattleNetConfig();
    if (config.isEmpty()) {
        QMessageBox::warning(this, "warning", "Failed to locate Battle.net.config");
        return;
    }
    const auto           configPath = string2path(config.toStdString());
    Mernel::PropertyTree doc;
    std::string          buffer;
    if (!Mernel::readFileIntoBufferNoexcept(configPath, buffer) || !readJsonFromBufferNoexcept(buffer, doc)) {
        QMessageBox::warning(this, "warning", "Failed to read data from Battle.net.config");
        return;
    }

    auto& valGames                      = doc["Games"].getMap();
    auto& valOsi                        = valGames["osi"];
    valOsi["AdditionalLaunchArguments"] = Mernel::PropertyTreeScalar(arg.toStdString());

    buffer.clear();
    writeJsonToBufferNoexcept(buffer, doc);
    // buffer.replace(QByteArray("/"), QByteArray("\\/")); // weird battlenet format.
    if (!Mernel::writeFileFromBufferNoexcept(configPath, buffer))
        QMessageBox::warning(this, "warning", "Failed to write data to Battle.net.config");
}

}
