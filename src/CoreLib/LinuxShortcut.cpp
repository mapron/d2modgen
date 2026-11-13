/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "LinuxShortcut.hpp"

#include "MernelPlatform/ShellUtils.hpp"

namespace D2ModGen {

namespace {

using Mernel::std_path;
namespace std_fs = Mernel::std_fs;

bool fileExists(const std_path& path)
{
    std::error_code ec;
    return std_fs::exists(path, ec);
}

bool hasProtonFiles(const std_path& dir)
{
    return fileExists(dir / "version") || fileExists(dir / "tracked_files");
}

bool isSteamD2RPrefix(const std_path& prefix)
{
    // <steam library>/steamapps/compatdata/2536520/pfx
    std::vector<std::string> parts;
    for (const auto& part : prefix)
        parts.push_back(Mernel::path2string(part));
    const size_t n = parts.size();
    return n >= 4 && parts[n - 4] == "steamapps" && parts[n - 3] == "compatdata" && parts[n - 2] == "2536520" && parts[n - 1] == "pfx";
}

bool hasGeProton(const std_path& homeDir)
{
    for (const auto& toolsDir : { homeDir / ".local/share/Steam/compatibilitytools.d", homeDir / ".steam/root/compatibilitytools.d" }) {
        std::error_code ec;
        for (const auto& entry : std_fs::directory_iterator(toolsDir, ec)) {
            if (entry.is_directory(ec) && Mernel::path2string(entry.path().filename()).starts_with("GE-Proton"))
                return true;
        }
    }
    return false;
}

bool isExecutable(const std_path& path)
{
    std::error_code ec;
    const auto      status = std_fs::status(path, ec);
    using std_fs::perms;
    return !ec && std_fs::is_regular_file(status) && (status.permissions() & (perms::owner_exec | perms::group_exec | perms::others_exec)) != perms::none;
}

// The runner's full path: as written if it contains a '/', else the first executable match in PATH (like a shell).
std_path findProgram(const std::string& pathEnv, const std::string& program)
{
    if (program.find('/') != std::string::npos)
        return isExecutable(Mernel::string2path(program)) ? Mernel::string2path(program) : std_path();

    size_t start = 0;
    while (start <= pathEnv.size()) {
        size_t end = pathEnv.find(':', start);
        if (end == std::string::npos)
            end = pathEnv.size();
        const std::string dir = pathEnv.substr(start, end - start);
        if (!dir.empty() && isExecutable(Mernel::string2path(dir) / program))
            return (Mernel::string2path(dir) / program).lexically_normal();
        start = end + 1;
    }
    return {};
}

std_path prefixFromUserHome(const std_path& userHome)
{
    std_path user = userHome.lexically_normal();
    if (!user.has_filename())
        user = user.parent_path();
    return user.parent_path().parent_path().parent_path();
}

struct Preset {
    std::string text;
    std::string runner; // program that must be installed, e.g. umu-run behind "env"
    std_path    prefix; // value of {prefix} for this preset
};

Preset makePreset(const std_path& userHome, bool legacy, const std_path& homeDir)
{
    const std_path prefix = prefixFromUserHome(userHome);
    if (!legacy && isSteamD2RPrefix(prefix))
        return { "steam -applaunch 2536520 {args}", "steam", prefix };

    // Proton keeps its own files next to pfx/ (Steam) or inside a umu-made prefix (pfx -> .).
    std::error_code ec;
    std_path        umuPrefix;
    if (prefix.filename() == "pfx" && hasProtonFiles(prefix.parent_path()))
        umuPrefix = prefix.parent_path();
    else if (hasProtonFiles(prefix) || std_fs::is_symlink(prefix / "pfx", ec))
        umuPrefix = prefix;
    else
        return {};

    std::string text = "env WINEPREFIX={prefix} ";
    if (hasGeProton(homeDir))
        text += "PROTONPATH=GE-Proton ";
    text += "GAMEID=umu-battlenet PROTON_VERB=run umu-run {exe} {args}";
    return { text, "umu-run", umuPrefix };
}

// Exec key of the Desktop Entry spec: quoting rules first, then escaping of the string value.
std::string escapeExecValue(const std::string& arg, bool quote)
{
    std::string quoted;
    if (quote) {
        quoted += '"';
        for (char c : arg) {
            if (c == '"' || c == '`' || c == '$' || c == '\\')
                quoted += '\\';
            quoted += c;
        }
        quoted += '"';
    } else {
        quoted = arg;
    }

    std::string result;
    for (char c : quoted) {
        if (c == '\\')
            result += "\\\\";
        else if (c == '\n')
            result += "\\n";
        else if (c == '\t')
            result += "\\t";
        else if (c == '\r')
            result += "\\r";
        else if (c == '%')
            result += "%%";
        else
            result += c;
    }
    return result;
}

bool needsQuotes(const std::string& arg)
{
    for (char c : arg) {
        const bool plain = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
                           || c == '-' || c == '_' || c == '.' || c == '/' || c == '=' || c == ':' || c == '+' || c == ',' || c == '@';
        if (!plain)
            return true;
    }
    return arg.empty();
}

std::string expandTemplate(const std::string& runnerTemplate, const std_path& prefix, const std_path& exe, const std::string& args)
{
    std::string argsValue;
    size_t      start = 0;
    while (start < args.size()) {
        size_t end = args.find(' ', start);
        if (end == std::string::npos)
            end = args.size();
        const std::string arg = args.substr(start, end - start);
        if (!arg.empty()) {
            if (!argsValue.empty())
                argsValue += ' ';
            argsValue += escapeExecValue(arg, needsQuotes(arg));
        }
        start = end + 1;
    }

    const std::pair<std::string, std::string> placeholders[] = {
        { "{prefix}", escapeExecValue(Mernel::path2string(prefix), true) },
        { "{exe}", escapeExecValue(Mernel::path2string(exe), true) },
        { "{args}", argsValue },
    };

    std::string result;
    for (size_t pos = 0; pos < runnerTemplate.size();) {
        bool replaced = false;
        for (const auto& [key, value] : placeholders) {
            if (runnerTemplate.compare(pos, key.size(), key) == 0) {
                result += value;
                pos += key.size();
                replaced = true;
                break;
            }
        }
        if (!replaced)
            result += runnerTemplate[pos++];
    }
    return result;
}

}

LinuxShortcutResult makeLinuxShortcut(const LinuxShortcutRequest& request)
{
    LinuxShortcutResult result;

    const bool   legacy = request.exe.filename() != "D2R.exe";
    const Preset preset = makePreset(request.userHome, legacy, request.homeDir);
    std::string  runnerTemplate;
    std_path     prefix;
    if (request.runnerTemplate.empty()) {
        if (preset.text.empty()) {
            result.status = LinuxShortcutResult::Status::NoPreset;
            return result;
        }
        runnerTemplate = preset.text;
        prefix         = preset.prefix;
    } else {
        runnerTemplate = request.runnerTemplate;
        prefix         = prefixFromUserHome(request.userHome);
    }

    // The first word of the template is the program the launcher runs; the rest are its arguments.
    const size_t      split   = runnerTemplate.find_first_of(" \t");
    const std::string first   = runnerTemplate.substr(0, split);
    const std_path    program = findProgram(request.pathEnv, first);
    result.runner             = request.runnerTemplate.empty() ? preset.runner : first;
    if (program.empty() || findProgram(request.pathEnv, result.runner).empty()) {
        result.status = LinuxShortcutResult::Status::RunnerNotFound;
        return result;
    }
    const std::string args  = split == std::string::npos ? std::string() : expandTemplate(runnerTemplate.substr(split + 1), prefix, request.exe, request.args);
    const std::string title = "Diablo II - " + request.modName + " Mod";
    const std_path    toDir = request.exe.parent_path();

    if (!Mernel::createShortCut(request.appsDir, title, toDir, Mernel::path2string(program), args, request.iconPath))
        return result;
    result.written.push_back(request.appsDir / (title + ".desktop"));

    // Desktop may be configured as the home dir itself (XDG_DESKTOP_DIR="$HOME/"); don't drop a launcher there.
    std::error_code ec;
    const bool      desktopIsHome = std_fs::equivalent(request.desktopDir, request.homeDir, ec);
    if (!request.desktopDir.empty() && !desktopIsHome && std_fs::is_directory(request.desktopDir, ec)
        && Mernel::createShortCut(request.desktopDir, title, toDir, Mernel::path2string(program), args, request.iconPath))
        result.written.push_back(request.desktopDir / (title + ".desktop"));

    result.status = LinuxShortcutResult::Status::Written;
    return result;
}

std::string linuxRunnerPreset(const Mernel::std_path& userHome, bool legacy, const Mernel::std_path& homeDir)
{
    return makePreset(userHome, legacy, homeDir).text;
}

}
