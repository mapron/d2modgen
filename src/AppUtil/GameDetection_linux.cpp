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
#include <cctype>
#include <charconv>
#include <map>
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

using RegKey  = std::map<std::string, std::string>; // lowercase value name -> value
using RegKeys = std::map<std::string, RegKey>;      // lowercase key path -> values

const std::string g_uninstallWow = "Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
const std::string g_uninstall    = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\";
const std::string g_blizzardWow  = "Software\\Wow6432Node\\Blizzard Entertainment\\Diablo II";
const std::string g_blizzard     = "Software\\Blizzard Entertainment\\Diablo II";
const std::string g_shellFolders = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";

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

void appendUtf8(std::string& out, unsigned code)
{
    if (code < 0x80) {
        out += char(code);
    } else if (code < 0x800) {
        out += char(0xC0 | (code >> 6));
        out += char(0x80 | (code & 0x3F));
    } else {
        out += char(0xE0 | (code >> 12));
        out += char(0x80 | ((code >> 6) & 0x3F));
        out += char(0x80 | (code & 0x3F));
    }
}

// Reads a quoted .reg string starting at pos (on the opening quote). Wine writes non-ASCII characters as \x
// with up to four hex digits (UTF-16 code units).
bool parseQuoted(const std::string& line, size_t& pos, std::string& out)
{
    if (pos >= line.size() || line[pos] != '"')
        return false;
    for (++pos; pos < line.size(); ++pos) {
        const char c = line[pos];
        if (c == '"') {
            ++pos;
            return true;
        }
        if (c != '\\' || pos + 1 >= line.size()) {
            out += c;
            continue;
        }
        const char next = line[++pos];
        if (next != 'x') {
            out += next;
            continue;
        }
        unsigned   code  = 0;
        const auto first = line.data() + pos + 1;
        const auto last  = line.data() + std::min(line.size(), pos + 5);
        const auto res   = std::from_chars(first, last, code, 16);
        if (res.ptr == first) {
            out += next;
            continue;
        }
        appendUtf8(out, code);
        pos += res.ptr - first;
    }
    return false;
}

std::string unescapeKey(const std::string& key)
{
    std::string result;
    for (size_t i = 0; i < key.size(); ++i) {
        if (key[i] == '\\' && i + 1 < key.size() && key[i + 1] == '\\')
            ++i;
        result += key[i];
    }
    return result;
}

// Wine .reg files: "[Key\\Path] <timestamp>" section headers followed by "Name"="value" lines.
RegKeys readRegKeys(const std_path& file, const std::vector<std::string>& keys)
{
    std::set<std::string> wanted;
    for (const auto& key : keys)
        wanted.insert(strToLower(key));

    RegKeys result;
    RegKey* current = nullptr;
    for (const std::string& line : readLines(file)) {
        if (line.starts_with('[')) {
            const size_t      end = line.rfind(']');
            const std::string key = end == std::string::npos ? std::string() : strToLower(unescapeKey(line.substr(1, end - 1)));
            current               = wanted.contains(key) ? &result[key] : nullptr;
            continue;
        }
        if (!current || !line.starts_with('"'))
            continue;
        size_t      pos = 0;
        std::string name, value;
        if (!parseQuoted(line, pos, name) || line.compare(pos, 1, "=") != 0)
            continue;
        ++pos;
        if (parseQuoted(line, pos, value))
            (*current)[strToLower(name)] = value;
    }
    return result;
}

