/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "MainWindow.hpp"

#include "FileIOUtils.hpp"

#include "ConfigPages/MainConfigPage.hpp"
#include "ConfigPages/ConfigPageMergeMods.hpp"
#include "ConfigPages/PluginConfigPage.hpp"
#include "ConfigPageFactory.hpp"

#include "IModule.hpp"

#include "HelpToolButton.hpp"
#include "ConfigHandler.hpp"
#include "Logger.hpp"

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
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QSettings>
#include <QToolButton>
#include <QTime>

namespace D2ModGen {

namespace {

QString ensureTrailingSlash(QString value)
{
    return QString::fromStdString(::D2ModGen::ensureTrailingSlash(value.toStdString()));
}

struct PageGroup {
    QString             title;
    QList<IConfigPage*> pages;
};

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

MainWindow::MainWindow(ConfigHandler& configHandler)
    : QMainWindow(nullptr)
    , m_configHandler(configHandler)
{
    setWindowTitle("Diablo II Resurrected mod generator by mapron");

    const auto appData = ensureTrailingSlash(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

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

    m_mainPage        = new MainConfigPage(createModule(std::string(IModule::Key::testConfig)), this);
    auto* pageGroup   = new QButtonGroup(this);
    auto* buttonPanel = new QWidget(this);

    QVBoxLayout*     buttonPanelLayout = new QVBoxLayout(buttonPanel);
    QList<PageGroup> pageGroups;
    pageGroups << PageGroup{
        "", QList<IConfigPage*>{ m_mainPage }
    };
    auto createConfigPages = [this](const std::vector<std::string_view>& keys) -> QList<IConfigPage*> {
        QList<IConfigPage*> result;
        for (auto key : keys)
            result << createConfigPage(m_configHandler.getModule(key), this);
        return result;
    };
    auto createPluginConfigPages = [this]() -> QList<IConfigPage*> {
        QList<IConfigPage*> result;
        for (auto key : m_configHandler.m_pluginIds)
            result << new PluginConfigPage(m_configHandler.getModule(key), this);
        return result;
    };
    pageGroups << [this, &createConfigPages, &createPluginConfigPages]() -> QList<PageGroup> {
        return QList<PageGroup>{
            PageGroup{
                QObject::tr("Randomizers"),
                createConfigPages({
                    IModule::Key::itemRandomizer,
                    IModule::Key::monsterRandomizer,
                    IModule::Key::skillRandomizer,
                }),

            },
            PageGroup{
                QObject::tr("Make harder"),
                createConfigPages({
                    IModule::Key::monsterStats,
                    IModule::Key::monsterDensity,
                    IModule::Key::challenge,
                }),

            },
            PageGroup{
                QObject::tr("Make easier"),
                createConfigPages({
                    IModule::Key::horadricCube,
                    IModule::Key::gambling,
                    IModule::Key::character,
                    IModule::Key::qualityOfLife,
                    IModule::Key::itemDrops,
                    IModule::Key::runeDrops,
                    IModule::Key::perfectRolls,
                }),

            },
            PageGroup{
                QObject::tr("Misc"),
                createConfigPages({
                    IModule::Key::dropFiltering,
                    IModule::Key::mergePregen,
                    IModule::Key::mergePostgen,
                }),
            },
            PageGroup{
                QObject::tr("Plugins"),
                createPluginConfigPages(),
            },
        };
    }();

    for (const auto& group : pageGroups) {
        if (!group.title.isEmpty()) {
            auto* groupTitle = new QLabel(group.title, this);
            groupTitle->setObjectName("groupTitleLabel");
            increaseFontSize(groupTitle, 2, true);
            buttonPanelLayout->addWidget(groupTitle);
        }

        for (IConfigPage* page : group.pages) {
            const bool isMainPage = page == m_mainPage;
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
            if (isMainPage) {
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

            auto presets      = page->getModule().presets();
            auto presetTitles = page->pagePresets();

            if (!presets.empty()) {
                presetCombo = new QComboBox(this);
                presetCombo->addItem(tr("Select preset..."));
                while (presetTitles.size() < presets.size())
                    presetTitles << QString("Preset #%1").arg(presetTitles.size() + 1);
                for (const auto& preset : presetTitles)
                    presetCombo->addItem(preset);

                connect(presetCombo, qOverload<int>(&QComboBox::currentIndexChanged), presetCombo, [this, presetCombo, page, presets](int index) {
                    if (index <= 0)
                        return;
                    updateUIFromSettings(page, presets[index - 1]);
                    writeSettingsFromUI(page);
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
            connect(resetButton, &QPushButton::clicked, this, [this, page]() {
                updateUIFromSettings(page, {});
                writeSettingsFromUI(page);
            });
            connect(page, &IConfigPage::dataChanged, this, [this, page] {
                writeSettingsFromUI(page);
            });

            if (!isMainPage) {
                connect(headerEnabler, &QCheckBox::toggled, sideEnabler, &QCheckBox::setChecked);
                connect(sideEnabler, &QCheckBox::toggled, headerEnabler, &QCheckBox::setChecked);
                connect(headerEnabler, &QCheckBox::toggled, page, &QWidget::setEnabled);

                connect(headerEnabler, &QCheckBox::clicked, page, [this, page, headerEnabler] {
                    m_configHandler.setConfigEnabled(page->getModule().settingKey(), headerEnabler->isChecked());
                    m_delayTimer->start();
                });
                connect(sideEnabler, &QCheckBox::clicked, page, [this, page, sideEnabler] {
                    m_configHandler.setConfigEnabled(page->getModule().settingKey(), sideEnabler->isChecked());
                    m_delayTimer->start();
                });

                m_enableButtons[page] = { headerEnabler, sideEnabler };
            } else {
                connect(page, &IConfigPage::dataChanged, this, [this] {
                    PropertyTree data;
                    m_mainPage->writeSettingsFromUIMain(data);
                    m_configHandler.m_currentMainConfig = std::move(data);
                    m_delayTimer->start();
                });
            }
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
    connect(browseToSettings, &QAction::triggered, this, [this, appData] {
        QFileInfo dir = appData;
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
    auto updateModList = [this] {
        auto mods = m_mainPage->getOtherMods();
        for (auto* page : m_pages)
            page->updateModList(mods);
    };
    connect(m_mainPage, &MainConfigPage::needUpdateModList, this, updateModList);

    connect(themeActionLight, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("themeId", "light"); });
    connect(themeActionDark, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("themeId", "dark"); });
    connect(langActionEn, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("langId", "en_US"); });
    connect(langActionRu, &QAction::triggered, this, [setNewAppValue] { setNewAppValue("langId", "ru_RU"); });

    // misc
    //updateModList();

    m_defaultConfig = appData + "config.json";
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
    m_status->setText(tr("Start..."));
    m_status->repaint();

    auto result = m_configHandler.generate();
    if (!result.m_success) {
        if (!result.m_error.empty())
            QMessageBox::warning(this, "error", QString::fromStdString(result.m_error));
        return;
    }

    m_status->setText(tr("Mod '%1' successfully updated (%2).")
                          .arg(QString::fromStdString(m_configHandler.getEnv().modName), QTime::currentTime().toString("mm:ss")));
}

bool MainWindow::saveConfig(const QString& filename) const
{
    Logger() << "Save:" << filename.toStdString();
    return m_configHandler.saveConfig(filename.toStdString());
}

bool MainWindow::loadConfig(const QString& filename)
{
    const auto result = m_configHandler.loadConfig(filename.toStdString());
    updateUIFromSettings();
    return result;
}

bool MainWindow::loadConfig(const PropertyTree& data)
{
    const auto result = m_configHandler.loadConfig(data);
    updateUIFromSettings();
    return result;
}

MainWindow::AppSettings MainWindow::getAppSettings()
{
    auto ini = makeAppSettings();
    return { ini.value("langId", "en_US").toString(), ini.value("themeId", "dark").toString() };
}

void MainWindow::pushUndo(const PropertyTree& data)
{
    Logger() << "pushing undo, current undo size=" << m_undo.size();
    m_undo << data;
    while (m_undo.size() > 50)
        m_undo.removeFirst();

    updateUndoAction();
}

void MainWindow::pushUndoCurrent()
{
    PropertyTree data;
    m_configHandler.saveConfig(data);
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

void MainWindow::updateUIFromSettings()
{
    for (auto* page : m_pages) {
        auto        key  = page->getModule().settingKey();
        const auto& mod  = m_configHandler.m_modules[key];
        const auto& data = mod.m_currentConfig;
        updateUIFromSettings(page, data);
        for (auto* cb : m_enableButtons[page])
            cb->setChecked(mod.m_enabled);
    }
    m_mainPage->updateUIFromSettingsMain(m_configHandler.m_currentMainConfig);
}

void MainWindow::updateUIFromSettings(IConfigPage* page, const PropertyTree& currentConfig)
{
    PropertyTree realData;
    realData.merge(page->getModule().defaultValues());
    realData.merge(currentConfig);

    page->updateUIFromSettings(realData);
}

void MainWindow::writeSettingsFromUI(IConfigPage* page)
{
    PropertyTree realData;
    page->writeSettingsFromUI(realData);
    realData.removeEqualValues(page->getModule().defaultValues());
    //    {
    //        std::string buffer;
    //        writeJsonToBuffer(buffer, realData);
    //        Logger() << "WRITE JSON data:" << buffer.c_str();
    //    }

    m_configHandler.m_modules.at(page->getModule().settingKey()).m_currentConfig = std::move(realData);

    m_delayTimer->start();
}

}
