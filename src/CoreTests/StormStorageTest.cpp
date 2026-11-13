/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "Storage/StormStorage.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

using namespace D2ModGen;

namespace {
const Mernel::std_path g_mpqDir = Mernel::std_path(D2MODGEN_TEST_DATA) / "mpq";

std::string readTextFile(const Mernel::std_path& path)
{
    std::ifstream      file(path, std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
}

TEST(StormStorageTest, ReadsTableFromMpq)
{
    const StormStorage storage(g_mpqDir);
    const IStorage&    istorage = storage;

    const IStorage::StoredData data = istorage.readData();

    ASSERT_TRUE(data.valid);
    ASSERT_TRUE(data.tablesMain.contains(TableId::weapons));
    EXPECT_EQ(data.tablesMain.at(TableId::weapons), readTextFile(g_mpqDir / "weapons.txt"));
}
