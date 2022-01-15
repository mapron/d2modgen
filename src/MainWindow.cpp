/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include "MainConfigPage.hpp"
#include "ConfigPages.hpp"
#include "ConfigPageMergeMods.hpp"
#include "Utils.hpp"
#include "CascWrapper.hpp"

#include <QDebug>
#include <QLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QProcess>
#include <QLabel>
#include <QDesktopServices>

MainWindow::MainWindow()
    : QMainWindow(nullptr)
{
    setWindowTitle("Diablo II Resurrected mod generator by mapron");

    // widgets
    QStackedWidget* stackedWidget = new QStackedWidget(this);
    QPushButton*    genButton     = new QPushButton("Generate", this);

    QList<QPushButton*> pageButtons;

    m_status = new QLabel("Status label.", this);

    auto* mainPage     = new MainConfigPage(this);
    auto* modMergePage = new ConfigPageMergeMods(this);
    m_pages << mainPage;
    m_pages << CreateConfigPages(this);
    m_pages << modMergePage;

    for (IConfigPage* page : m_pages) {
        QPushButton* pageButton = new QPushButton(page->caption(), this);
        pageButton->setCheckable(true);
        pageButtons << pageButton;
        stackedWidget->addWidget(page);
    }
    QMenuBar* mainMenu         = menuBar();
    QMenu*    fileMenu         = mainMenu->addMenu("File");
    QMenu*    actionsMenu      = mainMenu->addMenu("Actions");
    QAction*  about            = mainMenu->addAction("About");
    QAction*  saveConfigAction = fileMenu->addAction("Save config...");
    QAction*  loadConfigAction = fileMenu->addAction("Load config...");
    QAction*  clearConfig      = fileMenu->addAction("Clear config");
    QAction*  browseToSettings = fileMenu->addAction("Browse to settings folder");
    QAction*  quitAction       = fileMenu->addAction("Quit");
    QAction*  generateMod      = actionsMenu->addAction("Generate mod");
    QAction*  newSeed          = actionsMenu->addAction("Create seed");

    saveConfigAction->setShortcuts(QKeySequence::Save);
    loadConfigAction->setShortcuts(QKeySequence::Open);
    clearConfig->setShortcuts(QKeySequence::New);
    newSeed->setShortcuts(QKeySequence::Refresh);
    generateMod->setShortcut(QKeySequence(Qt::Key_F9));

    // layouts

    QVBoxLayout* topLayout = new QVBoxLayout();

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(topLayout);

    pageButtons.first()->setChecked(true);

    {
        QHBoxLayout* tabLayoutOuter = new QHBoxLayout();
        tabLayoutOuter->setSpacing(0);
        QVBoxLayout* tabLayoutInner = new QVBoxLayout();
        tabLayoutInner->setSpacing(5);
        for (auto* pageButton : pageButtons)
            tabLayoutInner->addWidget(pageButton);
        tabLayoutInner->addStretch();
        tabLayoutOuter->addLayout(tabLayoutInner);
        tabLayoutOuter->addWidget(stackedWidget);
        stackedWidget->setFrameStyle(QFrame::StyledPanel);
        topLayout->addLayout(tabLayoutOuter);
    }
    {
        QHBoxLayout* bottomLayout = new QHBoxLayout();
        bottomLayout->addStretch();
        bottomLayout->addWidget(m_status);
        bottomLayout->addWidget(genButton);
        topLayout->addLayout(bottomLayout);
    }

    // connections

    connect(genButton, &QPushButton::clicked, this, [this, mainPage] {
        generate(mainPage->getEnv());
    });

    for (auto* pageButton : pageButtons) {
        connect(pageButton, &QPushButton::clicked, this, [this, pageButton, pageButtons, stackedWidget]() {
            const int index = pageButtons.indexOf(qobject_cast<QPushButton*>(sender()));
            for (auto* modeButtonOther : pageButtons) {
                modeButtonOther->setChecked(modeButtonOther == pageButton);
            }
            stackedWidget->setCurrentWidget(m_pages[index]);
        });
    }

    connect(about, &QAction::triggered, this, [this] {
        QDesktopServices::openUrl(QUrl("https://github.com/mapron/d2modgen"));
    });
    connect(saveConfigAction, &QAction::triggered, this, [this] {
        auto file = QFileDialog::getSaveFileName(this, "", "", "*.json");
        if (!file.isEmpty())
            saveConfig(file);
    });
    connect(loadConfigAction, &QAction::triggered, this, [this] {
        auto file = QFileDialog::getOpenFileName(this, "", "", "*.json");
        if (!file.isEmpty())
            loadConfig(file);
    });
    connect(clearConfig, &QAction::triggered, this, [this] {
        for (auto* page : m_pages)
            page->readSettings({});
    });
    connect(browseToSettings, &QAction::triggered, this, [mainPage] {
        QFileInfo dir = mainPage->getEnv().appData;
        QProcess::startDetached("explorer.exe", QStringList() << QDir::toNativeSeparators(dir.canonicalFilePath()));
    });
    connect(quitAction, &QAction::triggered, this, [this] {
        close();
    });
    connect(generateMod, &QAction::triggered, this, [this, mainPage] {
        generate(mainPage->getEnv());
    });
    connect(newSeed, &QAction::triggered, this, [mainPage] {
        mainPage->createNewSeed();
    });
    auto updateModList = [modMergePage, mainPage] {
        modMergePage->setModList(mainPage->getOtherMods());
    };
    connect(mainPage, &MainConfigPage::updateModList, this, updateModList);

    // misc
    updateModList();

    m_defaultConfig = mainPage->getEnv().appData + "config.json";
    loadConfig(m_defaultConfig);
}

