/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMergeMods.hpp"

#include <QLabel>
#include <QListWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>

namespace D2ModGen {

namespace {
const bool s_init  = pageRegisterHelper<ConfigPageMergeModsPreload>();
const bool s_init2 = pageRegisterHelper<ConfigPageMergeModsPostGen>();
}

ConfigPageMergeModsItem::ConfigPageMergeModsItem(QWidget* parent)
    : QFrame(parent)
    , m_typeIndex({ StorageType::CsvFolder, StorageType::D2ResurrectedModFolder })
    , m_policyIndex({
          ConflictPolicy::Replace,
          ConflictPolicy::Append,
          ConflictPolicy::Update,
          ConflictPolicy::AppendNew,
          ConflictPolicy::Merge,
          ConflictPolicy::Skip,
          ConflictPolicy::RaiseError,
      })
{
    setFrameStyle(int(QFrame::Box) | int(QFrame::Raised));
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* topLayout  = new QHBoxLayout();
    mainLayout->addLayout(topLayout);
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addLayout(bottomLayout);

    m_typeSelect = new QComboBox(this);
    m_typeSelect->addItems({ tr("CSV files folder"), tr("D2R mod") });
    m_typeSelect->setEditable(false);

    m_policySelect = new QComboBox(this);
    m_policySelect->addItems({
        tr("Replace - clear previous data"),
        tr("Append - place all data at the end"),
        tr("Update - overwrite records with same key"),
        tr("AppendNew - add non-existent data to the end"),
        tr("Merge - try Update, then AppendNew"),
        tr("Skip"),
        tr("Raise error"),
    });
    m_policySelect->setEditable(false);

    QWidget*     modSelectWrap       = new QWidget(this);
    QHBoxLayout* modSelectWrapLayout = new QHBoxLayout(modSelectWrap);
    modSelectWrapLayout->setMargin(0);
    m_modSelect = new QComboBox(this);
    modSelectWrapLayout->addWidget(new QLabel(tr("Mod name:"), this));
    modSelectWrapLayout->addWidget(m_modSelect);
    modSelectWrap->hide();

    QWidget*     folderSelectWrap       = new QWidget(this);
    QHBoxLayout* folderSelectWrapLayout = new QHBoxLayout(folderSelectWrap);
    folderSelectWrapLayout->setMargin(0);
    m_folderCSV = new QLineEdit(this);
    m_folderCSV->setMinimumWidth(300);
    folderSelectWrapLayout->addWidget(new QLabel(tr("Folder with .txt files:"), this));
    folderSelectWrapLayout->addWidget(m_folderCSV);

    topLayout->addWidget(new QLabel(tr("Type:"), this));
    topLayout->addWidget(m_typeSelect);
    topLayout->addStretch();
    topLayout->addWidget(new QLabel(tr("Action on conflict:"), this));
    topLayout->addWidget(m_policySelect);
    bottomLayout->addWidget(modSelectWrap);
    bottomLayout->addWidget(folderSelectWrap);
    bottomLayout->addStretch();

    connect(m_typeSelect, qOverload<int>(&QComboBox::currentIndexChanged), this, [modSelectWrap, folderSelectWrap, this](int index) {
        modSelectWrap->setVisible(m_typeIndex[index] == StorageType::D2ResurrectedModFolder);
        folderSelectWrap->setVisible(m_typeIndex[index] == StorageType::CsvFolder);
        checkForChange();
    });
    connect(m_policySelect, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConfigPageMergeModsItem::checkForChange);
    connect(m_folderCSV, &QLineEdit::textEdited, this, &ConfigPageMergeModsItem::checkForChange);
    connect(m_modSelect, qOverload<int>(&QComboBox::currentIndexChanged), this, [this] { 
        m_currentMod = m_modSelect->currentText();
        emit checkForChange(); 
    });
}

void ConfigPageMergeModsItem::setModList(const QStringList& mods)
{
    m_modSelect->blockSignals(true);
    m_modSelect->clear();
    m_modSelect->addItem("");
    m_modSelect->addItems(mods);
    m_modSelect->blockSignals(false);
    setMod(m_currentMod);
}

void ConfigPageMergeModsItem::readSettings(const PropertyTree& data)
{
    m_readingSettings = true;
    m_typeSelect->setCurrentIndex(std::max(0, m_typeIndex.indexOf(static_cast<StorageType>(data["type"].getScalar().toInt()))));
    m_policySelect->setCurrentIndex(std::max(0, m_policyIndex.indexOf(static_cast<ConflictPolicy>(data["policy"].getScalar().toInt()))));
    setMod(QString::fromStdString(data["mod"].getScalar().toString()));
    m_folderCSV->setText(QString::fromStdString(data["folder"].getScalar().toString()));
    m_readingSettings = false;
}

void ConfigPageMergeModsItem::writeSettings(PropertyTree& data) const
{
    data["type"]   = PropertyTreeScalar{ static_cast<int64_t>(m_typeIndex[m_typeSelect->currentIndex()]) };
    data["policy"] = PropertyTreeScalar{ static_cast<int64_t>(m_policyIndex[m_policySelect->currentIndex()]) };
    data["mod"]    = PropertyTreeScalar{ m_currentMod.toStdString() };
    data["folder"] = PropertyTreeScalar{ m_folderCSV->text().toStdString() };
}

void ConfigPageMergeModsItem::setMod(const QString& mod)
{
    m_currentMod = mod;
    m_modSelect->setCurrentIndex(0);
    for (int i = 0; i < m_modSelect->count(); ++i) {
        if (m_modSelect->itemText(i) == mod) {
            m_modSelect->setCurrentIndex(i);
            break;
        }
    }
}

void ConfigPageMergeModsItem::checkForChange()
{
    if (m_readingSettings)
        return;
    emit dataChanged();
}

ConfigPageMergeMods::ConfigPageMergeMods(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
    : ConfigPageAbstract(localeId, module, parent)
{
    addWidget(new QLabel(tr("Overall order of data load is: D2 source from Main settings ->\n"
                            "-> Pre-gen data -> modgen features -> Post-gen data -> output folder."),
                         this));

    QWidget*     topRow       = new QWidget(this);
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRow);
    m_countSpinbox            = new QSpinBox(this);
    m_countSpinbox->setMinimum(0);
    m_countSpinbox->setMaximum(9);
    m_countSpinbox->setValue(0);
    topRowLayout->addWidget(new QLabel("Data source count (0-9):", this));
    topRowLayout->addWidget(m_countSpinbox);
    topRowLayout->addStretch();

    QWidget* bottomRow = new QWidget(this);
    m_bottomRowLayout  = new QVBoxLayout(bottomRow);

    connect(m_countSpinbox, qOverload<int>(&QSpinBox::valueChanged), this, &ConfigPageMergeMods::setItemsCount);
    connect(m_countSpinbox, qOverload<int>(&QSpinBox::valueChanged), this, &ConfigPageMergeMods::dataChanged);

    addWidget(topRow);
    addWidget(bottomRow);
    closeLayout();
}

void ConfigPageMergeMods::updateModList(const QStringList& mods)
{
    m_modList = mods;
    for (auto* item : m_items)
        item->setModList(mods);
}

void ConfigPageMergeMods::updateUIFromSettings(const PropertyTree& data)
{
    const int count = data.value("sourceCount", 0).toInt();
    m_countSpinbox->blockSignals(true);
    m_countSpinbox->setValue(count);
    m_countSpinbox->blockSignals(false);
    setItemsCount(count);
    for (int i = 0; i < count; ++i) {
        const PropertyTree& itemData = data[QString("item_%1").arg(i).toStdString()];
        m_items[i]->readSettings(itemData);
    }
}

void ConfigPageMergeMods::writeSettingsFromUI(PropertyTree& data) const
{
    const int64_t count = m_countSpinbox->value();
    data["sourceCount"] = PropertyTreeScalar{ count };
    for (int i = 0; i < count; ++i) {
        PropertyTree& itemData = data[QString("item_%1").arg(i).toStdString()];
        m_items[i]->writeSettings(itemData);
    }
}

QString ConfigPageMergeMods::pageHelp() const
{
    return tr("You can add aditional steps to mod generation, prior or after the d2modgen work\n"
              "Two additional type of sources are supported:\n"
              "1. folder with .txt files\n"
              "2. another D2R mod\n"
              "After selecting source type, you need to specify conflict resolution, \n"
              "for the case additional source contains a file which already been generated.");
}

void ConfigPageMergeMods::setItemsCount(int count)
{
    if (m_items.size() == count)
        return;

    while (m_items.size() > count)
        delete m_items.takeLast();
    while (m_items.size() < count) {
        auto* item = new ConfigPageMergeModsItem(this);
        m_items << item;
        connect(item, &ConfigPageMergeModsItem::dataChanged, this, &IConfigPage::dataChanged);
    }

    updateLayout();
}

void ConfigPageMergeMods::updateLayout()
{
    QLayoutItem* item;
    while ((item = m_bottomRowLayout->takeAt(0)) != nullptr)
        delete item;

    for (auto* item : m_items) {
        item->setModList(m_modList);
        m_bottomRowLayout->addWidget(item);
    }
}
}
