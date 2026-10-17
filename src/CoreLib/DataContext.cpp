/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "Logger.hpp"
#include "MernelPlatform/FileFormatCSV.hpp"
#include "MernelPlatform/FileFormatJson.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <algorithm>

#define parse(id) \
    if (!parseTable(id, TableId::id, data)) \
    return false

#define serialize(id) \
    if (!serializeTable(id, TableId::id, data)) \
    return false

namespace D2ModGen {

namespace {

static const std::map<TableId, std::vector<std::string>> s_tableKeys{
    { TableId::gamble, { "code" } },
    { TableId::misc, { "code" } },
    { TableId::gems, { "code" } },
    { TableId::uniqueitems, { "index" } },
    { TableId::setitems, { "index" } },
    { TableId::sets, { "index" } },
    { TableId::runes, { "Name" } },
    { TableId::skills, { "skill" } },
    { TableId::monstats, { "Id" } },
    { TableId::difficultylevels, { "Name" } },
    { TableId::levels, { "Name" } },
    { TableId::monlvl, { "Level" } },
    { TableId::charstats, { "class" } },
    { TableId::weapons, { "code" } },
    { TableId::armor, { "code" } },
};

using AliasConfig = std::vector<std::pair<std::string, std::string>>;
static const std::map<TableId, AliasConfig> s_tableColumnAliases{
    { TableId::itemtypes, AliasConfig{
                              { "MaxSockets3", "MaxSock40" },
                          } },
    { TableId::levels, AliasConfig{
                           { "MonLvlEx(N)", "MonLvl2Ex" },
                           { "MonLvlEx(H)", "MonLvl3Ex" },
                       } },
    { TableId::uniqueitems, AliasConfig{
                                { "diablocloneweight", "worldevent" },
                                { "level", "lvl" },
                            } },
    { TableId::setitems, AliasConfig{
                             { "diablocloneweight", "worldevent" },
                         } },
    { TableId::armor, AliasConfig{
                          { "diablocloneweight", "worldevent" },
                      } },
    { TableId::weapons, AliasConfig{
                            { "diablocloneweight", "worldevent" },
                        } },
    { TableId::misc, AliasConfig{
                         { "diablocloneweight", "worldevent" },
                     } },
    { TableId::cubemain, AliasConfig{
                             { "Name", "description" },
                         } },
    { TableId::monstats, AliasConfig{
                             { "TreasureClass1", "TreasureClass" },
                             { "TreasureClass1(N)", "TreasureClass(N)" },
                             { "TreasureClass1(H)", "TreasureClass(H)" },
                             { "TreasureClass2", "TreasureClassChamp" },
                             { "TreasureClass3", "TreasureClassUnique" },
                             { "TreasureClass4", "TreasureClassQuest" },
                             { "TreasureClass2(N)", "TreasureClassChamp(N)" },
                             { "TreasureClass3(N)", "TreasureClassUnique(N)" },
                             { "TreasureClass4(N)", "TreasureClassQuest(N)" },
                             { "TreasureClass2(H)", "TreasureClassChamp(H)" },
                             { "TreasureClass3(H)", "TreasureClassUnique(H)" },
                             { "TreasureClass4(H)", "TreasureClassQuest(H)" },
                         } },
};

}

DataContext::DataContext() = default;

DataContext::~DataContext() = default;

bool DataContext::parseData(const IStorage::StoredData& data)
{
    dataVersion = data.dataVersion;

    parse(weapons);
    parse(armor);
    parse(automagic);
    parse(charstats);
    parse(cubemain);
    parse(difficultylevels);
    parse(experience);
    parse(gamble);
    parse(gems);
    parse(hireling);
    parse(itemstatcost);
    parse(itemtypes);
    parse(levels);
    parse(magicprefix);
    parse(magicsuffix);
    parse(misc);
    parse(monlvl);
    parse(monstats);
    parse(runes);
    parse(setitems);
    parse(sets);
    parse(skilldesc);
    parse(skills);
    parse(treasureclassex);
    parse(uniqueitems);

    for (const auto& [id, fileData] : data.jsonFiles) {
        Mernel::PropertyTree doc;
        Logger() << "parse json: " << std::string(getJsonRelPath(id));
        if (!readJsonFromBufferNoexcept(fileData, doc)) {
            Logger(Logger::Warning) << "failed to parse json: " << getJsonRelPath(id);
            return false;
        }
        jsonFiles[id] = std::move(doc);
    }

    return true;
}

bool DataContext::serializeData(IStorage::StoredData& data) const
{
    data.valid       = true;
    data.dataVersion = dataVersion;

    serialize(weapons);
    serialize(armor);
    serialize(automagic);
    serialize(charstats);
    serialize(cubemain);
    serialize(difficultylevels);
    serialize(experience);
    serialize(gamble);
    serialize(gems);
    serialize(hireling);
    serialize(itemstatcost);
    serialize(itemtypes);
    serialize(levels);
    serialize(magicprefix);
    serialize(magicsuffix);
    serialize(misc);
    serialize(monlvl);
    serialize(monstats);
    serialize(runes);
    serialize(setitems);
    serialize(sets);
    serialize(skilldesc);
    serialize(skills);
    serialize(treasureclassex);
    serialize(uniqueitems);

    for (const auto& [id, json] : jsonFiles) {
        std::string buffer;
        writeJsonToBufferNoexcept(buffer, json);
        data.jsonFiles[id] = std::move(buffer);
    }

    return true;
}

template<class Row>
bool DataContext::parseTable(DataTable<Row>& table, TableId tableId, const IStorage::StoredData& data)
{
    if (!data.tables.contains(tableId))
        return true;
    const std::string& buffer = data.tables.at(tableId);

    Mernel::CSVTable csv;
    if (!Mernel::readCSVFromBuffer(buffer, csv)) {
        Logger(Logger::Warning) << "failed to parse csv:" << getTableIdString(tableId);
        return false;
    }
    table.columnsOriginal                      = csv.columns;
    table.valid                                = true;
    const std::vector<std::string> usedColumns = Row{}.getColumns();
    table.indicesUsed.resize(usedColumns.size(), size_t(-1));

    for (size_t i = 0; i < table.columnsOriginal.size(); i++) {
        std::string col = Mernel::strToLower(table.columnsOriginal[i]);

        auto it = std::find(usedColumns.cbegin(), usedColumns.cend(), col);
        if (it == usedColumns.cend()) {
            table.indicesUnused.push_back(i);
        } else {
            auto usedIndex               = std::distance(usedColumns.cbegin(), it);
            table.indicesUsed[usedIndex] = i;
            table.columnsUsed.push_back(col);
        }
    }
    for (size_t i = 0; i < table.indicesUsed.size(); i++) {
        if (table.indicesUsed[i] == size_t(-1)) {
            Logger(Logger::Warning) << "Missing column '" << usedColumns[i] << "' in table '" << getTableIdString(tableId) << "', generation may be incomplete.";
        }
    }

    for (auto& rowCsv : csv.rows) {
        Row row;
        row._ser_unusedCells.resize(table.indicesUnused.size());
        row._ser_usedCells.resize(table.indicesUsed.size());

        for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
            row._ser_unusedCells[i] = rowCsv.data[table.indicesUnused[i]].str;
        }
        for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
            if (table.indicesUsed[i] != size_t(-1))
                row._ser_usedCells[i] = rowCsv.data[table.indicesUsed[i]].str;
        }
        row._ser_read      = true;
        row._ser_cellIndex = 0;
        row.ser();
        row._ser_usedCells.clear();
        table.rows.push_back(std::move(row));
    }

    Logger() << "parse '" << getTableIdString(tableId) << "' buffer[" << buffer.size() << "] => csv[" << csv.columns.size() << "x" << csv.rows.size() << "]";

    return true;
}

