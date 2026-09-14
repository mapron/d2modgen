/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "ConfigHandler.hpp"

#include "Logger.hpp"

int main(int argc, char* argv[])
{
    using namespace D2ModGen;

    ConfigHandler configHandler;
    configHandler.loadConfig(configHandler.m_defaultPath);
    auto res = configHandler.generate();
    if (!res.m_success) {
        Logger(Logger::Err) << res.m_error;
        return 1;
    }
    configHandler.saveConfig(configHandler.m_defaultPath);
    Logger(Logger::Notice) << "Success!";
    return 0;
}
