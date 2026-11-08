/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <functional>
#include <set>
#include <string>

#include "MernelPlatform/FsUtils.hpp"
#include "ModgenCoreExport.hpp"

namespace D2ModGen {

struct Config;
using RandomGenerator = std::function<int(int)>; // generate [0, max)

enum class StorageType
{
    D2ResurrectedInternal,  // using CASC storage from D2R Installation,
    D2LegacyInternal,       // using Storm storage from D2 Legacy Installation (patch_d2.mpq)
    D2ResurrectedModFolder, // folder containing D2R mod structure (usually in mods/name/name.mpq)
    D2LegacyFolder,         // Folder containing 'data' folder where data can be extracted.
    CsvFolder,              // Folder containing plain 'txt' files.
    None,
};

struct MODGENCORE_EXPORT GenerationEnvironment {
    enum class VersionMajor
    {
        Legacy,
        D2R,
    };
    enum class VersionD2R
    {
        LoD,
        RotW,
    };

    enum class InputMode
    {
        Game,
        FullFolders,
        Txt,
    };
    enum class OutputMode
    {
        D2RMod,
        FullFolders,
        Txt,
        Dry,
    };

    std::string      d2rLocaleId;
    std::string      modName;
    Mernel::std_path inputPath;
    Mernel::std_path outputPath;
    bool             refreshSeed       = false;
    bool             needDataSubfolder = false;
    bool             isLegacy          = false;
    bool             isRotW            = false;
    StorageType      inputMode         = StorageType::D2ResurrectedInternal;
    StorageType      outputMode        = StorageType::D2ResurrectedModFolder;

    std::set<std::string> extraTrace;
    Mernel::std_path      extraFile;

    uint32_t seed{};

    bool init(const Config& config);
};

}
