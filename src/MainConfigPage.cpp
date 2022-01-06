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

QString getInstallLocationFromRegistry()
{
    QSettings set("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II Resurrected",
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
    QLineEdit* d2rPath;
    QLineEdit* d2rSaves;
    QLineEdit* d2rArgs;
    QLineEdit* seed;
    QLineEdit* outPath;
    QCheckBox* addKeys;
    QCheckBox* exportAll;
};

MainConfigPage::MainConfigPage(QWidget* parent)
    : IConfigPage(parent)
    , m_impl(new Impl())
{
    m_impl->modName       = new QLineEdit(this);
    QValidator* validator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_]+"), this);
    m_impl->modName->setValidator(validator);

    m_impl->d2rPath = new QLineEdit(this);
    m_impl->d2rPath->setText(getInstallLocationFromRegistry());

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
    m_impl->addKeys->setChecked(true);

    m_impl->exportAll = new QCheckBox("Export all *.txt (for further manual edit)", this);
    m_impl->exportAll->setChecked(false);

    QPushButton* copySettings = new QPushButton("Copy Settings.json", this);

    QPushButton* launchArgs      = new QPushButton("Set launch to mod", this);
    QPushButton* launchArgsClear = new QPushButton("Reset launch to unmodded", this);
    QPushButton* makeShortcut    = new QPushButton("Make shortcut on Desktop", this);

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
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel("D2R path:", this));
        rowLayout->addWidget(m_impl->d2rPath);
    }

    {
        QVBoxLayout* rowLayout = new QVBoxLayout();
        rowLayout->setSpacing(5);
        mainLayout->addLayout(rowLayout);
        rowLayout->addWidget(new QLabel("D2R save and user settings root:", this));
        rowLayout->addWidget(m_impl->d2rSaves);
        QHBoxLayout* rowLayoutButtons = new QHBoxLayout();
        rowLayout->addLayout(rowLayoutButtons);
        rowLayoutButtons->addWidget(copySettings);
        rowLayoutButtons->addWidget(new QLabel("(this will copy default settings to mod folder)", this));
        rowLayoutButtons->addStretch();
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
        QHBoxLayout* rowLayoutButtons = new QHBoxLayout();
        rowLayout->addLayout(rowLayoutButtons);
        rowLayoutButtons->addWidget(launchArgsClear);
        rowLayoutButtons->addWidget(launchArgs);
        rowLayoutButtons->addWidget(new QLabel("(<b>Close Battlenet launcher before actions!</b>)", this));
        rowLayoutButtons->addStretch();
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

    // connects

    connect(m_impl->modName, &QLineEdit::textChanged, this, [this](const QString& text) {
        m_impl->d2rArgs->setText(QString("-mod %1 -txt").arg(text));
    });
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
        createShortCut(desk + "Diablo II - " + env.modName + " Mod", env.d2rPath + "D2R.exe", m_impl->d2rArgs->text());
    });

    m_impl->modName->setText("rando");
}

MainConfigPage::~MainConfigPage() = default;

GenerationEnvironment MainConfigPage::getEnv() const
{
    GenerationEnvironment env;
    env.modName         = m_impl->modName->text();
    env.d2rPath         = ensureTrailingSlash(m_impl->d2rPath->text());
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

    m_impl->addKeys->setChecked(!data.contains("addKeys") || data["addKeys"].toBool());
}

void MainConfigPage::writeSettings(QJsonObject& data) const
{
    data["modname"] = m_impl->modName->text();
    data["seed"]    = m_impl->seed->text();
    data["addKeys"] = m_impl->addKeys->isChecked();
}

KeySet MainConfigPage::generate(TableSet& tableSet, QRandomGenerator& rng) const
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