MainWindow::~MainWindow()
{
    saveConfig(m_defaultConfig);
}

void MainWindow::generate(const GenerationEnvironment& env)
{
    if (env.d2rPath.isEmpty()) {
        qWarning() << "D2R path is empty";
        return;
    }
    const QString modRoot = env.isLegacy ? env.outPath : env.outPath + QString("mods/%1/%1.mpq/").arg(env.modName);
    m_status->setText("Start...");
    m_status->repaint();
    qDebug() << "started generation in " << modRoot;
    if (QFileInfo::exists(modRoot))
        QDir(modRoot).removeRecursively();

    if (!QFileInfo::exists(modRoot))
        QDir().mkpath(modRoot);
    if (!QFileInfo::exists(modRoot)) {
        QMessageBox::warning(this, "error", "Failed to create mod folder in D2R installation; try to launch as admin.");
        return;
    }
    if (!env.isLegacy) {
        QJsonObject modinfo;
        modinfo["name"]     = env.modName;
        modinfo["savepath"] = env.modName + "/";
        if (!writeJsonFile(modRoot + "modinfo.json", modinfo)) {
            QMessageBox::warning(this, "error", "Failed to write modinfo.");
            return;
        }
    }
    const QString excelRoot = modRoot + "data/global/excel/";
    if (!QFileInfo::exists(excelRoot))
        QDir().mkpath(excelRoot);
    for (auto path : QDir(excelRoot).entryInfoList(QStringList() << "*.txt")) {
        QFile::remove(path.absoluteFilePath());
    }

    KeySet      keySet;
    GenOutput   output;
    JsonFileSet requiredFiles;
    {
        for (auto* page : m_pages)
            requiredFiles += page->extraFiles();
    }
    {
        if (m_outputCache && m_cachedFilenames == requiredFiles) {
            output = *m_outputCache;
        } else {
            if (env.isLegacy && !ExtractTablesLegacy(env.d2rPath, output) || !env.isLegacy && !ExtractTables(env.d2rPath, output, requiredFiles)) {
                QMessageBox::warning(this, "error", "Failed to read csv data from D2R folder.");
                return;
            }
            m_cachedFilenames = requiredFiles;
            m_outputCache.reset(new GenOutput(output));
        }
        if (env.exportAllTables)
            for (auto id : output.tableSet.tables.keys())
                keySet << id;
    }
    qDebug() << "prepare ended; modifying tables. seed=" << env.seed;
    {
        QRandomGenerator rng;
        rng.seed(env.seed);
        for (auto* page : m_pages)
            keySet += page->generate(output, rng, env);
    }
    qDebug() << "writing output.";
    for (const auto& key : keySet)
        writeCSVfile(excelRoot + key + ".txt", output.tableSet.tables[key]);
    auto iter = QMapIterator(output.jsonFiles);
    while (iter.hasNext()) {
        iter.next();
        const QString folder = QFileInfo(modRoot + iter.key()).absolutePath();
        if (!QFileInfo::exists(folder))
            QDir().mkpath(folder);
        writeJsonFile(modRoot + iter.key(), iter.value());
    }
    for (auto& copyFile : output.copyFiles) {
        const QString src  = copyFile.srcModRoot + copyFile.relativePath;
        const QString dest = modRoot + copyFile.relativePath;
        if (QFile::exists(dest)) {
            QMessageBox::warning(this, "error", QString("Merge conflict in file: %1").arg(copyFile.relativePath));
            return;
        }
        QDir().mkpath(QFileInfo(dest).absolutePath());
        QFile::copy(src, dest);
    }

    qDebug() << "generation ends.";
    m_status->setText(QString("Mod '%1' successfully updated (%2).").arg(env.modName).arg(QTime::currentTime().toString("mm:ss")));
}

bool MainWindow::saveConfig(const QString& filename) const
{
    QJsonObject data;
    for (auto* page : m_pages) {
        QJsonObject pageData;
        page->writeSettings(pageData);
        data[page->settingKey()] = pageData;
    }
    QDir().mkpath(QFileInfo(filename).absolutePath());
    return writeJsonFile(filename, data);
}

bool MainWindow::loadConfig(const QString& filename)
{
    QJsonObject data;
    if (!readJsonFile(filename, data))
        return false;

    {
        for (auto* page : m_pages)
            page->readSettings(data[page->settingKey()].toObject());
    }
    return true;
}
