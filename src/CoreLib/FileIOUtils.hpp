/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include "ModgenCoreExport.hpp"

#include "MernelPlatform/FileIOUtils.hpp"

#include <filesystem>

namespace D2ModGen {

namespace std_fs = Mernel::std_fs;
using std_path   = Mernel::std_path;

inline std::string path2string(const std_path& path)
{
    return Mernel::path2string(path);
}

inline std_path string2path(const std::string& str)
{
    return Mernel::string2path(str);
}

MODGENCORE_EXPORT bool createDirectories(const std_path& folder);
MODGENCORE_EXPORT bool createDirectoriesForFile(const std_path& filename);

inline std::string ensureTrailingSlash(std::string path)
{
    if (path.empty())
        return {};

    const bool hasSlash = path.ends_with("\\") || path.ends_with("/");
    if (!hasSlash)
        path += "/";
    return path;
}

}
