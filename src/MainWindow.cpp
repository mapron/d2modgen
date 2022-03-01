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

#include "Storage/StorageCache.hpp"
#include "Storage/FolderStorage.hpp"

#include "HelpToolButton.hpp"

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
#include <QComboBox>
#include <QSettings>
#include <QToolButton>

namespace D2ModGen {

namespace {

QSettings makeAppSettings()
{
    return QSettings(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/settings.ini", QSettings::IniFormat);
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

MainWindow::MainWindow(bool autoSave)
    : QMainWindow(nullptr)
    , m_mainStorageCache(new StorageCache())
    , m_autoSave(autoSave)
{
    setWindowTitle("Diablo II Resurrected mod generator by mapron");

    m_delayTimer = new DelayedTimer(
        1000, [this] { pushUndoCurrent(); }, this);

    // widgets
    QStackedWidget* stackedWidget    = new QStackedWidget(this);
    QPushButton*    genButton        = new QPushButton(tr("Generate"), this);
    auto            increaseFontSize = [](QWidget* w, int pointPlus, bool bold) {
        auto f = w->font();
        f.setPointSize(f.pointSize() + pointPlus);
        f.setBold(bold);
        w->setFont(f);
    };
    increaseFontSize(genButton, 4, false);

    m_status = new QLabel(tr("Status label."), this);

    m_mainPage         = new MainConfigPage(this);
    auto* modMergePre  = new ConfigPageMergeModsPreload(this);
    auto* modMergePost = new ConfigPageMergeModsPostGen(this);
    auto* pageGroup    = new QButtonGroup(this);
    auto* buttonPanel  = new QWidget(this);

    QVBoxLayout*     buttonPanelLayout = new QVBoxLayout(buttonPanel);
    QList<PageGroup> pageGroups;
    pageGroups << PageGroup{
        "", QList<IConfigPage*>{ m_mainPage }
    };
    pageGroups << CreateConfigPages(this);
    pageGroups << PageGroup{
        "", QList<IConfigPage*>{ modMergePre, modMergePost }
    };

    for (const auto& group : pageGroups) {
        if (!group.title.isEmpty()) {
            auto* groupTitle = new QLabel(group.title, this);
            groupTitle->setObjectName("groupTitleLabel");
            increaseFontSize(groupTitle, 2, true);
            buttonPanelLayout->addWidget(groupTitle);
        }

        for (IConfigPage* page : group.pages) {
            m_pages << page;
            QPushButton* pageButton = new QPushButton(page->caption(), this);
            pageButton->setCheckable(true);
            pageGroup->addButton(pageButton);
            increaseFontSize(pageButton, 2, false);

            QPushButton* resetButton   = new QPushButton(tr("Reset to default"), this);
            QCheckBox*   headerEnabler = new QCheckBox(tr("Enable this tab"), this);
            QCheckBox*   sideEnabler   = new QCheckBox("", this);
            QComboBox*   presetCombo   = nullptr;
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

            auto* pageHelp = new HelpToolButton(page->pageHelp(), this);

            QVBoxLayout* pageWrapperMain = new QVBoxLayout(pageWrapper);
            pageWrapperMain->setMargin(8);
            pageWrapperMain->setSpacing(10);
            QHBoxLayout* pageWrapperHeader       = new QHBoxLayout();
            QHBoxLayout* pageWrapperPresetHeader = new QHBoxLayout();
            pageWrapperHeader->addWidget(pageHelp);
            pageWrapperHeader->addWidget(caption);
            pageWrapperHeader->addStretch();
            pageWrapperHeader->addWidget(headerEnabler);
            pageWrapperHeader->addWidget(resetButton);
            pageWrapperMain->addLayout(pageWrapperHeader);
            pageWrapperMain->addLayout(pageWrapperPresetHeader);
            pageWrapperMain->addWidget(page);
            stackedWidget->addWidget(pageWrapper);

            auto presets = page->pagePresets();

            if (!presets.isEmpty()) {
                presetCombo = new QComboBox(this);
                presetCombo->addItem(tr("Select preset..."));
                for (const auto& preset : presets) {
                    presetCombo->addItem(preset.title, preset.data);
                }
                connect(presetCombo, qOverload<int>(&QComboBox::currentIndexChanged), presetCombo, [presetCombo, page](int index) {
                    auto data = presetCombo->itemData(index);
                    if (!data.isValid())
                        return;
                    page->readSettings(DataContext::qjsonToProperty(data.toJsonObject()));
                });
                pageWrapperPresetHeader->addStretch();
                pageWrapperPresetHeader->addWidget(new QLabel(tr("Do not know where to start? Select a preset:"), this));
                pageWrapperPresetHeader->addWidget(presetCombo);
            }

            QHBoxLayout* buttonPanelRow = new QHBoxLayout();
            buttonPanelRow->setMargin(0);
            buttonPanelRow->setSpacing(3);
            buttonPanelRow->addWidget(sideEnabler);
            buttonPanelRow->addWidget(pageButton, 1);
            buttonPanelLayout->addLayout(buttonPanelRow);

            connect(pageButton, &QPushButton::clicked, this, [pageWrapper, stackedWidget, presetCombo]() {
                stackedWidget->setCurrentWidget(pageWrapper);
                if (presetCombo)
                    presetCombo->setCurrentIndex(0);
            });
            connect(resetButton, &QPushButton::clicked, this, [page]() {
                page->readSettings({});
            });
            connect(headerEnabler, &QCheckBox::toggled, sideEnabler, &QCheckBox::setChecked);
            connect(sideEnabler, &QCheckBox::toggled, headerEnabler, &QCheckBox::setChecked);
            connect(headerEnabler, &QCheckBox::toggled, page, &IConfigPage::setConfigEnabled);
            connect(headerEnabler, &QCheckBox::clicked, m_delayTimer, &DelayedTimer::start);
            connect(sideEnabler, &QCheckBox::clicked, m_delayTimer, &DelayedTimer::start);
            if (page->canBeDisabled())
                connect(headerEnabler, &QCheckBox::toggled, page, &QWidget::setEnabled);
            connect(page, &IConfigPage::dataChanged, this, [this] { m_delayTimer->start(); });
            m_enableButtons[page] = headerEnabler;
        }
    }

    QMenuBar* mainMenu         = menuBar();
    QMenu*    fileMenu         = mainMenu->addMenu(tr("File"));
    QMenu*    actionsMenu      = mainMenu->addMenu(tr("Actions"));
    QAction*  about            = mainMenu->addAction(tr("About"));
    QAction*  saveConfigAction = fileMenu->addAction(tr("Save config..."));
    QAction*  loadConfigAction = fileMenu->addAction(tr("Load config..."));
    QAction*  clearConfig      = fileMenu->addAction(tr("Clear config"));
    QAction*  browseToSettings = fileMenu->addAction(tr("Browse to settings folder"));
    fileMenu->addSeparator();
    QAction* quitNoSaveAction = fileMenu->addAction(tr("Quit without saving"));
    QAction* quitAction       = fileMenu->addAction(tr("Save and quit"));
    QAction* generateMod      = actionsMenu->addAction(tr("Generate mod"));
    QAction* newSeed          = actionsMenu->addAction(tr("Create seed"));
    m_undoAction              = actionsMenu->addAction(tr("Undo"));
    QMenu*   themeMenu        = actionsMenu->addMenu(tr("Theme"));
    QMenu*   langMenu         = actionsMenu->addMenu(tr("Language"));
    QAction* themeActionLight = themeMenu->addAction("Light");
    QAction* themeActionDark  = themeMenu->addAction("Dark");
    QAction* langActionEn     = langMenu->addAction("English");
    QAction* langActionRu     = langMenu->addAction("Russian");

    saveConfigAction->setShortcuts(QKeySequence::Save);
    loadConfigAction->setShortcuts(QKeySequence::Open);
    clearConfig->setShortcuts(QKeySequence::New);
    newSeed->setShortcuts(QKeySequence::Refresh);
    m_undoAction->setShortcut(QKeySequence::Undo);
    generateMod->setShortcut(QKeySequence(Qt::Key_F9));

    auto updateMenuState = [themeActionLight, themeActionDark, langActionEn, langActionRu]() {
        auto       appSettings = getAppSettings();
        const bool themeLight  = appSettings.m_themeId == "light";
        const bool langEn      = appSettings.m_langId == "en_US";
        themeActionLight->setCheckable(true);
        themeActionDark->setCheckable(true);
        langActionEn->setCheckable(true);
        langActionRu->setCheckable(true);
        themeActionLight->setChecked(themeLight);
        themeActionDark->setChecked(!themeLight);
        langActionEn->setChecked(langEn);
        langActionRu->setChecked(!langEn);
    };
    auto setNewAppValue = [updateMenuState, this](const QString& key, const QString& val) {
        auto ini = makeAppSettings();
        ini.setValue(key, val);
        QMessageBox::information(this, "Notice", tr("You need to restart for changes apply."));
    };
    updateMenuState();

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
        if (!file.isEmpty()) {
            loadConfig(file);
            pushUndoCurrent();
        }
    });
    connect(clearConfig, &QAction::triggered, this, [this] {
        loadConfig(PropertyTree{});
        pushUndo(PropertyTree{});
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
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::makeUndo);
    auto updateModList = [modMergePre, modMergePost, this] {
        modMergePre->setModList(m_mainPage->getOtherMods());
        modMergePost->setModList(m_mainPage->getOtherMods());
    };
    connect(m_mainPage, &MainConfigPage::updateModList, this, updateModList);

    connect(themeActionLight, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("themeId", "light"); });
    connect(themeActionDark, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("themeId", "dark"); });
    connect(langActionEn, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("langId", "en_US"); });
    connect(langActionRu, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("langId", "ru_RU"); });

    // misc
    updateModList();

    m_defaultConfig = m_mainPage->getEnv().appData + "config.json";
    loadConfig(m_defaultConfig);
    pushUndoCurrent();
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

    const StorageType storage    = (env.isLegacy) ? StorageType::D2LegacyInternal : StorageType::D2ResurrectedInternal;
    const StorageType storageOut = (env.isLegacy) ? StorageType::D2LegacyFolder : StorageType::D2ResurrectedModFolder;

    FolderStorage outStorage(env.outPath, storageOut, env.modName);
    if (!outStorage.prepareForWrite()) {
        QMessageBox::warning(this, "error", tr("Failed to write data in destination folder; try to launch as admin."));
        return;
    }

    auto mergeContext = [this, &env](DataContext& targetContext, const ExtraDependencies::Source& source) -> bool {
        const auto    logInfo = source.srcRoot + " / " + source.modname;
        const bool    isMod   = source.type == StorageType::D2ResurrectedModFolder;
        const auto    root    = isMod ? env.outPath : source.srcRoot;
        FolderStorage inStorage(root, source.type, isMod ? source.modname : "");
        auto          storedData = inStorage.readData({});
        if (!storedData.valid) {
            qWarning() << "Failed to read data files from D2 folder:" << logInfo;
            return false;
        }
        DataContext dataContext;
        if (!dataContext.readData(storedData)) {
            qWarning() << "Failed to parse files into context:" << logInfo;
            return false;
        }
        if (!targetContext.mergeWith(dataContext, source.policy)) {
            qWarning() << "Merge failed:" << logInfo;
            return false;
        }
        return true;
    };

    m_status->setText(tr("Start..."));
    m_status->repaint();

    DataContext output;

    PreGenerationContext pregenContext;
    {
        for (auto* page : m_pages)
            if (page->isConfigEnabled())
                page->gatherInfo(pregenContext, env);
    }
    {
        const IStorage::StoredData data = m_mainStorageCache->load(storage, env.d2rPath, pregenContext.m_extraJson);
        if (!data.valid) {
            QMessageBox::warning(this, "error", tr("Failed to read data files from D2 folder."));
            return;
        }
        if (!output.readData(data)) {
            QMessageBox::warning(this, "error", tr("Failed parse D2 data files."));
            return;
        }
        if (env.exportAllTables)
            for (auto& p : output.tableSet.tables)
                p.second.forceOutput = true;
    }
    qDebug() << "Loading pre-gen data.";
    {
        for (const auto& source : pregenContext.m_preGen.m_sources)
            if (!mergeContext(output, source)) {
                QMessageBox::warning(this, "error", tr("Failed to merge with source: %1 / %2").arg(source.srcRoot, source.modname));
                return;
            }
    }
    qDebug() << "prepare ended; Starting generate phase. seed=" << env.seed;
    {
        QRandomGenerator rng;
        rng.seed(env.seed);
        for (auto* page : m_pages)
            if (page->isConfigEnabled()) {
                qDebug() << "start page:" << page->settingKey().c_str();
                page->generate(output, rng, env);
            }
    }
    qDebug() << "Loading post-gen data.";
    {
        for (const auto& source : pregenContext.m_postGen.m_sources)
            if (!mergeContext(output, source)) {
                QMessageBox::warning(this, "error", tr("Failed to merge with source: %1 / %2").arg(source.srcRoot, source.modname));
                return;
            }
    }
    qDebug() << "prepare output data.";

    IStorage::StoredData outData;
    if (!output.writeData(outData)) {
        qDebug() << "Failed to prepare data buffers"; // highly unlikely.
        return;
    }

    qDebug() << "writing output to disk.";
    if (!outStorage.writeData(outData)) {
        QMessageBox::warning(this, "error", tr("Failed write output data to disk"));
        return;
    }

    qDebug() << "generation ends.";
    m_status->setText(tr("Mod '%1' successfully updated (%2).").arg(env.modName, QTime::currentTime().toString("mm:ss")));
}

