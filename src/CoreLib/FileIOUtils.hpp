/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include "CoreExport.hpp"

#include <filesystem>

namespace D2ModGen {

namespace std_fs = std::filesystem;
using std_path   = std_fs::path;

inline std::string path2string(const std_path& path)
{
    auto        str = path.u8string();
    std::string result;
    result.resize(str.size());
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(str[i]);
    }
    return result;
}

inline std_path string2path(const std::string& str)
{
    std::u8string result;
    result.resize(str.size());
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char8_t>(str[i]);
    }
    return result;
}

CORE_EXPORT bool readFileIntoBuffer(const std_path& filename, std::string& buffer);
CORE_EXPORT bool writeFileFromBuffer(const std_path& filename, const std::string& buffer);

CORE_EXPORT bool createDirectories(const std_path& folder);
CORE_EXPORT bool createDirectoriesForFile(const std_path& filename);

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
