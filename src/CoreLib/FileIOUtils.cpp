/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FileIOUtils.hpp"

#include <fstream>

namespace D2ModGen {

bool createDirectoriesForFile(const std_path& filename)
{
    return createDirectories(filename.parent_path());
}

bool createDirectories(const std_path& folder)
{
    std::error_code ec;
    if (!std_fs::exists(folder, ec))
        std_fs::create_directories(folder, ec);
    else
        return true;
    return !ec;
}

}
