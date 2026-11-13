/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "ModgenCoreExport.hpp"

#include "MernelPlatform/FsUtils.hpp"

#include <string>
#include <vector>

namespace D2ModGen {

struct LinuxShortcutRequest {
    Mernel::std_path userHome;       // Wine user root, <prefix>/drive_c/users/<user>/
    Mernel::std_path exe;            // <install path>/D2R.exe or Diablo II.exe
    std::string      args;           // game command line, e.g. "-mod rando -txt"
    std::string      runnerTemplate; // empty = preset for the prefix
    std::string      modName;
    Mernel::std_path appsDir;    // ~/.local/share/applications
    Mernel::std_path desktopDir; // skipped if empty, missing or the home dir itself
    Mernel::std_path iconPath;   // icon file for the launcher, may be empty
    Mernel::std_path homeDir;    // to find GE-Proton, and to tell a Desktop that is just the home dir
    std::string      pathEnv;    // to find the runner
};

struct LinuxShortcutResult {
    enum class Status
    {
        Written,
        NoPreset,
        RunnerNotFound,
        WriteFailed,
    };
    Status                        status = Status::WriteFailed;
    std::vector<Mernel::std_path> written;
    std::string                   runner; // first word of the template, e.g. "umu-run" or "steam"
};

/// Writes "Diablo II - <mod> Mod.desktop" launchers that start the game through a runner (umu-run, steam or a user template).
MODGENCORE_EXPORT LinuxShortcutResult makeLinuxShortcut(const LinuxShortcutRequest& request);

/// Runner template preset for the Wine prefix that holds userHome; empty if there is none (plain Wine prefix).
MODGENCORE_EXPORT std::string linuxRunnerPreset(const Mernel::std_path& userHome, bool legacy, const Mernel::std_path& homeDir);

}
