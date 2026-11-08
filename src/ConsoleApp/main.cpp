/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "ConfigHandler.hpp"

#include "Logger.hpp"
#include "MernelPlatform/Logger_details.hpp"

int main(int argc, char* argv[])
{
    using namespace D2ModGen;

    // verbose mode
    const bool verbose = (argc == 2 && argv[1] == std::string_view("-v"));

    Logger::SetLoggerBackend(std::make_unique<Mernel::LoggerBackendConsole>(verbose ? Logger::Debug : Logger::Info, false, false, false, Mernel::LoggerBackendConsole::Type::Cout));

    ConfigHandler configHandler;
    configHandler.loadConfig(configHandler.m_defaultPath);
    GenerationEnvironment env;
    if (!env.init(configHandler.m_config))
        return EXIT_FAILURE;

    if (!configHandler.generate(env)) {
        return EXIT_FAILURE;
    }
    configHandler.saveConfig(configHandler.m_defaultPath);
    Logger(Logger::Info) << "Success!";
    return EXIT_SUCCESS;
}