template<class Row>
bool DataContext::serializeTable(const DataTable<Row>& table, TableId tableId, IStorage::StoredData& data) const
{
    if (!table.valid)
        return true;

    std::string& buffer = data.tables[tableId];

    Mernel::CSVTable csv;
    csv.columns    = table.columnsOriginal;
    csv.endsWithNL = true;

    for (auto row : table.rows) {
        row._ser_usedCells.resize(table.indicesUsed.size());
        row._ser_read      = false;
        row._ser_cellIndex = 0;
        row.ser();
        Mernel::CSVTableRow rowCsv;
        rowCsv.data.resize(csv.columns.size());

        for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
            rowCsv.data[table.indicesUnused[i]].str = row._ser_unusedCells[i];
        }
        for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
            if (table.indicesUsed[i] != size_t(-1))
                rowCsv.data[table.indicesUsed[i]].str = row._ser_usedCells[i];
        }
        csv.rows.push_back(std::move(rowCsv));
    }

    if (!Mernel::writeCSVToBuffer(buffer, csv))
        return false;

    Logger() << "serialize '" << getTableIdString(tableId) << "' csv[" << csv.columns.size() << "x" << csv.rows.size() << "] => buffer[" << buffer.size() << "] ";

    return true;
}

std::vector<std::string> RowBaseAffixes::getColumns() const
{
    std::vector<std::string> result{ "level", "maxlevel", "levelreq", "spawnable" };
    for (size_t i = 0; i < maxAffixes; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("mod" + istr + "code");
        result.push_back("mod" + istr + "param");
        result.push_back("mod" + istr + "min");
        result.push_back("mod" + istr + "max");
    }

    return result;
}

