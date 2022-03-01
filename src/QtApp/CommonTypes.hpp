/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QStringList>
#include <QMap>
#include <QSet>
#include <QList>
#include <QWidget>
#include <QRandomGenerator>

#include <vector>

namespace D2ModGen {

using StringVector = std::vector<std::string>;

enum class StorageType
{
    D2ResurrectedInternal,  // using CASC storage from D2R Installation,
    D2LegacyInternal,       // using Storm storage from D2 Legacy Installation (patch_d2.mpq)
    D2ResurrectedModFolder, // folder containing D2R mod structure (usually in mods/name/name.mpq)
    D2LegacyFolder,         // Folder containing 'data' folder where data can be extracted.
    CsvFolder,              // Folder containing plain 'txt' files. Can not be selected as output.
};

struct GenerationEnvironment {
    QString  modName;
    QString  d2rPath;
    QString  appData;
    QString  outPath;
    bool     exportAllTables;
    bool     isLegacy;
    uint32_t seed;
};

enum class ConflictPolicy
{
    Replace,   // clear previous data
    Append,    // place new data at the end
    Update,    // overwrite records with same key
    AppendNew, // add non-existent data to the end
    Merge,     // try Update, then Append
    Skip,      // skip new version
    RaiseError,
};

struct ExtraDependencies {
    struct Source {
        StorageType    type   = StorageType::CsvFolder;
        ConflictPolicy policy = ConflictPolicy::RaiseError;
        QString        srcRoot;
        QString        modname;
    };
    QList<Source> m_sources;
};

struct PreGenerationContext {
    QSet<QString>     m_extraJson;
    ExtraDependencies m_preGen;
    ExtraDependencies m_postGen;
};

}
