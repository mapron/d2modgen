/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <map>
#include <string>
#include <set>
#include <vector>
#include <functional>

#include "MernelPlatform/FsUtils.hpp"

namespace D2ModGen {

using StringVector = std::vector<std::string>;
using StringSet    = std::set<std::string>;

inline StringVector& operator<<(StringVector& rh, const StringVector& lh)
{
    rh.insert(rh.end(), lh.cbegin(), lh.cend());
    return rh;
}

inline StringVector& operator<<(StringVector& rh, const std::string& lh)
{
    rh.push_back(lh);
    return rh;
}

inline std::string toLower(std::string s)
{
    for (auto& c : s)
        c = static_cast<char>(::tolower(c));
    return s;
}

template<class Val>
Val mapValue(const std::map<std::string, Val>& m, const std::string& key, const Val& d = Val())
{
    auto it = m.find(key);
    if (it == m.cend())
        return d;
    return it->second;
}

using RandomGenerator = std::function<int(int)>;

enum class StorageType
{
    D2ResurrectedInternal,  // using CASC storage from D2R Installation,
    D2LegacyInternal,       // using Storm storage from D2 Legacy Installation (patch_d2.mpq)
    D2ResurrectedModFolder, // folder containing D2R mod structure (usually in mods/name/name.mpq)
    D2LegacyFolder,         // Folder containing 'data' folder where data can be extracted.
    CsvFolder,              // Folder containing plain 'txt' files. Can not be selected as output.
};

struct GenerationEnvironment {
    enum class Version
    {
        Legacy,
        D2R_LoD,
        D2R_RotW,
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
    };

    std::string      modName;
    Mernel::std_path inputPath;
    Mernel::std_path outputPath;
    bool             exportAll         = false;
    bool             refreshSeed       = false;
    bool             needDataSubfolder = false;
    bool             isLegacy          = false;
    StorageType      inputMode         = StorageType::D2ResurrectedInternal;
    StorageType      outputMode        = StorageType::D2ResurrectedModFolder;

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

}
