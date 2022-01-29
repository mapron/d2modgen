/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPages.hpp"

#include "ConfigPageChallenge.hpp"
#include "ConfigPageCharacter.hpp"
#include "ConfigPageCube.hpp"
#include "ConfigPageDropFiltering.hpp"
#include "ConfigPageDrops.hpp"
#include "ConfigPageGambling.hpp"
#include "ConfigPageItemRandomizer.hpp"
#include "ConfigPageMonDensity.hpp"
#include "ConfigPageMonRandomizer.hpp"
#include "ConfigPageMonStats.hpp"
#include "ConfigPageQol.hpp"

namespace D2ModGen {

QList<PageGroup> CreateConfigPages(QWidget* parent)
{
    return QList<PageGroup>{
        PageGroup{
            QObject::tr("Randomizers"),
            QList<IConfigPage*>{
                new ConfigPageItemRandomizer(parent),
                new ConfigPageMonRandomizer(parent),
            },
        },
        PageGroup{
            QObject::tr("Make harder"),
            QList<IConfigPage*>{
                new ConfigPageMonStats(parent),
                new ConfigPageMonDensity(parent),
                new ConfigPageChallenge(parent),
            },
        },
        PageGroup{
            QObject::tr("Make easier"),
            QList<IConfigPage*>{
                new ConfigPageCube(parent),
                new ConfigPageGambling(parent),
                new ConfigPageCharacter(parent),
                new ConfigPageQol(parent),
                new ConfigPageDrops(parent),
            },
        },
        PageGroup{
            QObject::tr("Misc"),
            QList<IConfigPage*>{
                new ConfigPageDropFiltering(parent),
            },
        },
    };
}

}
