/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include "FileIOUtils.hpp"

#include "ConfigPages/MainConfigPage.hpp"
#include "ConfigPages/ConfigPages.hpp"
#include "ConfigPages/ConfigPageMergeMods.hpp"

#include "Storage/StorageConstants.hpp"
#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

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
#include <QJsonDocument>
#include <QDesktopServices>
#include <QButtonGroup>
#include <QCheckBox>

namespace D2ModGen {

MainWindow::MainWindow(bool autoSave)
    : QMainWindow(nullptr)
    , m_mainStorageCache(new StorageCache())
    , m_autoSave(autoSave)
{
    setWindowTitle("Diablo II Resurrected mod generator by mapron");

    // widgets
    QStackedWidget* stackedWidget    = new QStackedWidget(this);
    QPushButton*    genButton        = new QPushButton("Generate", this);
    auto            increaseFontSize = [](QWidget* w, int pointPlus, bool bold) {
        auto f = w->font();
        f.setPointSize(f.pointSize() + pointPlus);
        f.setBold(bold);
        w->setFont(f);
    };
    increaseFontSize(genButton, 4, false);

    m_status = new QLabel("Status label.", this);

    m_mainPage         = new MainConfigPage(this);
    auto* modMergePage = new ConfigPageMergeMods(this);
    auto* pageGroup    = new QButtonGroup(this);
    auto* buttonPanel  = new QWidget(this);

    QVBoxLayout*     buttonPanelLayout = new QVBoxLayout(buttonPanel);
    QList<PageGroup> pageGroups;
    pageGroups << PageGroup{
        "", QList<IConfigPage*>{ m_mainPage, modMergePage }
    };
    pageGroups << CreateConfigPages(this);

    for (const auto& group : pageGroups) {
        if (!group.title.isEmpty()) {
            auto* groupTitle = new QLabel(group.title, this);
            groupTitle->setStyleSheet("QLabel { padding-left: 20px; margin-top:8px; }");
            increaseFontSize(groupTitle, 2, true);
            buttonPanelLayout->addWidget(groupTitle);
        }

        for (IConfigPage* page : group.pages) {
            m_pages << page;
            QPushButton* pageButton = new QPushButton(page->caption(), this);
            pageButton->setCheckable(true);
            pageGroup->addButton(pageButton);
            increaseFontSize(pageButton, 2, false);

            QPushButton* resetButton   = new QPushButton("Reset to default", this);
            QCheckBox*   headerEnabler = new QCheckBox("Enable this tab", this);
            QCheckBox*   sideEnabler   = new QCheckBox("", this);
            headerEnabler->setChecked(true);
            sideEnabler->setChecked(true);
            if (!page->canBeDisabled()) {
                headerEnabler->hide();
                sideEnabler->hide();
            }

            QWidget* pageWrapper = new QWidget(this);
            pageWrapper->setContentsMargins(0, 0, 0, 0);
            auto* caption = new QLabel(page->caption(), this);
            increaseFontSize(caption, 2, true);

            QVBoxLayout* pageWrapperMain = new QVBoxLayout(pageWrapper);
            pageWrapperMain->setMargin(8);
            pageWrapperMain->setSpacing(10);
            QHBoxLayout* pageWrapperHeader = new QHBoxLayout();
            pageWrapperHeader->addWidget(caption);
            pageWrapperHeader->addStretch();
            pageWrapperHeader->addWidget(headerEnabler);
            pageWrapperHeader->addWidget(resetButton);
            pageWrapperMain->addLayout(pageWrapperHeader);
            pageWrapperMain->addWidget(page);
            stackedWidget->addWidget(pageWrapper);

            QHBoxLayout* buttonPanelRow = new QHBoxLayout();
            buttonPanelRow->setMargin(0);
            buttonPanelRow->setSpacing(3);
            buttonPanelRow->addWidget(sideEnabler);
            buttonPanelRow->addWidget(pageButton, 1);
            buttonPanelLayout->addLayout(buttonPanelRow);

            connect(pageButton, &QPushButton::clicked, this, [pageWrapper, stackedWidget]() {
                stackedWidget->setCurrentWidget(pageWrapper);
            });
            connect(resetButton, &QPushButton::clicked, this, [page]() {
                page->readSettings({});
            });
            connect(headerEnabler, &QCheckBox::toggled, sideEnabler, &QCheckBox::setChecked);
            connect(sideEnabler, &QCheckBox::toggled, headerEnabler, &QCheckBox::setChecked);
            connect(headerEnabler, &QCheckBox::toggled, page, &IConfigPage::setConfigEnabled);
            if (page->canBeDisabled())
                connect(headerEnabler, &QCheckBox::toggled, page, &QWidget::setEnabled);
            m_enableButtons[page] = headerEnabler;
        }
    }

    QMenuBar* mainMenu         = menuBar();
    QMenu*    fileMenu         = mainMenu->addMenu("File");
    QMenu*    actionsMenu      = mainMenu->addMenu("Actions");
    QAction*  about            = mainMenu->addAction("About");
    QAction*  saveConfigAction = fileMenu->addAction("Save config...");
    QAction*  loadConfigAction = fileMenu->addAction("Load config...");
    QAction*  clearConfig      = fileMenu->addAction("Clear config");
    QAction*  browseToSettings = fileMenu->addAction("Browse to settings folder");
    fileMenu->addSeparator();
    QAction* quitNoSaveAction = fileMenu->addAction("Quit without saving");
    QAction* quitAction       = fileMenu->addAction("Save and quit");
    QAction* generateMod      = actionsMenu->addAction("Generate mod");
    QAction* newSeed          = actionsMenu->addAction("Create seed");

    saveConfigAction->setShortcuts(QKeySequence::Save);
    loadConfigAction->setShortcuts(QKeySequence::Open);
    clearConfig->setShortcuts(QKeySequence::New);
    newSeed->setShortcuts(QKeySequence::Refresh);
    generateMod->setShortcut(QKeySequence(Qt::Key_F9));

    // layouts

    QVBoxLayout* topLayout = new QVBoxLayout();

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(topLayout);

    pageGroup->buttons().first()->setChecked(true);

    {
        QHBoxLayout* tabLayoutOuter = new QHBoxLayout();
        tabLayoutOuter->setSpacing(0);
        buttonPanel->setContentsMargins(0, 0, 0, 0);
        buttonPanelLayout->setMargin(0);
        buttonPanelLayout->setSpacing(5);
        buttonPanelLayout->addStretch();
        tabLayoutOuter->addWidget(buttonPanel);
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

    connect(genButton, &QPushButton::clicked, this, &MainWindow::generate);

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
        loadConfig(QJsonObject{});
    });
    connect(browseToSettings, &QAction::triggered, this, [this] {
        QFileInfo dir = m_mainPage->getEnv().appData;
        QProcess::startDetached("explorer.exe", QStringList() << QDir::toNativeSeparators(dir.canonicalFilePath()));
    });
    connect(quitAction, &QAction::triggered, this, [this] {
        close();
    });
    connect(quitNoSaveAction, &QAction::triggered, this, [this] {
        m_autoSave = false;
        close();
    });
    connect(generateMod, &QAction::triggered, this, &MainWindow::generate);
    connect(newSeed, &QAction::triggered, m_mainPage, &MainConfigPage::createNewSeed);
    auto updateModList = [modMergePage, this] {
        modMergePage->setModList(m_mainPage->getOtherMods());
    };
    connect(m_mainPage, &MainConfigPage::updateModList, this, updateModList);