void RowBaseAffixes::ser()
{
    serf(lvl);
    serf(maxlevel);
    serf(lvlreq);
    serf(spawnable);
    serf(affixes, maxAffixes);
}

std::vector<std::string> DataRow_weapons::getColumns() const
{
    return { "level", "levelreq", "reqstr", "reqdex", "showlevel", "mindam", "maxdam", "code", "type" };
}

void DataRow_weapons::ser()
{
    serf(lvl);
    serf(lvlreq);
    serf(reqstr);
    serf(reqdex);
    serf(mindam);
    serf(maxdam);
    serf(code);
    serf(type);
}

std::vector<std::string> DataRow_armor::getColumns() const
{
    return { "level", "levelreq", "reqstr", "reqdex", "showlevel", "minac", "maxac", "code", "type" };
}

void DataRow_armor::ser()
{
    serf(lvl);
    serf(lvlreq);
    serf(reqstr);
    serf(reqdex);
    serf(minac);
    serf(maxac);
    serf(code);
    serf(type);
}

std::vector<std::string> DataRow_charstats::getColumns() const
{
    std::vector<std::string> result{ "class", "statperlevel", "skillsperlevel" };
    for (size_t i = 0; i < maxItems; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("item" + istr + "count");
        result.push_back("item" + istr + "quality");
        result.push_back("item" + istr);
        result.push_back("item" + istr + "loc");
    }
    for (size_t i = 0; i < maxSkills; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("skill " + istr);
    }

    return result;
}

void DataRow_charstats::ser()
{
    serf(clas);
    serf(statPerLevel);
    serf(skillsPerLevel);
    serArray(items, maxItems, [this](Item& item) {
        serf(item.count);
        serf(item.quality);
        serf(item.code);
        serf(item.loc); }, [](Item& item) { return !item.code.empty(); });

    serf(skills, maxSkills);
}

std::vector<std::string> DataRow_cubemain::getColumns() const
{
    std::vector<std::string> result{
        "description",
        "enabled",
        "version",
        "op",
        "*eol",
        "numinputs",
        "output",
        "ilvl",
        "plvl",
        "lvl",
    };

    for (std::string a : { "", "b ", "c " }) {
        for (size_t i = 0; i < affixes_a.getLimit(); ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(a + "mod " + istr);
            result.push_back(a + "mod " + istr + " param");
            result.push_back(a + "mod " + istr + " min");
            result.push_back(a + "mod " + istr + " max");
            result.push_back(a + "mod " + istr + " chance");
        }
    }

    for (size_t i = 0; i < maxInputs; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("input " + istr);
    }

    return result;
}

void DataRow_cubemain::ser()
{
    serf(description);
    serf(enabled);
    serf(version);
    serf(op);
    serf(eol);
    serf(numinputs);
    serf(output);
    serf(ilvl);
    serf(plvl);
    serf(lvl);

    for (AffixPropList* list : affixLists()) {
        serf(*list, true);
    }
    serf(inputs, maxInputs);
}

std::vector<std::string> DataRow_difficultylevels::getColumns() const
{
    return {
        "name",
        "resistpenalty",
        "gamblerare",
        "gambleset",
        "gambleunique",
        "gambleuber",
        "gambleultra",
    };
}

