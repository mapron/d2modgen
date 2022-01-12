/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"

#include "ConfigPageCube.hpp"
#include "ConfigPageDifficulty.hpp"
#include "ConfigPageDropFiltering.hpp"
#include "ConfigPageDrops.hpp"
#include "ConfigPageGambling.hpp"
#include "ConfigPageRandomizer.hpp"

QList<IConfigPage*> CreateConfigPages(QWidget* parent)
{
    return QList<IConfigPage*>{
        new DifficultyPage(parent),
        new DropsPage(parent),
        new DropFiltering(parent),
        new RandomizerPage(parent),
        new CubePage(parent),
        new ConfigPageGambling(parent),
    };
}
