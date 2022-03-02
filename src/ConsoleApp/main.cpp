/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */

#include "ConfigHandler.hpp"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char* argv[])
{
    qDebug() << "application started";
    QCoreApplication app(argc, argv);

    D2ModGen::ConfigHandler configHandler;

    auto args = app.arguments();
    if (args.value(1) == "--generate") {
        QString file = args.value(2);
        if (!file.isEmpty())
            configHandler.loadConfig(file);
        configHandler.generate();
        return 0;
    }

    return 1;
}