void DataRow_difficultylevels::ser()
{
    serf(name);
    serf(resistPenalty);

    serf(gambleRare);
    serf(gambleSet);
    serf(gambleUnique);
    serf(gambleUber);
    serf(gambleUltra);
}

std::vector<std::string> DataRow_experience::getColumns() const
{
    return { "expratio" };
}

void DataRow_experience::ser()
{
    serf(expRatio);
}

std::vector<std::string> DataRow_gamble::getColumns() const
{
    return { "name", "code" };
}

void DataRow_gamble::ser()
{
    serf(name);
    serf(code);
}

std::vector<std::string> DataRow_gems::getColumns() const
{
    std::vector<std::string> result{};

    for (std::string a : { "weapon", "helm", "shield" }) {
        for (size_t i = 0; i < affixes_weapon.getLimit(); ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(a + "mod" + istr + "code");
            result.push_back(a + "mod" + istr + "param");
            result.push_back(a + "mod" + istr + "min");
            result.push_back(a + "mod" + istr + "max");
        }
    }

    return result;
}

void DataRow_gems::ser()
{
    for (AffixPropList* list : affixLists()) {
        serf(*list);
    }
}

std::vector<std::string> DataRow_hireling::getColumns() const
{
    return { "hp", "hp/lvl", "dmg-min", "dmg-max", "dmg/lvl" };
}

void DataRow_hireling::ser()
{
    serf(hp);
    serf(hp_lvl);
    serf(dmg_min);
    serf(dmg_max);
    serf(dmg_lvl);
}

std::vector<std::string> DataRow_itemstatcost::getColumns() const
{
    return { "multiply", "add" };
}

void DataRow_itemstatcost::ser()
{
    serf(multiply);
    serf(add);
}

std::vector<std::string> DataRow_itemtypes::getColumns() const
{
    return { "code", "itemtype", "equiv1", "equiv2", "throwable", "repair", "shoots", "maxsockets1", "maxsockets2", "maxsockets3" };
}

void DataRow_itemtypes::ser()
{
    serf(code);
    serf(itemType);
    serf(equiv1);
    serf(equiv2);

    serf(throwable);
    serf(repair);
    serf(shoots);
    for (size_t i = 0; i < 3; ++i)
        serf(sockets[i]);

    if (_ser_read) {
        flags.clear();
        if (throwable.value_or(false))
            flags.insert(AttributeFlag::Quantity);
        else if (repair.value_or(false))
            flags.insert(AttributeFlag::Durability);

        if (sockets[0].value_or(0))
            flags.insert(AttributeFlag::Sockets);
        if (shoots.value_or(false))
            flags.insert(AttributeFlag::Missile);
        if (itemType.starts_with("Map"))
            flags.insert(AttributeFlag::PD2Map);
    }
}

std::vector<std::string> DataRow_levels::getColumns() const
{
    std::vector<std::string> result{
        // clang-format off
        "name", "nummon",
        "monumin"  , "monumin(n)"  , "monumin(h)",
        "monumax"  , "monumax(n)"  , "monumax(h)",
        "monden"   , "monden(n)"   , "monden(h)" ,
        "monlvlex" , "monlvlex(n)" , "monlvlex(h)",
        // clang-format on
    };
    for (std::string colTpl : { "mon", "nmon", "umon" }) {
        for (size_t i = 0; i < maxTypes; ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(colTpl + istr);
        }
    }
    return result;
}

