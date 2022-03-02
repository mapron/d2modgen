/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainConfigPage.hpp"
#include "FileIOUtils.hpp"
#include "WinSpecific.hpp"
#include "TableUtils.hpp"
#include "ModuleFactory.hpp"

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
#include <QJsonDocument>

namespace D2ModGen {

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

struct MainConfigPage::Impl {
    QLineEdit* modName;
    QCheckBox* d2legacyMode;
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

    m_impl->modName       = new QLineEdit(this);
    QValidator* validator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_]+"), this);
    m_impl->modName->setValidator(validator);

    m_impl->d2legacyMode = new QCheckBox(tr("Use Diablo II legacy installation"), this);

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
    connect(m_impl->modName, &QLineEdit::textEdited, this, &IConfigPage::dataChanged);
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
        createShortCut(desk + "Diablo II - " + m_impl->modName->text() + " Mod", d2rpath + (legacy ? "Diablo II.exe" : "D2R.exe"), m_impl->d2rArgs->text());
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

    m_impl->modName->setText("rando");
}

MainConfigPage::~MainConfigPage() = default;

void MainConfigPage::createNewSeed()
{
    auto seed = QRandomGenerator::system()->generate();
    m_impl->seed->setText(QString("%1").arg(seed));
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

void MainConfigPage::updateUIFromSettings(const PropertyTree& data)
{
    m_impl->addKeys->setChecked(data.value("addKeys", true).toBool());
}

void MainConfigPage::writeSettingsFromUI(PropertyTree& data) const
{
    data["addKeys"] = PropertyTreeScalar{ m_impl->addKeys->isChecked() };
}

void MainConfigPage::updateUIFromSettingsMain(const PropertyTree& data)
{
    if (data.contains("modname"))
        m_impl->modName->setText(QString::fromStdString(data["modname"].toString()));
    else
        m_impl->modName->setText("rando");

    if (data.contains("seed"))
        m_impl->seed->setText(QString::fromStdString(data["seed"].toString()));
    else
        createNewSeed();

    if (data.contains("d2rPath"))
        m_impl->d2rPath->setText(QString::fromStdString(data["d2rPath"].toString()));
    else
        m_impl->d2rPath->setText(getInstallLocationFromRegistry(true));

    if (data.contains("d2legacyPath"))
        m_impl->d2legacyPath->setText(QString::fromStdString(data["d2legacyPath"].toString()));
    else
        m_impl->d2legacyPath->setText(getInstallLocationFromRegistry(false));

    m_impl->d2legacyMode->setChecked(data.value("isLegacy", false).toBool());
    m_impl->exportAll->setChecked(data.value("exportAllTables", false).toBool());
    m_impl->outPath->setText(QString::fromStdString(data.value("outPath", "").toString()));
}

void MainConfigPage::writeSettingsFromUIMain(PropertyTree& data) const
{
    data["modname"]         = PropertyTreeScalar{ m_impl->modName->text().toStdString() };
    data["seed"]            = PropertyTreeScalar{ m_impl->seed->text().toUInt() };
    data["d2rPath"]         = PropertyTreeScalar{ m_impl->d2rPath->text().toStdString() };
    data["d2legacyPath"]    = PropertyTreeScalar{ m_impl->d2legacyPath->text().toStdString() };
    data["isLegacy"]        = PropertyTreeScalar{ m_impl->d2legacyMode->isChecked() };
    data["exportAllTables"] = PropertyTreeScalar{ m_impl->exportAll->isChecked() };
    data["outPath"]         = PropertyTreeScalar{ m_impl->outPath->text().toStdString() };
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
    QJsonDocument doc;
    if (!readJsonFile(config, doc)) {
        QMessageBox::warning(this, "warning", "Failed to read data from Battle.net.config");
        return;
    }
    QJsonObject data = doc.object();

    auto valGames                       = data["Games"].toObject();
    auto valOsi                         = valGames["osi"].toObject();
    valOsi["AdditionalLaunchArguments"] = arg;
    valGames["osi"]                     = valOsi;
    data["Games"]                       = valGames;
    doc.setObject(data);
    if (!writeJsonFile(config, doc, true))
        QMessageBox::warning(this, "warning", "Failed to write data to Battle.net.config");
}

}
