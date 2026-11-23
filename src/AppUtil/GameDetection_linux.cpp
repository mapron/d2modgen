/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "GameDetection.hpp"

#include "MernelPlatform/FileIOUtils.hpp"
#include "MernelPlatform/Logger.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <algorithm>
#include <charconv>
#include <set>
#include <vector>

namespace D2ModGen {

namespace {

using Mernel::Logger;
using Mernel::path2string;
using Mernel::std_path;
using Mernel::string2path;
using Mernel::strToLower;
namespace std_fs = Mernel::std_fs;

std::vector<std::string> readLines(const std_path& file)
{
    std::string buffer;
    if (!Mernel::readFileIntoBufferNoexcept(file, buffer))
        return {};
    std::vector<std::string> lines;
    std::string              line;
    for (char c : buffer) {
        if (c != '\n') {
            line += c;
            continue;
        }
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        lines.push_back(std::move(line));
        line.clear();
    }
    if (!line.empty()) {
        if (line.back() == '\r')
            line.pop_back();
        lines.push_back(std::move(line));
    }
    return lines;
}

// Reads a quoted .vdf string starting at pos (on the opening quote). A backslash escapes the next character.
bool parseQuoted(const std::string& line, size_t& pos, std::string& out)
{
    if (pos >= line.size() || line[pos] != '"')
        return false;
    for (++pos; pos < line.size(); ++pos) {
        if (line[pos] == '"') {
            ++pos;
            return true;
        }
        if (line[pos] == '\\' && pos + 1 < line.size())
            ++pos;
        out += line[pos];
    }
    return false;
}

std_path findEntry(const std_path& dir, const std::string& name)
{
    const std_path  exact = dir / string2path(name);
    std::error_code ec;
    if (std_fs::exists(exact, ec))
        return exact;
    const std::string lower = strToLower(name);
    for (const auto& entry : std_fs::directory_iterator(dir, ec)) {
        if (Mernel::pathToLower(entry.path().filename()) == lower)
            return entry.path();
    }
    return {};
}

// Windows paths ignore case: "program files (x86)" finds "Program Files (x86)".
std_path findPath(std_path dir, const std::vector<std::string>& parts)
{
    for (const auto& part : parts) {
        dir = findEntry(dir, part);
        if (dir.empty())
            return {};
    }
    return dir;
}

bool validateInstall(GameKind kind, const std_path& dir, std::string& reason)
{
    if (kind == GameKind::Resurrected) {
        reason = "markers missing (D2R.exe)";
        return Mernel::isExistingFile(dir / "D2R.exe");
    }
    // d2data, d2exp and patch_d2 are the archives StormStorage opens.
    for (const char* marker : { "Game.exe", "d2data.mpq", "d2exp.mpq", "patch_d2.mpq" }) {
        if (!Mernel::isExistingFile(findEntry(dir, marker))) {
            reason = std::string("markers missing (") + marker + ")";
            return false;
        }
    }
    return true;
}

// Steam .vdf/.acf files: one "key"		"value" pair per line. Empty unless the line holds key.
std::string vdfValue(std::string line, const std::string& key)
{
    Mernel::ltrim(line);
    const std::string quotedKey = '"' + key + '"';
    size_t            pos       = line.find('"', quotedKey.size());
    std::string       value;
    if (!line.starts_with(quotedKey) || pos == std::string::npos || !parseQuoted(line, pos, value))
        return {};
    return value;
}

// D2R bought on Steam (app 2536520): the prefix is <library>/steamapps/compatdata/2536520/pfx and the game is in
// <library>/steamapps/common/<installdir from appmanifest_2536520.acf>, outside the prefix.
std_path steamInstall(const std_path& prefix)
{
    std_path pfx = prefix.lexically_normal();
    if (!pfx.has_filename())
        pfx = pfx.parent_path();
    const std_path compat = pfx.parent_path();
    if (pfx.filename() != "pfx" || compat.filename() != "2536520" || compat.parent_path().filename() != "compatdata")
        return {};
    const std_path steamapps = compat.parent_path().parent_path();
    for (const std::string& line : readLines(steamapps / "appmanifest_2536520.acf")) {
        if (const std::string dir = vdfValue(line, "installdir"); !dir.empty())
            return steamapps / "common" / string2path(dir);
    }
    return {};
}

// The installers' default folders in drive_c. A game installed elsewhere in the prefix is not found; Browse... still takes it.
std_path findInstall(GameKind kind, const std_path& prefix)
{
    const auto accept = [kind, &prefix](const std_path& dir) {
        std::string reason = "install dir missing";
        if (Mernel::isExistingDirectory(dir) && validateInstall(kind, dir, reason))
            return true;
        Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": " << path2string(dir) << ": " << reason;
        return false;
    };

    if (kind == GameKind::Resurrected) {
        if (const std_path dir = steamInstall(prefix); !dir.empty() && accept(dir))
            return dir;
    }
    const std::string game = kind == GameKind::Resurrected ? "Diablo II Resurrected" : "Diablo II";
    for (const auto& parts : std::vector<std::vector<std::string>>{ { "Program Files (x86)", game }, { "Program Files", game }, { game } }) {
        if (const std_path dir = findPath(prefix / "drive_c", parts); !dir.empty() && accept(dir))
            return dir;
    }
    Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": " << game << " not in its default folders";
    return {};
}

// The one user folder in drive_c/users with AppData. Proton adds a symlink named after the Linux user that points at
// steamuser; symlinks are skipped so that folder counts once.
std_path findUserHome(const std_path& prefix)
{
    std_path        found;
    std::error_code ec;
    for (const auto& entry : std_fs::directory_iterator(prefix / "drive_c/users", ec)) {
        if (entry.is_symlink(ec) || entry.path().filename() == "Public" || !std_fs::is_directory(entry.path() / "AppData", ec))
            continue;
        if (!found.empty())
            return {};
        found = entry.path();
    }
    return found;
}

std::vector<std_path> listDirs(const std_path& dir)
{
    std::vector<std_path> result;
    std::error_code       ec;
    for (const auto& entry : std_fs::directory_iterator(dir, ec)) {
        if (entry.is_directory(ec))
            result.push_back(entry.path());
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::string unquote(const std::string& str)
{
    if (str.size() >= 2 && (str.front() == '"' || str.front() == '\'') && str.back() == str.front())
        return str.substr(1, str.size() - 2);
    return str;
}

// Lutris game .yml: the prefix is a "prefix:" entry inside the top-level "game:" block.
std_path lutrisPrefix(const std_path& file, const std_path& homeDir)
{
    bool inGame = false;
    for (std::string line : readLines(file)) {
        const bool indented = line.starts_with(' ') || line.starts_with('\t');
        Mernel::ltrim(line);
        Mernel::rtrim(line);
        if (line.empty())
            continue;
        if (!indented) {
            inGame = line == "game:";
            continue;
        }
        if (!inGame || !line.starts_with("prefix:"))
            continue;
        std::string value = line.substr(7);
        Mernel::ltrim(value);
        value = unquote(value);
        if (value == "~")
            return homeDir;
        if (value.starts_with("~/"))
            return homeDir / string2path(value.substr(2));
        return string2path(value);
    }
    return {};
}

// libraryfolders.vdf: one "path"		"<library dir>" line per Steam library.
std::vector<std_path> steamLibraries(const std_path& root)
{
    std::vector<std_path> result{ root };
    for (const std::string& line : readLines(root / "steamapps/libraryfolders.vdf")) {
        if (const std::string path = vdfValue(line, "path"); !path.empty())
            result.push_back(string2path(path));
    }
    return result;
}

bool isGameCompatId(const std::string& name)
{
    unsigned long long id  = 0;
    const auto         res = std::from_chars(name.data(), name.data() + name.size(), id);
    if (res.ec != std::errc() || res.ptr != name.data() + name.size())
        return false;
    return id == 2536520 || id >= 2147483648ULL;
}

std::vector<std_path> collectCandidates(const std_path& homeDir, const std::string& prefixEnv)
{
    std::vector<std_path> result;
    auto                  append = [&result](const std::vector<std_path>& dirs) { result.insert(result.end(), dirs.cbegin(), dirs.cend()); };

    if (!prefixEnv.empty())
        result.push_back(string2path(prefixEnv));
    result.push_back(homeDir / ".wine");
    append(listDirs(homeDir / "Games"));
    append(listDirs(homeDir / "Games/umu"));

    for (const auto& gamesDir : { homeDir / ".config/lutris/games", homeDir / ".local/share/lutris/games" }) {
        std::vector<std_path> files;
        std::error_code       ec;
        for (const auto& entry : std_fs::directory_iterator(gamesDir, ec)) {
            if (entry.path().extension() == ".yml")
                files.push_back(entry.path());
        }
        std::sort(files.begin(), files.end());
        for (const auto& file : files) {
            if (const std_path prefix = lutrisPrefix(file, homeDir); !prefix.empty())
                result.push_back(prefix);
        }
    }

    for (const auto& dir : listDirs(homeDir / "Games/Heroic/Prefixes"))
        append(listDirs(dir));

    append(listDirs(homeDir / ".local/share/bottles/bottles"));
    append(listDirs(homeDir / ".var/app/com.usebottles.bottles/data/bottles/bottles"));

    for (const auto& root : { homeDir / ".steam/steam", homeDir / ".local/share/Steam", homeDir / ".var/app/com.valvesoftware.Steam/.local/share/Steam" }) {
        for (const auto& library : steamLibraries(root)) {
            for (const auto& dir : listDirs(library / "steamapps/compatdata")) {
                if (isGameCompatId(path2string(dir.filename())))
                    result.push_back(dir / "pfx");
            }
        }
    }
    return result;
}

struct Found {
    GameMatch              match;
    std_fs::file_time_type userRegTime;
};

}

std::vector<GameMatch> findGames(GameKind kind, const Mernel::std_path& homeDir, const std::string& prefixEnv)
{
    std::vector<Found> found;
    std::set<std_path> checked;
    for (const auto& dir : collectCandidates(homeDir, prefixEnv)) {
        std_path prefix = dir;
        if (!Mernel::isExistingDirectory(prefix / "drive_c"))
            prefix = dir / "pfx";
        if (!Mernel::isExistingDirectory(prefix / "drive_c")) {
            Logger(Logger::Info) << "Wine detection: " << path2string(dir) << ": no drive_c";
            continue;
        }
        std::error_code ec;
        const std_path  canonical = std_fs::weakly_canonical(prefix, ec);
        if (!checked.insert(ec ? prefix : canonical).second) {
            Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": already checked";
            continue;
        }

        GameMatch match;
        match.prefix      = prefix;
        match.installPath = findInstall(kind, prefix);
        if (match.installPath.empty())
            continue;
        const std_path user = findUserHome(prefix);
        if (user.empty()) {
            Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": no user dir";
            continue;
        }
        match.userHome = user / "";
        if (kind == GameKind::Resurrected) {
            match.hasSavedGames      = Mernel::isExistingDirectory(user / "Saved Games/Diablo II Resurrected");
            match.hasBattleNetConfig = Mernel::isExistingFile(user / "AppData/Roaming/Battle.net/Battle.net.config");
        } else {
            match.hasSavedGames = Mernel::isExistingDirectory(user / "Saved Games/Diablo II");
        }
        Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": accepted, game in " << path2string(match.installPath);

        // Wine writes user.reg when the prefix shuts down, so its time is when the prefix was last used.
        const auto userRegTime = std_fs::last_write_time(prefix / "user.reg", ec);
        found.push_back({ match, ec ? std_fs::file_time_type::min() : userRegTime });
    }

    std::stable_sort(found.begin(), found.end(), [](const Found& l, const Found& r) {
        if (l.match.hasSavedGames != r.match.hasSavedGames)
            return l.match.hasSavedGames;
        if (l.match.hasBattleNetConfig != r.match.hasBattleNetConfig)
            return l.match.hasBattleNetConfig;
        return l.userRegTime > r.userRegTime;
    });

    std::vector<GameMatch> result;
    for (const auto& item : found)
        result.push_back(item.match);

    const char* game = kind == GameKind::Resurrected ? "Diablo II Resurrected" : "Diablo II";
    if (result.empty())
        Logger(Logger::Notice) << "Wine detection: no Wine prefix with " << game << " found";
    for (size_t i = 0; i < result.size(); ++i) {
        const auto& match = result[i];
        Logger(Logger::Notice) << "Wine detection: " << game << " #" << (i + 1) << ": prefix " << path2string(match.prefix)
                               << ", game " << path2string(match.installPath) << ", user " << path2string(match.userHome)
                               << ", saved games " << match.hasSavedGames << ", Battle.net config " << match.hasBattleNetConfig;
    }
    return result;
}

std::string getInstallLocationFromRegistry(GameKind)
{
    return {};
}

}
