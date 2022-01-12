/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainConfigPage.hpp"
#include "Utils.hpp"
#include "WinSpecific.hpp"

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

namespace {

QString ensureTrailingSlash(QString path)
{
    if (path.isEmpty())
        return {};

    const bool hasSlash = path.endsWith("\\") || path.endsWith("/");
    if (!hasSlash)
        path += "/";
    return path;
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
    QLineEdit* d2rPath;
    QLineEdit* d2legacyPath;
    QLineEdit* d2rSaves;
    QLineEdit* d2rArgs;
    QLineEdit* seed;
    QLineEdit* outPath;
    QCheckBox* addKeys;
    QCheckBox* exportAll;

    std::function<void(bool)> setMode;
};

MainConfigPage::MainConfigPage(QWidget* parent)
    : IConfigPage(parent)
    , m_impl(new Impl())
{
    m_impl->modName       = new QLineEdit(this);
    QValidator* validator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_]+"), this);
    m_impl->modName->setValidator(validator);

    m_impl->d2legacyMode = new QCheckBox("Use Diablo II legacy installation", this);

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

    m_impl->addKeys = new QCheckBox("Add key to new char inventory (Basic mod test)", this);

    m_impl->exportAll = new QCheckBox("Export all *.txt (for further manual edit)", this);
    m_impl->exportAll->setChecked(false);

    QPushButton* copySettings = new QPushButton("Copy Settings.json", this);

    QPushButton* launchArgs      = new QPushButton("Set launch to mod", this);
    QPushButton* launchArgsClear = new QPushButton("Reset launch to unmodded", this);
    QPushButton* makeShortcut    = new QPushButton("Make shortcut on Desktop", this);

    QList<QWidget*> d2resurrectedWidgets;
    QList<QWidget*> d2legacyWidgets;

    // layouts

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    {
        QHBoxLayout* rowLayout = new QHBoxLayout();
        rowLayout->setSpacing(15);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel("Mod id:", this));
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
        rowLayout->addWidget(new QLabel("D2R path:", this));
        rowLayout->addWidget(m_impl->d2rPath);
        d2resurrectedWidgets << rowWidget;
    }

    {
        QWidget*     rowWidget = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setSpacing(5);
        rowLayout->setMargin(0);
        mainLayout->addWidget(rowWidget);
        rowLayout->addWidget(new QLabel("D2 legacy path:", this));
        rowLayout->addWidget(m_impl->d2legacyPath);
        d2legacyWidgets << rowWidget;
    }

    {
        QWidget*     rowWidget = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowWidget);
        rowLayout->setSpacing(5);
        rowLayout->setMargin(0);
        mainLayout->addWidget(rowWidget);
        rowLayout->addWidget(new QLabel("D2R save and user settings root:", this));
        rowLayout->addWidget(m_impl->d2rSaves);
        QHBoxLayout* rowLayoutButtons = new QHBoxLayout();
        rowLayout->addLayout(rowLayoutButtons);
        rowLayoutButtons->addWidget(copySettings);
        rowLayoutButtons->addWidget(new QLabel("(this will copy default settings to mod folder)", this));
        rowLayoutButtons->addStretch();
        d2resurrectedWidgets << rowWidget;
    }

    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel("D2R command arguments (read-only):", this));
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
        rowLayoutButtons->addWidget(new QLabel("(<b>Close Battlenet launcher before actions!</b>)", this));
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
        rowLayout->addWidget(new QLabel("Output directory (leave empty to output to D2R path):", this));
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
    };

    connect(m_impl->modName, &QLineEdit::textChanged, this, updateArgs);
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
        auto env  = getEnv();
        auto desk = ensureTrailingSlash(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        createShortCut(desk + "Diablo II - " + env.modName + " Mod", env.d2rPath + (env.isLegacy ? "Diablo II.exe" : "D2R.exe"), m_impl->d2rArgs->text());
    });
    connect(m_impl->d2legacyMode, &QCheckBox::stateChanged, this, [this, d2resurrectedWidgets, d2legacyWidgets, updateArgs](int) {
        const bool legacy = m_impl->d2legacyMode->isChecked();
        for (QWidget* w : d2resurrectedWidgets)
            w->setVisible(!legacy);
        for (QWidget* w : d2legacyWidgets)
            w->setVisible(legacy);
        updateArgs();
    });

    m_impl->modName->setText("rando");
}

