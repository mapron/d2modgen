/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "GameDetection.hpp"

#include <gtest/gtest.h>

#include <fstream>

using namespace D2ModGen;
using Mernel::std_path;
namespace std_fs = Mernel::std_fs;

namespace {

// Key paths and values below are written as they appear in a .reg file, with doubled backslashes.
const std::string g_uninstallKey = R"(Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II Resurrected)";
const std::string g_shellFolders = R"(Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders)";
const std::string g_installPath  = R"(C:\\Program Files (x86)\\Diablo II Resurrected)";
const std::string g_appData      = R"(C:\\users\\steamuser\\AppData\\Roaming)";
const std::string g_gameDir      = "drive_c/Program Files (x86)/Diablo II Resurrected";

void touch(const std_path& file, const std::string& content = {})
{
    std_fs::create_directories(file.parent_path());
    std::ofstream(file, std::ios::binary) << content;
}

std::string regSection(const std::string& key, const std::string& name, const std::string& value)
{
    return "\n[" + key + "] 1700000000\n#time=1da0000000000000\n\"" + name + "\"=\"" + value + "\"\n";
}

void writeReg(const std_path& file, const std::string& sections)
{
    touch(file, "WINE REGISTRY Version 2\n;; All keys relative to REGISTRY\\\\Machine\n\n#arch=win64\n" + sections);
}

void makePrefix(const std_path& prefix, const std::string& systemSections, const std::string& userSections)
{
    writeReg(prefix / "system.reg", systemSections);
    writeReg(prefix / "user.reg", userSections);
    std_fs::create_directories(prefix / "dosdevices");
    std_fs::create_directories(prefix / "drive_c/users/steamuser/AppData/Roaming");
    std_fs::create_directories(prefix / "drive_c/users/Public");
    std_fs::create_directory_symlink("../drive_c", prefix / "dosdevices/c:");
}

void makeD2RPrefix(const std_path& prefix, const std::string& installPath = g_installPath)
{
    makePrefix(prefix, regSection(g_uninstallKey, "InstallLocation", installPath), regSection(g_shellFolders, "AppData", g_appData));
    touch(prefix / g_gameDir / "D2R.exe");
}

void setUserRegTime(const std_path& prefix, int ageSeconds)
{
    std_fs::last_write_time(prefix / "user.reg", std_fs::file_time_type::clock::now() - std::chrono::seconds(ageSeconds));
}

class GameDetectionTest : public testing::Test {
protected:
    void SetUp() override
    {
        // A fixed folder per test under the working dir (the build dir under ctest), not in the system temp.
        m_root = std_fs::absolute("GameDetectionTestData") / testing::UnitTest::GetInstance()->current_test_info()->name();
        std_fs::remove_all(m_root);
        m_home = m_root / "home";
        std_fs::create_directories(m_home);
    }
    void TearDown() override { std_fs::remove_all(m_root); }

    std::vector<GameMatch> find(GameKind kind = GameKind::Resurrected, const std::string& prefixEnv = {})
    {
        return findGames(kind, m_home, prefixEnv);
    }

    std_path m_root;
    std_path m_home;
};

}

TEST_F(GameDetectionTest, UmuPrefix)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix);
    std_fs::create_directories(prefix / "drive_c/users/steamuser/Saved Games/Diablo II Resurrected");
    touch(prefix / "drive_c/users/steamuser/AppData/Roaming/Battle.net/Battle.net.config", "{}");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, prefix);
    EXPECT_EQ(matches[0].installPath, prefix / g_gameDir);
    EXPECT_EQ(matches[0].userHome, prefix / "drive_c/users/steamuser/");
    EXPECT_EQ(matches[0].userHome.string().back(), '/');
    EXPECT_TRUE(matches[0].hasSavedGames);
    EXPECT_TRUE(matches[0].hasBattleNetConfig);
}

TEST_F(GameDetectionTest, SamePrefixReachedThreeWays)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix);
    std_fs::create_directory_symlink(".", prefix / "pfx");

    const auto matches = find(GameKind::Resurrected, (prefix / "pfx").string());
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(std_fs::canonical(matches[0].prefix), std_fs::canonical(prefix));
}

TEST_F(GameDetectionTest, SteamCompatdataOnlyD2RAndShortcuts)
{
    const std_path compatdata = m_home / ".local/share/Steam/steamapps/compatdata";
    makeD2RPrefix(compatdata / "2536520/pfx");
    makeD2RPrefix(compatdata / "123/pfx");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, compatdata / "2536520/pfx");
}

TEST_F(GameDetectionTest, SteamAppInstall)
{
    const std_path steamapps = m_home / ".local/share/Steam/steamapps";
    const std_path prefix    = steamapps / "compatdata/2536520/pfx";
    makePrefix(prefix, {}, regSection(g_shellFolders, "AppData", g_appData));
    touch(steamapps / "appmanifest_2536520.acf", "\"AppState\"\n{\n\t\"appid\"\t\t\"2536520\"\n\t\"installdir\"\t\t\"Diablo II Resurrected\"\n}\n");
    touch(steamapps / "common/Diablo II Resurrected/D2R.exe");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, prefix);
    EXPECT_EQ(matches[0].installPath, steamapps / "common/Diablo II Resurrected");
    EXPECT_EQ(matches[0].userHome, prefix / "drive_c/users/steamuser/");
}

