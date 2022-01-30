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

    void readSettings(const QJsonObject& data);
    void writeSettings(QJsonObject& data) const;

    void gatherInfoInternal(ExtraDependencies& output, const GenerationEnvironment& env) const;

private:
    void setMod(const QString& mod);

private:
    const QList<StorageType>    m_typeIndex;
    const QList<ConflictPolicy> m_policyIndex;

    QComboBox* m_typeSelect;
    QComboBox* m_policySelect;
    QComboBox* m_modSelect;
    QLineEdit* m_folderCSV;
};

class ConfigPageMergeMods : public ConfigPageAbstract {
    Q_OBJECT
public:
    ConfigPageMergeMods(QWidget* parent);

    void setModList(const QStringList& mods);

    // IConfigPage interface
public:
    void readSettings(const QJsonObject& data) override;
    void writeSettings(QJsonObject& data) const override;

    void generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const override {}

protected:
    void gatherInfoInternal(ExtraDependencies& output, const GenerationEnvironment& env) const;

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
    using ConfigPageMergeMods::ConfigPageMergeMods;

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Pre-gen data");
    }
    QString settingKey() const override
    {
        return "mergePregen";
    }
    void gatherInfo(PreGenerationContext& output, const GenerationEnvironment& env) const override
    {
        gatherInfoInternal(output.m_preGen, env);
    }
};

class ConfigPageMergeModsPostGen : public ConfigPageMergeMods {
    Q_OBJECT
public:
    using ConfigPageMergeMods::ConfigPageMergeMods;

    // IConfigPage interface
public:
    QString caption() const override
    {
        return tr("Post-gen data");
    }
    QString settingKey() const override
    {
        return "mergePostgen";
    }
    void gatherInfo(PreGenerationContext& output, const GenerationEnvironment& env) const override
    {
        gatherInfoInternal(output.m_postGen, env);
    }
};

}
