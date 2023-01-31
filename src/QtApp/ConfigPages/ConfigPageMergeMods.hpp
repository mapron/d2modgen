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

    void readSettings(const Mernel::PropertyTree& data);
    void writeSettings(Mernel::PropertyTree& data) const;

signals:
    void dataChanged();

private:
    void setMod(const QString& mod);
    void checkForChange();

private:
    const QList<StorageType>    m_typeIndex;
    const QList<ConflictPolicy> m_policyIndex;

    QString    m_currentMod;
    QComboBox* m_typeSelect;
    QComboBox* m_policySelect;
    QComboBox* m_modSelect;
    QLineEdit* m_folderCSV;
    bool       m_readingSettings = false;
};

class ConfigPageMergeMods : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMergeMods(const std::string& localeId, const IModule::Ptr& module, QWidget* parent);

    void setModList(const QStringList& mods);

    void updateModList(const QStringList& mods) override;

    // IConfigPage interface
public:
    void updateUIFromSettings(const Mernel::PropertyTree& data) override;
    void writeSettingsFromUI(Mernel::PropertyTree& data) const override;

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

    ConfigPageMergeModsPreload(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
        : ConfigPageMergeMods(localeId, module, parent)
    {
    }
};

class ConfigPageMergeModsPostGen : public ConfigPageMergeMods {
    Q_OBJECT
public:
    static constexpr const std::string_view key = IModule::Key::mergePostgen;

    ConfigPageMergeModsPostGen(const std::string& localeId, const IModule::Ptr& module, QWidget* parent)
        : ConfigPageMergeMods(localeId, module, parent)
    {
    }
};

}
