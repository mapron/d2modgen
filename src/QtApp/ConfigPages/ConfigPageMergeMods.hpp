/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ConfigPageAbstract.hpp"

#include <QFrame>

class QComboBox;
class QSpinBox;
class QLineEdit;

namespace D2ModGen {

class ConfigPageMergeModsItem : public QFrame {
    Q_OBJECT
public:
    ConfigPageMergeModsItem(QWidget* parent);

    void setModList(const QStringList& mods);

    void readSettings(const PropertyTree& data);
    void writeSettings(PropertyTree& data) const;

signals:
    void dataChanged();

private:
    void setMod(const QString& mod);
    void checkForChange();

private:
    const QList<StorageType>    m_typeIndex;
    const QList<ConflictPolicy> m_policyIndex;

    QComboBox* m_typeSelect;
    QComboBox* m_policySelect;
    QComboBox* m_modSelect;
    QLineEdit* m_folderCSV;
    bool       m_readingSettings = false;
};

class ConfigPageMergeMods : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMergeMods(QWidget* parent);

    void setModList(const QStringList& mods);

    // IConfigPage interface
public:
    void readSettings(const PropertyTree& data) override;
    void writeSettings(PropertyTree& data) const override;

    QString pageHelp() const override;

private:
    void setItemsCount(int count);
    void updateLayout();

private:
    QList<ConfigPageMergeModsItem*> m_items;
    QSpinBox*                       m_countSpinbox;
    QVBoxLayout*                    m_bottomRowLayout;
    QStringList                     m_modList;
};

class ConfigPageMergeModsPreload : public ConfigPageMergeMods {
    Q_OBJECT
public:
    static constexpr const std::string_view key = IModule::Key::mergePregen;

    ConfigPageMergeModsPreload(QWidget* parent)
        : ConfigPageMergeMods(parent)
    {
        initModule();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Pre-gen data");
    }
    std::string settingKey() const override
    {
        return std::string(key);
    }
};

class ConfigPageMergeModsPostGen : public ConfigPageMergeMods {
    Q_OBJECT
public:
    static constexpr const std::string_view key = IModule::Key::mergePostgen;

    ConfigPageMergeModsPostGen(QWidget* parent)
        : ConfigPageMergeMods(parent)
    {
        initModule();
    }

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Post-gen data");
    }
    std::string settingKey() const override
    {
        return std::string(key);
    }
};

}