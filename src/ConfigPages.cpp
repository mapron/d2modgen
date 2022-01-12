/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"

#include "ConfigPageCharacter.hpp"
#include "ConfigPageCube.hpp"
#include "ConfigPageDifficulty.hpp"
#include "ConfigPageDropFiltering.hpp"
#include "ConfigPageDrops.hpp"
#include "ConfigPageGambling.hpp"
#include "ConfigPageQol.hpp"
#include "ConfigPageRandomizer.hpp"

QList<IConfigPage*> CreateConfigPages(QWidget* parent)
{
    return QList<IConfigPage*>{
        new ConfigPageDifficulty(parent),
        new ConfigPageDrops(parent),
        new ConfigPageDropFiltering(parent),
        new ConfigPageRandomizer(parent),
        new ConfigPageCube(parent),
        new ConfigPageGambling(parent),
        new ConfigPageCharacter(parent),
        new ConfigPageQol(parent),
    };
}
