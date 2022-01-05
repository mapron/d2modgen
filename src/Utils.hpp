/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

bool readJsonFile(const QString& file, QJsonObject& data);
bool writeJsonFile(const QString& file, const QJsonObject& data, bool escape = false);

bool readCSV(const QString& csvData, Table& table);
bool writeCSV(QString& csvData, const Table& table);

bool readCSVfile(const QString& csvFilename, Table& table);
bool writeCSVfile(const QString& csvFilename, const Table& table);

namespace details {

template<typename F>
struct ScopeExitFunctionWrapper {
    ScopeExitFunctionWrapper(const F& f)
        : f(f)
    {}

    ScopeExitFunctionWrapper(const ScopeExitFunctionWrapper&) = delete;
    ScopeExitFunctionWrapper(ScopeExitFunctionWrapper&&)      = default;

    ScopeExitFunctionWrapper& operator=(const ScopeExitFunctionWrapper&) = delete;
    ScopeExitFunctionWrapper& operator=(ScopeExitFunctionWrapper&&) = default;

    ~ScopeExitFunctionWrapper() { f(); }
    F f;
};

template<typename F>
static constexpr ScopeExitFunctionWrapper<F> createScopeExitFunctionWrapper(const F& f)
{
    return ScopeExitFunctionWrapper<F>(f);
}

}
#define MODGEN_DO_STRING_JOIN2(arg1, arg2) arg1##arg2
#define MODGEN_STRING_JOIN2(arg1, arg2) MODGEN_DO_STRING_JOIN2(arg1, arg2)

#define MODGEN_SCOPE_EXIT(...) \
    auto MODGEN_STRING_JOIN2(scope_exit_, __LINE__) = details::createScopeExitFunctionWrapper(__VA_ARGS__); \
    (void) MODGEN_STRING_JOIN2(scope_exit_, __LINE__)

static const QStringList g_tableNames{
    "weapons",
    "actinfo",
    "armor",
    "armtype",
    "automagic",
    "automap",
    "belts",
    "bodylocs",
    "books",
    "charstats",
    "colors",
    "compcode",
    "composit",
    "cubemain",
    "cubemod",
    "difficultylevels",
    "elemtypes",
    "events",
    "experience",
    "gamble",
    "gems",
    "hireling",
    "hitclass",
    "inventory",
    "itemratio",
    "itemstatcost",
    "itemtypes",
    "levels",
    "lowqualityitems",
    "lvlmaze",
    "lvlprest",
    "lvlsub",
    "lvltypes",
    "lvlwarp",
    "magicprefix",
    "magicsuffix",
    "misc",
    "misscalc",
    "missiles",
    "monai",
    "monequip",
    "monlvl",
    "monmode",
    "monplace",
    "monpreset",
    "monprop",
    "monseq",
    "monsounds",
    "monstats",
    "monstats2",
    "montype",
    "monumod",
    "npc",
    "objects",
    "objgroup",
    "objmode",
    "objpreset",
    "objtype",
    "overlay",
    "pettype",
    "playerclass",
    "plrmode",
    "plrtype",
    "properties",
    "qualityitems",
    "rareprefix",
    "raresuffix",
    "runes",
    "setitems",
    "sets",
    "shrines",
    "skillcalc",
    "skilldesc",
    "skills",
    "soundenviron",
    "sounds",
    "states",
    "storepage",
    "superuniques",
    "treasureclassex",
    "uniqueappellation",
    "uniqueitems",
    "uniqueprefix",
    "uniquesuffix",
    "wanderingmon",
};
