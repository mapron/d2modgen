/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ExeIcon.hpp"
#include "TestPeImage.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace D2ModGen;
using namespace D2ModGen::Test;

namespace {

class TempFile {
public:
    explicit TempFile(const std::string& content)
        : m_path(std::filesystem::temp_directory_path() / ("d2modgen-exeicon-" + std::to_string(reinterpret_cast<uintptr_t>(this)) + ".bin"))
    {
        std::ofstream(m_path, std::ios::binary) << content;
    }
    ~TempFile()
    {
        std::error_code ec;
        std::filesystem::remove(m_path, ec);
    }
    const Mernel::std_path& path() const { return m_path; }

private:
    Mernel::std_path m_path;
};

}

TEST(ExeIconTest, ExtractsPngFrameFromPe32)
{
    const TempFile file(buildPe(false, { 1, 2 }, { { 1, g_bmp }, { 2, g_png } }));
    EXPECT_EQ(extractExePngIcon(file.path()), g_png);
}

TEST(ExeIconTest, ExtractsPngFrameFromPe32Plus)
{
    const TempFile file(buildPe(true, { 1, 2 }, { { 1, g_bmp }, { 2, g_png } }));
    EXPECT_EQ(extractExePngIcon(file.path()), g_png);
}

TEST(ExeIconTest, MissingIconFrameYieldsNothing)
{
    const TempFile file(buildPe(false, { 1, 7 }, { { 1, g_bmp }, { 2, g_png } }));
    EXPECT_EQ(extractExePngIcon(file.path()), std::nullopt);
}

TEST(ExeIconTest, NonPeFileYieldsNothing)
{
    const TempFile file("just some text, definitely not an executable");
    EXPECT_EQ(extractExePngIcon(file.path()), std::nullopt);
    EXPECT_EQ(extractExePngIcon(std::filesystem::temp_directory_path() / "d2modgen-exeicon-missing.exe"), std::nullopt);
}
