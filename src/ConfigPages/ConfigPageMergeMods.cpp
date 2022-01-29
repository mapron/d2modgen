/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMergeMods.hpp"

#include <QLabel>
#include <QListWidget>
#include <QDirIterator>

namespace D2ModGen {

ConfigPageMergeMods::ConfigPageMergeMods(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    m_modList = new QListWidget(this);
    m_modList->setSelectionMode(QListWidget::ExtendedSelection);
    addWidget(new QLabel("Select other mods you need to be merged into current:", this));
    addWidget(m_modList);
    addWidget(new QLabel("You can use CTRL+click to select multiple items.", this));
    closeLayout();
}

void ConfigPageMergeMods::setModList(QStringList mods)
{
    QJsonObject s;
    writeSettings(s);
    m_modList->clear();
    m_modList->addItems(mods);
    readSettings(s);
}

void ConfigPageMergeMods::readSettings(const QJsonObject& data)
{
    auto mods = data["modList"].toString().split(';', Qt::SkipEmptyParts);
    for (int i = 0; i < m_modList->count(); ++i)
        m_modList->item(i)->setSelected(mods.contains(m_modList->item(i)->text()));
}

void ConfigPageMergeMods::writeSettings(QJsonObject& data) const
{
    const QStringList mods = getSelected();

    data["modList"] = mods.join(';');
}

KeySet ConfigPageMergeMods::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (env.isLegacy)
        return {};

    const QStringList mods = getSelected();
    for (QString mod : mods) {
        const QString modDest = env.outPath + QString("mods/%1/%1.mpq/").arg(env.modName);
        const QString modSrc  = env.outPath + QString("mods/%1/%1.mpq/").arg(mod);

        QDir         modDir(modSrc);
        QDirIterator it(modSrc, QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            auto fi = it.fileInfo();
            if (fi.isFile()) {
                QString relPath = modDir.relativeFilePath(fi.absoluteFilePath());
                output.copyFiles << CopyFileInfo{ modSrc, relPath };
            }
            it.next();
        }
    }

    return {};
}

QStringList ConfigPageMergeMods::getSelected() const
{
    QStringList mods;
    for (int i = 0; i < m_modList->count(); ++i)
        if (m_modList->item(i)->isSelected())
            mods << m_modList->item(i)->text();
    return mods;
}

}
