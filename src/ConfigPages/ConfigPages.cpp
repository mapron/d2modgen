/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"

#include "ConfigPageChallenge.hpp"
#include "ConfigPageCharacter.hpp"
#include "ConfigPageCube.hpp"
#include "ConfigPageDifficulty.hpp"
#include "ConfigPageDropFiltering.hpp"
#include "ConfigPageDrops.hpp"
#include "ConfigPageGambling.hpp"
#include "ConfigPageMonRandomizer.hpp"
#include "ConfigPageQol.hpp"
#include "ConfigPageRandomizer.hpp"

namespace D2ModGen {

QList<PageGroup> CreateConfigPages(QWidget* parent)
{
    return QList<PageGroup>{
        PageGroup{
            "Randomizers",
            QList<IConfigPage*>{
                new ConfigPageRandomizer(parent),
                new ConfigPageMonRandomizer(parent),
            },
        },
        PageGroup{
            "Make harder",
            QList<IConfigPage*>{
                new ConfigPageDifficulty(parent),
                new ConfigPageChallenge(parent),
            },
        },
        PageGroup{
            "Make easier",
            QList<IConfigPage*>{
                new ConfigPageCube(parent),
                new ConfigPageGambling(parent),
                new ConfigPageCharacter(parent),
                new ConfigPageQol(parent),
                new ConfigPageDrops(parent),
                new ConfigPageDropFiltering(parent),
            },
        },
    };
}

}
