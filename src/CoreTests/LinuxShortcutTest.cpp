/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#ifdef __linux__
#include "LinuxShortcut.hpp"
#include "TestPeImage.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <random>
#include <sstream>

using namespace D2ModGen;
using Mernel::std_path;
namespace std_fs = Mernel::std_fs;

namespace {

void touch(const std_path& file, const std::string& content = {})
{
    std_fs::create_directories(file.parent_path());
    std::ofstream(file, std::ios::binary) << content;
}

std::string readFile(const std_path& file)
{
    std::ifstream      in(file, std::ios::binary);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

std::string entryValue(const std::string& entry, const std::string& key)
{
    std::istringstream in(entry);
    for (std::string line; std::getline(in, line);) {
        if (line.starts_with(key + "="))
            return line.substr(key.size() + 1);
    }
    return {};
}

class LinuxShortcutTest : public testing::Test {
protected:
    void SetUp() override
    {
        m_root = std_fs::temp_directory_path() / ("d2modgen-test-" + std::to_string(std::random_device{}()));
        std_fs::create_directories(m_root / "Desktop");
        touch(m_root / "bin/umu-run");
        touch(m_root / "bin/steam");
    }
    void TearDown() override { std_fs::remove_all(m_root); }

    // Creates <prefix>/drive_c/users/steamuser and returns it with a trailing slash, like the app stores it.
    std::string makeUserHome(const std_path& prefix)
    {
        const std_path user = prefix / "drive_c/users/steamuser";
        std_fs::create_directories(user);
        return user.string() + "/";
    }

    LinuxShortcutRequest makeRequest(const std::string& userHome, const std::string& exeName = "D2R.exe")
    {
        LinuxShortcutRequest request;
        request.userHome   = userHome;
        request.exe        = m_root / "game" / exeName;
        request.args       = exeName == "D2R.exe" ? "-mod rando -txt" : "-direct -txt";
        request.modName    = "rando";
        request.appsDir    = m_root / "apps";
        request.desktopDir = m_root / "Desktop";
        request.iconDir    = m_root / "icons";
        request.homeDir    = m_root / "home";
        request.pathEnv    = "/nonexistent:" + (m_root / "bin").string();
        touch(request.exe, "not a PE file");
        return request;
    }

    std::string appsEntry() const { return readFile(m_root / "apps/d2modgen-rando.desktop"); }

    std_path m_root;
};

}

TEST_F(LinuxShortcutTest, SteamD2RUsesSteam)
{
    const std_path prefix = m_root / "steam/steamapps/compatdata/2536520/pfx";
    touch(prefix.parent_path() / "version");

    const auto result = makeLinuxShortcut(makeRequest(makeUserHome(prefix)));

    EXPECT_EQ(result.status, LinuxShortcutResult::Status::Written);
    EXPECT_FALSE(result.runnerMissing);
    EXPECT_EQ(entryValue(appsEntry(), "Exec"), "steam -applaunch 2536520 -mod rando -txt");
}

TEST_F(LinuxShortcutTest, LegacyInSteamD2RPrefixUsesUmu)
{
    const std_path compat = m_root / "steam/steamapps/compatdata/2536520";
    touch(compat / "version");
    const auto request = makeRequest(makeUserHome(compat / "pfx"), "Diablo II.exe");

    const auto result = makeLinuxShortcut(request);

    EXPECT_EQ(result.status, LinuxShortcutResult::Status::Written);
    EXPECT_EQ(entryValue(appsEntry(), "Exec"),
              "env WINEPREFIX=\"" + compat.string() + "\" GAMEID=umu-battlenet PROTON_VERB=run umu-run \"" + request.exe.string() + "\" -direct -txt");
}

TEST_F(LinuxShortcutTest, ProtonCompatDirIsTheUmuPrefix)
{
    const std_path compat = m_root / "steam/steamapps/compatdata/123";
    touch(compat / "version");
    const auto request = makeRequest(makeUserHome(compat / "pfx"));

    makeLinuxShortcut(request);

    EXPECT_EQ(entryValue(appsEntry(), "Exec"),
              "env WINEPREFIX=\"" + compat.string() + "\" GAMEID=umu-battlenet PROTON_VERB=run umu-run \"" + request.exe.string() + "\" -mod rando -txt");
}

TEST_F(LinuxShortcutTest, UmuMadePrefixUsesUmu)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "tracked_files");
    const auto userHome = makeUserHome(prefix);
    std_fs::create_directory_symlink(".", prefix / "pfx");

    makeLinuxShortcut(makeRequest(userHome));

    EXPECT_TRUE(entryValue(appsEntry(), "Exec").starts_with("env WINEPREFIX=\"" + prefix.string() + "\" GAMEID=umu-battlenet"));
}

TEST_F(LinuxShortcutTest, GeProtonAddsProtonPath)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    std_fs::create_directories(m_root / "home/.local/share/Steam/compatibilitytools.d/GE-Proton11-7");

    makeLinuxShortcut(makeRequest(makeUserHome(prefix)));

    EXPECT_TRUE(entryValue(appsEntry(), "Exec").starts_with("env WINEPREFIX=\"" + prefix.string() + "\" PROTONPATH=GE-Proton GAMEID=umu-battlenet"));
}