    // misc
    updateModList();

    m_defaultConfig = m_mainPage->getEnv().appData + "config.json";
    loadConfig(m_defaultConfig);
}

MainWindow::~MainWindow()
{
    if (m_autoSave)
        saveConfig(m_defaultConfig);
}

void MainWindow::generate()
{
    const GenerationEnvironment& env = m_mainPage->getEnv();
    if (env.d2rPath.isEmpty()) {
        qWarning() << "D2R path is empty";
        return;
    }

    StorageType storage;
    if (env.isLegacy)
        storage = StorageType::D2LegacyInternal;
    else
        storage = StorageType::D2ResurrectedInternal;

    StorageType storageOut;
    if (env.isLegacy)
        storageOut = StorageType::D2LegacyFolder;
    else
        storageOut = StorageType::D2ResurrectedModFolder;

    FolderStorage outStorage(env.outPath, storageOut, env.modName);
    if (!outStorage.prepareForWrite()) {
        QMessageBox::warning(this, "error", "Failed to write data in destination folder; try to launch as admin.");
        return;
    }

    m_status->setText("Start...");
    m_status->repaint();

    KeySet         keySet;
    DataContextPtr output;
    JsonFileSet    requiredFiles;
    {
        for (auto* page : m_pages)
            if (page->isConfigEnabled())
                requiredFiles += page->extraFiles();
    }
    {
        output = m_mainStorageCache->Load(storage, env.d2rPath, g_tableNames, requiredFiles);
        if (!output) {
            QMessageBox::warning(this, "error", "Failed to read csv data from D2R folder.");
            return;
        }
        if (env.exportAllTables)
            for (auto id : output->tableSet.tables.keys())
                keySet << id;
    }
    qDebug() << "prepare ended; modifying tables. seed=" << env.seed;
    {
        QRandomGenerator rng;
        rng.seed(env.seed);
        for (auto* page : m_pages)
            if (page->isConfigEnabled())
                keySet += page->generate(*output, rng, env);
    }
    qDebug() << "prepare output data.";
    IOutputStorage::OutFileList outData;
    for (const auto& key : keySet) {
        QString data;
        writeCSV(data, output->tableSet.tables[key]);
        outData << IOutputStorage::OutFile{ data.toUtf8(), IStorage::makeTableRelativePath(key, false) };
    }
    auto iter = QMapIterator(output->jsonFiles);
    while (iter.hasNext()) {
        iter.next();
        QByteArray datastr = iter.value().toJson(QJsonDocument::Indented);
        datastr.replace(QByteArray("\xC3\x83\xC2\xBF\x63"), QByteArray("\xC3\xBF\x63")); // hack: replace color codes converter to UTF-8 from latin-1.
        outData << IOutputStorage::OutFile{ datastr, iter.key() };
    }
    QSet<QString> existent;
    for (auto& copyFile : output->copyFiles) {
        const QString src  = copyFile.srcModRoot + copyFile.relativePath;
        const QString dest = copyFile.relativePath;
        if (existent.contains(dest)) {
            QMessageBox::warning(this, "error", QString("Merge conflict in file: %1").arg(dest));
            return;
        }
        existent << dest;
        outData << IOutputStorage::OutFile{ {}, dest, src };
    }
    qDebug() << "writing output to disk.";
    outStorage.writeData(outData);

    qDebug() << "generation ends.";
    m_status->setText(QString("Mod '%1' successfully updated (%2).").arg(env.modName, QTime::currentTime().toString("mm:ss")));
}

bool MainWindow::saveConfig(const QString& filename) const
{
    qDebug() << "Save:" << filename;
    QJsonObject data;
    for (auto* page : m_pages) {
        QJsonObject pageData;
        page->writeSettings(pageData);
        data[page->settingKey()]              = pageData;
        data[page->settingKey() + "_enabled"] = page->isConfigEnabled();
    }
    QDir().mkpath(QFileInfo(filename).absolutePath());
    return writeJsonFile(filename, QJsonDocument(data));
}

bool MainWindow::loadConfig(const QString& filename)
{
    qDebug() << "Load:" << filename;
    QJsonDocument doc;
    if (!readJsonFile(filename, doc))
        return false;
    QJsonObject data = doc.object();
    return loadConfig(data);
}

bool MainWindow::loadConfig(const QJsonObject& data)
{
    for (auto* page : m_pages) {
        page->setConfigEnabled(data[page->settingKey() + "_enabled"].toBool());
        page->readSettings(data[page->settingKey()].toObject());
        m_enableButtons[page]->setChecked(page->isConfigEnabled());
    }

    return true;
}

}
