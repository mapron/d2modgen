/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMergeMods.hpp"

#include "FileIOUtils.hpp"

#include <QLabel>
#include <QListWidget>
#include <QDirIterator>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QBoxLayout>

namespace D2ModGen {

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
    });
}

void ConfigPageMergeModsItem::setModList(const QStringList& mods)
{
    QString prev = m_modSelect->currentText();
    m_modSelect->clear();
    m_modSelect->addItem("");
    m_modSelect->addItems(mods);

    setMod(prev);
}

void ConfigPageMergeModsItem::readSettings(const QJsonObject& data)
{
    m_typeSelect->setCurrentIndex(std::max(0, m_typeIndex.indexOf(static_cast<StorageType>(data["type"].toInt()))));
    m_policySelect->setCurrentIndex(std::max(0, m_policyIndex.indexOf(static_cast<ConflictPolicy>(data["policy"].toInt()))));
    setMod(data["mod"].toString());
    m_folderCSV->setText(data["folder"].toString());
}

void ConfigPageMergeModsItem::writeSettings(QJsonObject& data) const
{
    data["type"]   = static_cast<int>(m_typeIndex[m_typeSelect->currentIndex()]);
    data["policy"] = static_cast<int>(m_policyIndex[m_policySelect->currentIndex()]);
    data["mod"]    = m_modSelect->currentText();
    data["folder"] = m_folderCSV->text();
}

void ConfigPageMergeModsItem::gatherInfoInternal(ExtraDependencies& output, const GenerationEnvironment& env) const
{
    const auto    type       = m_typeIndex[m_typeSelect->currentIndex()];
    const auto    policy     = m_policyIndex[m_policySelect->currentIndex()];
    const QString folderRoot = ensureTrailingSlash(m_folderCSV->text());
    const QString modname    = m_modSelect->currentText();
    output.m_sources << ExtraDependencies::Source{
        type,
        policy,
        folderRoot,
        modname,
    };
}

void ConfigPageMergeModsItem::setMod(const QString& mod)
{
    m_modSelect->setCurrentIndex(0);
    for (int i = 0; i < m_modSelect->count(); ++i) {
        if (m_modSelect->itemText(i) == mod) {
            m_modSelect->setCurrentIndex(i);
            break;
        }
    }
}

ConfigPageMergeMods::ConfigPageMergeMods(QWidget* parent)
    : ConfigPageAbstract(parent)
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

    addWidget(topRow);
    addWidget(bottomRow);
    closeLayout();
}

void ConfigPageMergeMods::setModList(const QStringList& mods)
{
    m_modList = mods;
    for (auto* item : m_items)
        item->setModList(mods);
}

void ConfigPageMergeMods::readSettings(const QJsonObject& data)
{
    const int count = data["sourceCount"].toInt();
    m_countSpinbox->setValue(count);
    for (int i = 0; i < count; ++i) {
        const QJsonObject& itemData = data[QString("item_%1").arg(i)].toObject();
        m_items[i]->readSettings(itemData);
    }
}

void ConfigPageMergeMods::writeSettings(QJsonObject& data) const
{
    const int count     = m_countSpinbox->value();
    data["sourceCount"] = count;
    for (int i = 0; i < count; ++i) {
        QJsonObject itemData;
        m_items[i]->writeSettings(itemData);
        data[QString("item_%1").arg(i)] = itemData;
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

void ConfigPageMergeMods::gatherInfoInternal(ExtraDependencies& output, const GenerationEnvironment& env) const
{
    for (auto* item : m_items) {
        item->gatherInfoInternal(output, env);
    }
}

void ConfigPageMergeMods::setItemsCount(int count)
{
    if (m_items.size() == count)
        return;

    while (m_items.size() > count)
        delete m_items.takeLast();
    while (m_items.size() < count)
        m_items << new ConfigPageMergeModsItem(this);

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