void DataRow_levels::ser()
{
    serf(name);
    serf(numMon);
    for (size_t i = 0; i < 3; ++i)
        serf(monUMin[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(monUMax[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(monDen[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(monLvlEx[i]);

    serf(mon, maxTypes);
    serf(nmon, maxTypes);
    serf(umon, maxTypes);
}

std::vector<std::string> DataRow_misc::getColumns() const
{
    return { "code", "type", "maxstack", "levelreq", "level" };
}

void DataRow_misc::ser()
{
    serf(code);
    serf(type);
    serf(maxstack);
    serf(lvlreq);
    serf(lvl);
}

std::vector<std::string> DataRow_monlvl::getColumns() const
{
    return {
        // clang-format off
         "th", "th(n)", "th(h)", "l-th", "l-th(n)", "l-th(h)",
         "ac", "ac(n)", "ac(h)", "l-ac", "l-ac(n)", "l-ac(h)",
         "hp", "hp(n)", "hp(h)", "l-hp", "l-hp(n)", "l-hp(h)",
         "dm", "dm(n)", "dm(h)", "l-dm", "l-dm(n)", "l-dm(h)",
         "xp", "xp(n)", "xp(h)", "l-xp", "l-xp(n)", "l-xp(h)",
        // clang-format on
        "level",
    };
}

void DataRow_monlvl::ser()
{
    for (size_t i = 0; i < 6; ++i)
        serf(TH[i]);
    for (size_t i = 0; i < 6; ++i)
        serf(AC[i]);
    for (size_t i = 0; i < 6; ++i)
        serf(HP[i]);
    for (size_t i = 0; i < 6; ++i)
        serf(DM[i]);
    for (size_t i = 0; i < 6; ++i)
        serf(XP[i]);

    serf(level);
}

std::vector<std::string> DataRow_monstats::getColumns() const
{
    return {
        // clang-format off
        "resfi" , "resfi(n)", "resfi(h)",
        "resli" , "resli(n)", "resli(h)",
        "resco" , "resco(n)", "resco(h)",
        "respo" , "respo(n)", "respo(h)",
        "resma" , "resma(n)", "resma(h)",
        "resdm" , "resdm(n)", "resdm(h)",

        "treasureclass1",
        "treasureclass2",
        "treasureclass3",
        "treasureclass4",

        "TreasureClassDesecrated",
        "TreasureClassChampDesecrated",
        "TreasureClassUniqueDesecrated",
        "TreasureClassDesecrated(N)",
        "TreasureClassChampDesecrated(N)",
        "TreasureClassUniqueDesecrated(N)",
        "TreasureClassDesecrated(H)",
        "TreasureClassChampDesecrated(H)",
        "TreasureClassUniqueDesecrated(H)",

        "sk1lvl",
        "sk2lvl",
        "sk3lvl",
        "sk4lvl",
        "sk5lvl",
        "sk6lvl",
        "sk7lvl",
        "sk8lvl",

        "spawn", "minion1", "minion2",

        "isspawn", "killable", "npc", "boss",
        "level", "mingrp", "maxgrp",
        "id", "baseid", "*hcidx",
        // clang-format on
    };
}

void DataRow_monstats::ser()
{
    for (size_t i = 0; i < 3; ++i)
        serf(resFi[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(resLi[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(resCo[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(resPo[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(resMa[i]);
    for (size_t i = 0; i < 3; ++i)
        serf(resDm[i]);

    serf(tc, tcCount);
    serf(skLevels, skCount);
    serf(minionIds, minionCount);

    serf(spawn);
    serf(killable);
    serf(npc);
    serf(boss);

    serf(level);
    serf(minGrp);
    serf(maxGrp);

    serf(id);
    serf(baseId);
    serf(hcIdx);
}

std::vector<std::string> DataRow_runes::getColumns() const
{
    std::vector<std::string> result{ "complete" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("t1code" + istr);
        result.push_back("t1param" + istr);
        result.push_back("t1min" + istr);
        result.push_back("t1max" + istr);
    }
    for (size_t i = 0; i < maxRunes; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("rune" + istr);
    }
    return result;
}

void DataRow_runes::ser()
{
    serf(complete);
    serf(affixes);
    serf(runeCodes, maxRunes);
}

std::vector<std::string> DataRow_setitems::getColumns() const
{
    std::vector<std::string> result{ "lvl", "lvl req" };
    for (size_t i = 0; i < affixes_base.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prop" + istr);
        result.push_back("par" + istr);
        result.push_back("min" + istr);
        result.push_back("max" + istr);
    }
    for (size_t i = 0; i < affixes_partial1.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("aprop" + istr + "a");
        result.push_back("apar" + istr + "a");
        result.push_back("amin" + istr + "a");
        result.push_back("amax" + istr + "a");
    }
    for (size_t i = 0; i < affixes_partial2.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("aprop" + istr + "b");
        result.push_back("apar" + istr + "b");
        result.push_back("amin" + istr + "b");
        result.push_back("amax" + istr + "b");
    }

    return result;
}

void DataRow_setitems::ser()
{
    serf(lvl);
    serf(lvlreq);

    for (auto* affixList : affixLists())
        serf(*affixList);
}

std::vector<std::string> DataRow_sets::getColumns() const
{
    std::vector<std::string> result{ "lvl" };
    for (size_t i = 0; i < affixes_full.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("fcode" + istr);
        result.push_back("fparam" + istr);
        result.push_back("fmin" + istr);
        result.push_back("fmax" + istr);
    }
    for (size_t i = 0; i < affixes_partial1.getLimit(); ++i) {
        auto istr = std::to_string(i + 2);
        result.push_back("pcode" + istr + "a");
        result.push_back("pparam" + istr + "a");
        result.push_back("pmin" + istr + "a");
        result.push_back("pmax" + istr + "a");
    }
    for (size_t i = 0; i < affixes_partial2.getLimit(); ++i) {
        auto istr = std::to_string(i + 2);
        result.push_back("pcode" + istr + "b");
        result.push_back("pparam" + istr + "b");
        result.push_back("pmin" + istr + "b");
        result.push_back("pmax" + istr + "b");
    }

    return result;
}

void DataRow_sets::ser()
{
    serf(lvl);
    for (auto* affixList : affixLists())
        serf(*affixList);
}

std::vector<std::string> DataRow_skilldesc::getColumns() const
{
    return {
        "skilldesc",
        "skillpage",
        "skillrow",
        "skillcolumn",
        "listrow",
    };
}

void DataRow_skilldesc::ser()
{
    serf(skilldesc);
    serf(skillPage);
    serf(skillRow);
    serf(skillColumn);
    serf(listRow);
}

std::vector<std::string> DataRow_skills::getColumns() const
{
    // clang-format off
    return { "skill", "maxlvl", "reqlevel", "intown", "cost mult", "cost add" ,
             "elen", "hitshift", "etype", "charclass",
             "reqskill1", "reqskill2", "reqskill3"};
    // clang-format on
}

void DataRow_skills::ser()
{
    serf(skill);
    serf(maxlvl);
    serf(lvlreq);
    serf(inTown);
    serf(costMult);
    serf(costAdd);

    serf(eLen);
    serf(hitShift);
    serf(eType);
    serf(charclass);

    serf(reqskills, 3);
}

std::vector<std::string> DataRow_treasureclassex::getColumns() const
{
    std::vector<std::string> result{
        "treasure class",
        "unique",
        "set",
        "rare",
        "magic",
        "nodrop",
        "group",
        "picks",
        "level",
    };
    for (size_t i = 0; i < maxItems; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prob" + istr);
        result.push_back("item" + istr);
    }

    return result;
}

void DataRow_treasureclassex::ser()
{
    serf(name);
    serf(k_unique);
    serf(k_set);
    serf(k_rare);
    serf(k_magic);
    serf(noDrop);
    serf(group);
    serf(picks);
    serf(level);

    serArray(items, maxItems, [this](Item& item) {
        serf(item.prob);
        serf(item.tc); }, [](Item& item) { return !item.tc.empty(); });
}

int DataRow_treasureclassex::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : items)
        totalOther += item.prob.value_or(0);
    return totalOther;
}

std::vector<std::string> DataRow_uniqueitems::getColumns() const
{
    std::vector<std::string> result{ "rarity", "lvl", "lvl req", "carry1" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prop" + istr);
        result.push_back("par" + istr);
        result.push_back("min" + istr);
        result.push_back("max" + istr);
    }
    return result;
}

void DataRow_uniqueitems::ser()
{
    serf(rarity);
    serf(lvl);
    serf(lvlreq);
    serf(carry1);
    serf(affixes);
}

void RowBase::serf(int& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data = 0;
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        str = std::to_string(data);
    }
}

void RowBase::serf(std::optional<int>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string(data.value());
        else
            str = {};
    }
}

void RowBase::serf(std::optional<bool>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string((int) data.value());
        else
            str = {};
    }
}

void RowBase::serf(std::string& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        data = str;
    } else {
        str = data;
    }
}

}