bool MainWindow::saveConfig(const QString& filename) const
{
    qDebug() << "Save:" << filename;
    PropertyTree data;
    for (auto* page : m_pages) {
        page->writeSettings(data[page->settingKey()]);
        data[page->settingKey() + "_enabled"] = PropertyTreeScalar{ page->isConfigEnabled() };
    }
    QDir().mkpath(QFileInfo(filename).absolutePath());
    return writeJsonFile(filename, DataContext::propertyToDoc(data));
}

bool MainWindow::loadConfig(const QString& filename)
{
    qDebug() << "Load:" << filename;
    QJsonDocument doc;
    if (!readJsonFile(filename, doc)) {
        loadConfig(PropertyTree{});
        return false;
    }
    return loadConfig(DataContext::qjsonToProperty(doc));
}

bool MainWindow::loadConfig(const PropertyTree& data)
{
    for (auto* page : m_pages) {
        page->setConfigEnabled(data.value(page->settingKey() + "_enabled", false).toBool());
        if (data.contains(page->settingKey()))
            page->readSettings(data[page->settingKey()]);
        else
            page->readSettings(PropertyTree{});
        m_enableButtons[page]->setChecked(page->isConfigEnabled());
    }
    return true;
}

MainWindow::AppSettings MainWindow::getAppSettings()
{
    auto ini = makeAppSettings();
    return { ini.value("langId", "en_US").toString(), ini.value("themeId", "dark").toString() };
}

void MainWindow::pushUndo(const PropertyTree& data)
{
    qDebug() << "pushing undo, current undo size=" << m_undo.size();
    m_undo << data;
    while (m_undo.size() > 50)
        m_undo.removeFirst();

    updateUndoAction();
}

void MainWindow::pushUndoCurrent()
{
    PropertyTree data;
    for (auto* page : m_pages) {
        PropertyTree pageData;
        page->writeSettings(pageData);
        data[page->settingKey()]              = pageData;
        data[page->settingKey() + "_enabled"] = PropertyTreeScalar{ page->isConfigEnabled() };
    }
    pushUndo(data);
}

void MainWindow::makeUndo()
{
    m_delayTimer->stop();
    m_undo.removeLast();
    loadConfig(m_undo.last());
    updateUndoAction();
}

void MainWindow::updateUndoAction()
{
    m_undoAction->setEnabled(m_undo.size() > 1);
}

}
