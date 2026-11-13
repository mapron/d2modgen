/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "LinuxShortcut.hpp"

#include "ExeIcon.hpp"

#include "MernelPlatform/FileIOUtils.hpp"
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

bool isInPath(const std::string& pathEnv, const std::string& program)
{
    size_t start = 0;
    while (start <= pathEnv.size()) {
        size_t end = pathEnv.find(':', start);
        if (end == std::string::npos)
            end = pathEnv.size();
        const std::string dir = pathEnv.substr(start, end - start);
        if (!dir.empty() && fileExists(Mernel::string2path(dir) / program))
            return true;
        start = end + 1;
    }
    return false;
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
    std::string runner;
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

std::string sanitizeFileName(const std::string& name)
{
    std::string result = name;
    for (char& c : result) {
        const bool keep = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_';
        if (!keep)
            c = '_';
    }
    return result;
}

std_path extractIcon(const std_path& exe, const std_path& iconDir, bool legacy)
{
    const auto png = extractExePngIcon(exe);
    if (!png)
        return "applications-games";

    std::error_code ec;
    std_fs::create_directories(iconDir, ec);
    const std_path iconPath = iconDir / (legacy ? "d2legacy.png" : "d2r.png");
    if (ec || !Mernel::writeFileFromBufferNoexcept(iconPath, *png))
        return "applications-games";
    return iconPath;
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
        runnerTemplate       = preset.text;
        prefix               = preset.prefix;
        result.runnerMissing = !isInPath(request.pathEnv, preset.runner);
    } else {
        runnerTemplate = request.runnerTemplate;
        prefix         = prefixFromUserHome(request.userHome);
    }

    const std::string execLine = expandTemplate(runnerTemplate, prefix, request.exe, request.args);
    const std::string title    = "Diablo II - " + request.modName + " Mod";
    const std::string fileName = "d2modgen-" + sanitizeFileName(request.modName) + ".desktop";
    const std_path    icon     = extractIcon(request.exe, request.iconDir, legacy);
    const std_path    workDir  = request.exe.parent_path();

    if (!Mernel::createDesktopEntry(request.appsDir, fileName, title, execLine, workDir, icon))
        return result;
    result.written.push_back(request.appsDir / fileName);

    std::error_code ec;
    if (!request.desktopDir.empty() && std_fs::is_directory(request.desktopDir, ec)
        && Mernel::createDesktopEntry(request.desktopDir, fileName, title, execLine, workDir, icon))
        result.written.push_back(request.desktopDir / fileName);

    result.status = LinuxShortcutResult::Status::Written;
    return result;
}

std::string linuxRunnerPreset(const Mernel::std_path& userHome, bool legacy, const Mernel::std_path& homeDir)
{
    return makePreset(userHome, legacy, homeDir).text;
}

}
