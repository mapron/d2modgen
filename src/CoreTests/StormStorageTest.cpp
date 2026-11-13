/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "Storage/StormStorage.hpp"

#include "MernelPlatform/FileIOUtils.hpp"

#include <gtest/gtest.h>

using namespace D2ModGen;

TEST(StormStorageTest, ReadsTableFromMpq)
{
    // CMake copies src/CoreTests/data to CoreTestsData in the build dir; ctest runs from there.
    const Mernel::std_path mpqDir = "CoreTestsData/mpq";

    std::string expectedWeapons;
    ASSERT_TRUE(Mernel::readFileIntoBufferNoexcept(mpqDir / "weapons.txt", expectedWeapons));

    const StormStorage storage(mpqDir);
    const IStorage&    istorage = storage;

    const IStorage::StoredData data = istorage.readData();

    ASSERT_TRUE(data.valid);
    ASSERT_TRUE(data.tablesMain.contains(TableId::weapons));
    EXPECT_EQ(data.tablesMain.at(TableId::weapons), expectedWeapons);
}
