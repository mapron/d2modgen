/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "FileIOUtils.hpp"

#include <fstream>

namespace D2ModGen {

bool readFileIntoBuffer(const std_path& filename, std::string& buffer)
{
    std::ifstream ifs((filename), std::ios_base::in | std::ios_base::binary);
    if (!ifs)
        return false;

    ifs.seekg(0, std::ios::end);

    buffer.resize(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ifs.read(buffer.data(), buffer.size());
    return true;
}

bool writeFileFromBuffer(const std_path& filename, const std::string& buffer)
{
    std::ofstream ofs((filename), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if (!ofs)
        return false;

    if (buffer.empty())
        return true;

    ofs.write(buffer.data(), buffer.size());
    if (!ofs)
        return false;
    return true;
}

bool createDirectoriesForFile(const std::string& filename)
{
    auto            path = string2path(filename);
    std::error_code ec;
    if (!std_fs::exists(path.parent_path(), ec))
        std_fs::create_directories(path.parent_path(), ec);
    else
        return true;
    return !ec;
}

bool createDirectories(const std::string& folder)
{
    auto            path = string2path(folder);
    std::error_code ec;
    if (!std_fs::exists(path, ec))
        std_fs::create_directories(path, ec);
    else
        return true;
    return !ec;
}

}
