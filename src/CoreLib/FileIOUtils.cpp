/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "FileIOUtils.hpp"

#include <algorithm>
#include <fstream>

namespace D2ModGen {

bool createDirectoriesForFile(const std_path& filename)
{
    return createDirectories(filename.parent_path());
}

std_path toFsPath(const std_path& path)
{
    auto str = path.native();
    std::replace(str.begin(), str.end(), std_path::value_type('\\'), std_path::preferred_separator);
    return str;
}

bool createDirectories(const std_path& path)
{
    const auto      folder = toFsPath(path);
    std::error_code ec;
    if (!std_fs::exists(folder, ec))
        std_fs::create_directories(folder, ec);
    else
        return true;
    return !ec;
}

}