std::string regValue(const RegKeys& keys, const std::string& key, const std::string& name)
{
    const auto keyIt = keys.find(strToLower(key));
    if (keyIt == keys.cend())
        return {};
    const auto valueIt = keyIt->second.find(strToLower(name));
    return valueIt == keyIt->second.cend() ? std::string() : valueIt->second;
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

std_path driveRoot(const std_path& prefix, const std::string& winPath)
{
    if (winPath.size() < 2 || !std::isalpha(static_cast<unsigned char>(winPath[0])) || winPath[1] != ':')
        return {};
    const std_path  dosdevices = prefix / "dosdevices";
    const std_path  drive      = dosdevices / strToLower(winPath.substr(0, 2));
    std::error_code ec;
    std_path        target = std_fs::read_symlink(drive, ec);
    if (ec)
        target = drive;
    else if (target.is_relative())
        target = (dosdevices / target).lexically_normal();
    return Mernel::isExistingDirectory(target) ? target : std_path();
}

std_path resolveWinPath(const std_path& root, const std::string& winPath)
{
    std_path result = root;
    size_t   start  = 2;
    while (start < winPath.size()) {
        size_t end = winPath.find_first_of("\\/", start);
        if (end == std::string::npos)
            end = winPath.size();
        const std::string part = winPath.substr(start, end - start);
        if (!part.empty()) {
            result = findEntry(result, part);
            if (result.empty())
                return {};
        }
        start = end + 1;
    }
    return result;
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
// <library>/steamapps/common/<installdir from appmanifest_2536520.acf>. No Blizzard uninstall key in the prefix.
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

std_path findInstall(GameKind kind, const std_path& prefix)
{
    const auto accept = [kind, &prefix](const std_path& dir, const std::string& source) {
        std::string reason = "install dir missing";
        if (Mernel::isExistingDirectory(dir) && validateInstall(kind, dir, reason))
            return true;
        Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": " << source << ": " << reason;
        return false;
    };

    std::vector<std::string> winPaths;
    if (kind == GameKind::Resurrected) {
        if (const std_path dir = steamInstall(prefix); !dir.empty() && accept(dir, path2string(dir)))
            return dir;
        const RegKeys system = readRegKeys(prefix / "system.reg", { g_uninstallWow + "Diablo II Resurrected", g_uninstall + "Diablo II Resurrected" });
        winPaths.push_back(regValue(system, g_uninstallWow + "Diablo II Resurrected", "InstallLocation"));
        winPaths.push_back(regValue(system, g_uninstall + "Diablo II Resurrected", "InstallLocation"));
    } else {
        const RegKeys system = readRegKeys(prefix / "system.reg", { g_uninstallWow + "Diablo II", g_uninstall + "Diablo II", g_blizzardWow, g_blizzard });
        const RegKeys user   = readRegKeys(prefix / "user.reg", { g_blizzard });
        winPaths.push_back(regValue(system, g_uninstallWow + "Diablo II", "InstallLocation"));
        winPaths.push_back(regValue(system, g_uninstall + "Diablo II", "InstallLocation"));
        winPaths.push_back(regValue(system, g_blizzardWow, "InstallPath"));
        winPaths.push_back(regValue(system, g_blizzard, "InstallPath"));
        winPaths.push_back(regValue(user, g_blizzard, "InstallPath"));
        winPaths.push_back("C:\\Program Files (x86)\\Diablo II");
        winPaths.push_back("C:\\Program Files\\Diablo II");
        winPaths.push_back("C:\\Diablo II");
    }
    std::erase(winPaths, std::string());
    if (winPaths.empty()) {
        Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": no uninstall key";
        return {};
    }

    for (const auto& winPath : winPaths) {
        const std_path root = driveRoot(prefix, winPath);
        if (root.empty()) {
            Logger(Logger::Info) << "Wine detection: " << path2string(prefix) << ": " << winPath << ": drive not mapped";
            continue;
        }
        if (const std_path dir = resolveWinPath(root, winPath); accept(dir, winPath))
            return dir;
    }
    return {};
}

std_path findUserHome(const std_path& prefix)
{
    const RegKeys     user    = readRegKeys(prefix / "user.reg", { g_shellFolders });
    const std::string appData = regValue(user, g_shellFolders, "AppData");
    if (const std_path root = driveRoot(prefix, appData); !root.empty()) {
        const std_path dir = resolveWinPath(root, appData);
        if (!dir.empty())
            return dir.parent_path().parent_path();
    }

    std_path        found;
    std::error_code ec;
    for (const auto& entry : std_fs::directory_iterator(prefix / "drive_c/users", ec)) {
        if (entry.path().filename() == "Public" || !std_fs::is_directory(entry.path() / "AppData", ec))
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
        if (!Mernel::isExistingFile(prefix / "system.reg"))
            prefix = dir / "pfx";
        if (!Mernel::isExistingFile(prefix / "system.reg")) {
            Logger(Logger::Info) << "Wine detection: " << path2string(dir) << ": no system.reg";
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

}