MainConfigPage::~MainConfigPage() = default;

GenerationEnvironment MainConfigPage::getEnv() const
{
    GenerationEnvironment env;
    env.modName         = m_impl->modName->text();
    env.isLegacy        = m_impl->d2legacyMode->isChecked();
    env.d2rPath         = ensureTrailingSlash(env.isLegacy ? m_impl->d2legacyPath->text() : m_impl->d2rPath->text());
    env.exportAllTables = m_impl->exportAll->isChecked();
    env.appData         = ensureTrailingSlash(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    env.seed            = m_impl->seed->text().toInt();
    env.outPath         = ensureTrailingSlash(m_impl->outPath->text());
    if (env.outPath.isEmpty())
        env.outPath = env.d2rPath;
    return env;
}

void MainConfigPage::createNewSeed()
{
    auto seed = QRandomGenerator::system()->generate();
    m_impl->seed->setText(QString("%1").arg(seed));
}

QString MainConfigPage::caption() const
{
    return "Main";
}

QString MainConfigPage::settingKey() const
{
    return "main";
}

void MainConfigPage::readSettings(const QJsonObject& data)
{
    if (data.contains("modname"))
        m_impl->modName->setText(data["modname"].toString());
    else
        m_impl->modName->setText("rando");

    if (data.contains("seed"))
        m_impl->seed->setText(data["seed"].toString());
    else
        createNewSeed();

    if (data.contains("d2rpath"))
        m_impl->d2rPath->setText(data["d2rpath"].toString());
    else
        m_impl->d2rPath->setText(getInstallLocationFromRegistry(true));

    if (data.contains("d2legacyPath"))
        m_impl->d2legacyPath->setText(data["d2legacyPath"].toString());
    else
        m_impl->d2legacyPath->setText(getInstallLocationFromRegistry(false));

    m_impl->addKeys->setChecked(!data.contains("addKeys") || data["addKeys"].toBool());
    m_impl->d2legacyMode->setChecked(data["isLegacy"].toBool());
}

void MainConfigPage::writeSettings(QJsonObject& data) const
{
    data["modname"]      = m_impl->modName->text();
    data["seed"]         = m_impl->seed->text();
    data["d2rPath"]      = m_impl->d2rPath->text();
    data["d2legacyPath"] = m_impl->d2legacyPath->text();
    data["addKeys"]      = m_impl->addKeys->isChecked();
    data["isLegacy"]     = m_impl->d2legacyMode->isChecked();
}

KeySet MainConfigPage::generate(TableSet& tableSet, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (!m_impl->addKeys->isChecked())
        return {};
    Table&    charTable = tableSet.tables["charstats"];
    TableView charTableView(charTable);

    for (auto& rowView : charTableView) {
        if (rowView["class"] == "Expansion")
            continue;
        for (int i = 1; i <= 10; ++i) {
            const auto codeColumn  = QString("item%1").arg(i);
            const auto countColumn = QString("item%1count").arg(i);

            QString& code  = rowView[codeColumn];
            QString& count = rowView[countColumn];
            if (count == "0") {
                count = "1";
                code  = "key";
                break;
            }
        }
    }
    return { "charstats" };
}

void MainConfigPage::setLaunch(QString arg)
{
    const QString config = getBattleNetConfig();
    if (config.isEmpty()) {
        QMessageBox::warning(this, "warning", "Failed to locate Battle.net.config");
        return;
    }
    QJsonObject data;
    if (!readJsonFile(config, data)) {
        QMessageBox::warning(this, "warning", "Failed to read data from Battle.net.config");
        return;
    }
    auto valGames                       = data["Games"].toObject();
    auto valOsi                         = valGames["osi"].toObject();
    valOsi["AdditionalLaunchArguments"] = arg;
    valGames["osi"]                     = valOsi;
    data["Games"]                       = valGames;
    if (!writeJsonFile(config, data, true))
        QMessageBox::warning(this, "warning", "Failed to write data to Battle.net.config");
}
