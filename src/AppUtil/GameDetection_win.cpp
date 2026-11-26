/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * Copyright (C) 2026 Bertus Steenberg / bertus.steenberg@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "GameDetection.hpp"

#include <QSettings>

namespace D2ModGen {

std::string getInstallLocationFromRegistry(GameKind kind)
{
    static const QString base("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Diablo II");
    QSettings            set(base + (kind == GameKind::Resurrected ? " Resurrected" : ""),
                             QSettings::Registry32Format);
    return set.value("InstallLocation").toString().toStdString();
}

std::vector<GameMatch> findGames(GameKind, const Mernel::std_path&, const std::string&)
{
    return {};
}

}
