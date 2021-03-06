/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

const StringVector& getTableNames();

enum class TableId
{
    weapons,
    actinfo,
    armor,
    armtype,
    automagic,
    automap,
    belts,
    bodylocs,
    books,
    charstats,
    colors,
    compcode,
    composit,
    cubemain,
    cubemod,
    difficultylevels,
    elemtypes,
    events,
    experience,
    gamble,
    gems,
    hireling,
    hitclass,
    inventory,
    itemratio,
    itemstatcost,
    itemtypes,
    levels,
    lowqualityitems,
    lvlmaze,
    lvlprest,
    lvlsub,
    lvltypes,
    lvlwarp,
    magicprefix,
    magicsuffix,
    misc,
    misscalc,
    missiles,
    monai,
    monequip,
    monlvl,
    monmode,
    monplace,
    monpreset,
    monprop,
    monseq,
    monsounds,
    monstats,
    monstats2,
    montype,
    monumod,
    npc,
    objects,
    objgroup,
    objmode,
    objpreset,
    objtype,
    overlay,
    pettype,
    playerclass,
    plrmode,
    plrtype,
    properties,
    qualityitems,
    rareprefix,
    raresuffix,
    runes,
    setitems,
    sets,
    shrines,
    skillcalc,
    skilldesc,
    skills,
    soundenviron,
    sounds,
    states,
    storepage,
    superuniques,
    treasureclassex,
    uniqueappellation,
    uniqueitems,
    uniqueprefix,
    uniquesuffix,
    wanderingmon,
};

std::string getTableIdString(TableId id);
bool        findTableId(const std::string& name, TableId& id);

}
