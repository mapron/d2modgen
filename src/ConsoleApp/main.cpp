/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "ConfigHandler.hpp"

#include "Logger.hpp"

int main(int argc, char* argv[])
{
    using namespace D2ModGen;
    Logger() << "application started";

    ConfigHandler configHandler;

    if (argc == 3 && std::string(argv[1]) == "--generate") {
        std::string file = argv[2];
        if (!file.empty())
            configHandler.loadConfig(file);
        configHandler.generate();
        return 0;
    }

    return 1;
}