TEST_F(LinuxShortcutTest, MissingRunnerStillWrites)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    auto request    = makeRequest(makeUserHome(prefix));
    request.pathEnv = "/nonexistent";

    const auto result = makeLinuxShortcut(request);

    EXPECT_EQ(result.status, LinuxShortcutResult::Status::Written);
    EXPECT_TRUE(result.runnerMissing);
}

TEST_F(LinuxShortcutTest, WinePrefixHasNoPreset)
{
    const std_path prefix   = m_root / "wine";
    const auto     userHome = makeUserHome(prefix);

    const auto result = makeLinuxShortcut(makeRequest(userHome));

    EXPECT_EQ(result.status, LinuxShortcutResult::Status::NoPreset);
    EXPECT_TRUE(result.written.empty());
    EXPECT_FALSE(std_fs::exists(m_root / "apps"));
    EXPECT_EQ(linuxRunnerPreset(userHome, false, m_root / "home"), "");
}

TEST_F(LinuxShortcutTest, TemplateIsExpanded)
{
    const std_path prefix  = m_root / "wine";
    auto           request = makeRequest(makeUserHome(prefix));
    request.runnerTemplate = "env WINEPREFIX={prefix} wine {exe} {args} --flag";

    const auto result = makeLinuxShortcut(request);

    EXPECT_EQ(result.status, LinuxShortcutResult::Status::Written);
    EXPECT_FALSE(result.runnerMissing);
    EXPECT_EQ(entryValue(appsEntry(), "Exec"),
              "env WINEPREFIX=\"" + prefix.string() + "\" wine \"" + request.exe.string() + "\" -mod rando -txt --flag");
}

TEST_F(LinuxShortcutTest, ExecValuesAreQuotedAndEscaped)
{
    auto request           = makeRequest(makeUserHome(m_root / "wine"));
    request.exe            = m_root / "my $game \"x\" 100%" / "D2R.exe";
    request.args           = "-mod my'mod -txt";
    request.runnerTemplate = "{exe} {args}";
    touch(request.exe);

    makeLinuxShortcut(request);

    // Desktop Entry quoting (\$, \"), then string escaping (\\) and %%.
    EXPECT_EQ(entryValue(appsEntry(), "Exec"),
              "\"" + m_root.string() + "/my \\\\$game \\\\\"x\\\\\" 100%%/D2R.exe\" -mod \"my'mod\" -txt");
}

TEST_F(LinuxShortcutTest, UserHomeIsNormalized)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    const std::string userHome = makeUserHome(prefix);
    const std::string preset   = linuxRunnerPreset(userHome, false, m_root / "home");

    for (const std::string& variant : { userHome + ".", userHome + "../steamuser", userHome.substr(0, userHome.size() - 1) }) {
        auto request = makeRequest(variant);
        makeLinuxShortcut(request);
        EXPECT_TRUE(entryValue(appsEntry(), "Exec").starts_with("env WINEPREFIX=\"" + prefix.string() + "\" ")) << variant;
        EXPECT_EQ(linuxRunnerPreset(variant, false, m_root / "home"), preset) << variant;
    }
}

TEST_F(LinuxShortcutTest, WritesAppsAndDesktopEntries)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    auto request    = makeRequest(makeUserHome(prefix));
    request.modName = "a b/c";

    const auto result = makeLinuxShortcut(request);

    const std_path apps    = m_root / "apps/d2modgen-a_b_c.desktop";
    const std_path desktop = m_root / "Desktop/d2modgen-a_b_c.desktop";
    EXPECT_EQ(result.written, (std::vector<std_path>{ apps, desktop }));
    EXPECT_EQ(readFile(apps), readFile(desktop));

    const std::string entry = readFile(apps);
    EXPECT_TRUE(entry.starts_with("[Desktop Entry]\nType=Application\n"));
    EXPECT_EQ(entryValue(entry, "Name"), "Diablo II - a b/c Mod");
    EXPECT_EQ(entryValue(entry, "Path"), request.exe.parent_path().string());
    EXPECT_EQ(entryValue(entry, "Icon"), "applications-games");
    EXPECT_EQ(std_fs::status(apps).permissions() & std_fs::perms::all, std_fs::perms(0755));
}

TEST_F(LinuxShortcutTest, MissingDesktopDirIsSkipped)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    auto request       = makeRequest(makeUserHome(prefix));
    request.desktopDir = m_root / "NoDesktop";

    const auto result = makeLinuxShortcut(request);

    EXPECT_EQ(result.written, (std::vector<std_path>{ m_root / "apps/d2modgen-rando.desktop" }));
    EXPECT_FALSE(std_fs::exists(m_root / "NoDesktop"));
}

TEST_F(LinuxShortcutTest, GameIconIsExtracted)
{
    const std_path prefix = m_root / "Games/battlenet";
    touch(prefix / "version");
    const auto request = makeRequest(makeUserHome(prefix));
    touch(request.exe, D2ModGen::Test::buildPe(false, { 1 }, { { 1, D2ModGen::Test::g_png } }));

    makeLinuxShortcut(request);

    const std_path icon = m_root / "icons/d2r.png";
    EXPECT_EQ(entryValue(appsEntry(), "Icon"), icon.string());
    EXPECT_EQ(readFile(icon), D2ModGen::Test::g_png);
}
#endif