TEST_F(GameDetectionTest, SteamLibraryFromVdf)
{
    const std_path library = m_root / "mnt/SteamLibrary";
    touch(m_home / ".var/app/com.valvesoftware.Steam/.local/share/Steam/steamapps/libraryfolders.vdf",
          "\"libraryfolders\"\n{\n\t\"0\"\n\t{\n\t\t\"path\"\t\t\"" + library.string() + "\"\n\t\t\"label\"\t\t\"\"\n\t}\n}\n");
    makeD2RPrefix(library / "steamapps/compatdata/2147483650/pfx");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, library / "steamapps/compatdata/2147483650/pfx");
}

TEST_F(GameDetectionTest, LutrisPrefix)
{
    touch(m_home / ".local/share/lutris/games/battlenet-1700000000.yml",
          "game:\n  arch: win64\n  exe: drive_c/Program Files (x86)/Battle.net/Battle.net Launcher.exe\n  prefix: ~/elsewhere/bnet\n"
          "game_slug: battlenet\nname: Battle.net\nrunner: wine\nwine:\n  version: lutris-GE-Proton8-26-x86_64\n");
    makeD2RPrefix(m_home / "elsewhere/bnet");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, m_home / "elsewhere/bnet");
}

TEST_F(GameDetectionTest, InstallLocationCaseDiffers)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix, R"(c:\\program files (x86)\\diablo ii resurrected)");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].installPath, prefix / g_gameDir);
}

TEST_F(GameDetectionTest, InstallLocationHexEscape)
{
    const std_path prefix  = m_home / "Games/battlenet";
    const std_path gameDir = prefix / "drive_c/Games" / Mernel::string2path("Diablo II R\xC3\xA9surrected");
    makeD2RPrefix(prefix, R"(C:\\Games\\Diablo II R\x00e9surrected)");
    touch(gameDir / "D2R.exe");

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].installPath, gameDir);
}

TEST_F(GameDetectionTest, MissingExeIsNoMatch)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix);
    std_fs::remove(prefix / g_gameDir / "D2R.exe");

    EXPECT_TRUE(find().empty());
}

TEST_F(GameDetectionTest, InstallDirSymlinkLoopIsNoMatch)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix);
    std_fs::remove_all(prefix / g_gameDir);
    std_fs::create_directory_symlink("Diablo II Resurrected", prefix / g_gameDir);

    EXPECT_TRUE(find().empty());
}

TEST_F(GameDetectionTest, UserHomeFromUsersDir)
{
    const std_path prefix = m_home / "Games/battlenet";
    makeD2RPrefix(prefix);
    writeReg(prefix / "user.reg", {});

    const auto matches = find();
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].userHome, prefix / "drive_c/users/steamuser/");
}

TEST_F(GameDetectionTest, SavedGamesRankFirst)
{
    makeD2RPrefix(m_home / "Games/a");
    makeD2RPrefix(m_home / "Games/b");
    std_fs::create_directories(m_home / "Games/b/drive_c/users/steamuser/Saved Games/Diablo II Resurrected");
    setUserRegTime(m_home / "Games/a", 10);
    setUserRegTime(m_home / "Games/b", 1000);

    const auto matches = find();
    ASSERT_EQ(matches.size(), 2u);
    EXPECT_EQ(matches[0].prefix, m_home / "Games/b");
    EXPECT_EQ(matches[1].prefix, m_home / "Games/a");
}

TEST_F(GameDetectionTest, NewerUserRegRanksFirst)
{
    makeD2RPrefix(m_home / "Games/a");
    makeD2RPrefix(m_home / "Games/b");
    setUserRegTime(m_home / "Games/a", 1000);
    setUserRegTime(m_home / "Games/b", 10);

    const auto matches = find();
    ASSERT_EQ(matches.size(), 2u);
    EXPECT_EQ(matches[0].prefix, m_home / "Games/b");
    EXPECT_EQ(matches[1].prefix, m_home / "Games/a");
}

TEST_F(GameDetectionTest, LegacyFromUserRegInstallPath)
{
    const std_path prefix = m_home / ".wine";
    makePrefix(prefix,
               {},
               regSection(R"(Software\\Blizzard Entertainment\\Diablo II)", "InstallPath", R"(C:\\Games\\Diablo II)")
                   + regSection(g_shellFolders, "AppData", g_appData));
    touch(prefix / "drive_c/Games/Diablo II/GAME.EXE");
    touch(prefix / "drive_c/Games/Diablo II/D2Data.MPQ");
    touch(prefix / "drive_c/Games/Diablo II/D2EXP.MPQ");
    touch(prefix / "drive_c/Games/Diablo II/Patch_D2.mpq");
    touch(prefix / "drive_c/users/steamuser/AppData/Roaming/Battle.net/Battle.net.config", "{}");

    const auto matches = find(GameKind::Legacy);
    ASSERT_EQ(matches.size(), 1u);
    EXPECT_EQ(matches[0].prefix, prefix);
    EXPECT_EQ(matches[0].installPath, prefix / "drive_c/Games/Diablo II");
    EXPECT_FALSE(matches[0].hasBattleNetConfig);
}

TEST_F(GameDetectionTest, LegacyMarkersIncomplete)
{
    const std_path prefix = m_home / ".wine";
    makePrefix(prefix,
               {},
               regSection(R"(Software\\Blizzard Entertainment\\Diablo II)", "InstallPath", R"(C:\\Games\\Diablo II)")
                   + regSection(g_shellFolders, "AppData", g_appData));
    touch(prefix / "drive_c/Games/Diablo II/Game.exe");
    touch(prefix / "drive_c/Games/Diablo II/d2data.mpq");
    touch(prefix / "drive_c/Games/Diablo II/patch_d2.mpq");

    EXPECT_TRUE(find(GameKind::Legacy).empty());
}
