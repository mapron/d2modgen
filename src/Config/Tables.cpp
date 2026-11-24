// Do not edit. Generated from Tables.yaml using tables_update.py.

#include "Tables.hpp"

namespace D2ModGen {
// clang-format off

enum DataRow_itemtypes_codegen::Column : std::size_t {
    c_clas,
    c_code,
    c_itemType,
    c_equiv1,
    c_equiv2,
    c_bodyLoc1,
    c_throwable,
    c_repair,
    c_shoots,
    c_sockets0,
    c_sockets1,
    c_sockets2,
};

const GeneratedTableSchema& DataRow_itemtypes_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "class",
            "code",
            "itemtype",
            "equiv1",
            "equiv2",
            "bodyloc1",
            "throwable",
            "repair",
            "shoots",
            "maxsockets1",
            "maxsockets2",
            "maxsockets3",
        },
        .aliases = {
            { "maxsock1", "maxsockets1" },
            { "maxsock25", "maxsockets2" },
            { "maxsock40", "maxsockets3" },
        },
    };
    return schema;
}

void DataRow_itemtypes_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_clas, clas);
    reader.read(Column::c_code, code);
    reader.read(Column::c_itemType, itemType);
    reader.read(Column::c_equiv1, equiv1);
    reader.read(Column::c_equiv2, equiv2);
    reader.read(Column::c_bodyLoc1, bodyLoc1);
    reader.read(Column::c_throwable, throwable);
    reader.read(Column::c_repair, repair);
    reader.read(Column::c_shoots, shoots);
    reader.read(Column::c_sockets0, sockets[0]);
    reader.read(Column::c_sockets1, sockets[1]);
    reader.read(Column::c_sockets2, sockets[2]);
}

void DataRow_itemtypes_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_clas, clas);
    writer.write(Column::c_code, code);
    writer.write(Column::c_itemType, itemType);
    writer.write(Column::c_equiv1, equiv1);
    writer.write(Column::c_equiv2, equiv2);
    writer.write(Column::c_bodyLoc1, bodyLoc1);
    writer.write(Column::c_throwable, throwable);
    writer.write(Column::c_repair, repair);
    writer.write(Column::c_shoots, shoots);
    writer.write(Column::c_sockets0, sockets[0]);
    writer.write(Column::c_sockets1, sockets[1]);
    writer.write(Column::c_sockets2, sockets[2]);
}

enum DataRow_charstats_codegen::Column : std::size_t {
    c_clas,
    c_statPerLevel,
    c_skillsPerLevel,
    c_hpPerLevel,
    c_manaPerLevel,
    c_hpPotEfficacy,
    c_manaPotEfficacy,
    c_startSkill,
    c_items0_count,
    c_items0_code,
    c_items0_loc,
    c_items1_count,
    c_items1_code,
    c_items1_loc,
    c_items2_count,
    c_items2_code,
    c_items2_loc,
    c_items3_count,
    c_items3_code,
    c_items3_loc,
    c_items4_count,
    c_items4_code,
    c_items4_loc,
    c_items5_count,
    c_items5_code,
    c_items5_loc,
    c_items6_count,
    c_items6_code,
    c_items6_loc,
    c_items7_count,
    c_items7_code,
    c_items7_loc,
    c_items8_count,
    c_items8_code,
    c_items8_loc,
    c_items9_count,
    c_items9_code,
    c_items9_loc,
    c_skills0,
    c_skills1,
    c_skills2,
    c_skills3,
    c_skills4,
    c_skills5,
    c_skills6,
    c_skills7,
    c_skills8,
    c_skills9,
};

const GeneratedTableSchema& DataRow_charstats_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "class",
            "statperlevel",
            "skillsperlevel",
            "lifeperlevel",
            "manaperlevel",
            "healthpotionpercent",
            "manapotionpercent",
            "startskill",
            "item1count",
            "item1",
            "item1loc",
            "item2count",
            "item2",
            "item2loc",
            "item3count",
            "item3",
            "item3loc",
            "item4count",
            "item4",
            "item4loc",
            "item5count",
            "item5",
            "item5loc",
            "item6count",
            "item6",
            "item6loc",
            "item7count",
            "item7",
            "item7loc",
            "item8count",
            "item8",
            "item8loc",
            "item9count",
            "item9",
            "item9loc",
            "item10count",
            "item10",
            "item10loc",
            "skill 1",
            "skill 2",
            "skill 3",
            "skill 4",
            "skill 5",
            "skill 6",
            "skill 7",
            "skill 8",
            "skill 9",
            "skill 10",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_charstats_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_clas, clas);
    reader.read(Column::c_statPerLevel, statPerLevel);
    reader.read(Column::c_skillsPerLevel, skillsPerLevel);
    reader.read(Column::c_hpPerLevel, hpPerLevel);
    reader.read(Column::c_manaPerLevel, manaPerLevel);
    reader.read(Column::c_hpPotEfficacy, hpPotEfficacy);
    reader.read(Column::c_manaPotEfficacy, manaPotEfficacy);
    reader.read(Column::c_startSkill, startSkill);
    items.clear();
    items.reserve(10);
    {
        Item item;
        reader.read(Column::c_items0_count, item.count);
        reader.read(Column::c_items0_code, item.code);
        reader.read(Column::c_items0_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items1_count, item.count);
        reader.read(Column::c_items1_code, item.code);
        reader.read(Column::c_items1_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items2_count, item.count);
        reader.read(Column::c_items2_code, item.code);
        reader.read(Column::c_items2_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items3_count, item.count);
        reader.read(Column::c_items3_code, item.code);
        reader.read(Column::c_items3_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items4_count, item.count);
        reader.read(Column::c_items4_code, item.code);
        reader.read(Column::c_items4_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items5_count, item.count);
        reader.read(Column::c_items5_code, item.code);
        reader.read(Column::c_items5_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items6_count, item.count);
        reader.read(Column::c_items6_code, item.code);
        reader.read(Column::c_items6_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items7_count, item.count);
        reader.read(Column::c_items7_code, item.code);
        reader.read(Column::c_items7_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items8_count, item.count);
        reader.read(Column::c_items8_code, item.code);
        reader.read(Column::c_items8_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items9_count, item.count);
        reader.read(Column::c_items9_code, item.code);
        reader.read(Column::c_items9_loc, item.loc);
        if (!item.code.empty() && item.code != "0")
            items.push_back(item);
    }
    skills.clear();
    skills.reserve(10);
    {
        std::string value;
        reader.read(Column::c_skills0, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills1, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills2, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills3, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills4, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills5, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills6, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills7, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills8, value);
        if (!value.empty())
            skills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_skills9, value);
        if (!value.empty())
            skills.push_back(value);
    }
}

void DataRow_charstats_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_clas, clas);
    writer.write(Column::c_statPerLevel, statPerLevel);
    writer.write(Column::c_skillsPerLevel, skillsPerLevel);
    writer.write(Column::c_hpPerLevel, hpPerLevel);
    writer.write(Column::c_manaPerLevel, manaPerLevel);
    writer.write(Column::c_hpPotEfficacy, hpPotEfficacy);
    writer.write(Column::c_manaPotEfficacy, manaPotEfficacy);
    writer.write(Column::c_startSkill, startSkill);
    if (items.size() > 0)
    {
        writer.write(Column::c_items0_count, items[0].count);
        writer.write(Column::c_items0_code, items[0].code);
        writer.write(Column::c_items0_loc, items[0].loc);
    }
    if (items.size() > 1)
    {
        writer.write(Column::c_items1_count, items[1].count);
        writer.write(Column::c_items1_code, items[1].code);
        writer.write(Column::c_items1_loc, items[1].loc);
    }
    if (items.size() > 2)
    {
        writer.write(Column::c_items2_count, items[2].count);
        writer.write(Column::c_items2_code, items[2].code);
        writer.write(Column::c_items2_loc, items[2].loc);
    }
    if (items.size() > 3)
    {
        writer.write(Column::c_items3_count, items[3].count);
        writer.write(Column::c_items3_code, items[3].code);
        writer.write(Column::c_items3_loc, items[3].loc);
    }
    if (items.size() > 4)
    {
        writer.write(Column::c_items4_count, items[4].count);
        writer.write(Column::c_items4_code, items[4].code);
        writer.write(Column::c_items4_loc, items[4].loc);
    }
    if (items.size() > 5)
    {
        writer.write(Column::c_items5_count, items[5].count);
        writer.write(Column::c_items5_code, items[5].code);
        writer.write(Column::c_items5_loc, items[5].loc);
    }
    if (items.size() > 6)
    {
        writer.write(Column::c_items6_count, items[6].count);
        writer.write(Column::c_items6_code, items[6].code);
        writer.write(Column::c_items6_loc, items[6].loc);
    }
    if (items.size() > 7)
    {
        writer.write(Column::c_items7_count, items[7].count);
        writer.write(Column::c_items7_code, items[7].code);
        writer.write(Column::c_items7_loc, items[7].loc);
    }
    if (items.size() > 8)
    {
        writer.write(Column::c_items8_count, items[8].count);
        writer.write(Column::c_items8_code, items[8].code);
        writer.write(Column::c_items8_loc, items[8].loc);
    }
    if (items.size() > 9)
    {
        writer.write(Column::c_items9_count, items[9].count);
        writer.write(Column::c_items9_code, items[9].code);
        writer.write(Column::c_items9_loc, items[9].loc);
    }
    if (skills.size() > 0)
    {
        writer.write(Column::c_skills0, skills[0]);
    }
    if (skills.size() > 1)
    {
        writer.write(Column::c_skills1, skills[1]);
    }
    if (skills.size() > 2)
    {
        writer.write(Column::c_skills2, skills[2]);
    }
    if (skills.size() > 3)
    {
        writer.write(Column::c_skills3, skills[3]);
    }
    if (skills.size() > 4)
    {
        writer.write(Column::c_skills4, skills[4]);
    }
    if (skills.size() > 5)
    {
        writer.write(Column::c_skills5, skills[5]);
    }
    if (skills.size() > 6)
    {
        writer.write(Column::c_skills6, skills[6]);
    }
    if (skills.size() > 7)
    {
        writer.write(Column::c_skills7, skills[7]);
    }
    if (skills.size() > 8)
    {
        writer.write(Column::c_skills8, skills[8]);
    }
    if (skills.size() > 9)
    {
        writer.write(Column::c_skills9, skills[9]);
    }
}

enum DataRow_difficultylevels_codegen::Column : std::size_t {
    c_name,
    c_resistPenalty,
    c_gambleRare,
    c_gambleSet,
    c_gambleUnique,
    c_gambleUber,
    c_gambleUltra,
};

const GeneratedTableSchema& DataRow_difficultylevels_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "resistpenalty",
            "gamblerare",
            "gambleset",
            "gambleunique",
            "gambleuber",
            "gambleultra",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_difficultylevels_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_resistPenalty, resistPenalty);
    reader.read(Column::c_gambleRare, gambleRare);
    reader.read(Column::c_gambleSet, gambleSet);
    reader.read(Column::c_gambleUnique, gambleUnique);
    reader.read(Column::c_gambleUber, gambleUber);
    reader.read(Column::c_gambleUltra, gambleUltra);
}

void DataRow_difficultylevels_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_resistPenalty, resistPenalty);
    writer.write(Column::c_gambleRare, gambleRare);
    writer.write(Column::c_gambleSet, gambleSet);
    writer.write(Column::c_gambleUnique, gambleUnique);
    writer.write(Column::c_gambleUber, gambleUber);
    writer.write(Column::c_gambleUltra, gambleUltra);
}

enum DataRow_experience_codegen::Column : std::size_t {
    c_expRatio,
};

const GeneratedTableSchema& DataRow_experience_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "expratio",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_experience_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_expRatio, expRatio);
}

void DataRow_experience_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_expRatio, expRatio);
}

enum DataRow_gamble_codegen::Column : std::size_t {
    c_name,
    c_code,
};

const GeneratedTableSchema& DataRow_gamble_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "code",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_gamble_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_code, code);
}

void DataRow_gamble_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_code, code);
}

enum DataRow_hireling_codegen::Column : std::size_t {
    c_hp,
    c_hp_lvl,
    c_dmg_min,
    c_dmg_max,
    c_dmg_lvl,
};

const GeneratedTableSchema& DataRow_hireling_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "hp",
            "hp/lvl",
            "dmg-min",
            "dmg-max",
            "dmg/lvl",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_hireling_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_hp, hp);
    reader.read(Column::c_hp_lvl, hp_lvl);
    reader.read(Column::c_dmg_min, dmg_min);
    reader.read(Column::c_dmg_max, dmg_max);
    reader.read(Column::c_dmg_lvl, dmg_lvl);
}

void DataRow_hireling_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_hp, hp);
    writer.write(Column::c_hp_lvl, hp_lvl);
    writer.write(Column::c_dmg_min, dmg_min);
    writer.write(Column::c_dmg_max, dmg_max);
    writer.write(Column::c_dmg_lvl, dmg_lvl);
}

enum DataRow_itemstatcost_codegen::Column : std::size_t {
    c_stat,
    c_multiply,
    c_add,
    c_sendParamBits,
    c_saveParamBits,
};

const GeneratedTableSchema& DataRow_itemstatcost_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "stat",
            "multiply",
            "add",
            "send param bits",
            "save param bits",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_itemstatcost_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_stat, stat);
    reader.read(Column::c_multiply, multiply);
    reader.read(Column::c_add, add);
    reader.read(Column::c_sendParamBits, sendParamBits);
    reader.read(Column::c_saveParamBits, saveParamBits);
}

void DataRow_itemstatcost_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_stat, stat);
    writer.write(Column::c_multiply, multiply);
    writer.write(Column::c_add, add);
    writer.write(Column::c_sendParamBits, sendParamBits);
    writer.write(Column::c_saveParamBits, saveParamBits);
}

enum DataRow_levels_codegen::Column : std::size_t {
    c_id,
    c_name,
    c_numMon,
    c_monUMin0,
    c_monUMin1,
    c_monUMin2,
    c_monUMax0,
    c_monUMax1,
    c_monUMax2,
    c_monDen0,
    c_monDen1,
    c_monDen2,
    c_monLvlEx0,
    c_monLvlEx1,
    c_monLvlEx2,
    c_mon0,
    c_mon1,
    c_mon2,
    c_mon3,
    c_mon4,
    c_mon5,
    c_mon6,
    c_mon7,
    c_mon8,
    c_mon9,
    c_mon10,
    c_mon11,
    c_mon12,
    c_mon13,
    c_mon14,
    c_mon15,
    c_mon16,
    c_mon17,
    c_mon18,
    c_mon19,
    c_mon20,
    c_mon21,
    c_mon22,
    c_mon23,
    c_mon24,
    c_nmon0,
    c_nmon1,
    c_nmon2,
    c_nmon3,
    c_nmon4,
    c_nmon5,
    c_nmon6,
    c_nmon7,
    c_nmon8,
    c_nmon9,
    c_nmon10,
    c_nmon11,
    c_nmon12,
    c_nmon13,
    c_nmon14,
    c_nmon15,
    c_nmon16,
    c_nmon17,
    c_nmon18,
    c_nmon19,
    c_nmon20,
    c_nmon21,
    c_nmon22,
    c_nmon23,
    c_nmon24,
    c_umon0,
    c_umon1,
    c_umon2,
    c_umon3,
    c_umon4,
    c_umon5,
    c_umon6,
    c_umon7,
    c_umon8,
    c_umon9,
    c_umon10,
    c_umon11,
    c_umon12,
    c_umon13,
    c_umon14,
    c_umon15,
    c_umon16,
    c_umon17,
    c_umon18,
    c_umon19,
    c_umon20,
    c_umon21,
    c_umon22,
    c_umon23,
    c_umon24,
};

const GeneratedTableSchema& DataRow_levels_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "id",
            "name",
            "nummon",
            "monumin",
            "monumin(n)",
            "monumin(h)",
            "monumax",
            "monumax(n)",
            "monumax(h)",
            "monden",
            "monden(n)",
            "monden(h)",
            "monlvlex",
            "monlvlex(n)",
            "monlvlex(h)",
            "mon1",
            "mon2",
            "mon3",
            "mon4",
            "mon5",
            "mon6",
            "mon7",
            "mon8",
            "mon9",
            "mon10",
            "mon11",
            "mon12",
            "mon13",
            "mon14",
            "mon15",
            "mon16",
            "mon17",
            "mon18",
            "mon19",
            "mon20",
            "mon21",
            "mon22",
            "mon23",
            "mon24",
            "mon25",
            "nmon1",
            "nmon2",
            "nmon3",
            "nmon4",
            "nmon5",
            "nmon6",
            "nmon7",
            "nmon8",
            "nmon9",
            "nmon10",
            "nmon11",
            "nmon12",
            "nmon13",
            "nmon14",
            "nmon15",
            "nmon16",
            "nmon17",
            "nmon18",
            "nmon19",
            "nmon20",
            "nmon21",
            "nmon22",
            "nmon23",
            "nmon24",
            "nmon25",
            "umon1",
            "umon2",
            "umon3",
            "umon4",
            "umon5",
            "umon6",
            "umon7",
            "umon8",
            "umon9",
            "umon10",
            "umon11",
            "umon12",
            "umon13",
            "umon14",
            "umon15",
            "umon16",
            "umon17",
            "umon18",
            "umon19",
            "umon20",
            "umon21",
            "umon22",
            "umon23",
            "umon24",
            "umon25",
        },
        .aliases = {
            { "monlvl1ex", "monlvlex" },
            { "monlvl2ex", "monlvlex(n)" },
            { "monlvl3ex", "monlvlex(h)" },
        },
    };
    return schema;
}

void DataRow_levels_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_id, id);
    reader.read(Column::c_name, name);
    reader.read(Column::c_numMon, numMon);
    reader.read(Column::c_monUMin0, monUMin[0]);
    reader.read(Column::c_monUMin1, monUMin[1]);
    reader.read(Column::c_monUMin2, monUMin[2]);
    reader.read(Column::c_monUMax0, monUMax[0]);
    reader.read(Column::c_monUMax1, monUMax[1]);
    reader.read(Column::c_monUMax2, monUMax[2]);
    reader.read(Column::c_monDen0, monDen[0]);
    reader.read(Column::c_monDen1, monDen[1]);
    reader.read(Column::c_monDen2, monDen[2]);
    reader.read(Column::c_monLvlEx0, monLvlEx[0]);
    reader.read(Column::c_monLvlEx1, monLvlEx[1]);
    reader.read(Column::c_monLvlEx2, monLvlEx[2]);
    mon.clear();
    mon.reserve(25);
    {
        std::string value;
        reader.read(Column::c_mon0, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon1, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon2, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon3, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon4, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon5, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon6, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon7, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon8, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon9, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon10, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon11, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon12, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon13, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon14, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon15, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon16, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon17, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon18, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon19, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon20, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon21, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon22, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon23, value);
        if (!value.empty())
            mon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_mon24, value);
        if (!value.empty())
            mon.push_back(value);
    }
    nmon.clear();
    nmon.reserve(25);
    {
        std::string value;
        reader.read(Column::c_nmon0, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon1, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon2, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon3, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon4, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon5, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon6, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon7, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon8, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon9, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon10, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon11, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon12, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon13, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon14, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon15, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon16, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon17, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon18, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon19, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon20, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon21, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon22, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon23, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_nmon24, value);
        if (!value.empty())
            nmon.push_back(value);
    }
    umon.clear();
    umon.reserve(25);
    {
        std::string value;
        reader.read(Column::c_umon0, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon1, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon2, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon3, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon4, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon5, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon6, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon7, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon8, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon9, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon10, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon11, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon12, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon13, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon14, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon15, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon16, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon17, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon18, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon19, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon20, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon21, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon22, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon23, value);
        if (!value.empty())
            umon.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_umon24, value);
        if (!value.empty())
            umon.push_back(value);
    }
}

void DataRow_levels_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_id, id);
    writer.write(Column::c_name, name);
    writer.write(Column::c_numMon, numMon);
    writer.write(Column::c_monUMin0, monUMin[0]);
    writer.write(Column::c_monUMin1, monUMin[1]);
    writer.write(Column::c_monUMin2, monUMin[2]);
    writer.write(Column::c_monUMax0, monUMax[0]);
    writer.write(Column::c_monUMax1, monUMax[1]);
    writer.write(Column::c_monUMax2, monUMax[2]);
    writer.write(Column::c_monDen0, monDen[0]);
    writer.write(Column::c_monDen1, monDen[1]);
    writer.write(Column::c_monDen2, monDen[2]);
    writer.write(Column::c_monLvlEx0, monLvlEx[0]);
    writer.write(Column::c_monLvlEx1, monLvlEx[1]);
    writer.write(Column::c_monLvlEx2, monLvlEx[2]);
    if (mon.size() > 0)
    {
        writer.write(Column::c_mon0, mon[0]);
    }
    if (mon.size() > 1)
    {
        writer.write(Column::c_mon1, mon[1]);
    }
    if (mon.size() > 2)
    {
        writer.write(Column::c_mon2, mon[2]);
    }
    if (mon.size() > 3)
    {
        writer.write(Column::c_mon3, mon[3]);
    }
    if (mon.size() > 4)
    {
        writer.write(Column::c_mon4, mon[4]);
    }
    if (mon.size() > 5)
    {
        writer.write(Column::c_mon5, mon[5]);
    }
    if (mon.size() > 6)
    {
        writer.write(Column::c_mon6, mon[6]);
    }
    if (mon.size() > 7)
    {
        writer.write(Column::c_mon7, mon[7]);
    }
    if (mon.size() > 8)
    {
        writer.write(Column::c_mon8, mon[8]);
    }
    if (mon.size() > 9)
    {
        writer.write(Column::c_mon9, mon[9]);
    }
    if (mon.size() > 10)
    {
        writer.write(Column::c_mon10, mon[10]);
    }
    if (mon.size() > 11)
    {
        writer.write(Column::c_mon11, mon[11]);
    }
    if (mon.size() > 12)
    {
        writer.write(Column::c_mon12, mon[12]);
    }
    if (mon.size() > 13)
    {
        writer.write(Column::c_mon13, mon[13]);
    }
    if (mon.size() > 14)
    {
        writer.write(Column::c_mon14, mon[14]);
    }
    if (mon.size() > 15)
    {
        writer.write(Column::c_mon15, mon[15]);
    }
    if (mon.size() > 16)
    {
        writer.write(Column::c_mon16, mon[16]);
    }
    if (mon.size() > 17)
    {
        writer.write(Column::c_mon17, mon[17]);
    }
    if (mon.size() > 18)
    {
        writer.write(Column::c_mon18, mon[18]);
    }
    if (mon.size() > 19)
    {
        writer.write(Column::c_mon19, mon[19]);
    }
    if (mon.size() > 20)
    {
        writer.write(Column::c_mon20, mon[20]);
    }
    if (mon.size() > 21)
    {
        writer.write(Column::c_mon21, mon[21]);
    }
    if (mon.size() > 22)
    {
        writer.write(Column::c_mon22, mon[22]);
    }
    if (mon.size() > 23)
    {
        writer.write(Column::c_mon23, mon[23]);
    }
    if (mon.size() > 24)
    {
        writer.write(Column::c_mon24, mon[24]);
    }
    if (nmon.size() > 0)
    {
        writer.write(Column::c_nmon0, nmon[0]);
    }
    if (nmon.size() > 1)
    {
        writer.write(Column::c_nmon1, nmon[1]);
    }
    if (nmon.size() > 2)
    {
        writer.write(Column::c_nmon2, nmon[2]);
    }
    if (nmon.size() > 3)
    {
        writer.write(Column::c_nmon3, nmon[3]);
    }
    if (nmon.size() > 4)
    {
        writer.write(Column::c_nmon4, nmon[4]);
    }
    if (nmon.size() > 5)
    {
        writer.write(Column::c_nmon5, nmon[5]);
    }
    if (nmon.size() > 6)
    {
        writer.write(Column::c_nmon6, nmon[6]);
    }
    if (nmon.size() > 7)
    {
        writer.write(Column::c_nmon7, nmon[7]);
    }
    if (nmon.size() > 8)
    {
        writer.write(Column::c_nmon8, nmon[8]);
    }
    if (nmon.size() > 9)
    {
        writer.write(Column::c_nmon9, nmon[9]);
    }
    if (nmon.size() > 10)
    {
        writer.write(Column::c_nmon10, nmon[10]);
    }
    if (nmon.size() > 11)
    {
        writer.write(Column::c_nmon11, nmon[11]);
    }
    if (nmon.size() > 12)
    {
        writer.write(Column::c_nmon12, nmon[12]);
    }
    if (nmon.size() > 13)
    {
        writer.write(Column::c_nmon13, nmon[13]);
    }
    if (nmon.size() > 14)
    {
        writer.write(Column::c_nmon14, nmon[14]);
    }
    if (nmon.size() > 15)
    {
        writer.write(Column::c_nmon15, nmon[15]);
    }
    if (nmon.size() > 16)
    {
        writer.write(Column::c_nmon16, nmon[16]);
    }
    if (nmon.size() > 17)
    {
        writer.write(Column::c_nmon17, nmon[17]);
    }
    if (nmon.size() > 18)
    {
        writer.write(Column::c_nmon18, nmon[18]);
    }
    if (nmon.size() > 19)
    {
        writer.write(Column::c_nmon19, nmon[19]);
    }
    if (nmon.size() > 20)
    {
        writer.write(Column::c_nmon20, nmon[20]);
    }
    if (nmon.size() > 21)
    {
        writer.write(Column::c_nmon21, nmon[21]);
    }
    if (nmon.size() > 22)
    {
        writer.write(Column::c_nmon22, nmon[22]);
    }
    if (nmon.size() > 23)
    {
        writer.write(Column::c_nmon23, nmon[23]);
    }
    if (nmon.size() > 24)
    {
        writer.write(Column::c_nmon24, nmon[24]);
    }
    if (umon.size() > 0)
    {
        writer.write(Column::c_umon0, umon[0]);
    }
    if (umon.size() > 1)
    {
        writer.write(Column::c_umon1, umon[1]);
    }
    if (umon.size() > 2)
    {
        writer.write(Column::c_umon2, umon[2]);
    }
    if (umon.size() > 3)
    {
        writer.write(Column::c_umon3, umon[3]);
    }
    if (umon.size() > 4)
    {
        writer.write(Column::c_umon4, umon[4]);
    }
    if (umon.size() > 5)
    {
        writer.write(Column::c_umon5, umon[5]);
    }
    if (umon.size() > 6)
    {
        writer.write(Column::c_umon6, umon[6]);
    }
    if (umon.size() > 7)
    {
        writer.write(Column::c_umon7, umon[7]);
    }
    if (umon.size() > 8)
    {
        writer.write(Column::c_umon8, umon[8]);
    }
    if (umon.size() > 9)
    {
        writer.write(Column::c_umon9, umon[9]);
    }
    if (umon.size() > 10)
    {
        writer.write(Column::c_umon10, umon[10]);
    }
    if (umon.size() > 11)
    {
        writer.write(Column::c_umon11, umon[11]);
    }
    if (umon.size() > 12)
    {
        writer.write(Column::c_umon12, umon[12]);
    }
    if (umon.size() > 13)
    {
        writer.write(Column::c_umon13, umon[13]);
    }
    if (umon.size() > 14)
    {
        writer.write(Column::c_umon14, umon[14]);
    }
    if (umon.size() > 15)
    {
        writer.write(Column::c_umon15, umon[15]);
    }
    if (umon.size() > 16)
    {
        writer.write(Column::c_umon16, umon[16]);
    }
    if (umon.size() > 17)
    {
        writer.write(Column::c_umon17, umon[17]);
    }
    if (umon.size() > 18)
    {
        writer.write(Column::c_umon18, umon[18]);
    }
    if (umon.size() > 19)
    {
        writer.write(Column::c_umon19, umon[19]);
    }
    if (umon.size() > 20)
    {
        writer.write(Column::c_umon20, umon[20]);
    }
    if (umon.size() > 21)
    {
        writer.write(Column::c_umon21, umon[21]);
    }
    if (umon.size() > 22)
    {
        writer.write(Column::c_umon22, umon[22]);
    }
    if (umon.size() > 23)
    {
        writer.write(Column::c_umon23, umon[23]);
    }
    if (umon.size() > 24)
    {
        writer.write(Column::c_umon24, umon[24]);
    }
}

enum DataRow_monlvl_codegen::Column : std::size_t {
    c_TH0,
    c_TH1,
    c_TH2,
    c_TH3,
    c_TH4,
    c_TH5,
    c_AC0,
    c_AC1,
    c_AC2,
    c_AC3,
    c_AC4,
    c_AC5,
    c_HP0,
    c_HP1,
    c_HP2,
    c_HP3,
    c_HP4,
    c_HP5,
    c_DM0,
    c_DM1,
    c_DM2,
    c_DM3,
    c_DM4,
    c_DM5,
    c_XP0,
    c_XP1,
    c_XP2,
    c_XP3,
    c_XP4,
    c_XP5,
    c_level,
};

const GeneratedTableSchema& DataRow_monlvl_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "th",
            "th(n)",
            "th(h)",
            "l-th",
            "l-th(n)",
            "l-th(h)",
            "ac",
            "ac(n)",
            "ac(h)",
            "l-ac",
            "l-ac(n)",
            "l-ac(h)",
            "hp",
            "hp(n)",
            "hp(h)",
            "l-hp",
            "l-hp(n)",
            "l-hp(h)",
            "dm",
            "dm(n)",
            "dm(h)",
            "l-dm",
            "l-dm(n)",
            "l-dm(h)",
            "xp",
            "xp(n)",
            "xp(h)",
            "l-xp",
            "l-xp(n)",
            "l-xp(h)",
            "level",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_monlvl_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_TH0, TH[0]);
    reader.read(Column::c_TH1, TH[1]);
    reader.read(Column::c_TH2, TH[2]);
    reader.read(Column::c_TH3, TH[3]);
    reader.read(Column::c_TH4, TH[4]);
    reader.read(Column::c_TH5, TH[5]);
    reader.read(Column::c_AC0, AC[0]);
    reader.read(Column::c_AC1, AC[1]);
    reader.read(Column::c_AC2, AC[2]);
    reader.read(Column::c_AC3, AC[3]);
    reader.read(Column::c_AC4, AC[4]);
    reader.read(Column::c_AC5, AC[5]);
    reader.read(Column::c_HP0, HP[0]);
    reader.read(Column::c_HP1, HP[1]);
    reader.read(Column::c_HP2, HP[2]);
    reader.read(Column::c_HP3, HP[3]);
    reader.read(Column::c_HP4, HP[4]);
    reader.read(Column::c_HP5, HP[5]);
    reader.read(Column::c_DM0, DM[0]);
    reader.read(Column::c_DM1, DM[1]);
    reader.read(Column::c_DM2, DM[2]);
    reader.read(Column::c_DM3, DM[3]);
    reader.read(Column::c_DM4, DM[4]);
    reader.read(Column::c_DM5, DM[5]);
    reader.read(Column::c_XP0, XP[0]);
    reader.read(Column::c_XP1, XP[1]);
    reader.read(Column::c_XP2, XP[2]);
    reader.read(Column::c_XP3, XP[3]);
    reader.read(Column::c_XP4, XP[4]);
    reader.read(Column::c_XP5, XP[5]);
    reader.read(Column::c_level, level);
}

void DataRow_monlvl_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_TH0, TH[0]);
    writer.write(Column::c_TH1, TH[1]);
    writer.write(Column::c_TH2, TH[2]);
    writer.write(Column::c_TH3, TH[3]);
    writer.write(Column::c_TH4, TH[4]);
    writer.write(Column::c_TH5, TH[5]);
    writer.write(Column::c_AC0, AC[0]);
    writer.write(Column::c_AC1, AC[1]);
    writer.write(Column::c_AC2, AC[2]);
    writer.write(Column::c_AC3, AC[3]);
    writer.write(Column::c_AC4, AC[4]);
    writer.write(Column::c_AC5, AC[5]);
    writer.write(Column::c_HP0, HP[0]);
    writer.write(Column::c_HP1, HP[1]);
    writer.write(Column::c_HP2, HP[2]);
    writer.write(Column::c_HP3, HP[3]);
    writer.write(Column::c_HP4, HP[4]);
    writer.write(Column::c_HP5, HP[5]);
    writer.write(Column::c_DM0, DM[0]);
    writer.write(Column::c_DM1, DM[1]);
    writer.write(Column::c_DM2, DM[2]);
    writer.write(Column::c_DM3, DM[3]);
    writer.write(Column::c_DM4, DM[4]);
    writer.write(Column::c_DM5, DM[5]);
    writer.write(Column::c_XP0, XP[0]);
    writer.write(Column::c_XP1, XP[1]);
    writer.write(Column::c_XP2, XP[2]);
    writer.write(Column::c_XP3, XP[3]);
    writer.write(Column::c_XP4, XP[4]);
    writer.write(Column::c_XP5, XP[5]);
    writer.write(Column::c_level, level);
}

enum DataRow_monseq_codegen::Column : std::size_t {
    c_mode,
    c_sequence,
    c_frame,
    c_dir,
    c_event,
    c_eol,
};

const GeneratedTableSchema& DataRow_monseq_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "mode",
            "sequence",
            "frame",
            "dir",
            "event",
            "*eol",
        },
        .aliases = {
            { "eol", "*eol" },
        },
    };
    return schema;
}

void DataRow_monseq_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_mode, mode);
    reader.read(Column::c_sequence, sequence);
    reader.read(Column::c_frame, frame);
    reader.read(Column::c_dir, dir);
    reader.read(Column::c_event, event);
    reader.read(Column::c_eol, eol);
}

void DataRow_monseq_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_mode, mode);
    writer.write(Column::c_sequence, sequence);
    writer.write(Column::c_frame, frame);
    writer.write(Column::c_dir, dir);
    writer.write(Column::c_event, event);
    writer.write(Column::c_eol, eol);
}

enum DataRow_monstats_codegen::Column : std::size_t {
    c_resFi0,
    c_resFi1,
    c_resFi2,
    c_resLi0,
    c_resLi1,
    c_resLi2,
    c_resCo0,
    c_resCo1,
    c_resCo2,
    c_resPo0,
    c_resPo1,
    c_resPo2,
    c_resMa0,
    c_resMa1,
    c_resMa2,
    c_resDm0,
    c_resDm1,
    c_resDm2,
    c_tc0,
    c_tc1,
    c_tc2,
    c_tc3,
    c_tc4,
    c_tc5,
    c_tc6,
    c_tc7,
    c_tc8,
    c_tc9,
    c_tc10,
    c_tc11,
    c_skills0_name,
    c_skills0_mode,
    c_skills0_level,
    c_skills1_name,
    c_skills1_mode,
    c_skills1_level,
    c_skills2_name,
    c_skills2_mode,
    c_skills2_level,
    c_skills3_name,
    c_skills3_mode,
    c_skills3_level,
    c_skills4_name,
    c_skills4_mode,
    c_skills4_level,
    c_skills5_name,
    c_skills5_mode,
    c_skills5_level,
    c_skills6_name,
    c_skills6_mode,
    c_skills6_level,
    c_skills7_name,
    c_skills7_mode,
    c_skills7_level,
    c_aipar0,
    c_aipar1,
    c_aipar2,
    c_aipar3,
    c_aipar4,
    c_aipar5,
    c_aipar6,
    c_aipar7,
    c_aipar8,
    c_aipar9,
    c_aipar10,
    c_aipar11,
    c_aipar12,
    c_aipar13,
    c_aipar14,
    c_aipar15,
    c_aipar16,
    c_aipar17,
    c_aipar18,
    c_aipar19,
    c_aipar20,
    c_aipar21,
    c_aipar22,
    c_aipar23,
    c_battleStats0_minHP,
    c_battleStats0_maxHP,
    c_battleStats0_AC,
    c_battleStats0_exp,
    c_battleStats0_A1minD,
    c_battleStats0_A1maxD,
    c_battleStats0_A1TH,
    c_battleStats0_A2minD,
    c_battleStats0_A2maxD,
    c_battleStats0_A2TH,
    c_battleStats0_S1minD,
    c_battleStats0_S1maxD,
    c_battleStats0_S1TH,
    c_battleStats0_el1Pct,
    c_battleStats0_el1MinD,
    c_battleStats0_el1MaxD,
    c_battleStats0_el1Dur,
    c_battleStats1_minHP,
    c_battleStats1_maxHP,
    c_battleStats1_AC,
    c_battleStats1_exp,
    c_battleStats1_A1minD,
    c_battleStats1_A1maxD,
    c_battleStats1_A1TH,
    c_battleStats1_A2minD,
    c_battleStats1_A2maxD,
    c_battleStats1_A2TH,
    c_battleStats1_S1minD,
    c_battleStats1_S1maxD,
    c_battleStats1_S1TH,
    c_battleStats1_el1Pct,
    c_battleStats1_el1MinD,
    c_battleStats1_el1MaxD,
    c_battleStats1_el1Dur,
    c_battleStats2_minHP,
    c_battleStats2_maxHP,
    c_battleStats2_AC,
    c_battleStats2_exp,
    c_battleStats2_A1minD,
    c_battleStats2_A1maxD,
    c_battleStats2_A1TH,
    c_battleStats2_A2minD,
    c_battleStats2_A2maxD,
    c_battleStats2_A2TH,
    c_battleStats2_S1minD,
    c_battleStats2_S1maxD,
    c_battleStats2_S1TH,
    c_battleStats2_el1Pct,
    c_battleStats2_el1MinD,
    c_battleStats2_el1MaxD,
    c_battleStats2_el1Dur,
    c_walkSpeed,
    c_runSpeed,
    c_toBlock,
    c_el1Mode,
    c_el1Type,
    c_minionIds0,
    c_minionIds1,
    c_minionIds2,
    c_enabled,
    c_isspawn,
    c_killable,
    c_npc,
    c_boss,
    c_rangedtype,
    c_melee,
    c_flying,
    c_level,
    c_minGrp,
    c_maxGrp,
    c_id,
    c_baseId,
    c_hcIdx,
    c_monStatsEx,
    c_AI,
};

const GeneratedTableSchema& DataRow_monstats_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "resfi",
            "resfi(n)",
            "resfi(h)",
            "resli",
            "resli(n)",
            "resli(h)",
            "resco",
            "resco(n)",
            "resco(h)",
            "respo",
            "respo(n)",
            "respo(h)",
            "resma",
            "resma(n)",
            "resma(h)",
            "resdm",
            "resdm(n)",
            "resdm(h)",
            "treasureclass",
            "treasureclass(n)",
            "treasureclass(h)",
            "treasureclasschamp",
            "treasureclasschamp(n)",
            "treasureclasschamp(h)",
            "treasureclassunique",
            "treasureclassunique(n)",
            "treasureclassunique(h)",
            "treasureclassquest",
            "treasureclassquest(n)",
            "treasureclassquest(h)",
            "skill1",
            "sk1mode",
            "sk1lvl",
            "skill2",
            "sk2mode",
            "sk2lvl",
            "skill3",
            "sk3mode",
            "sk3lvl",
            "skill4",
            "sk4mode",
            "sk4lvl",
            "skill5",
            "sk5mode",
            "sk5lvl",
            "skill6",
            "sk6mode",
            "sk6lvl",
            "skill7",
            "sk7mode",
            "sk7lvl",
            "skill8",
            "sk8mode",
            "sk8lvl",
            "aip1",
            "aip1(n)",
            "aip1(h)",
            "aip2",
            "aip2(n)",
            "aip2(h)",
            "aip3",
            "aip3(n)",
            "aip3(h)",
            "aip4",
            "aip4(n)",
            "aip4(h)",
            "aip5",
            "aip5(n)",
            "aip5(h)",
            "aip6",
            "aip6(n)",
            "aip6(h)",
            "aip7",
            "aip7(n)",
            "aip7(h)",
            "aip8",
            "aip8(n)",
            "aip8(h)",
            "minhp",
            "maxhp",
            "ac",
            "exp",
            "a1mind",
            "a1maxd",
            "a1th",
            "a2mind",
            "a2maxd",
            "a2th",
            "s1mind",
            "s1maxd",
            "s1th",
            "el1pct",
            "el1mind",
            "el1maxd",
            "el1dur",
            "minhp(n)",
            "maxhp(n)",
            "ac(n)",
            "exp(n)",
            "a1mind(n)",
            "a1maxd(n)",
            "a1th(n)",
            "a2mind(n)",
            "a2maxd(n)",
            "a2th(n)",
            "s1mind(n)",
            "s1maxd(n)",
            "s1th(n)",
            "el1pct(n)",
            "el1mind(n)",
            "el1maxd(n)",
            "el1dur(n)",
            "minhp(h)",
            "maxhp(h)",
            "ac(h)",
            "exp(h)",
            "a1mind(h)",
            "a1maxd(h)",
            "a1th(h)",
            "a2mind(h)",
            "a2maxd(h)",
            "a2th(h)",
            "s1mind(h)",
            "s1maxd(h)",
            "s1th(h)",
            "el1pct(h)",
            "el1mind(h)",
            "el1maxd(h)",
            "el1dur(h)",
            "velocity",
            "run",
            "toblock",
            "el1mode",
            "el1type",
            "spawn",
            "minion1",
            "minion2",
            "enabled",
            "isspawn",
            "killable",
            "npc",
            "boss",
            "rangedtype",
            "ismelee",
            "flying",
            "level",
            "mingrp",
            "maxgrp",
            "id",
            "baseid",
            "*hcidx",
            "monstatsex",
            "ai",
        },
        .aliases = {
            { "treasureclass1", "treasureclass" },
            { "treasureclass1(n)", "treasureclass(n)" },
            { "treasureclass1(h)", "treasureclass(h)" },
            { "treasureclass2", "treasureclasschamp" },
            { "treasureclass2(n)", "treasureclasschamp(n)" },
            { "treasureclass2(h)", "treasureclasschamp(h)" },
            { "treasureclass3", "treasureclassunique" },
            { "treasureclass3(n)", "treasureclassunique(n)" },
            { "treasureclass3(h)", "treasureclassunique(h)" },
            { "treasureclass4", "treasureclassquest" },
            { "treasureclass4(n)", "treasureclassquest(n)" },
            { "treasureclass4(h)", "treasureclassquest(h)" },
            { "hcidx", "*hcidx" },
        },
    };
    return schema;
}

void DataRow_monstats_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_resFi0, resFi[0]);
    reader.read(Column::c_resFi1, resFi[1]);
    reader.read(Column::c_resFi2, resFi[2]);
    reader.read(Column::c_resLi0, resLi[0]);
    reader.read(Column::c_resLi1, resLi[1]);
    reader.read(Column::c_resLi2, resLi[2]);
    reader.read(Column::c_resCo0, resCo[0]);
    reader.read(Column::c_resCo1, resCo[1]);
    reader.read(Column::c_resCo2, resCo[2]);
    reader.read(Column::c_resPo0, resPo[0]);
    reader.read(Column::c_resPo1, resPo[1]);
    reader.read(Column::c_resPo2, resPo[2]);
    reader.read(Column::c_resMa0, resMa[0]);
    reader.read(Column::c_resMa1, resMa[1]);
    reader.read(Column::c_resMa2, resMa[2]);
    reader.read(Column::c_resDm0, resDm[0]);
    reader.read(Column::c_resDm1, resDm[1]);
    reader.read(Column::c_resDm2, resDm[2]);
    reader.read(Column::c_tc0, tc[0][0]);
    reader.read(Column::c_tc1, tc[0][1]);
    reader.read(Column::c_tc2, tc[0][2]);
    reader.read(Column::c_tc3, tc[1][0]);
    reader.read(Column::c_tc4, tc[1][1]);
    reader.read(Column::c_tc5, tc[1][2]);
    reader.read(Column::c_tc6, tc[2][0]);
    reader.read(Column::c_tc7, tc[2][1]);
    reader.read(Column::c_tc8, tc[2][2]);
    reader.read(Column::c_tc9, tc[3][0]);
    reader.read(Column::c_tc10, tc[3][1]);
    reader.read(Column::c_tc11, tc[3][2]);
    skills.clear();
    skills.reserve(8);
    {
        Skill item;
        reader.read(Column::c_skills0_name, item.name);
        reader.read(Column::c_skills0_mode, item.mode);
        reader.read(Column::c_skills0_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills1_name, item.name);
        reader.read(Column::c_skills1_mode, item.mode);
        reader.read(Column::c_skills1_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills2_name, item.name);
        reader.read(Column::c_skills2_mode, item.mode);
        reader.read(Column::c_skills2_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills3_name, item.name);
        reader.read(Column::c_skills3_mode, item.mode);
        reader.read(Column::c_skills3_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills4_name, item.name);
        reader.read(Column::c_skills4_mode, item.mode);
        reader.read(Column::c_skills4_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills5_name, item.name);
        reader.read(Column::c_skills5_mode, item.mode);
        reader.read(Column::c_skills5_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills6_name, item.name);
        reader.read(Column::c_skills6_mode, item.mode);
        reader.read(Column::c_skills6_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    {
        Skill item;
        reader.read(Column::c_skills7_name, item.name);
        reader.read(Column::c_skills7_mode, item.mode);
        reader.read(Column::c_skills7_level, item.level);
        if (!item.name.empty())
            skills.push_back(item);
    }
    reader.read(Column::c_aipar0, aipar[0]);
    reader.read(Column::c_aipar1, aipar[1]);
    reader.read(Column::c_aipar2, aipar[2]);
    reader.read(Column::c_aipar3, aipar[3]);
    reader.read(Column::c_aipar4, aipar[4]);
    reader.read(Column::c_aipar5, aipar[5]);
    reader.read(Column::c_aipar6, aipar[6]);
    reader.read(Column::c_aipar7, aipar[7]);
    reader.read(Column::c_aipar8, aipar[8]);
    reader.read(Column::c_aipar9, aipar[9]);
    reader.read(Column::c_aipar10, aipar[10]);
    reader.read(Column::c_aipar11, aipar[11]);
    reader.read(Column::c_aipar12, aipar[12]);
    reader.read(Column::c_aipar13, aipar[13]);
    reader.read(Column::c_aipar14, aipar[14]);
    reader.read(Column::c_aipar15, aipar[15]);
    reader.read(Column::c_aipar16, aipar[16]);
    reader.read(Column::c_aipar17, aipar[17]);
    reader.read(Column::c_aipar18, aipar[18]);
    reader.read(Column::c_aipar19, aipar[19]);
    reader.read(Column::c_aipar20, aipar[20]);
    reader.read(Column::c_aipar21, aipar[21]);
    reader.read(Column::c_aipar22, aipar[22]);
    reader.read(Column::c_aipar23, aipar[23]);
    reader.read(Column::c_battleStats0_minHP, battleStats[0].minHP);
    reader.read(Column::c_battleStats0_maxHP, battleStats[0].maxHP);
    reader.read(Column::c_battleStats0_AC, battleStats[0].AC);
    reader.read(Column::c_battleStats0_exp, battleStats[0].exp);
    reader.read(Column::c_battleStats0_A1minD, battleStats[0].A1minD);
    reader.read(Column::c_battleStats0_A1maxD, battleStats[0].A1maxD);
    reader.read(Column::c_battleStats0_A1TH, battleStats[0].A1TH);
    reader.read(Column::c_battleStats0_A2minD, battleStats[0].A2minD);
    reader.read(Column::c_battleStats0_A2maxD, battleStats[0].A2maxD);
    reader.read(Column::c_battleStats0_A2TH, battleStats[0].A2TH);
    reader.read(Column::c_battleStats0_S1minD, battleStats[0].S1minD);
    reader.read(Column::c_battleStats0_S1maxD, battleStats[0].S1maxD);
    reader.read(Column::c_battleStats0_S1TH, battleStats[0].S1TH);
    reader.read(Column::c_battleStats0_el1Pct, battleStats[0].el1Pct);
    reader.read(Column::c_battleStats0_el1MinD, battleStats[0].el1MinD);
    reader.read(Column::c_battleStats0_el1MaxD, battleStats[0].el1MaxD);
    reader.read(Column::c_battleStats0_el1Dur, battleStats[0].el1Dur);
    reader.read(Column::c_battleStats1_minHP, battleStats[1].minHP);
    reader.read(Column::c_battleStats1_maxHP, battleStats[1].maxHP);
    reader.read(Column::c_battleStats1_AC, battleStats[1].AC);
    reader.read(Column::c_battleStats1_exp, battleStats[1].exp);
    reader.read(Column::c_battleStats1_A1minD, battleStats[1].A1minD);
    reader.read(Column::c_battleStats1_A1maxD, battleStats[1].A1maxD);
    reader.read(Column::c_battleStats1_A1TH, battleStats[1].A1TH);
    reader.read(Column::c_battleStats1_A2minD, battleStats[1].A2minD);
    reader.read(Column::c_battleStats1_A2maxD, battleStats[1].A2maxD);
    reader.read(Column::c_battleStats1_A2TH, battleStats[1].A2TH);
    reader.read(Column::c_battleStats1_S1minD, battleStats[1].S1minD);
    reader.read(Column::c_battleStats1_S1maxD, battleStats[1].S1maxD);
    reader.read(Column::c_battleStats1_S1TH, battleStats[1].S1TH);
    reader.read(Column::c_battleStats1_el1Pct, battleStats[1].el1Pct);
    reader.read(Column::c_battleStats1_el1MinD, battleStats[1].el1MinD);
    reader.read(Column::c_battleStats1_el1MaxD, battleStats[1].el1MaxD);
    reader.read(Column::c_battleStats1_el1Dur, battleStats[1].el1Dur);
    reader.read(Column::c_battleStats2_minHP, battleStats[2].minHP);
    reader.read(Column::c_battleStats2_maxHP, battleStats[2].maxHP);
    reader.read(Column::c_battleStats2_AC, battleStats[2].AC);
    reader.read(Column::c_battleStats2_exp, battleStats[2].exp);
    reader.read(Column::c_battleStats2_A1minD, battleStats[2].A1minD);
    reader.read(Column::c_battleStats2_A1maxD, battleStats[2].A1maxD);
    reader.read(Column::c_battleStats2_A1TH, battleStats[2].A1TH);
    reader.read(Column::c_battleStats2_A2minD, battleStats[2].A2minD);
    reader.read(Column::c_battleStats2_A2maxD, battleStats[2].A2maxD);
    reader.read(Column::c_battleStats2_A2TH, battleStats[2].A2TH);
    reader.read(Column::c_battleStats2_S1minD, battleStats[2].S1minD);
    reader.read(Column::c_battleStats2_S1maxD, battleStats[2].S1maxD);
    reader.read(Column::c_battleStats2_S1TH, battleStats[2].S1TH);
    reader.read(Column::c_battleStats2_el1Pct, battleStats[2].el1Pct);
    reader.read(Column::c_battleStats2_el1MinD, battleStats[2].el1MinD);
    reader.read(Column::c_battleStats2_el1MaxD, battleStats[2].el1MaxD);
    reader.read(Column::c_battleStats2_el1Dur, battleStats[2].el1Dur);
    reader.read(Column::c_walkSpeed, walkSpeed);
    reader.read(Column::c_runSpeed, runSpeed);
    reader.read(Column::c_toBlock, toBlock);
    reader.read(Column::c_el1Mode, el1Mode);
    reader.read(Column::c_el1Type, el1Type);
    reader.read(Column::c_minionIds0, minionIds[0]);
    reader.read(Column::c_minionIds1, minionIds[1]);
    reader.read(Column::c_minionIds2, minionIds[2]);
    reader.read(Column::c_enabled, enabled);
    reader.read(Column::c_isspawn, isspawn);
    reader.read(Column::c_killable, killable);
    reader.read(Column::c_npc, npc);
    reader.read(Column::c_boss, boss);
    reader.read(Column::c_rangedtype, rangedtype);
    reader.read(Column::c_melee, melee);
    reader.read(Column::c_flying, flying);
    reader.read(Column::c_level, level);
    reader.read(Column::c_minGrp, minGrp);
    reader.read(Column::c_maxGrp, maxGrp);
    reader.read(Column::c_id, id);
    reader.read(Column::c_baseId, baseId);
    reader.read(Column::c_hcIdx, hcIdx);
    reader.read(Column::c_monStatsEx, monStatsEx);
    reader.read(Column::c_AI, AI);
}

void DataRow_monstats_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_resFi0, resFi[0]);
    writer.write(Column::c_resFi1, resFi[1]);
    writer.write(Column::c_resFi2, resFi[2]);
    writer.write(Column::c_resLi0, resLi[0]);
    writer.write(Column::c_resLi1, resLi[1]);
    writer.write(Column::c_resLi2, resLi[2]);
    writer.write(Column::c_resCo0, resCo[0]);
    writer.write(Column::c_resCo1, resCo[1]);
    writer.write(Column::c_resCo2, resCo[2]);
    writer.write(Column::c_resPo0, resPo[0]);
    writer.write(Column::c_resPo1, resPo[1]);
    writer.write(Column::c_resPo2, resPo[2]);
    writer.write(Column::c_resMa0, resMa[0]);
    writer.write(Column::c_resMa1, resMa[1]);
    writer.write(Column::c_resMa2, resMa[2]);
    writer.write(Column::c_resDm0, resDm[0]);
    writer.write(Column::c_resDm1, resDm[1]);
    writer.write(Column::c_resDm2, resDm[2]);
    writer.write(Column::c_tc0, tc[0][0]);
    writer.write(Column::c_tc1, tc[0][1]);
    writer.write(Column::c_tc2, tc[0][2]);
    writer.write(Column::c_tc3, tc[1][0]);
    writer.write(Column::c_tc4, tc[1][1]);
    writer.write(Column::c_tc5, tc[1][2]);
    writer.write(Column::c_tc6, tc[2][0]);
    writer.write(Column::c_tc7, tc[2][1]);
    writer.write(Column::c_tc8, tc[2][2]);
    writer.write(Column::c_tc9, tc[3][0]);
    writer.write(Column::c_tc10, tc[3][1]);
    writer.write(Column::c_tc11, tc[3][2]);
    if (skills.size() > 0)
    {
        writer.write(Column::c_skills0_name, skills[0].name);
        writer.write(Column::c_skills0_mode, skills[0].mode);
        writer.write(Column::c_skills0_level, skills[0].level);
    }
    if (skills.size() > 1)
    {
        writer.write(Column::c_skills1_name, skills[1].name);
        writer.write(Column::c_skills1_mode, skills[1].mode);
        writer.write(Column::c_skills1_level, skills[1].level);
    }
    if (skills.size() > 2)
    {
        writer.write(Column::c_skills2_name, skills[2].name);
        writer.write(Column::c_skills2_mode, skills[2].mode);
        writer.write(Column::c_skills2_level, skills[2].level);
    }
    if (skills.size() > 3)
    {
        writer.write(Column::c_skills3_name, skills[3].name);
        writer.write(Column::c_skills3_mode, skills[3].mode);
        writer.write(Column::c_skills3_level, skills[3].level);
    }
    if (skills.size() > 4)
    {
        writer.write(Column::c_skills4_name, skills[4].name);
        writer.write(Column::c_skills4_mode, skills[4].mode);
        writer.write(Column::c_skills4_level, skills[4].level);
    }
    if (skills.size() > 5)
    {
        writer.write(Column::c_skills5_name, skills[5].name);
        writer.write(Column::c_skills5_mode, skills[5].mode);
        writer.write(Column::c_skills5_level, skills[5].level);
    }
    if (skills.size() > 6)
    {
        writer.write(Column::c_skills6_name, skills[6].name);
        writer.write(Column::c_skills6_mode, skills[6].mode);
        writer.write(Column::c_skills6_level, skills[6].level);
    }
    if (skills.size() > 7)
    {
        writer.write(Column::c_skills7_name, skills[7].name);
        writer.write(Column::c_skills7_mode, skills[7].mode);
        writer.write(Column::c_skills7_level, skills[7].level);
    }
    writer.write(Column::c_aipar0, aipar[0]);
    writer.write(Column::c_aipar1, aipar[1]);
    writer.write(Column::c_aipar2, aipar[2]);
    writer.write(Column::c_aipar3, aipar[3]);
    writer.write(Column::c_aipar4, aipar[4]);
    writer.write(Column::c_aipar5, aipar[5]);
    writer.write(Column::c_aipar6, aipar[6]);
    writer.write(Column::c_aipar7, aipar[7]);
    writer.write(Column::c_aipar8, aipar[8]);
    writer.write(Column::c_aipar9, aipar[9]);
    writer.write(Column::c_aipar10, aipar[10]);
    writer.write(Column::c_aipar11, aipar[11]);
    writer.write(Column::c_aipar12, aipar[12]);
    writer.write(Column::c_aipar13, aipar[13]);
    writer.write(Column::c_aipar14, aipar[14]);
    writer.write(Column::c_aipar15, aipar[15]);
    writer.write(Column::c_aipar16, aipar[16]);
    writer.write(Column::c_aipar17, aipar[17]);
    writer.write(Column::c_aipar18, aipar[18]);
    writer.write(Column::c_aipar19, aipar[19]);
    writer.write(Column::c_aipar20, aipar[20]);
    writer.write(Column::c_aipar21, aipar[21]);
    writer.write(Column::c_aipar22, aipar[22]);
    writer.write(Column::c_aipar23, aipar[23]);
    writer.write(Column::c_battleStats0_minHP, battleStats[0].minHP);
    writer.write(Column::c_battleStats0_maxHP, battleStats[0].maxHP);
    writer.write(Column::c_battleStats0_AC, battleStats[0].AC);
    writer.write(Column::c_battleStats0_exp, battleStats[0].exp);
    writer.write(Column::c_battleStats0_A1minD, battleStats[0].A1minD);
    writer.write(Column::c_battleStats0_A1maxD, battleStats[0].A1maxD);
    writer.write(Column::c_battleStats0_A1TH, battleStats[0].A1TH);
    writer.write(Column::c_battleStats0_A2minD, battleStats[0].A2minD);
    writer.write(Column::c_battleStats0_A2maxD, battleStats[0].A2maxD);
    writer.write(Column::c_battleStats0_A2TH, battleStats[0].A2TH);
    writer.write(Column::c_battleStats0_S1minD, battleStats[0].S1minD);
    writer.write(Column::c_battleStats0_S1maxD, battleStats[0].S1maxD);
    writer.write(Column::c_battleStats0_S1TH, battleStats[0].S1TH);
    writer.write(Column::c_battleStats0_el1Pct, battleStats[0].el1Pct);
    writer.write(Column::c_battleStats0_el1MinD, battleStats[0].el1MinD);
    writer.write(Column::c_battleStats0_el1MaxD, battleStats[0].el1MaxD);
    writer.write(Column::c_battleStats0_el1Dur, battleStats[0].el1Dur);
    writer.write(Column::c_battleStats1_minHP, battleStats[1].minHP);
    writer.write(Column::c_battleStats1_maxHP, battleStats[1].maxHP);
    writer.write(Column::c_battleStats1_AC, battleStats[1].AC);
    writer.write(Column::c_battleStats1_exp, battleStats[1].exp);
    writer.write(Column::c_battleStats1_A1minD, battleStats[1].A1minD);
    writer.write(Column::c_battleStats1_A1maxD, battleStats[1].A1maxD);
    writer.write(Column::c_battleStats1_A1TH, battleStats[1].A1TH);
    writer.write(Column::c_battleStats1_A2minD, battleStats[1].A2minD);
    writer.write(Column::c_battleStats1_A2maxD, battleStats[1].A2maxD);
    writer.write(Column::c_battleStats1_A2TH, battleStats[1].A2TH);
    writer.write(Column::c_battleStats1_S1minD, battleStats[1].S1minD);
    writer.write(Column::c_battleStats1_S1maxD, battleStats[1].S1maxD);
    writer.write(Column::c_battleStats1_S1TH, battleStats[1].S1TH);
    writer.write(Column::c_battleStats1_el1Pct, battleStats[1].el1Pct);
    writer.write(Column::c_battleStats1_el1MinD, battleStats[1].el1MinD);
    writer.write(Column::c_battleStats1_el1MaxD, battleStats[1].el1MaxD);
    writer.write(Column::c_battleStats1_el1Dur, battleStats[1].el1Dur);
    writer.write(Column::c_battleStats2_minHP, battleStats[2].minHP);
    writer.write(Column::c_battleStats2_maxHP, battleStats[2].maxHP);
    writer.write(Column::c_battleStats2_AC, battleStats[2].AC);
    writer.write(Column::c_battleStats2_exp, battleStats[2].exp);
    writer.write(Column::c_battleStats2_A1minD, battleStats[2].A1minD);
    writer.write(Column::c_battleStats2_A1maxD, battleStats[2].A1maxD);
    writer.write(Column::c_battleStats2_A1TH, battleStats[2].A1TH);
    writer.write(Column::c_battleStats2_A2minD, battleStats[2].A2minD);
    writer.write(Column::c_battleStats2_A2maxD, battleStats[2].A2maxD);
    writer.write(Column::c_battleStats2_A2TH, battleStats[2].A2TH);
    writer.write(Column::c_battleStats2_S1minD, battleStats[2].S1minD);
    writer.write(Column::c_battleStats2_S1maxD, battleStats[2].S1maxD);
    writer.write(Column::c_battleStats2_S1TH, battleStats[2].S1TH);
    writer.write(Column::c_battleStats2_el1Pct, battleStats[2].el1Pct);
    writer.write(Column::c_battleStats2_el1MinD, battleStats[2].el1MinD);
    writer.write(Column::c_battleStats2_el1MaxD, battleStats[2].el1MaxD);
    writer.write(Column::c_battleStats2_el1Dur, battleStats[2].el1Dur);
    writer.write(Column::c_walkSpeed, walkSpeed);
    writer.write(Column::c_runSpeed, runSpeed);
    writer.write(Column::c_toBlock, toBlock);
    writer.write(Column::c_el1Mode, el1Mode);
    writer.write(Column::c_el1Type, el1Type);
    writer.write(Column::c_minionIds0, minionIds[0]);
    writer.write(Column::c_minionIds1, minionIds[1]);
    writer.write(Column::c_minionIds2, minionIds[2]);
    writer.write(Column::c_enabled, enabled);
    writer.write(Column::c_isspawn, isspawn);
    writer.write(Column::c_killable, killable);
    writer.write(Column::c_npc, npc);
    writer.write(Column::c_boss, boss);
    writer.write(Column::c_rangedtype, rangedtype);
    writer.write(Column::c_melee, melee);
    writer.write(Column::c_flying, flying);
    writer.write(Column::c_level, level);
    writer.write(Column::c_minGrp, minGrp);
    writer.write(Column::c_maxGrp, maxGrp);
    writer.write(Column::c_id, id);
    writer.write(Column::c_baseId, baseId);
    writer.write(Column::c_hcIdx, hcIdx);
    writer.write(Column::c_monStatsEx, monStatsEx);
    writer.write(Column::c_AI, AI);
}

enum DataRow_monstats2_codegen::Column : std::size_t {
    c_id,
    c_hitClass,
    c_hcIdx,
    c_modes0,
    c_modes1,
    c_modes2,
    c_modes3,
    c_modes4,
    c_modes5,
    c_modes6,
    c_modes7,
    c_modes8,
    c_modes9,
    c_modes10,
    c_modes11,
    c_modes12,
    c_modes13,
    c_modes14,
    c_modes15,
};

const GeneratedTableSchema& DataRow_monstats2_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "id",
            "hitclass",
            "*hcidx",
            "mdt",
            "mnu",
            "mwl",
            "mgh",
            "ma1",
            "ma2",
            "mbl",
            "msc",
            "ms1",
            "ms2",
            "ms3",
            "ms4",
            "mdd",
            "mkb",
            "msq",
            "mrn",
        },
        .aliases = {
            { "hcidx", "*hcidx" },
        },
    };
    return schema;
}

void DataRow_monstats2_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_id, id);
    reader.read(Column::c_hitClass, hitClass);
    reader.read(Column::c_hcIdx, hcIdx);
    reader.read(Column::c_modes0, modes[0], true);
    reader.read(Column::c_modes1, modes[1], true);
    reader.read(Column::c_modes2, modes[2], true);
    reader.read(Column::c_modes3, modes[3], true);
    reader.read(Column::c_modes4, modes[4], true);
    reader.read(Column::c_modes5, modes[5], true);
    reader.read(Column::c_modes6, modes[6], true);
    reader.read(Column::c_modes7, modes[7], true);
    reader.read(Column::c_modes8, modes[8], true);
    reader.read(Column::c_modes9, modes[9], true);
    reader.read(Column::c_modes10, modes[10], true);
    reader.read(Column::c_modes11, modes[11], true);
    reader.read(Column::c_modes12, modes[12], true);
    reader.read(Column::c_modes13, modes[13], true);
    reader.read(Column::c_modes14, modes[14], true);
    reader.read(Column::c_modes15, modes[15], true);
}

void DataRow_monstats2_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_id, id);
    writer.write(Column::c_hitClass, hitClass);
    writer.write(Column::c_hcIdx, hcIdx);
    writer.write(Column::c_modes0, modes[0]);
    writer.write(Column::c_modes1, modes[1]);
    writer.write(Column::c_modes2, modes[2]);
    writer.write(Column::c_modes3, modes[3]);
    writer.write(Column::c_modes4, modes[4]);
    writer.write(Column::c_modes5, modes[5]);
    writer.write(Column::c_modes6, modes[6]);
    writer.write(Column::c_modes7, modes[7]);
    writer.write(Column::c_modes8, modes[8]);
    writer.write(Column::c_modes9, modes[9]);
    writer.write(Column::c_modes10, modes[10]);
    writer.write(Column::c_modes11, modes[11]);
    writer.write(Column::c_modes12, modes[12]);
    writer.write(Column::c_modes13, modes[13]);
    writer.write(Column::c_modes14, modes[14]);
    writer.write(Column::c_modes15, modes[15]);
}

enum DataRow_skilldesc_codegen::Column : std::size_t {
    c_skilldesc,
    c_skillPage,
    c_skillRow,
    c_skillColumn,
    c_listRow,
    c_strName,
    c_iconCel,
    c_missile0,
    c_missile1,
    c_missile2,
    c_calc0,
    c_calc1,
    c_calc2,
    c_calc3,
    c_calc4,
    c_calc5,
    c_calc6,
    c_calc7,
    c_calc8,
    c_calc9,
    c_calc10,
    c_calc11,
    c_calc12,
    c_calc13,
    c_calc14,
    c_calc15,
    c_calc16,
    c_calc17,
    c_calc18,
    c_calc19,
    c_calc20,
    c_calc21,
    c_calc22,
    c_calc23,
    c_calc24,
    c_calc25,
    c_calc26,
    c_calc27,
    c_calc28,
    c_calc29,
    c_calc30,
    c_calc31,
    c_calc32,
    c_calc33,
    c_calc34,
    c_calc35,
    c_desc0,
    c_desc1,
    c_desc2,
    c_desc3,
    c_desc4,
    c_desc5,
    c_desc6,
    c_desc7,
    c_desc8,
    c_desc9,
    c_desc10,
    c_desc11,
    c_desc12,
    c_desc13,
    c_desc14,
    c_desc15,
    c_desc16,
    c_desc17,
    c_desc18,
    c_desc19,
    c_desc20,
    c_desc21,
    c_desc22,
    c_desc23,
    c_desc24,
    c_desc25,
    c_desc26,
    c_desc27,
    c_desc28,
    c_desc29,
    c_desc30,
    c_desc31,
    c_desc32,
    c_desc33,
    c_desc34,
    c_desc35,
    c_descLine0,
    c_descLine1,
    c_descLine2,
    c_descLine3,
    c_descLine4,
    c_descLine5,
    c_descLine6,
    c_descLine7,
    c_descLine8,
    c_descLine9,
    c_descLine10,
    c_descLine11,
    c_descLine12,
    c_descLine13,
    c_descLine14,
    c_descLine15,
    c_descLine16,
    c_descLine17,
};

const GeneratedTableSchema& DataRow_skilldesc_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "skilldesc",
            "skillpage",
            "skillrow",
            "skillcolumn",
            "listrow",
            "str name",
            "iconcel",
            "descmissile1",
            "descmissile2",
            "descmissile3",
            "desccalca1",
            "desccalcb1",
            "desccalca2",
            "desccalcb2",
            "desccalca3",
            "desccalcb3",
            "desccalca4",
            "desccalcb4",
            "desccalca5",
            "desccalcb5",
            "desccalca6",
            "desccalcb6",
            "dsc2calca1",
            "dsc2calcb1",
            "dsc2calca2",
            "dsc2calcb2",
            "dsc2calca3",
            "dsc2calcb3",
            "dsc2calca4",
            "dsc2calcb4",
            "dsc2calca5",
            "dsc2calcb5",
            "dsc3calca1",
            "dsc3calcb1",
            "dsc3calca2",
            "dsc3calcb2",
            "dsc3calca3",
            "dsc3calcb3",
            "dsc3calca4",
            "dsc3calcb4",
            "dsc3calca5",
            "dsc3calcb5",
            "dsc3calca6",
            "dsc3calcb6",
            "dsc3calca7",
            "dsc3calcb7",
            "desctexta1",
            "desctextb1",
            "desctexta2",
            "desctextb2",
            "desctexta3",
            "desctextb3",
            "desctexta4",
            "desctextb4",
            "desctexta5",
            "desctextb5",
            "desctexta6",
            "desctextb6",
            "dsc2texta1",
            "dsc2textb1",
            "dsc2texta2",
            "dsc2textb2",
            "dsc2texta3",
            "dsc2textb3",
            "dsc2texta4",
            "dsc2textb4",
            "dsc2texta5",
            "dsc2textb5",
            "dsc3texta1",
            "dsc3textb1",
            "dsc3texta2",
            "dsc3textb2",
            "dsc3texta3",
            "dsc3textb3",
            "dsc3texta4",
            "dsc3textb4",
            "dsc3texta5",
            "dsc3textb5",
            "dsc3texta6",
            "dsc3textb6",
            "dsc3texta7",
            "dsc3textb7",
            "descline1",
            "descline2",
            "descline3",
            "descline4",
            "descline5",
            "descline6",
            "dsc2line1",
            "dsc2line2",
            "dsc2line3",
            "dsc2line4",
            "dsc2line5",
            "dsc3line1",
            "dsc3line2",
            "dsc3line3",
            "dsc3line4",
            "dsc3line5",
            "dsc3line6",
            "dsc3line7",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_skilldesc_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_skilldesc, skilldesc);
    reader.read(Column::c_skillPage, skillPage);
    reader.read(Column::c_skillRow, skillRow);
    reader.read(Column::c_skillColumn, skillColumn);
    reader.read(Column::c_listRow, listRow);
    reader.read(Column::c_strName, strName);
    reader.read(Column::c_iconCel, iconCel);
    reader.read(Column::c_missile0, missile[0]);
    reader.read(Column::c_missile1, missile[1]);
    reader.read(Column::c_missile2, missile[2]);
    reader.read(Column::c_calc0, calc[0]);
    reader.read(Column::c_calc1, calc[1]);
    reader.read(Column::c_calc2, calc[2]);
    reader.read(Column::c_calc3, calc[3]);
    reader.read(Column::c_calc4, calc[4]);
    reader.read(Column::c_calc5, calc[5]);
    reader.read(Column::c_calc6, calc[6]);
    reader.read(Column::c_calc7, calc[7]);
    reader.read(Column::c_calc8, calc[8]);
    reader.read(Column::c_calc9, calc[9]);
    reader.read(Column::c_calc10, calc[10]);
    reader.read(Column::c_calc11, calc[11]);
    reader.read(Column::c_calc12, calc[12]);
    reader.read(Column::c_calc13, calc[13]);
    reader.read(Column::c_calc14, calc[14]);
    reader.read(Column::c_calc15, calc[15]);
    reader.read(Column::c_calc16, calc[16]);
    reader.read(Column::c_calc17, calc[17]);
    reader.read(Column::c_calc18, calc[18]);
    reader.read(Column::c_calc19, calc[19]);
    reader.read(Column::c_calc20, calc[20]);
    reader.read(Column::c_calc21, calc[21]);
    reader.read(Column::c_calc22, calc[22]);
    reader.read(Column::c_calc23, calc[23]);
    reader.read(Column::c_calc24, calc[24]);
    reader.read(Column::c_calc25, calc[25]);
    reader.read(Column::c_calc26, calc[26]);
    reader.read(Column::c_calc27, calc[27]);
    reader.read(Column::c_calc28, calc[28]);
    reader.read(Column::c_calc29, calc[29]);
    reader.read(Column::c_calc30, calc[30]);
    reader.read(Column::c_calc31, calc[31]);
    reader.read(Column::c_calc32, calc[32]);
    reader.read(Column::c_calc33, calc[33]);
    reader.read(Column::c_calc34, calc[34]);
    reader.read(Column::c_calc35, calc[35]);
    reader.read(Column::c_desc0, desc[0]);
    reader.read(Column::c_desc1, desc[1]);
    reader.read(Column::c_desc2, desc[2]);
    reader.read(Column::c_desc3, desc[3]);
    reader.read(Column::c_desc4, desc[4]);
    reader.read(Column::c_desc5, desc[5]);
    reader.read(Column::c_desc6, desc[6]);
    reader.read(Column::c_desc7, desc[7]);
    reader.read(Column::c_desc8, desc[8]);
    reader.read(Column::c_desc9, desc[9]);
    reader.read(Column::c_desc10, desc[10]);
    reader.read(Column::c_desc11, desc[11]);
    reader.read(Column::c_desc12, desc[12]);
    reader.read(Column::c_desc13, desc[13]);
    reader.read(Column::c_desc14, desc[14]);
    reader.read(Column::c_desc15, desc[15]);
    reader.read(Column::c_desc16, desc[16]);
    reader.read(Column::c_desc17, desc[17]);
    reader.read(Column::c_desc18, desc[18]);
    reader.read(Column::c_desc19, desc[19]);
    reader.read(Column::c_desc20, desc[20]);
    reader.read(Column::c_desc21, desc[21]);
    reader.read(Column::c_desc22, desc[22]);
    reader.read(Column::c_desc23, desc[23]);
    reader.read(Column::c_desc24, desc[24]);
    reader.read(Column::c_desc25, desc[25]);
    reader.read(Column::c_desc26, desc[26]);
    reader.read(Column::c_desc27, desc[27]);
    reader.read(Column::c_desc28, desc[28]);
    reader.read(Column::c_desc29, desc[29]);
    reader.read(Column::c_desc30, desc[30]);
    reader.read(Column::c_desc31, desc[31]);
    reader.read(Column::c_desc32, desc[32]);
    reader.read(Column::c_desc33, desc[33]);
    reader.read(Column::c_desc34, desc[34]);
    reader.read(Column::c_desc35, desc[35]);
    reader.read(Column::c_descLine0, descLine[0]);
    reader.read(Column::c_descLine1, descLine[1]);
    reader.read(Column::c_descLine2, descLine[2]);
    reader.read(Column::c_descLine3, descLine[3]);
    reader.read(Column::c_descLine4, descLine[4]);
    reader.read(Column::c_descLine5, descLine[5]);
    reader.read(Column::c_descLine6, descLine[6]);
    reader.read(Column::c_descLine7, descLine[7]);
    reader.read(Column::c_descLine8, descLine[8]);
    reader.read(Column::c_descLine9, descLine[9]);
    reader.read(Column::c_descLine10, descLine[10]);
    reader.read(Column::c_descLine11, descLine[11]);
    reader.read(Column::c_descLine12, descLine[12]);
    reader.read(Column::c_descLine13, descLine[13]);
    reader.read(Column::c_descLine14, descLine[14]);
    reader.read(Column::c_descLine15, descLine[15]);
    reader.read(Column::c_descLine16, descLine[16]);
    reader.read(Column::c_descLine17, descLine[17]);
}

void DataRow_skilldesc_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_skilldesc, skilldesc);
    writer.write(Column::c_skillPage, skillPage);
    writer.write(Column::c_skillRow, skillRow);
    writer.write(Column::c_skillColumn, skillColumn);
    writer.write(Column::c_listRow, listRow);
    writer.write(Column::c_strName, strName);
    writer.write(Column::c_iconCel, iconCel);
    writer.write(Column::c_missile0, missile[0]);
    writer.write(Column::c_missile1, missile[1]);
    writer.write(Column::c_missile2, missile[2]);
    writer.write(Column::c_calc0, calc[0]);
    writer.write(Column::c_calc1, calc[1]);
    writer.write(Column::c_calc2, calc[2]);
    writer.write(Column::c_calc3, calc[3]);
    writer.write(Column::c_calc4, calc[4]);
    writer.write(Column::c_calc5, calc[5]);
    writer.write(Column::c_calc6, calc[6]);
    writer.write(Column::c_calc7, calc[7]);
    writer.write(Column::c_calc8, calc[8]);
    writer.write(Column::c_calc9, calc[9]);
    writer.write(Column::c_calc10, calc[10]);
    writer.write(Column::c_calc11, calc[11]);
    writer.write(Column::c_calc12, calc[12]);
    writer.write(Column::c_calc13, calc[13]);
    writer.write(Column::c_calc14, calc[14]);
    writer.write(Column::c_calc15, calc[15]);
    writer.write(Column::c_calc16, calc[16]);
    writer.write(Column::c_calc17, calc[17]);
    writer.write(Column::c_calc18, calc[18]);
    writer.write(Column::c_calc19, calc[19]);
    writer.write(Column::c_calc20, calc[20]);
    writer.write(Column::c_calc21, calc[21]);
    writer.write(Column::c_calc22, calc[22]);
    writer.write(Column::c_calc23, calc[23]);
    writer.write(Column::c_calc24, calc[24]);
    writer.write(Column::c_calc25, calc[25]);
    writer.write(Column::c_calc26, calc[26]);
    writer.write(Column::c_calc27, calc[27]);
    writer.write(Column::c_calc28, calc[28]);
    writer.write(Column::c_calc29, calc[29]);
    writer.write(Column::c_calc30, calc[30]);
    writer.write(Column::c_calc31, calc[31]);
    writer.write(Column::c_calc32, calc[32]);
    writer.write(Column::c_calc33, calc[33]);
    writer.write(Column::c_calc34, calc[34]);
    writer.write(Column::c_calc35, calc[35]);
    writer.write(Column::c_desc0, desc[0]);
    writer.write(Column::c_desc1, desc[1]);
    writer.write(Column::c_desc2, desc[2]);
    writer.write(Column::c_desc3, desc[3]);
    writer.write(Column::c_desc4, desc[4]);
    writer.write(Column::c_desc5, desc[5]);
    writer.write(Column::c_desc6, desc[6]);
    writer.write(Column::c_desc7, desc[7]);
    writer.write(Column::c_desc8, desc[8]);
    writer.write(Column::c_desc9, desc[9]);
    writer.write(Column::c_desc10, desc[10]);
    writer.write(Column::c_desc11, desc[11]);
    writer.write(Column::c_desc12, desc[12]);
    writer.write(Column::c_desc13, desc[13]);
    writer.write(Column::c_desc14, desc[14]);
    writer.write(Column::c_desc15, desc[15]);
    writer.write(Column::c_desc16, desc[16]);
    writer.write(Column::c_desc17, desc[17]);
    writer.write(Column::c_desc18, desc[18]);
    writer.write(Column::c_desc19, desc[19]);
    writer.write(Column::c_desc20, desc[20]);
    writer.write(Column::c_desc21, desc[21]);
    writer.write(Column::c_desc22, desc[22]);
    writer.write(Column::c_desc23, desc[23]);
    writer.write(Column::c_desc24, desc[24]);
    writer.write(Column::c_desc25, desc[25]);
    writer.write(Column::c_desc26, desc[26]);
    writer.write(Column::c_desc27, desc[27]);
    writer.write(Column::c_desc28, desc[28]);
    writer.write(Column::c_desc29, desc[29]);
    writer.write(Column::c_desc30, desc[30]);
    writer.write(Column::c_desc31, desc[31]);
    writer.write(Column::c_desc32, desc[32]);
    writer.write(Column::c_desc33, desc[33]);
    writer.write(Column::c_desc34, desc[34]);
    writer.write(Column::c_desc35, desc[35]);
    writer.write(Column::c_descLine0, descLine[0]);
    writer.write(Column::c_descLine1, descLine[1]);
    writer.write(Column::c_descLine2, descLine[2]);
    writer.write(Column::c_descLine3, descLine[3]);
    writer.write(Column::c_descLine4, descLine[4]);
    writer.write(Column::c_descLine5, descLine[5]);
    writer.write(Column::c_descLine6, descLine[6]);
    writer.write(Column::c_descLine7, descLine[7]);
    writer.write(Column::c_descLine8, descLine[8]);
    writer.write(Column::c_descLine9, descLine[9]);
    writer.write(Column::c_descLine10, descLine[10]);
    writer.write(Column::c_descLine11, descLine[11]);
    writer.write(Column::c_descLine12, descLine[12]);
    writer.write(Column::c_descLine13, descLine[13]);
    writer.write(Column::c_descLine14, descLine[14]);
    writer.write(Column::c_descLine15, descLine[15]);
    writer.write(Column::c_descLine16, descLine[16]);
    writer.write(Column::c_descLine17, descLine[17]);
}

enum DataRow_skills_codegen::Column : std::size_t {
    c_skill,
    c_id,
    c_skilldesc,
    c_maxlvl,
    c_lvlreq,
    c_inTown,
    c_leftskill,
    c_costMult,
    c_costAdd,
    c_eLen,
    c_hitShift,
    c_manaShift,
    c_eType,
    c_classSpecific,
    c_itypea1,
    c_anim,
    c_seqtrans,
    c_reqskills0,
    c_reqskills1,
    c_reqskills2,
    c_missile0,
    c_missile1,
    c_missile2,
    c_missile3,
    c_missile4,
    c_missile5,
    c_missile6,
    c_missile7,
    c_missile8,
    c_missile9,
    c_calc0,
    c_calc1,
    c_calc2,
    c_calc3,
    c_calc4,
    c_calc5,
    c_calc6,
    c_calc7,
    c_calc8,
    c_calc9,
    c_calc10,
    c_calc11,
    c_calc12,
    c_calc13,
    c_calc14,
    c_calc15,
    c_calc16,
    c_calc17,
    c_calc18,
    c_calc19,
    c_calc20,
    c_calc21,
    c_calc22,
    c_calc23,
    c_calc24,
    c_calc25,
    c_calc26,
    c_calc27,
    c_calc28,
    c_calc29,
    c_calc30,
    c_calc31,
    c_calc32,
    c_calc33,
    c_calc34,
    c_calc35,
    c_calc36,
    c_calc37,
    c_calc38,
    c_calc39,
    c_calc40,
    c_aurastatcalc0,
    c_aurastatcalc1,
    c_aurastatcalc2,
    c_aurastatcalc3,
    c_aurastatcalc4,
    c_aurastatcalc5,
    c_aurastat0,
    c_aurastat1,
    c_aurastat2,
    c_aurastat3,
    c_aurastat4,
    c_aurastat5,
    c_passivestat0,
    c_passivestat1,
    c_passivestat2,
    c_passivestat3,
    c_passivestat4,
    c_passivestat5,
    c_passivestat6,
    c_passivestat7,
    c_passivestat8,
    c_passivestat9,
    c_passivestat10,
    c_passivestat11,
    c_passivestat12,
    c_passivestat13,
};

const GeneratedTableSchema& DataRow_skills_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "skill",
            "*id",
            "skilldesc",
            "maxlvl",
            "reqlevel",
            "intown",
            "leftskill",
            "cost mult",
            "cost add",
            "elen",
            "hitshift",
            "manashift",
            "etype",
            "charclass",
            "itypea1",
            "anim",
            "seqtrans",
            "reqskill1",
            "reqskill2",
            "reqskill3",
            "srvmissile",
            "srvmissilea",
            "srvmissileb",
            "srvmissilec",
            "srvmissiled",
            "cltmissile",
            "cltmissilea",
            "cltmissileb",
            "cltmissilec",
            "cltmissiled",
            "tohitcalc",
            "dmgsympercalc",
            "edmgsympercalc",
            "elensympercalc",
            "auralencalc",
            "aurarangecalc",
            "passivecalc1",
            "passivecalc2",
            "passivecalc3",
            "passivecalc4",
            "passivecalc5",
            "passivecalc6",
            "passivecalc7",
            "passivecalc8",
            "passivecalc9",
            "passivecalc10",
            "passivecalc11",
            "passivecalc12",
            "passivecalc13",
            "passivecalc14",
            "prgcalc1",
            "prgcalc2",
            "prgcalc3",
            "sumsk1calc",
            "sumsk2calc",
            "sumsk3calc",
            "sumsk4calc",
            "sumsk5calc",
            "cltcalc1",
            "cltcalc2",
            "cltcalc3",
            "calc1",
            "calc2",
            "calc3",
            "calc4",
            "calc5",
            "calc6",
            "calc7",
            "calc8",
            "calc9",
            "calc10",
            "aurastatcalc1",
            "aurastatcalc2",
            "aurastatcalc3",
            "aurastatcalc4",
            "aurastatcalc5",
            "aurastatcalc6",
            "aurastat1",
            "aurastat2",
            "aurastat3",
            "aurastat4",
            "aurastat5",
            "aurastat6",
            "passivestat1",
            "passivestat2",
            "passivestat3",
            "passivestat4",
            "passivestat5",
            "passivestat6",
            "passivestat7",
            "passivestat8",
            "passivestat9",
            "passivestat10",
            "passivestat11",
            "passivestat12",
            "passivestat13",
            "passivestat14",
        },
        .aliases = {
            { "id", "*id" },
        },
    };
    return schema;
}

void DataRow_skills_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_skill, skill);
    reader.read(Column::c_id, id);
    reader.read(Column::c_skilldesc, skilldesc);
    reader.read(Column::c_maxlvl, maxlvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_inTown, inTown);
    reader.read(Column::c_leftskill, leftskill);
    reader.read(Column::c_costMult, costMult);
    reader.read(Column::c_costAdd, costAdd);
    reader.read(Column::c_eLen, eLen);
    reader.read(Column::c_hitShift, hitShift);
    reader.read(Column::c_manaShift, manaShift);
    reader.read(Column::c_eType, eType);
    reader.read(Column::c_classSpecific, classSpecific);
    reader.read(Column::c_itypea1, itypea1);
    reader.read(Column::c_anim, anim);
    reader.read(Column::c_seqtrans, seqtrans);
    reqskills.clear();
    reqskills.reserve(3);
    {
        std::string value;
        reader.read(Column::c_reqskills0, value);
        if (!value.empty())
            reqskills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_reqskills1, value);
        if (!value.empty())
            reqskills.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_reqskills2, value);
        if (!value.empty())
            reqskills.push_back(value);
    }
    reader.read(Column::c_missile0, missile[0]);
    reader.read(Column::c_missile1, missile[1]);
    reader.read(Column::c_missile2, missile[2]);
    reader.read(Column::c_missile3, missile[3]);
    reader.read(Column::c_missile4, missile[4]);
    reader.read(Column::c_missile5, missile[5]);
    reader.read(Column::c_missile6, missile[6]);
    reader.read(Column::c_missile7, missile[7]);
    reader.read(Column::c_missile8, missile[8]);
    reader.read(Column::c_missile9, missile[9]);
    reader.read(Column::c_calc0, calc[0]);
    reader.read(Column::c_calc1, calc[1]);
    reader.read(Column::c_calc2, calc[2]);
    reader.read(Column::c_calc3, calc[3]);
    reader.read(Column::c_calc4, calc[4]);
    reader.read(Column::c_calc5, calc[5]);
    reader.read(Column::c_calc6, calc[6]);
    reader.read(Column::c_calc7, calc[7]);
    reader.read(Column::c_calc8, calc[8]);
    reader.read(Column::c_calc9, calc[9]);
    reader.read(Column::c_calc10, calc[10]);
    reader.read(Column::c_calc11, calc[11]);
    reader.read(Column::c_calc12, calc[12]);
    reader.read(Column::c_calc13, calc[13]);
    reader.read(Column::c_calc14, calc[14]);
    reader.read(Column::c_calc15, calc[15]);
    reader.read(Column::c_calc16, calc[16]);
    reader.read(Column::c_calc17, calc[17]);
    reader.read(Column::c_calc18, calc[18]);
    reader.read(Column::c_calc19, calc[19]);
    reader.read(Column::c_calc20, calc[20]);
    reader.read(Column::c_calc21, calc[21]);
    reader.read(Column::c_calc22, calc[22]);
    reader.read(Column::c_calc23, calc[23]);
    reader.read(Column::c_calc24, calc[24]);
    reader.read(Column::c_calc25, calc[25]);
    reader.read(Column::c_calc26, calc[26]);
    reader.read(Column::c_calc27, calc[27]);
    reader.read(Column::c_calc28, calc[28]);
    reader.read(Column::c_calc29, calc[29]);
    reader.read(Column::c_calc30, calc[30]);
    reader.read(Column::c_calc31, calc[31]);
    reader.read(Column::c_calc32, calc[32]);
    reader.read(Column::c_calc33, calc[33]);
    reader.read(Column::c_calc34, calc[34]);
    reader.read(Column::c_calc35, calc[35]);
    reader.read(Column::c_calc36, calc[36]);
    reader.read(Column::c_calc37, calc[37]);
    reader.read(Column::c_calc38, calc[38]);
    reader.read(Column::c_calc39, calc[39]);
    reader.read(Column::c_calc40, calc[40]);
    aurastatcalc.clear();
    aurastatcalc.reserve(6);
    {
        std::string value;
        reader.read(Column::c_aurastatcalc0, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastatcalc1, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastatcalc2, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastatcalc3, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastatcalc4, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastatcalc5, value);
        if (!value.empty())
            aurastatcalc.push_back(value);
    }
    aurastat.clear();
    aurastat.reserve(6);
    {
        std::string value;
        reader.read(Column::c_aurastat0, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastat1, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastat2, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastat3, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastat4, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_aurastat5, value);
        if (!value.empty())
            aurastat.push_back(value);
    }
    passivestat.clear();
    passivestat.reserve(14);
    {
        std::string value;
        reader.read(Column::c_passivestat0, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat1, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat2, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat3, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat4, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat5, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat6, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat7, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat8, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat9, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat10, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat11, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat12, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_passivestat13, value);
        if (!value.empty())
            passivestat.push_back(value);
    }
}

void DataRow_skills_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_skill, skill);
    writer.write(Column::c_id, id);
    writer.write(Column::c_skilldesc, skilldesc);
    writer.write(Column::c_maxlvl, maxlvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_inTown, inTown);
    writer.write(Column::c_leftskill, leftskill);
    writer.write(Column::c_costMult, costMult);
    writer.write(Column::c_costAdd, costAdd);
    writer.write(Column::c_eLen, eLen);
    writer.write(Column::c_hitShift, hitShift);
    writer.write(Column::c_manaShift, manaShift);
    writer.write(Column::c_eType, eType);
    writer.write(Column::c_classSpecific, classSpecific);
    writer.write(Column::c_itypea1, itypea1);
    writer.write(Column::c_anim, anim);
    writer.write(Column::c_seqtrans, seqtrans);
    if (reqskills.size() > 0)
    {
        writer.write(Column::c_reqskills0, reqskills[0]);
    }
    if (reqskills.size() > 1)
    {
        writer.write(Column::c_reqskills1, reqskills[1]);
    }
    if (reqskills.size() > 2)
    {
        writer.write(Column::c_reqskills2, reqskills[2]);
    }
    writer.write(Column::c_missile0, missile[0]);
    writer.write(Column::c_missile1, missile[1]);
    writer.write(Column::c_missile2, missile[2]);
    writer.write(Column::c_missile3, missile[3]);
    writer.write(Column::c_missile4, missile[4]);
    writer.write(Column::c_missile5, missile[5]);
    writer.write(Column::c_missile6, missile[6]);
    writer.write(Column::c_missile7, missile[7]);
    writer.write(Column::c_missile8, missile[8]);
    writer.write(Column::c_missile9, missile[9]);
    writer.write(Column::c_calc0, calc[0]);
    writer.write(Column::c_calc1, calc[1]);
    writer.write(Column::c_calc2, calc[2]);
    writer.write(Column::c_calc3, calc[3]);
    writer.write(Column::c_calc4, calc[4]);
    writer.write(Column::c_calc5, calc[5]);
    writer.write(Column::c_calc6, calc[6]);
    writer.write(Column::c_calc7, calc[7]);
    writer.write(Column::c_calc8, calc[8]);
    writer.write(Column::c_calc9, calc[9]);
    writer.write(Column::c_calc10, calc[10]);
    writer.write(Column::c_calc11, calc[11]);
    writer.write(Column::c_calc12, calc[12]);
    writer.write(Column::c_calc13, calc[13]);
    writer.write(Column::c_calc14, calc[14]);
    writer.write(Column::c_calc15, calc[15]);
    writer.write(Column::c_calc16, calc[16]);
    writer.write(Column::c_calc17, calc[17]);
    writer.write(Column::c_calc18, calc[18]);
    writer.write(Column::c_calc19, calc[19]);
    writer.write(Column::c_calc20, calc[20]);
    writer.write(Column::c_calc21, calc[21]);
    writer.write(Column::c_calc22, calc[22]);
    writer.write(Column::c_calc23, calc[23]);
    writer.write(Column::c_calc24, calc[24]);
    writer.write(Column::c_calc25, calc[25]);
    writer.write(Column::c_calc26, calc[26]);
    writer.write(Column::c_calc27, calc[27]);
    writer.write(Column::c_calc28, calc[28]);
    writer.write(Column::c_calc29, calc[29]);
    writer.write(Column::c_calc30, calc[30]);
    writer.write(Column::c_calc31, calc[31]);
    writer.write(Column::c_calc32, calc[32]);
    writer.write(Column::c_calc33, calc[33]);
    writer.write(Column::c_calc34, calc[34]);
    writer.write(Column::c_calc35, calc[35]);
    writer.write(Column::c_calc36, calc[36]);
    writer.write(Column::c_calc37, calc[37]);
    writer.write(Column::c_calc38, calc[38]);
    writer.write(Column::c_calc39, calc[39]);
    writer.write(Column::c_calc40, calc[40]);
    if (aurastatcalc.size() > 0)
    {
        writer.write(Column::c_aurastatcalc0, aurastatcalc[0]);
    }
    if (aurastatcalc.size() > 1)
    {
        writer.write(Column::c_aurastatcalc1, aurastatcalc[1]);
    }
    if (aurastatcalc.size() > 2)
    {
        writer.write(Column::c_aurastatcalc2, aurastatcalc[2]);
    }
    if (aurastatcalc.size() > 3)
    {
        writer.write(Column::c_aurastatcalc3, aurastatcalc[3]);
    }
    if (aurastatcalc.size() > 4)
    {
        writer.write(Column::c_aurastatcalc4, aurastatcalc[4]);
    }
    if (aurastatcalc.size() > 5)
    {
        writer.write(Column::c_aurastatcalc5, aurastatcalc[5]);
    }
    if (aurastat.size() > 0)
    {
        writer.write(Column::c_aurastat0, aurastat[0]);
    }
    if (aurastat.size() > 1)
    {
        writer.write(Column::c_aurastat1, aurastat[1]);
    }
    if (aurastat.size() > 2)
    {
        writer.write(Column::c_aurastat2, aurastat[2]);
    }
    if (aurastat.size() > 3)
    {
        writer.write(Column::c_aurastat3, aurastat[3]);
    }
    if (aurastat.size() > 4)
    {
        writer.write(Column::c_aurastat4, aurastat[4]);
    }
    if (aurastat.size() > 5)
    {
        writer.write(Column::c_aurastat5, aurastat[5]);
    }
    if (passivestat.size() > 0)
    {
        writer.write(Column::c_passivestat0, passivestat[0]);
    }
    if (passivestat.size() > 1)
    {
        writer.write(Column::c_passivestat1, passivestat[1]);
    }
    if (passivestat.size() > 2)
    {
        writer.write(Column::c_passivestat2, passivestat[2]);
    }
    if (passivestat.size() > 3)
    {
        writer.write(Column::c_passivestat3, passivestat[3]);
    }
    if (passivestat.size() > 4)
    {
        writer.write(Column::c_passivestat4, passivestat[4]);
    }
    if (passivestat.size() > 5)
    {
        writer.write(Column::c_passivestat5, passivestat[5]);
    }
    if (passivestat.size() > 6)
    {
        writer.write(Column::c_passivestat6, passivestat[6]);
    }
    if (passivestat.size() > 7)
    {
        writer.write(Column::c_passivestat7, passivestat[7]);
    }
    if (passivestat.size() > 8)
    {
        writer.write(Column::c_passivestat8, passivestat[8]);
    }
    if (passivestat.size() > 9)
    {
        writer.write(Column::c_passivestat9, passivestat[9]);
    }
    if (passivestat.size() > 10)
    {
        writer.write(Column::c_passivestat10, passivestat[10]);
    }
    if (passivestat.size() > 11)
    {
        writer.write(Column::c_passivestat11, passivestat[11]);
    }
    if (passivestat.size() > 12)
    {
        writer.write(Column::c_passivestat12, passivestat[12]);
    }
    if (passivestat.size() > 13)
    {
        writer.write(Column::c_passivestat13, passivestat[13]);
    }
}

enum DataRow_treasureclassex_codegen::Column : std::size_t {
    c_name,
    c_k_unique,
    c_k_set,
    c_k_rare,
    c_k_magic,
    c_noDrop,
    c_group,
    c_picks,
    c_level,
    c_items0_prob,
    c_items0_tc,
    c_items1_prob,
    c_items1_tc,
    c_items2_prob,
    c_items2_tc,
    c_items3_prob,
    c_items3_tc,
    c_items4_prob,
    c_items4_tc,
    c_items5_prob,
    c_items5_tc,
    c_items6_prob,
    c_items6_tc,
    c_items7_prob,
    c_items7_tc,
    c_items8_prob,
    c_items8_tc,
    c_items9_prob,
    c_items9_tc,
};

const GeneratedTableSchema& DataRow_treasureclassex_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "treasure class",
            "unique",
            "set",
            "rare",
            "magic",
            "nodrop",
            "group",
            "picks",
            "level",
            "prob1",
            "item1",
            "prob2",
            "item2",
            "prob3",
            "item3",
            "prob4",
            "item4",
            "prob5",
            "item5",
            "prob6",
            "item6",
            "prob7",
            "item7",
            "prob8",
            "item8",
            "prob9",
            "item9",
            "prob10",
            "item10",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_treasureclassex_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_k_unique, k_unique);
    reader.read(Column::c_k_set, k_set);
    reader.read(Column::c_k_rare, k_rare);
    reader.read(Column::c_k_magic, k_magic);
    reader.read(Column::c_noDrop, noDrop);
    reader.read(Column::c_group, group);
    reader.read(Column::c_picks, picks);
    reader.read(Column::c_level, level);
    items.clear();
    items.reserve(10);
    {
        Item item;
        reader.read(Column::c_items0_prob, item.prob);
        reader.read(Column::c_items0_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items1_prob, item.prob);
        reader.read(Column::c_items1_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items2_prob, item.prob);
        reader.read(Column::c_items2_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items3_prob, item.prob);
        reader.read(Column::c_items3_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items4_prob, item.prob);
        reader.read(Column::c_items4_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items5_prob, item.prob);
        reader.read(Column::c_items5_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items6_prob, item.prob);
        reader.read(Column::c_items6_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items7_prob, item.prob);
        reader.read(Column::c_items7_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items8_prob, item.prob);
        reader.read(Column::c_items8_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
    {
        Item item;
        reader.read(Column::c_items9_prob, item.prob);
        reader.read(Column::c_items9_tc, item.tc);
        if (!item.tc.empty())
            items.push_back(item);
    }
}

void DataRow_treasureclassex_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_k_unique, k_unique);
    writer.write(Column::c_k_set, k_set);
    writer.write(Column::c_k_rare, k_rare);
    writer.write(Column::c_k_magic, k_magic);
    writer.write(Column::c_noDrop, noDrop);
    writer.write(Column::c_group, group);
    writer.write(Column::c_picks, picks);
    writer.write(Column::c_level, level);
    if (items.size() > 0)
    {
        writer.write(Column::c_items0_prob, items[0].prob);
        writer.write(Column::c_items0_tc, items[0].tc);
    }
    if (items.size() > 1)
    {
        writer.write(Column::c_items1_prob, items[1].prob);
        writer.write(Column::c_items1_tc, items[1].tc);
    }
    if (items.size() > 2)
    {
        writer.write(Column::c_items2_prob, items[2].prob);
        writer.write(Column::c_items2_tc, items[2].tc);
    }
    if (items.size() > 3)
    {
        writer.write(Column::c_items3_prob, items[3].prob);
        writer.write(Column::c_items3_tc, items[3].tc);
    }
    if (items.size() > 4)
    {
        writer.write(Column::c_items4_prob, items[4].prob);
        writer.write(Column::c_items4_tc, items[4].tc);
    }
    if (items.size() > 5)
    {
        writer.write(Column::c_items5_prob, items[5].prob);
        writer.write(Column::c_items5_tc, items[5].tc);
    }
    if (items.size() > 6)
    {
        writer.write(Column::c_items6_prob, items[6].prob);
        writer.write(Column::c_items6_tc, items[6].tc);
    }
    if (items.size() > 7)
    {
        writer.write(Column::c_items7_prob, items[7].prob);
        writer.write(Column::c_items7_tc, items[7].tc);
    }
    if (items.size() > 8)
    {
        writer.write(Column::c_items8_prob, items[8].prob);
        writer.write(Column::c_items8_tc, items[8].tc);
    }
    if (items.size() > 9)
    {
        writer.write(Column::c_items9_prob, items[9].prob);
        writer.write(Column::c_items9_tc, items[9].tc);
    }
}

enum DataRow_missiles_codegen::Column : std::size_t {
    c_name,
    c_id,
    c_eType,
    c_color_red,
    c_color_green,
    c_color_blue,
    c_skill,
    c_missileSkill,
    c_EDmgSymPerCalc,
    c_childmissile0,
    c_childmissile1,
    c_childmissile2,
    c_childmissile3,
    c_childmissile4,
    c_childmissile5,
    c_childmissile6,
    c_childmissile7,
    c_childmissile8,
    c_childmissile9,
    c_childmissile10,
    c_childmissile11,
    c_childmissile12,
    c_childmissile13,
    c_childmissile14,
};

const GeneratedTableSchema& DataRow_missiles_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "missile",
            "*id",
            "etype",
            "red",
            "green",
            "blue",
            "skill",
            "missileskill",
            "edmgsympercalc",
            "explosionmissile",
            "submissile1",
            "submissile2",
            "submissile3",
            "hitsubmissile1",
            "hitsubmissile2",
            "hitsubmissile3",
            "hitsubmissile4",
            "cltsubmissile1",
            "cltsubmissile2",
            "cltsubmissile3",
            "clthitsubmissile1",
            "clthitsubmissile2",
            "clthitsubmissile3",
            "clthitsubmissile4",
        },
        .aliases = {
            { "id", "*id" },
        },
    };
    return schema;
}

void DataRow_missiles_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_id, id);
    reader.read(Column::c_eType, eType);
    reader.read(Column::c_color_red, color.red);
    reader.read(Column::c_color_green, color.green);
    reader.read(Column::c_color_blue, color.blue);
    reader.read(Column::c_skill, skill);
    reader.read(Column::c_missileSkill, missileSkill);
    reader.read(Column::c_EDmgSymPerCalc, EDmgSymPerCalc);
    reader.read(Column::c_childmissile0, childmissile[0]);
    reader.read(Column::c_childmissile1, childmissile[1]);
    reader.read(Column::c_childmissile2, childmissile[2]);
    reader.read(Column::c_childmissile3, childmissile[3]);
    reader.read(Column::c_childmissile4, childmissile[4]);
    reader.read(Column::c_childmissile5, childmissile[5]);
    reader.read(Column::c_childmissile6, childmissile[6]);
    reader.read(Column::c_childmissile7, childmissile[7]);
    reader.read(Column::c_childmissile8, childmissile[8]);
    reader.read(Column::c_childmissile9, childmissile[9]);
    reader.read(Column::c_childmissile10, childmissile[10]);
    reader.read(Column::c_childmissile11, childmissile[11]);
    reader.read(Column::c_childmissile12, childmissile[12]);
    reader.read(Column::c_childmissile13, childmissile[13]);
    reader.read(Column::c_childmissile14, childmissile[14]);
}

void DataRow_missiles_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_id, id);
    writer.write(Column::c_eType, eType);
    writer.write(Column::c_color_red, color.red);
    writer.write(Column::c_color_green, color.green);
    writer.write(Column::c_color_blue, color.blue);
    writer.write(Column::c_skill, skill);
    writer.write(Column::c_missileSkill, missileSkill);
    writer.write(Column::c_EDmgSymPerCalc, EDmgSymPerCalc);
    writer.write(Column::c_childmissile0, childmissile[0]);
    writer.write(Column::c_childmissile1, childmissile[1]);
    writer.write(Column::c_childmissile2, childmissile[2]);
    writer.write(Column::c_childmissile3, childmissile[3]);
    writer.write(Column::c_childmissile4, childmissile[4]);
    writer.write(Column::c_childmissile5, childmissile[5]);
    writer.write(Column::c_childmissile6, childmissile[6]);
    writer.write(Column::c_childmissile7, childmissile[7]);
    writer.write(Column::c_childmissile8, childmissile[8]);
    writer.write(Column::c_childmissile9, childmissile[9]);
    writer.write(Column::c_childmissile10, childmissile[10]);
    writer.write(Column::c_childmissile11, childmissile[11]);
    writer.write(Column::c_childmissile12, childmissile[12]);
    writer.write(Column::c_childmissile13, childmissile[13]);
    writer.write(Column::c_childmissile14, childmissile[14]);
}

enum DataRow_inventory_codegen::Column : std::size_t {
    c_charclass,
    c_gridX,
    c_gridY,
};

const GeneratedTableSchema& DataRow_inventory_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "class",
            "gridx",
            "gridy",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_inventory_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_charclass, charclass);
    reader.read(Column::c_gridX, gridX);
    reader.read(Column::c_gridY, gridY);
}

void DataRow_inventory_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_charclass, charclass);
    writer.write(Column::c_gridX, gridX);
    writer.write(Column::c_gridY, gridY);
}

enum DataRow_lvlmaze_codegen::Column : std::size_t {
    c_name,
    c_levelId,
    c_rooms0,
    c_rooms1,
    c_rooms2,
};

const GeneratedTableSchema& DataRow_lvlmaze_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "rooms",
            "rooms(n)",
            "rooms(h)",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_lvlmaze_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_levelId, levelId);
    reader.read(Column::c_rooms0, rooms[0]);
    reader.read(Column::c_rooms1, rooms[1]);
    reader.read(Column::c_rooms2, rooms[2]);
}

void DataRow_lvlmaze_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_levelId, levelId);
    writer.write(Column::c_rooms0, rooms[0]);
    writer.write(Column::c_rooms1, rooms[1]);
    writer.write(Column::c_rooms2, rooms[2]);
}

enum DataRow_superuniques_codegen::Column : std::size_t {
    c_monsterClass,
};

const GeneratedTableSchema& DataRow_superuniques_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "class",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_superuniques_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_monsterClass, monsterClass);
}

void DataRow_superuniques_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_monsterClass, monsterClass);
}

enum DataRow_weapons_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_lvlreq,
    c_code,
    c_itemTypeCode,
    c_reqstr,
    c_reqdex,
    c_showLevel,
    c_normcode,
    c_ubercode,
    c_ultracode,
    c_mindam,
    c_maxdam,
};

const GeneratedTableSchema& DataRow_weapons_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "levelreq",
            "code",
            "type",
            "reqstr",
            "reqdex",
            "showlevel",
            "normcode",
            "ubercode",
            "ultracode",
            "mindam",
            "maxdam",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_weapons_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_code, code);
    reader.read(Column::c_itemTypeCode, itemTypeCode);
    reader.read(Column::c_reqstr, reqstr);
    reader.read(Column::c_reqdex, reqdex);
    reader.read(Column::c_showLevel, showLevel);
    reader.read(Column::c_normcode, normcode);
    reader.read(Column::c_ubercode, ubercode);
    reader.read(Column::c_ultracode, ultracode);
    reader.read(Column::c_mindam, mindam);
    reader.read(Column::c_maxdam, maxdam);
}

void DataRow_weapons_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_code, code);
    writer.write(Column::c_itemTypeCode, itemTypeCode);
    writer.write(Column::c_reqstr, reqstr);
    writer.write(Column::c_reqdex, reqdex);
    writer.write(Column::c_showLevel, showLevel);
    writer.write(Column::c_normcode, normcode);
    writer.write(Column::c_ubercode, ubercode);
    writer.write(Column::c_ultracode, ultracode);
    writer.write(Column::c_mindam, mindam);
    writer.write(Column::c_maxdam, maxdam);
}

enum DataRow_armor_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_lvlreq,
    c_code,
    c_itemTypeCode,
    c_reqstr,
    c_reqdex,
    c_showLevel,
    c_normcode,
    c_ubercode,
    c_ultracode,
    c_minac,
    c_maxac,
};

const GeneratedTableSchema& DataRow_armor_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "levelreq",
            "code",
            "type",
            "reqstr",
            "reqdex",
            "showlevel",
            "normcode",
            "ubercode",
            "ultracode",
            "minac",
            "maxac",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_armor_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_code, code);
    reader.read(Column::c_itemTypeCode, itemTypeCode);
    reader.read(Column::c_reqstr, reqstr);
    reader.read(Column::c_reqdex, reqdex);
    reader.read(Column::c_showLevel, showLevel);
    reader.read(Column::c_normcode, normcode);
    reader.read(Column::c_ubercode, ubercode);
    reader.read(Column::c_ultracode, ultracode);
    reader.read(Column::c_minac, minac);
    reader.read(Column::c_maxac, maxac);
}

void DataRow_armor_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_code, code);
    writer.write(Column::c_itemTypeCode, itemTypeCode);
    writer.write(Column::c_reqstr, reqstr);
    writer.write(Column::c_reqdex, reqdex);
    writer.write(Column::c_showLevel, showLevel);
    writer.write(Column::c_normcode, normcode);
    writer.write(Column::c_ubercode, ubercode);
    writer.write(Column::c_ultracode, ultracode);
    writer.write(Column::c_minac, minac);
    writer.write(Column::c_maxac, maxac);
}

enum DataRow_misc_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_lvlreq,
    c_code,
    c_itemTypeCode,
    c_maxstack,
};

const GeneratedTableSchema& DataRow_misc_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "levelreq",
            "code",
            "type",
            "maxstack",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_misc_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_code, code);
    reader.read(Column::c_itemTypeCode, itemTypeCode);
    reader.read(Column::c_maxstack, maxstack);
}

void DataRow_misc_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_code, code);
    writer.write(Column::c_itemTypeCode, itemTypeCode);
    writer.write(Column::c_maxstack, maxstack);
}

enum DataRow_automagic_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_maxlevel,
    c_lvlreq,
    c_spawnable,
    c_affixes0_code,
    c_affixes0_par,
    c_affixes0_min,
    c_affixes0_max,
    c_affixes1_code,
    c_affixes1_par,
    c_affixes1_min,
    c_affixes1_max,
    c_affixes2_code,
    c_affixes2_par,
    c_affixes2_min,
    c_affixes2_max,
    c_itype0,
    c_itype1,
    c_itype2,
    c_itype3,
    c_itype4,
    c_itype5,
    c_itype6,
    c_etype0,
    c_etype1,
    c_etype2,
    c_etype3,
    c_etype4,
};

const GeneratedTableSchema& DataRow_automagic_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "maxlevel",
            "levelreq",
            "spawnable",
            "mod1code",
            "mod1param",
            "mod1min",
            "mod1max",
            "mod2code",
            "mod2param",
            "mod2min",
            "mod2max",
            "mod3code",
            "mod3param",
            "mod3min",
            "mod3max",
            "itype1",
            "itype2",
            "itype3",
            "itype4",
            "itype5",
            "itype6",
            "itype7",
            "etype1",
            "etype2",
            "etype3",
            "etype4",
            "etype5",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_automagic_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_maxlevel, maxlevel);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_spawnable, spawnable);
    affixes_limit = 0;
    if (reader.available(Column::c_affixes0_code) || reader.available(Column::c_affixes0_par) || reader.available(Column::c_affixes0_min) || reader.available(Column::c_affixes0_max))
        affixes_limit = 1;
    if (reader.available(Column::c_affixes1_code) || reader.available(Column::c_affixes1_par) || reader.available(Column::c_affixes1_min) || reader.available(Column::c_affixes1_max))
        affixes_limit = 2;
    if (reader.available(Column::c_affixes2_code) || reader.available(Column::c_affixes2_par) || reader.available(Column::c_affixes2_min) || reader.available(Column::c_affixes2_max))
        affixes_limit = 3;
    affixes.clear();
    affixes.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes0_code, item.code);
        reader.read(Column::c_affixes0_par, item.par);
        reader.read(Column::c_affixes0_min, item.min);
        reader.read(Column::c_affixes0_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes1_code, item.code);
        reader.read(Column::c_affixes1_par, item.par);
        reader.read(Column::c_affixes1_min, item.min);
        reader.read(Column::c_affixes1_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes2_code, item.code);
        reader.read(Column::c_affixes2_par, item.par);
        reader.read(Column::c_affixes2_min, item.min);
        reader.read(Column::c_affixes2_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    itype.clear();
    itype.reserve(7);
    {
        std::string value;
        reader.read(Column::c_itype0, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype1, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype2, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype3, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype4, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype5, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype6, value);
        if (!value.empty())
            itype.push_back(value);
    }
    etype.clear();
    etype.reserve(5);
    {
        std::string value;
        reader.read(Column::c_etype0, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype1, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype2, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype3, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype4, value);
        if (!value.empty())
            etype.push_back(value);
    }
}

void DataRow_automagic_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_maxlevel, maxlevel);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_spawnable, spawnable);
    if (affixes.size() > 0)
    {
        writer.write(Column::c_affixes0_code, affixes[0].code);
        writer.write(Column::c_affixes0_par, affixes[0].par);
        writer.write(Column::c_affixes0_min, affixes[0].min);
        writer.write(Column::c_affixes0_max, affixes[0].max);
    }
    if (affixes.size() > 1)
    {
        writer.write(Column::c_affixes1_code, affixes[1].code);
        writer.write(Column::c_affixes1_par, affixes[1].par);
        writer.write(Column::c_affixes1_min, affixes[1].min);
        writer.write(Column::c_affixes1_max, affixes[1].max);
    }
    if (affixes.size() > 2)
    {
        writer.write(Column::c_affixes2_code, affixes[2].code);
        writer.write(Column::c_affixes2_par, affixes[2].par);
        writer.write(Column::c_affixes2_min, affixes[2].min);
        writer.write(Column::c_affixes2_max, affixes[2].max);
    }
    if (itype.size() > 0)
    {
        writer.write(Column::c_itype0, itype[0]);
    }
    if (itype.size() > 1)
    {
        writer.write(Column::c_itype1, itype[1]);
    }
    if (itype.size() > 2)
    {
        writer.write(Column::c_itype2, itype[2]);
    }
    if (itype.size() > 3)
    {
        writer.write(Column::c_itype3, itype[3]);
    }
    if (itype.size() > 4)
    {
        writer.write(Column::c_itype4, itype[4]);
    }
    if (itype.size() > 5)
    {
        writer.write(Column::c_itype5, itype[5]);
    }
    if (itype.size() > 6)
    {
        writer.write(Column::c_itype6, itype[6]);
    }
    if (etype.size() > 0)
    {
        writer.write(Column::c_etype0, etype[0]);
    }
    if (etype.size() > 1)
    {
        writer.write(Column::c_etype1, etype[1]);
    }
    if (etype.size() > 2)
    {
        writer.write(Column::c_etype2, etype[2]);
    }
    if (etype.size() > 3)
    {
        writer.write(Column::c_etype3, etype[3]);
    }
    if (etype.size() > 4)
    {
        writer.write(Column::c_etype4, etype[4]);
    }
}

enum DataRow_magicprefix_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_maxlevel,
    c_lvlreq,
    c_spawnable,
    c_affixes0_code,
    c_affixes0_par,
    c_affixes0_min,
    c_affixes0_max,
    c_affixes1_code,
    c_affixes1_par,
    c_affixes1_min,
    c_affixes1_max,
    c_affixes2_code,
    c_affixes2_par,
    c_affixes2_min,
    c_affixes2_max,
    c_itype0,
    c_itype1,
    c_itype2,
    c_itype3,
    c_itype4,
    c_itype5,
    c_itype6,
    c_etype0,
    c_etype1,
    c_etype2,
    c_etype3,
    c_etype4,
};

const GeneratedTableSchema& DataRow_magicprefix_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "maxlevel",
            "levelreq",
            "spawnable",
            "mod1code",
            "mod1param",
            "mod1min",
            "mod1max",
            "mod2code",
            "mod2param",
            "mod2min",
            "mod2max",
            "mod3code",
            "mod3param",
            "mod3min",
            "mod3max",
            "itype1",
            "itype2",
            "itype3",
            "itype4",
            "itype5",
            "itype6",
            "itype7",
            "etype1",
            "etype2",
            "etype3",
            "etype4",
            "etype5",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_magicprefix_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_maxlevel, maxlevel);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_spawnable, spawnable);
    affixes_limit = 0;
    if (reader.available(Column::c_affixes0_code) || reader.available(Column::c_affixes0_par) || reader.available(Column::c_affixes0_min) || reader.available(Column::c_affixes0_max))
        affixes_limit = 1;
    if (reader.available(Column::c_affixes1_code) || reader.available(Column::c_affixes1_par) || reader.available(Column::c_affixes1_min) || reader.available(Column::c_affixes1_max))
        affixes_limit = 2;
    if (reader.available(Column::c_affixes2_code) || reader.available(Column::c_affixes2_par) || reader.available(Column::c_affixes2_min) || reader.available(Column::c_affixes2_max))
        affixes_limit = 3;
    affixes.clear();
    affixes.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes0_code, item.code);
        reader.read(Column::c_affixes0_par, item.par);
        reader.read(Column::c_affixes0_min, item.min);
        reader.read(Column::c_affixes0_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes1_code, item.code);
        reader.read(Column::c_affixes1_par, item.par);
        reader.read(Column::c_affixes1_min, item.min);
        reader.read(Column::c_affixes1_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes2_code, item.code);
        reader.read(Column::c_affixes2_par, item.par);
        reader.read(Column::c_affixes2_min, item.min);
        reader.read(Column::c_affixes2_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    itype.clear();
    itype.reserve(7);
    {
        std::string value;
        reader.read(Column::c_itype0, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype1, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype2, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype3, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype4, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype5, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype6, value);
        if (!value.empty())
            itype.push_back(value);
    }
    etype.clear();
    etype.reserve(5);
    {
        std::string value;
        reader.read(Column::c_etype0, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype1, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype2, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype3, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype4, value);
        if (!value.empty())
            etype.push_back(value);
    }
}

void DataRow_magicprefix_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_maxlevel, maxlevel);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_spawnable, spawnable);
    if (affixes.size() > 0)
    {
        writer.write(Column::c_affixes0_code, affixes[0].code);
        writer.write(Column::c_affixes0_par, affixes[0].par);
        writer.write(Column::c_affixes0_min, affixes[0].min);
        writer.write(Column::c_affixes0_max, affixes[0].max);
    }
    if (affixes.size() > 1)
    {
        writer.write(Column::c_affixes1_code, affixes[1].code);
        writer.write(Column::c_affixes1_par, affixes[1].par);
        writer.write(Column::c_affixes1_min, affixes[1].min);
        writer.write(Column::c_affixes1_max, affixes[1].max);
    }
    if (affixes.size() > 2)
    {
        writer.write(Column::c_affixes2_code, affixes[2].code);
        writer.write(Column::c_affixes2_par, affixes[2].par);
        writer.write(Column::c_affixes2_min, affixes[2].min);
        writer.write(Column::c_affixes2_max, affixes[2].max);
    }
    if (itype.size() > 0)
    {
        writer.write(Column::c_itype0, itype[0]);
    }
    if (itype.size() > 1)
    {
        writer.write(Column::c_itype1, itype[1]);
    }
    if (itype.size() > 2)
    {
        writer.write(Column::c_itype2, itype[2]);
    }
    if (itype.size() > 3)
    {
        writer.write(Column::c_itype3, itype[3]);
    }
    if (itype.size() > 4)
    {
        writer.write(Column::c_itype4, itype[4]);
    }
    if (itype.size() > 5)
    {
        writer.write(Column::c_itype5, itype[5]);
    }
    if (itype.size() > 6)
    {
        writer.write(Column::c_itype6, itype[6]);
    }
    if (etype.size() > 0)
    {
        writer.write(Column::c_etype0, etype[0]);
    }
    if (etype.size() > 1)
    {
        writer.write(Column::c_etype1, etype[1]);
    }
    if (etype.size() > 2)
    {
        writer.write(Column::c_etype2, etype[2]);
    }
    if (etype.size() > 3)
    {
        writer.write(Column::c_etype3, etype[3]);
    }
    if (etype.size() > 4)
    {
        writer.write(Column::c_etype4, etype[4]);
    }
}

enum DataRow_magicsuffix_codegen::Column : std::size_t {
    c_name,
    c_lvl,
    c_maxlevel,
    c_lvlreq,
    c_spawnable,
    c_affixes0_code,
    c_affixes0_par,
    c_affixes0_min,
    c_affixes0_max,
    c_affixes1_code,
    c_affixes1_par,
    c_affixes1_min,
    c_affixes1_max,
    c_affixes2_code,
    c_affixes2_par,
    c_affixes2_min,
    c_affixes2_max,
    c_itype0,
    c_itype1,
    c_itype2,
    c_itype3,
    c_itype4,
    c_itype5,
    c_itype6,
    c_etype0,
    c_etype1,
    c_etype2,
    c_etype3,
    c_etype4,
};

const GeneratedTableSchema& DataRow_magicsuffix_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "name",
            "level",
            "maxlevel",
            "levelreq",
            "spawnable",
            "mod1code",
            "mod1param",
            "mod1min",
            "mod1max",
            "mod2code",
            "mod2param",
            "mod2min",
            "mod2max",
            "mod3code",
            "mod3param",
            "mod3min",
            "mod3max",
            "itype1",
            "itype2",
            "itype3",
            "itype4",
            "itype5",
            "itype6",
            "itype7",
            "etype1",
            "etype2",
            "etype3",
            "etype4",
            "etype5",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_magicsuffix_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_name, name);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_maxlevel, maxlevel);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_spawnable, spawnable);
    affixes_limit = 0;
    if (reader.available(Column::c_affixes0_code) || reader.available(Column::c_affixes0_par) || reader.available(Column::c_affixes0_min) || reader.available(Column::c_affixes0_max))
        affixes_limit = 1;
    if (reader.available(Column::c_affixes1_code) || reader.available(Column::c_affixes1_par) || reader.available(Column::c_affixes1_min) || reader.available(Column::c_affixes1_max))
        affixes_limit = 2;
    if (reader.available(Column::c_affixes2_code) || reader.available(Column::c_affixes2_par) || reader.available(Column::c_affixes2_min) || reader.available(Column::c_affixes2_max))
        affixes_limit = 3;
    affixes.clear();
    affixes.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes0_code, item.code);
        reader.read(Column::c_affixes0_par, item.par);
        reader.read(Column::c_affixes0_min, item.min);
        reader.read(Column::c_affixes0_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes1_code, item.code);
        reader.read(Column::c_affixes1_par, item.par);
        reader.read(Column::c_affixes1_min, item.min);
        reader.read(Column::c_affixes1_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes2_code, item.code);
        reader.read(Column::c_affixes2_par, item.par);
        reader.read(Column::c_affixes2_min, item.min);
        reader.read(Column::c_affixes2_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    itype.clear();
    itype.reserve(7);
    {
        std::string value;
        reader.read(Column::c_itype0, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype1, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype2, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype3, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype4, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype5, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype6, value);
        if (!value.empty())
            itype.push_back(value);
    }
    etype.clear();
    etype.reserve(5);
    {
        std::string value;
        reader.read(Column::c_etype0, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype1, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype2, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype3, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype4, value);
        if (!value.empty())
            etype.push_back(value);
    }
}

void DataRow_magicsuffix_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_name, name);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_maxlevel, maxlevel);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_spawnable, spawnable);
    if (affixes.size() > 0)
    {
        writer.write(Column::c_affixes0_code, affixes[0].code);
        writer.write(Column::c_affixes0_par, affixes[0].par);
        writer.write(Column::c_affixes0_min, affixes[0].min);
        writer.write(Column::c_affixes0_max, affixes[0].max);
    }
    if (affixes.size() > 1)
    {
        writer.write(Column::c_affixes1_code, affixes[1].code);
        writer.write(Column::c_affixes1_par, affixes[1].par);
        writer.write(Column::c_affixes1_min, affixes[1].min);
        writer.write(Column::c_affixes1_max, affixes[1].max);
    }
    if (affixes.size() > 2)
    {
        writer.write(Column::c_affixes2_code, affixes[2].code);
        writer.write(Column::c_affixes2_par, affixes[2].par);
        writer.write(Column::c_affixes2_min, affixes[2].min);
        writer.write(Column::c_affixes2_max, affixes[2].max);
    }
    if (itype.size() > 0)
    {
        writer.write(Column::c_itype0, itype[0]);
    }
    if (itype.size() > 1)
    {
        writer.write(Column::c_itype1, itype[1]);
    }
    if (itype.size() > 2)
    {
        writer.write(Column::c_itype2, itype[2]);
    }
    if (itype.size() > 3)
    {
        writer.write(Column::c_itype3, itype[3]);
    }
    if (itype.size() > 4)
    {
        writer.write(Column::c_itype4, itype[4]);
    }
    if (itype.size() > 5)
    {
        writer.write(Column::c_itype5, itype[5]);
    }
    if (itype.size() > 6)
    {
        writer.write(Column::c_itype6, itype[6]);
    }
    if (etype.size() > 0)
    {
        writer.write(Column::c_etype0, etype[0]);
    }
    if (etype.size() > 1)
    {
        writer.write(Column::c_etype1, etype[1]);
    }
    if (etype.size() > 2)
    {
        writer.write(Column::c_etype2, etype[2]);
    }
    if (etype.size() > 3)
    {
        writer.write(Column::c_etype3, etype[3]);
    }
    if (etype.size() > 4)
    {
        writer.write(Column::c_etype4, etype[4]);
    }
}

enum DataRow_uniqueitems_codegen::Column : std::size_t {
    c_index,
    c_eqipmentCode,
    c_rarity,
    c_lvl,
    c_lvlreq,
    c_spawnable,
    c_disableChronicle,
    c_cost_mul,
    c_cost_add,
    c_id,
    c_itemName,
    c_chrtransform,
    c_invtransform,
    c_flippyfile,
    c_invfile,
    c_dropsound,
    c_dropsfxframe,
    c_usesound,
    c_eol,
    c_carry1,
    c_version,
    c_affixes0_code,
    c_affixes0_par,
    c_affixes0_min,
    c_affixes0_max,
    c_affixes1_code,
    c_affixes1_par,
    c_affixes1_min,
    c_affixes1_max,
    c_affixes2_code,
    c_affixes2_par,
    c_affixes2_min,
    c_affixes2_max,
    c_affixes3_code,
    c_affixes3_par,
    c_affixes3_min,
    c_affixes3_max,
    c_affixes4_code,
    c_affixes4_par,
    c_affixes4_min,
    c_affixes4_max,
    c_affixes5_code,
    c_affixes5_par,
    c_affixes5_min,
    c_affixes5_max,
    c_affixes6_code,
    c_affixes6_par,
    c_affixes6_min,
    c_affixes6_max,
    c_affixes7_code,
    c_affixes7_par,
    c_affixes7_min,
    c_affixes7_max,
    c_affixes8_code,
    c_affixes8_par,
    c_affixes8_min,
    c_affixes8_max,
    c_affixes9_code,
    c_affixes9_par,
    c_affixes9_min,
    c_affixes9_max,
    c_affixes10_code,
    c_affixes10_par,
    c_affixes10_min,
    c_affixes10_max,
    c_affixes11_code,
    c_affixes11_par,
    c_affixes11_min,
    c_affixes11_max,
};

const GeneratedTableSchema& DataRow_uniqueitems_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "index",
            "code",
            "rarity",
            "lvl",
            "lvl req",
            "spawnable",
            "disablechronicle",
            "cost mult",
            "cost add",
            "*id",
            "*itemname",
            "chrtransform",
            "invtransform",
            "flippyfile",
            "invfile",
            "dropsound",
            "dropsfxframe",
            "usesound",
            "*eol",
            "carry1",
            "version",
            "prop1",
            "par1",
            "min1",
            "max1",
            "prop2",
            "par2",
            "min2",
            "max2",
            "prop3",
            "par3",
            "min3",
            "max3",
            "prop4",
            "par4",
            "min4",
            "max4",
            "prop5",
            "par5",
            "min5",
            "max5",
            "prop6",
            "par6",
            "min6",
            "max6",
            "prop7",
            "par7",
            "min7",
            "max7",
            "prop8",
            "par8",
            "min8",
            "max8",
            "prop9",
            "par9",
            "min9",
            "max9",
            "prop10",
            "par10",
            "min10",
            "max10",
            "prop11",
            "par11",
            "min11",
            "max11",
            "prop12",
            "par12",
            "min12",
            "max12",
        },
        .aliases = {
            { "enabled", "spawnable" },
            { "level", "lvl" },
            { "item", "code" },
        },
    };
    return schema;
}

void DataRow_uniqueitems_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_index, index);
    reader.read(Column::c_eqipmentCode, eqipmentCode);
    reader.read(Column::c_rarity, rarity);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_spawnable, spawnable, true);
    reader.read(Column::c_disableChronicle, disableChronicle);
    reader.read(Column::c_cost_mul, cost_mul);
    reader.read(Column::c_cost_add, cost_add);
    reader.read(Column::c_id, id);
    reader.read(Column::c_itemName, itemName);
    reader.read(Column::c_chrtransform, chrtransform);
    reader.read(Column::c_invtransform, invtransform);
    reader.read(Column::c_flippyfile, flippyfile);
    reader.read(Column::c_invfile, invfile);
    reader.read(Column::c_dropsound, dropsound);
    reader.read(Column::c_dropsfxframe, dropsfxframe);
    reader.read(Column::c_usesound, usesound);
    reader.read(Column::c_eol, eol);
    reader.read(Column::c_carry1, carry1);
    reader.read(Column::c_version, version);
    affixes_limit = 0;
    if (reader.available(Column::c_affixes0_code) || reader.available(Column::c_affixes0_par) || reader.available(Column::c_affixes0_min) || reader.available(Column::c_affixes0_max))
        affixes_limit = 1;
    if (reader.available(Column::c_affixes1_code) || reader.available(Column::c_affixes1_par) || reader.available(Column::c_affixes1_min) || reader.available(Column::c_affixes1_max))
        affixes_limit = 2;
    if (reader.available(Column::c_affixes2_code) || reader.available(Column::c_affixes2_par) || reader.available(Column::c_affixes2_min) || reader.available(Column::c_affixes2_max))
        affixes_limit = 3;
    if (reader.available(Column::c_affixes3_code) || reader.available(Column::c_affixes3_par) || reader.available(Column::c_affixes3_min) || reader.available(Column::c_affixes3_max))
        affixes_limit = 4;
    if (reader.available(Column::c_affixes4_code) || reader.available(Column::c_affixes4_par) || reader.available(Column::c_affixes4_min) || reader.available(Column::c_affixes4_max))
        affixes_limit = 5;
    if (reader.available(Column::c_affixes5_code) || reader.available(Column::c_affixes5_par) || reader.available(Column::c_affixes5_min) || reader.available(Column::c_affixes5_max))
        affixes_limit = 6;
    if (reader.available(Column::c_affixes6_code) || reader.available(Column::c_affixes6_par) || reader.available(Column::c_affixes6_min) || reader.available(Column::c_affixes6_max))
        affixes_limit = 7;
    if (reader.available(Column::c_affixes7_code) || reader.available(Column::c_affixes7_par) || reader.available(Column::c_affixes7_min) || reader.available(Column::c_affixes7_max))
        affixes_limit = 8;
    if (reader.available(Column::c_affixes8_code) || reader.available(Column::c_affixes8_par) || reader.available(Column::c_affixes8_min) || reader.available(Column::c_affixes8_max))
        affixes_limit = 9;
    if (reader.available(Column::c_affixes9_code) || reader.available(Column::c_affixes9_par) || reader.available(Column::c_affixes9_min) || reader.available(Column::c_affixes9_max))
        affixes_limit = 10;
    if (reader.available(Column::c_affixes10_code) || reader.available(Column::c_affixes10_par) || reader.available(Column::c_affixes10_min) || reader.available(Column::c_affixes10_max))
        affixes_limit = 11;
    if (reader.available(Column::c_affixes11_code) || reader.available(Column::c_affixes11_par) || reader.available(Column::c_affixes11_min) || reader.available(Column::c_affixes11_max))
        affixes_limit = 12;
    affixes.clear();
    affixes.reserve(12);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes0_code, item.code);
        reader.read(Column::c_affixes0_par, item.par);
        reader.read(Column::c_affixes0_min, item.min);
        reader.read(Column::c_affixes0_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes1_code, item.code);
        reader.read(Column::c_affixes1_par, item.par);
        reader.read(Column::c_affixes1_min, item.min);
        reader.read(Column::c_affixes1_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes2_code, item.code);
        reader.read(Column::c_affixes2_par, item.par);
        reader.read(Column::c_affixes2_min, item.min);
        reader.read(Column::c_affixes2_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes3_code, item.code);
        reader.read(Column::c_affixes3_par, item.par);
        reader.read(Column::c_affixes3_min, item.min);
        reader.read(Column::c_affixes3_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes4_code, item.code);
        reader.read(Column::c_affixes4_par, item.par);
        reader.read(Column::c_affixes4_min, item.min);
        reader.read(Column::c_affixes4_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes5_code, item.code);
        reader.read(Column::c_affixes5_par, item.par);
        reader.read(Column::c_affixes5_min, item.min);
        reader.read(Column::c_affixes5_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes6_code, item.code);
        reader.read(Column::c_affixes6_par, item.par);
        reader.read(Column::c_affixes6_min, item.min);
        reader.read(Column::c_affixes6_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes7_code, item.code);
        reader.read(Column::c_affixes7_par, item.par);
        reader.read(Column::c_affixes7_min, item.min);
        reader.read(Column::c_affixes7_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes8_code, item.code);
        reader.read(Column::c_affixes8_par, item.par);
        reader.read(Column::c_affixes8_min, item.min);
        reader.read(Column::c_affixes8_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes9_code, item.code);
        reader.read(Column::c_affixes9_par, item.par);
        reader.read(Column::c_affixes9_min, item.min);
        reader.read(Column::c_affixes9_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes10_code, item.code);
        reader.read(Column::c_affixes10_par, item.par);
        reader.read(Column::c_affixes10_min, item.min);
        reader.read(Column::c_affixes10_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes11_code, item.code);
        reader.read(Column::c_affixes11_par, item.par);
        reader.read(Column::c_affixes11_min, item.min);
        reader.read(Column::c_affixes11_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
}

void DataRow_uniqueitems_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_index, index);
    writer.write(Column::c_eqipmentCode, eqipmentCode);
    writer.write(Column::c_rarity, rarity);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_spawnable, spawnable);
    writer.write(Column::c_disableChronicle, disableChronicle);
    writer.write(Column::c_cost_mul, cost_mul);
    writer.write(Column::c_cost_add, cost_add);
    writer.write(Column::c_id, id);
    writer.write(Column::c_itemName, itemName);
    writer.write(Column::c_chrtransform, chrtransform);
    writer.write(Column::c_invtransform, invtransform);
    writer.write(Column::c_flippyfile, flippyfile);
    writer.write(Column::c_invfile, invfile);
    writer.write(Column::c_dropsound, dropsound);
    writer.write(Column::c_dropsfxframe, dropsfxframe);
    writer.write(Column::c_usesound, usesound);
    writer.write(Column::c_eol, eol);
    writer.write(Column::c_carry1, carry1);
    writer.write(Column::c_version, version);
    if (affixes.size() > 0)
    {
        writer.write(Column::c_affixes0_code, affixes[0].code);
        writer.write(Column::c_affixes0_par, affixes[0].par);
        writer.write(Column::c_affixes0_min, affixes[0].min);
        writer.write(Column::c_affixes0_max, affixes[0].max);
    }
    if (affixes.size() > 1)
    {
        writer.write(Column::c_affixes1_code, affixes[1].code);
        writer.write(Column::c_affixes1_par, affixes[1].par);
        writer.write(Column::c_affixes1_min, affixes[1].min);
        writer.write(Column::c_affixes1_max, affixes[1].max);
    }
    if (affixes.size() > 2)
    {
        writer.write(Column::c_affixes2_code, affixes[2].code);
        writer.write(Column::c_affixes2_par, affixes[2].par);
        writer.write(Column::c_affixes2_min, affixes[2].min);
        writer.write(Column::c_affixes2_max, affixes[2].max);
    }
    if (affixes.size() > 3)
    {
        writer.write(Column::c_affixes3_code, affixes[3].code);
        writer.write(Column::c_affixes3_par, affixes[3].par);
        writer.write(Column::c_affixes3_min, affixes[3].min);
        writer.write(Column::c_affixes3_max, affixes[3].max);
    }
    if (affixes.size() > 4)
    {
        writer.write(Column::c_affixes4_code, affixes[4].code);
        writer.write(Column::c_affixes4_par, affixes[4].par);
        writer.write(Column::c_affixes4_min, affixes[4].min);
        writer.write(Column::c_affixes4_max, affixes[4].max);
    }
    if (affixes.size() > 5)
    {
        writer.write(Column::c_affixes5_code, affixes[5].code);
        writer.write(Column::c_affixes5_par, affixes[5].par);
        writer.write(Column::c_affixes5_min, affixes[5].min);
        writer.write(Column::c_affixes5_max, affixes[5].max);
    }
    if (affixes.size() > 6)
    {
        writer.write(Column::c_affixes6_code, affixes[6].code);
        writer.write(Column::c_affixes6_par, affixes[6].par);
        writer.write(Column::c_affixes6_min, affixes[6].min);
        writer.write(Column::c_affixes6_max, affixes[6].max);
    }
    if (affixes.size() > 7)
    {
        writer.write(Column::c_affixes7_code, affixes[7].code);
        writer.write(Column::c_affixes7_par, affixes[7].par);
        writer.write(Column::c_affixes7_min, affixes[7].min);
        writer.write(Column::c_affixes7_max, affixes[7].max);
    }
    if (affixes.size() > 8)
    {
        writer.write(Column::c_affixes8_code, affixes[8].code);
        writer.write(Column::c_affixes8_par, affixes[8].par);
        writer.write(Column::c_affixes8_min, affixes[8].min);
        writer.write(Column::c_affixes8_max, affixes[8].max);
    }
    if (affixes.size() > 9)
    {
        writer.write(Column::c_affixes9_code, affixes[9].code);
        writer.write(Column::c_affixes9_par, affixes[9].par);
        writer.write(Column::c_affixes9_min, affixes[9].min);
        writer.write(Column::c_affixes9_max, affixes[9].max);
    }
    if (affixes.size() > 10)
    {
        writer.write(Column::c_affixes10_code, affixes[10].code);
        writer.write(Column::c_affixes10_par, affixes[10].par);
        writer.write(Column::c_affixes10_min, affixes[10].min);
        writer.write(Column::c_affixes10_max, affixes[10].max);
    }
    if (affixes.size() > 11)
    {
        writer.write(Column::c_affixes11_code, affixes[11].code);
        writer.write(Column::c_affixes11_par, affixes[11].par);
        writer.write(Column::c_affixes11_min, affixes[11].min);
        writer.write(Column::c_affixes11_max, affixes[11].max);
    }
}

enum DataRow_setitems_codegen::Column : std::size_t {
    c_index,
    c_eqipmentCode,
    c_rarity,
    c_lvl,
    c_lvlreq,
    c_spawnable,
    c_disableChronicle,
    c_cost_mul,
    c_cost_add,
    c_id,
    c_itemName,
    c_chrtransform,
    c_invtransform,
    c_flippyfile,
    c_invfile,
    c_dropsound,
    c_dropsfxframe,
    c_usesound,
    c_eol,
    c_addfunc,
    c_setIndex,
    c_affixes_base0_code,
    c_affixes_base0_par,
    c_affixes_base0_min,
    c_affixes_base0_max,
    c_affixes_base1_code,
    c_affixes_base1_par,
    c_affixes_base1_min,
    c_affixes_base1_max,
    c_affixes_base2_code,
    c_affixes_base2_par,
    c_affixes_base2_min,
    c_affixes_base2_max,
    c_affixes_base3_code,
    c_affixes_base3_par,
    c_affixes_base3_min,
    c_affixes_base3_max,
    c_affixes_base4_code,
    c_affixes_base4_par,
    c_affixes_base4_min,
    c_affixes_base4_max,
    c_affixes_base5_code,
    c_affixes_base5_par,
    c_affixes_base5_min,
    c_affixes_base5_max,
    c_affixes_base6_code,
    c_affixes_base6_par,
    c_affixes_base6_min,
    c_affixes_base6_max,
    c_affixes_base7_code,
    c_affixes_base7_par,
    c_affixes_base7_min,
    c_affixes_base7_max,
    c_affixes_base8_code,
    c_affixes_base8_par,
    c_affixes_base8_min,
    c_affixes_base8_max,
    c_affixes_partial10_code,
    c_affixes_partial10_par,
    c_affixes_partial10_min,
    c_affixes_partial10_max,
    c_affixes_partial11_code,
    c_affixes_partial11_par,
    c_affixes_partial11_min,
    c_affixes_partial11_max,
    c_affixes_partial12_code,
    c_affixes_partial12_par,
    c_affixes_partial12_min,
    c_affixes_partial12_max,
    c_affixes_partial13_code,
    c_affixes_partial13_par,
    c_affixes_partial13_min,
    c_affixes_partial13_max,
    c_affixes_partial14_code,
    c_affixes_partial14_par,
    c_affixes_partial14_min,
    c_affixes_partial14_max,
    c_affixes_partial20_code,
    c_affixes_partial20_par,
    c_affixes_partial20_min,
    c_affixes_partial20_max,
    c_affixes_partial21_code,
    c_affixes_partial21_par,
    c_affixes_partial21_min,
    c_affixes_partial21_max,
    c_affixes_partial22_code,
    c_affixes_partial22_par,
    c_affixes_partial22_min,
    c_affixes_partial22_max,
    c_affixes_partial23_code,
    c_affixes_partial23_par,
    c_affixes_partial23_min,
    c_affixes_partial23_max,
    c_affixes_partial24_code,
    c_affixes_partial24_par,
    c_affixes_partial24_min,
    c_affixes_partial24_max,
};

const GeneratedTableSchema& DataRow_setitems_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "index",
            "code",
            "rarity",
            "lvl",
            "lvl req",
            "spawnable",
            "disablechronicle",
            "cost mult",
            "cost add",
            "*id",
            "*itemname",
            "chrtransform",
            "invtransform",
            "flippyfile",
            "invfile",
            "dropsound",
            "dropsfxframe",
            "usesound",
            "*eol",
            "add func",
            "set",
            "prop1",
            "par1",
            "min1",
            "max1",
            "prop2",
            "par2",
            "min2",
            "max2",
            "prop3",
            "par3",
            "min3",
            "max3",
            "prop4",
            "par4",
            "min4",
            "max4",
            "prop5",
            "par5",
            "min5",
            "max5",
            "prop6",
            "par6",
            "min6",
            "max6",
            "prop7",
            "par7",
            "min7",
            "max7",
            "prop8",
            "par8",
            "min8",
            "max8",
            "prop9",
            "par9",
            "min9",
            "max9",
            "aprop1a",
            "apar1a",
            "amin1a",
            "amax1a",
            "aprop2a",
            "apar2a",
            "amin2a",
            "amax2a",
            "aprop3a",
            "apar3a",
            "amin3a",
            "amax3a",
            "aprop4a",
            "apar4a",
            "amin4a",
            "amax4a",
            "aprop5a",
            "apar5a",
            "amin5a",
            "amax5a",
            "aprop1b",
            "apar1b",
            "amin1b",
            "amax1b",
            "aprop2b",
            "apar2b",
            "amin2b",
            "amax2b",
            "aprop3b",
            "apar3b",
            "amin3b",
            "amax3b",
            "aprop4b",
            "apar4b",
            "amin4b",
            "amax4b",
            "aprop5b",
            "apar5b",
            "amin5b",
            "amax5b",
        },
        .aliases = {
            { "enabled", "spawnable" },
            { "level", "lvl" },
            { "item", "code" },
        },
    };
    return schema;
}

void DataRow_setitems_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_index, index);
    reader.read(Column::c_eqipmentCode, eqipmentCode);
    reader.read(Column::c_rarity, rarity);
    reader.read(Column::c_lvl, lvl);
    reader.read(Column::c_lvlreq, lvlreq);
    reader.read(Column::c_spawnable, spawnable, true);
    reader.read(Column::c_disableChronicle, disableChronicle);
    reader.read(Column::c_cost_mul, cost_mul);
    reader.read(Column::c_cost_add, cost_add);
    reader.read(Column::c_id, id);
    reader.read(Column::c_itemName, itemName);
    reader.read(Column::c_chrtransform, chrtransform);
    reader.read(Column::c_invtransform, invtransform);
    reader.read(Column::c_flippyfile, flippyfile);
    reader.read(Column::c_invfile, invfile);
    reader.read(Column::c_dropsound, dropsound);
    reader.read(Column::c_dropsfxframe, dropsfxframe);
    reader.read(Column::c_usesound, usesound);
    reader.read(Column::c_eol, eol);
    reader.read(Column::c_addfunc, addfunc);
    reader.read(Column::c_setIndex, setIndex);
    affixes_base_limit = 0;
    if (reader.available(Column::c_affixes_base0_code) || reader.available(Column::c_affixes_base0_par) || reader.available(Column::c_affixes_base0_min) || reader.available(Column::c_affixes_base0_max))
        affixes_base_limit = 1;
    if (reader.available(Column::c_affixes_base1_code) || reader.available(Column::c_affixes_base1_par) || reader.available(Column::c_affixes_base1_min) || reader.available(Column::c_affixes_base1_max))
        affixes_base_limit = 2;
    if (reader.available(Column::c_affixes_base2_code) || reader.available(Column::c_affixes_base2_par) || reader.available(Column::c_affixes_base2_min) || reader.available(Column::c_affixes_base2_max))
        affixes_base_limit = 3;
    if (reader.available(Column::c_affixes_base3_code) || reader.available(Column::c_affixes_base3_par) || reader.available(Column::c_affixes_base3_min) || reader.available(Column::c_affixes_base3_max))
        affixes_base_limit = 4;
    if (reader.available(Column::c_affixes_base4_code) || reader.available(Column::c_affixes_base4_par) || reader.available(Column::c_affixes_base4_min) || reader.available(Column::c_affixes_base4_max))
        affixes_base_limit = 5;
    if (reader.available(Column::c_affixes_base5_code) || reader.available(Column::c_affixes_base5_par) || reader.available(Column::c_affixes_base5_min) || reader.available(Column::c_affixes_base5_max))
        affixes_base_limit = 6;
    if (reader.available(Column::c_affixes_base6_code) || reader.available(Column::c_affixes_base6_par) || reader.available(Column::c_affixes_base6_min) || reader.available(Column::c_affixes_base6_max))
        affixes_base_limit = 7;
    if (reader.available(Column::c_affixes_base7_code) || reader.available(Column::c_affixes_base7_par) || reader.available(Column::c_affixes_base7_min) || reader.available(Column::c_affixes_base7_max))
        affixes_base_limit = 8;
    if (reader.available(Column::c_affixes_base8_code) || reader.available(Column::c_affixes_base8_par) || reader.available(Column::c_affixes_base8_min) || reader.available(Column::c_affixes_base8_max))
        affixes_base_limit = 9;
    affixes_base.clear();
    affixes_base.reserve(9);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base0_code, item.code);
        reader.read(Column::c_affixes_base0_par, item.par);
        reader.read(Column::c_affixes_base0_min, item.min);
        reader.read(Column::c_affixes_base0_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base1_code, item.code);
        reader.read(Column::c_affixes_base1_par, item.par);
        reader.read(Column::c_affixes_base1_min, item.min);
        reader.read(Column::c_affixes_base1_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base2_code, item.code);
        reader.read(Column::c_affixes_base2_par, item.par);
        reader.read(Column::c_affixes_base2_min, item.min);
        reader.read(Column::c_affixes_base2_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base3_code, item.code);
        reader.read(Column::c_affixes_base3_par, item.par);
        reader.read(Column::c_affixes_base3_min, item.min);
        reader.read(Column::c_affixes_base3_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base4_code, item.code);
        reader.read(Column::c_affixes_base4_par, item.par);
        reader.read(Column::c_affixes_base4_min, item.min);
        reader.read(Column::c_affixes_base4_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base5_code, item.code);
        reader.read(Column::c_affixes_base5_par, item.par);
        reader.read(Column::c_affixes_base5_min, item.min);
        reader.read(Column::c_affixes_base5_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base6_code, item.code);
        reader.read(Column::c_affixes_base6_par, item.par);
        reader.read(Column::c_affixes_base6_min, item.min);
        reader.read(Column::c_affixes_base6_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base7_code, item.code);
        reader.read(Column::c_affixes_base7_par, item.par);
        reader.read(Column::c_affixes_base7_min, item.min);
        reader.read(Column::c_affixes_base7_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_base8_code, item.code);
        reader.read(Column::c_affixes_base8_par, item.par);
        reader.read(Column::c_affixes_base8_min, item.min);
        reader.read(Column::c_affixes_base8_max, item.max);
        if (!item.code.empty())
            affixes_base.push_back(item);
    }
    affixes_partial1_limit = 0;
    if (reader.available(Column::c_affixes_partial10_code) || reader.available(Column::c_affixes_partial10_par) || reader.available(Column::c_affixes_partial10_min) || reader.available(Column::c_affixes_partial10_max))
        affixes_partial1_limit = 1;
    if (reader.available(Column::c_affixes_partial11_code) || reader.available(Column::c_affixes_partial11_par) || reader.available(Column::c_affixes_partial11_min) || reader.available(Column::c_affixes_partial11_max))
        affixes_partial1_limit = 2;
    if (reader.available(Column::c_affixes_partial12_code) || reader.available(Column::c_affixes_partial12_par) || reader.available(Column::c_affixes_partial12_min) || reader.available(Column::c_affixes_partial12_max))
        affixes_partial1_limit = 3;
    if (reader.available(Column::c_affixes_partial13_code) || reader.available(Column::c_affixes_partial13_par) || reader.available(Column::c_affixes_partial13_min) || reader.available(Column::c_affixes_partial13_max))
        affixes_partial1_limit = 4;
    if (reader.available(Column::c_affixes_partial14_code) || reader.available(Column::c_affixes_partial14_par) || reader.available(Column::c_affixes_partial14_min) || reader.available(Column::c_affixes_partial14_max))
        affixes_partial1_limit = 5;
    affixes_partial1.clear();
    affixes_partial1.reserve(5);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial10_code, item.code);
        reader.read(Column::c_affixes_partial10_par, item.par);
        reader.read(Column::c_affixes_partial10_min, item.min);
        reader.read(Column::c_affixes_partial10_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial11_code, item.code);
        reader.read(Column::c_affixes_partial11_par, item.par);
        reader.read(Column::c_affixes_partial11_min, item.min);
        reader.read(Column::c_affixes_partial11_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial12_code, item.code);
        reader.read(Column::c_affixes_partial12_par, item.par);
        reader.read(Column::c_affixes_partial12_min, item.min);
        reader.read(Column::c_affixes_partial12_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial13_code, item.code);
        reader.read(Column::c_affixes_partial13_par, item.par);
        reader.read(Column::c_affixes_partial13_min, item.min);
        reader.read(Column::c_affixes_partial13_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial14_code, item.code);
        reader.read(Column::c_affixes_partial14_par, item.par);
        reader.read(Column::c_affixes_partial14_min, item.min);
        reader.read(Column::c_affixes_partial14_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    affixes_partial2_limit = 0;
    if (reader.available(Column::c_affixes_partial20_code) || reader.available(Column::c_affixes_partial20_par) || reader.available(Column::c_affixes_partial20_min) || reader.available(Column::c_affixes_partial20_max))
        affixes_partial2_limit = 1;
    if (reader.available(Column::c_affixes_partial21_code) || reader.available(Column::c_affixes_partial21_par) || reader.available(Column::c_affixes_partial21_min) || reader.available(Column::c_affixes_partial21_max))
        affixes_partial2_limit = 2;
    if (reader.available(Column::c_affixes_partial22_code) || reader.available(Column::c_affixes_partial22_par) || reader.available(Column::c_affixes_partial22_min) || reader.available(Column::c_affixes_partial22_max))
        affixes_partial2_limit = 3;
    if (reader.available(Column::c_affixes_partial23_code) || reader.available(Column::c_affixes_partial23_par) || reader.available(Column::c_affixes_partial23_min) || reader.available(Column::c_affixes_partial23_max))
        affixes_partial2_limit = 4;
    if (reader.available(Column::c_affixes_partial24_code) || reader.available(Column::c_affixes_partial24_par) || reader.available(Column::c_affixes_partial24_min) || reader.available(Column::c_affixes_partial24_max))
        affixes_partial2_limit = 5;
    affixes_partial2.clear();
    affixes_partial2.reserve(5);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial20_code, item.code);
        reader.read(Column::c_affixes_partial20_par, item.par);
        reader.read(Column::c_affixes_partial20_min, item.min);
        reader.read(Column::c_affixes_partial20_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial21_code, item.code);
        reader.read(Column::c_affixes_partial21_par, item.par);
        reader.read(Column::c_affixes_partial21_min, item.min);
        reader.read(Column::c_affixes_partial21_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial22_code, item.code);
        reader.read(Column::c_affixes_partial22_par, item.par);
        reader.read(Column::c_affixes_partial22_min, item.min);
        reader.read(Column::c_affixes_partial22_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial23_code, item.code);
        reader.read(Column::c_affixes_partial23_par, item.par);
        reader.read(Column::c_affixes_partial23_min, item.min);
        reader.read(Column::c_affixes_partial23_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial24_code, item.code);
        reader.read(Column::c_affixes_partial24_par, item.par);
        reader.read(Column::c_affixes_partial24_min, item.min);
        reader.read(Column::c_affixes_partial24_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
}

void DataRow_setitems_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_index, index);
    writer.write(Column::c_eqipmentCode, eqipmentCode);
    writer.write(Column::c_rarity, rarity);
    writer.write(Column::c_lvl, lvl);
    writer.write(Column::c_lvlreq, lvlreq);
    writer.write(Column::c_spawnable, spawnable);
    writer.write(Column::c_disableChronicle, disableChronicle);
    writer.write(Column::c_cost_mul, cost_mul);
    writer.write(Column::c_cost_add, cost_add);
    writer.write(Column::c_id, id);
    writer.write(Column::c_itemName, itemName);
    writer.write(Column::c_chrtransform, chrtransform);
    writer.write(Column::c_invtransform, invtransform);
    writer.write(Column::c_flippyfile, flippyfile);
    writer.write(Column::c_invfile, invfile);
    writer.write(Column::c_dropsound, dropsound);
    writer.write(Column::c_dropsfxframe, dropsfxframe);
    writer.write(Column::c_usesound, usesound);
    writer.write(Column::c_eol, eol);
    writer.write(Column::c_addfunc, addfunc);
    writer.write(Column::c_setIndex, setIndex);
    if (affixes_base.size() > 0)
    {
        writer.write(Column::c_affixes_base0_code, affixes_base[0].code);
        writer.write(Column::c_affixes_base0_par, affixes_base[0].par);
        writer.write(Column::c_affixes_base0_min, affixes_base[0].min);
        writer.write(Column::c_affixes_base0_max, affixes_base[0].max);
    }
    if (affixes_base.size() > 1)
    {
        writer.write(Column::c_affixes_base1_code, affixes_base[1].code);
        writer.write(Column::c_affixes_base1_par, affixes_base[1].par);
        writer.write(Column::c_affixes_base1_min, affixes_base[1].min);
        writer.write(Column::c_affixes_base1_max, affixes_base[1].max);
    }
    if (affixes_base.size() > 2)
    {
        writer.write(Column::c_affixes_base2_code, affixes_base[2].code);
        writer.write(Column::c_affixes_base2_par, affixes_base[2].par);
        writer.write(Column::c_affixes_base2_min, affixes_base[2].min);
        writer.write(Column::c_affixes_base2_max, affixes_base[2].max);
    }
    if (affixes_base.size() > 3)
    {
        writer.write(Column::c_affixes_base3_code, affixes_base[3].code);
        writer.write(Column::c_affixes_base3_par, affixes_base[3].par);
        writer.write(Column::c_affixes_base3_min, affixes_base[3].min);
        writer.write(Column::c_affixes_base3_max, affixes_base[3].max);
    }
    if (affixes_base.size() > 4)
    {
        writer.write(Column::c_affixes_base4_code, affixes_base[4].code);
        writer.write(Column::c_affixes_base4_par, affixes_base[4].par);
        writer.write(Column::c_affixes_base4_min, affixes_base[4].min);
        writer.write(Column::c_affixes_base4_max, affixes_base[4].max);
    }
    if (affixes_base.size() > 5)
    {
        writer.write(Column::c_affixes_base5_code, affixes_base[5].code);
        writer.write(Column::c_affixes_base5_par, affixes_base[5].par);
        writer.write(Column::c_affixes_base5_min, affixes_base[5].min);
        writer.write(Column::c_affixes_base5_max, affixes_base[5].max);
    }
    if (affixes_base.size() > 6)
    {
        writer.write(Column::c_affixes_base6_code, affixes_base[6].code);
        writer.write(Column::c_affixes_base6_par, affixes_base[6].par);
        writer.write(Column::c_affixes_base6_min, affixes_base[6].min);
        writer.write(Column::c_affixes_base6_max, affixes_base[6].max);
    }
    if (affixes_base.size() > 7)
    {
        writer.write(Column::c_affixes_base7_code, affixes_base[7].code);
        writer.write(Column::c_affixes_base7_par, affixes_base[7].par);
        writer.write(Column::c_affixes_base7_min, affixes_base[7].min);
        writer.write(Column::c_affixes_base7_max, affixes_base[7].max);
    }
    if (affixes_base.size() > 8)
    {
        writer.write(Column::c_affixes_base8_code, affixes_base[8].code);
        writer.write(Column::c_affixes_base8_par, affixes_base[8].par);
        writer.write(Column::c_affixes_base8_min, affixes_base[8].min);
        writer.write(Column::c_affixes_base8_max, affixes_base[8].max);
    }
    if (affixes_partial1.size() > 0)
    {
        writer.write(Column::c_affixes_partial10_code, affixes_partial1[0].code);
        writer.write(Column::c_affixes_partial10_par, affixes_partial1[0].par);
        writer.write(Column::c_affixes_partial10_min, affixes_partial1[0].min);
        writer.write(Column::c_affixes_partial10_max, affixes_partial1[0].max);
    }
    if (affixes_partial1.size() > 1)
    {
        writer.write(Column::c_affixes_partial11_code, affixes_partial1[1].code);
        writer.write(Column::c_affixes_partial11_par, affixes_partial1[1].par);
        writer.write(Column::c_affixes_partial11_min, affixes_partial1[1].min);
        writer.write(Column::c_affixes_partial11_max, affixes_partial1[1].max);
    }
    if (affixes_partial1.size() > 2)
    {
        writer.write(Column::c_affixes_partial12_code, affixes_partial1[2].code);
        writer.write(Column::c_affixes_partial12_par, affixes_partial1[2].par);
        writer.write(Column::c_affixes_partial12_min, affixes_partial1[2].min);
        writer.write(Column::c_affixes_partial12_max, affixes_partial1[2].max);
    }
    if (affixes_partial1.size() > 3)
    {
        writer.write(Column::c_affixes_partial13_code, affixes_partial1[3].code);
        writer.write(Column::c_affixes_partial13_par, affixes_partial1[3].par);
        writer.write(Column::c_affixes_partial13_min, affixes_partial1[3].min);
        writer.write(Column::c_affixes_partial13_max, affixes_partial1[3].max);
    }
    if (affixes_partial1.size() > 4)
    {
        writer.write(Column::c_affixes_partial14_code, affixes_partial1[4].code);
        writer.write(Column::c_affixes_partial14_par, affixes_partial1[4].par);
        writer.write(Column::c_affixes_partial14_min, affixes_partial1[4].min);
        writer.write(Column::c_affixes_partial14_max, affixes_partial1[4].max);
    }
    if (affixes_partial2.size() > 0)
    {
        writer.write(Column::c_affixes_partial20_code, affixes_partial2[0].code);
        writer.write(Column::c_affixes_partial20_par, affixes_partial2[0].par);
        writer.write(Column::c_affixes_partial20_min, affixes_partial2[0].min);
        writer.write(Column::c_affixes_partial20_max, affixes_partial2[0].max);
    }
    if (affixes_partial2.size() > 1)
    {
        writer.write(Column::c_affixes_partial21_code, affixes_partial2[1].code);
        writer.write(Column::c_affixes_partial21_par, affixes_partial2[1].par);
        writer.write(Column::c_affixes_partial21_min, affixes_partial2[1].min);
        writer.write(Column::c_affixes_partial21_max, affixes_partial2[1].max);
    }
    if (affixes_partial2.size() > 2)
    {
        writer.write(Column::c_affixes_partial22_code, affixes_partial2[2].code);
        writer.write(Column::c_affixes_partial22_par, affixes_partial2[2].par);
        writer.write(Column::c_affixes_partial22_min, affixes_partial2[2].min);
        writer.write(Column::c_affixes_partial22_max, affixes_partial2[2].max);
    }
    if (affixes_partial2.size() > 3)
    {
        writer.write(Column::c_affixes_partial23_code, affixes_partial2[3].code);
        writer.write(Column::c_affixes_partial23_par, affixes_partial2[3].par);
        writer.write(Column::c_affixes_partial23_min, affixes_partial2[3].min);
        writer.write(Column::c_affixes_partial23_max, affixes_partial2[3].max);
    }
    if (affixes_partial2.size() > 4)
    {
        writer.write(Column::c_affixes_partial24_code, affixes_partial2[4].code);
        writer.write(Column::c_affixes_partial24_par, affixes_partial2[4].par);
        writer.write(Column::c_affixes_partial24_min, affixes_partial2[4].min);
        writer.write(Column::c_affixes_partial24_max, affixes_partial2[4].max);
    }
}

enum DataRow_sets_codegen::Column : std::size_t {
    c_index,
    c_name,
    c_uiClass,
    c_version,
    c_affixes_full0_code,
    c_affixes_full0_par,
    c_affixes_full0_min,
    c_affixes_full0_max,
    c_affixes_full1_code,
    c_affixes_full1_par,
    c_affixes_full1_min,
    c_affixes_full1_max,
    c_affixes_full2_code,
    c_affixes_full2_par,
    c_affixes_full2_min,
    c_affixes_full2_max,
    c_affixes_full3_code,
    c_affixes_full3_par,
    c_affixes_full3_min,
    c_affixes_full3_max,
    c_affixes_full4_code,
    c_affixes_full4_par,
    c_affixes_full4_min,
    c_affixes_full4_max,
    c_affixes_full5_code,
    c_affixes_full5_par,
    c_affixes_full5_min,
    c_affixes_full5_max,
    c_affixes_full6_code,
    c_affixes_full6_par,
    c_affixes_full6_min,
    c_affixes_full6_max,
    c_affixes_full7_code,
    c_affixes_full7_par,
    c_affixes_full7_min,
    c_affixes_full7_max,
    c_affixes_partial10_code,
    c_affixes_partial10_par,
    c_affixes_partial10_min,
    c_affixes_partial10_max,
    c_affixes_partial11_code,
    c_affixes_partial11_par,
    c_affixes_partial11_min,
    c_affixes_partial11_max,
    c_affixes_partial12_code,
    c_affixes_partial12_par,
    c_affixes_partial12_min,
    c_affixes_partial12_max,
    c_affixes_partial13_code,
    c_affixes_partial13_par,
    c_affixes_partial13_min,
    c_affixes_partial13_max,
    c_affixes_partial20_code,
    c_affixes_partial20_par,
    c_affixes_partial20_min,
    c_affixes_partial20_max,
    c_affixes_partial21_code,
    c_affixes_partial21_par,
    c_affixes_partial21_min,
    c_affixes_partial21_max,
    c_affixes_partial22_code,
    c_affixes_partial22_par,
    c_affixes_partial22_min,
    c_affixes_partial22_max,
    c_affixes_partial23_code,
    c_affixes_partial23_par,
    c_affixes_partial23_min,
    c_affixes_partial23_max,
};

const GeneratedTableSchema& DataRow_sets_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "index",
            "name",
            "uiclass",
            "version",
            "fcode1",
            "fparam1",
            "fmin1",
            "fmax1",
            "fcode2",
            "fparam2",
            "fmin2",
            "fmax2",
            "fcode3",
            "fparam3",
            "fmin3",
            "fmax3",
            "fcode4",
            "fparam4",
            "fmin4",
            "fmax4",
            "fcode5",
            "fparam5",
            "fmin5",
            "fmax5",
            "fcode6",
            "fparam6",
            "fmin6",
            "fmax6",
            "fcode7",
            "fparam7",
            "fmin7",
            "fmax7",
            "fcode8",
            "fparam8",
            "fmin8",
            "fmax8",
            "pcode2a",
            "pparam2a",
            "pmin2a",
            "pmax2a",
            "pcode3a",
            "pparam3a",
            "pmin3a",
            "pmax3a",
            "pcode4a",
            "pparam4a",
            "pmin4a",
            "pmax4a",
            "pcode5a",
            "pparam5a",
            "pmin5a",
            "pmax5a",
            "pcode2b",
            "pparam2b",
            "pmin2b",
            "pmax2b",
            "pcode3b",
            "pparam3b",
            "pmin3b",
            "pmax3b",
            "pcode4b",
            "pparam4b",
            "pmin4b",
            "pmax4b",
            "pcode5b",
            "pparam5b",
            "pmin5b",
            "pmax5b",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_sets_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_index, index);
    reader.read(Column::c_name, name);
    reader.read(Column::c_uiClass, uiClass);
    reader.read(Column::c_version, version);
    affixes_full_limit = 0;
    if (reader.available(Column::c_affixes_full0_code) || reader.available(Column::c_affixes_full0_par) || reader.available(Column::c_affixes_full0_min) || reader.available(Column::c_affixes_full0_max))
        affixes_full_limit = 1;
    if (reader.available(Column::c_affixes_full1_code) || reader.available(Column::c_affixes_full1_par) || reader.available(Column::c_affixes_full1_min) || reader.available(Column::c_affixes_full1_max))
        affixes_full_limit = 2;
    if (reader.available(Column::c_affixes_full2_code) || reader.available(Column::c_affixes_full2_par) || reader.available(Column::c_affixes_full2_min) || reader.available(Column::c_affixes_full2_max))
        affixes_full_limit = 3;
    if (reader.available(Column::c_affixes_full3_code) || reader.available(Column::c_affixes_full3_par) || reader.available(Column::c_affixes_full3_min) || reader.available(Column::c_affixes_full3_max))
        affixes_full_limit = 4;
    if (reader.available(Column::c_affixes_full4_code) || reader.available(Column::c_affixes_full4_par) || reader.available(Column::c_affixes_full4_min) || reader.available(Column::c_affixes_full4_max))
        affixes_full_limit = 5;
    if (reader.available(Column::c_affixes_full5_code) || reader.available(Column::c_affixes_full5_par) || reader.available(Column::c_affixes_full5_min) || reader.available(Column::c_affixes_full5_max))
        affixes_full_limit = 6;
    if (reader.available(Column::c_affixes_full6_code) || reader.available(Column::c_affixes_full6_par) || reader.available(Column::c_affixes_full6_min) || reader.available(Column::c_affixes_full6_max))
        affixes_full_limit = 7;
    if (reader.available(Column::c_affixes_full7_code) || reader.available(Column::c_affixes_full7_par) || reader.available(Column::c_affixes_full7_min) || reader.available(Column::c_affixes_full7_max))
        affixes_full_limit = 8;
    affixes_full.clear();
    affixes_full.reserve(8);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full0_code, item.code);
        reader.read(Column::c_affixes_full0_par, item.par);
        reader.read(Column::c_affixes_full0_min, item.min);
        reader.read(Column::c_affixes_full0_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full1_code, item.code);
        reader.read(Column::c_affixes_full1_par, item.par);
        reader.read(Column::c_affixes_full1_min, item.min);
        reader.read(Column::c_affixes_full1_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full2_code, item.code);
        reader.read(Column::c_affixes_full2_par, item.par);
        reader.read(Column::c_affixes_full2_min, item.min);
        reader.read(Column::c_affixes_full2_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full3_code, item.code);
        reader.read(Column::c_affixes_full3_par, item.par);
        reader.read(Column::c_affixes_full3_min, item.min);
        reader.read(Column::c_affixes_full3_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full4_code, item.code);
        reader.read(Column::c_affixes_full4_par, item.par);
        reader.read(Column::c_affixes_full4_min, item.min);
        reader.read(Column::c_affixes_full4_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full5_code, item.code);
        reader.read(Column::c_affixes_full5_par, item.par);
        reader.read(Column::c_affixes_full5_min, item.min);
        reader.read(Column::c_affixes_full5_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full6_code, item.code);
        reader.read(Column::c_affixes_full6_par, item.par);
        reader.read(Column::c_affixes_full6_min, item.min);
        reader.read(Column::c_affixes_full6_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_full7_code, item.code);
        reader.read(Column::c_affixes_full7_par, item.par);
        reader.read(Column::c_affixes_full7_min, item.min);
        reader.read(Column::c_affixes_full7_max, item.max);
        if (!item.code.empty())
            affixes_full.push_back(item);
    }
    affixes_partial1_limit = 0;
    if (reader.available(Column::c_affixes_partial10_code) || reader.available(Column::c_affixes_partial10_par) || reader.available(Column::c_affixes_partial10_min) || reader.available(Column::c_affixes_partial10_max))
        affixes_partial1_limit = 1;
    if (reader.available(Column::c_affixes_partial11_code) || reader.available(Column::c_affixes_partial11_par) || reader.available(Column::c_affixes_partial11_min) || reader.available(Column::c_affixes_partial11_max))
        affixes_partial1_limit = 2;
    if (reader.available(Column::c_affixes_partial12_code) || reader.available(Column::c_affixes_partial12_par) || reader.available(Column::c_affixes_partial12_min) || reader.available(Column::c_affixes_partial12_max))
        affixes_partial1_limit = 3;
    if (reader.available(Column::c_affixes_partial13_code) || reader.available(Column::c_affixes_partial13_par) || reader.available(Column::c_affixes_partial13_min) || reader.available(Column::c_affixes_partial13_max))
        affixes_partial1_limit = 4;
    affixes_partial1.clear();
    affixes_partial1.reserve(4);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial10_code, item.code);
        reader.read(Column::c_affixes_partial10_par, item.par);
        reader.read(Column::c_affixes_partial10_min, item.min);
        reader.read(Column::c_affixes_partial10_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial11_code, item.code);
        reader.read(Column::c_affixes_partial11_par, item.par);
        reader.read(Column::c_affixes_partial11_min, item.min);
        reader.read(Column::c_affixes_partial11_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial12_code, item.code);
        reader.read(Column::c_affixes_partial12_par, item.par);
        reader.read(Column::c_affixes_partial12_min, item.min);
        reader.read(Column::c_affixes_partial12_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial13_code, item.code);
        reader.read(Column::c_affixes_partial13_par, item.par);
        reader.read(Column::c_affixes_partial13_min, item.min);
        reader.read(Column::c_affixes_partial13_max, item.max);
        if (!item.code.empty())
            affixes_partial1.push_back(item);
    }
    affixes_partial2_limit = 0;
    if (reader.available(Column::c_affixes_partial20_code) || reader.available(Column::c_affixes_partial20_par) || reader.available(Column::c_affixes_partial20_min) || reader.available(Column::c_affixes_partial20_max))
        affixes_partial2_limit = 1;
    if (reader.available(Column::c_affixes_partial21_code) || reader.available(Column::c_affixes_partial21_par) || reader.available(Column::c_affixes_partial21_min) || reader.available(Column::c_affixes_partial21_max))
        affixes_partial2_limit = 2;
    if (reader.available(Column::c_affixes_partial22_code) || reader.available(Column::c_affixes_partial22_par) || reader.available(Column::c_affixes_partial22_min) || reader.available(Column::c_affixes_partial22_max))
        affixes_partial2_limit = 3;
    if (reader.available(Column::c_affixes_partial23_code) || reader.available(Column::c_affixes_partial23_par) || reader.available(Column::c_affixes_partial23_min) || reader.available(Column::c_affixes_partial23_max))
        affixes_partial2_limit = 4;
    affixes_partial2.clear();
    affixes_partial2.reserve(4);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial20_code, item.code);
        reader.read(Column::c_affixes_partial20_par, item.par);
        reader.read(Column::c_affixes_partial20_min, item.min);
        reader.read(Column::c_affixes_partial20_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial21_code, item.code);
        reader.read(Column::c_affixes_partial21_par, item.par);
        reader.read(Column::c_affixes_partial21_min, item.min);
        reader.read(Column::c_affixes_partial21_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial22_code, item.code);
        reader.read(Column::c_affixes_partial22_par, item.par);
        reader.read(Column::c_affixes_partial22_min, item.min);
        reader.read(Column::c_affixes_partial22_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_partial23_code, item.code);
        reader.read(Column::c_affixes_partial23_par, item.par);
        reader.read(Column::c_affixes_partial23_min, item.min);
        reader.read(Column::c_affixes_partial23_max, item.max);
        if (!item.code.empty())
            affixes_partial2.push_back(item);
    }
}

void DataRow_sets_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_index, index);
    writer.write(Column::c_name, name);
    writer.write(Column::c_uiClass, uiClass);
    writer.write(Column::c_version, version);
    if (affixes_full.size() > 0)
    {
        writer.write(Column::c_affixes_full0_code, affixes_full[0].code);
        writer.write(Column::c_affixes_full0_par, affixes_full[0].par);
        writer.write(Column::c_affixes_full0_min, affixes_full[0].min);
        writer.write(Column::c_affixes_full0_max, affixes_full[0].max);
    }
    if (affixes_full.size() > 1)
    {
        writer.write(Column::c_affixes_full1_code, affixes_full[1].code);
        writer.write(Column::c_affixes_full1_par, affixes_full[1].par);
        writer.write(Column::c_affixes_full1_min, affixes_full[1].min);
        writer.write(Column::c_affixes_full1_max, affixes_full[1].max);
    }
    if (affixes_full.size() > 2)
    {
        writer.write(Column::c_affixes_full2_code, affixes_full[2].code);
        writer.write(Column::c_affixes_full2_par, affixes_full[2].par);
        writer.write(Column::c_affixes_full2_min, affixes_full[2].min);
        writer.write(Column::c_affixes_full2_max, affixes_full[2].max);
    }
    if (affixes_full.size() > 3)
    {
        writer.write(Column::c_affixes_full3_code, affixes_full[3].code);
        writer.write(Column::c_affixes_full3_par, affixes_full[3].par);
        writer.write(Column::c_affixes_full3_min, affixes_full[3].min);
        writer.write(Column::c_affixes_full3_max, affixes_full[3].max);
    }
    if (affixes_full.size() > 4)
    {
        writer.write(Column::c_affixes_full4_code, affixes_full[4].code);
        writer.write(Column::c_affixes_full4_par, affixes_full[4].par);
        writer.write(Column::c_affixes_full4_min, affixes_full[4].min);
        writer.write(Column::c_affixes_full4_max, affixes_full[4].max);
    }
    if (affixes_full.size() > 5)
    {
        writer.write(Column::c_affixes_full5_code, affixes_full[5].code);
        writer.write(Column::c_affixes_full5_par, affixes_full[5].par);
        writer.write(Column::c_affixes_full5_min, affixes_full[5].min);
        writer.write(Column::c_affixes_full5_max, affixes_full[5].max);
    }
    if (affixes_full.size() > 6)
    {
        writer.write(Column::c_affixes_full6_code, affixes_full[6].code);
        writer.write(Column::c_affixes_full6_par, affixes_full[6].par);
        writer.write(Column::c_affixes_full6_min, affixes_full[6].min);
        writer.write(Column::c_affixes_full6_max, affixes_full[6].max);
    }
    if (affixes_full.size() > 7)
    {
        writer.write(Column::c_affixes_full7_code, affixes_full[7].code);
        writer.write(Column::c_affixes_full7_par, affixes_full[7].par);
        writer.write(Column::c_affixes_full7_min, affixes_full[7].min);
        writer.write(Column::c_affixes_full7_max, affixes_full[7].max);
    }
    if (affixes_partial1.size() > 0)
    {
        writer.write(Column::c_affixes_partial10_code, affixes_partial1[0].code);
        writer.write(Column::c_affixes_partial10_par, affixes_partial1[0].par);
        writer.write(Column::c_affixes_partial10_min, affixes_partial1[0].min);
        writer.write(Column::c_affixes_partial10_max, affixes_partial1[0].max);
    }
    if (affixes_partial1.size() > 1)
    {
        writer.write(Column::c_affixes_partial11_code, affixes_partial1[1].code);
        writer.write(Column::c_affixes_partial11_par, affixes_partial1[1].par);
        writer.write(Column::c_affixes_partial11_min, affixes_partial1[1].min);
        writer.write(Column::c_affixes_partial11_max, affixes_partial1[1].max);
    }
    if (affixes_partial1.size() > 2)
    {
        writer.write(Column::c_affixes_partial12_code, affixes_partial1[2].code);
        writer.write(Column::c_affixes_partial12_par, affixes_partial1[2].par);
        writer.write(Column::c_affixes_partial12_min, affixes_partial1[2].min);
        writer.write(Column::c_affixes_partial12_max, affixes_partial1[2].max);
    }
    if (affixes_partial1.size() > 3)
    {
        writer.write(Column::c_affixes_partial13_code, affixes_partial1[3].code);
        writer.write(Column::c_affixes_partial13_par, affixes_partial1[3].par);
        writer.write(Column::c_affixes_partial13_min, affixes_partial1[3].min);
        writer.write(Column::c_affixes_partial13_max, affixes_partial1[3].max);
    }
    if (affixes_partial2.size() > 0)
    {
        writer.write(Column::c_affixes_partial20_code, affixes_partial2[0].code);
        writer.write(Column::c_affixes_partial20_par, affixes_partial2[0].par);
        writer.write(Column::c_affixes_partial20_min, affixes_partial2[0].min);
        writer.write(Column::c_affixes_partial20_max, affixes_partial2[0].max);
    }
    if (affixes_partial2.size() > 1)
    {
        writer.write(Column::c_affixes_partial21_code, affixes_partial2[1].code);
        writer.write(Column::c_affixes_partial21_par, affixes_partial2[1].par);
        writer.write(Column::c_affixes_partial21_min, affixes_partial2[1].min);
        writer.write(Column::c_affixes_partial21_max, affixes_partial2[1].max);
    }
    if (affixes_partial2.size() > 2)
    {
        writer.write(Column::c_affixes_partial22_code, affixes_partial2[2].code);
        writer.write(Column::c_affixes_partial22_par, affixes_partial2[2].par);
        writer.write(Column::c_affixes_partial22_min, affixes_partial2[2].min);
        writer.write(Column::c_affixes_partial22_max, affixes_partial2[2].max);
    }
    if (affixes_partial2.size() > 3)
    {
        writer.write(Column::c_affixes_partial23_code, affixes_partial2[3].code);
        writer.write(Column::c_affixes_partial23_par, affixes_partial2[3].par);
        writer.write(Column::c_affixes_partial23_min, affixes_partial2[3].min);
        writer.write(Column::c_affixes_partial23_max, affixes_partial2[3].max);
    }
}

enum DataRow_runes_codegen::Column : std::size_t {
    c_complete,
    c_name,
    c_affixes0_code,
    c_affixes0_par,
    c_affixes0_min,
    c_affixes0_max,
    c_affixes1_code,
    c_affixes1_par,
    c_affixes1_min,
    c_affixes1_max,
    c_affixes2_code,
    c_affixes2_par,
    c_affixes2_min,
    c_affixes2_max,
    c_affixes3_code,
    c_affixes3_par,
    c_affixes3_min,
    c_affixes3_max,
    c_affixes4_code,
    c_affixes4_par,
    c_affixes4_min,
    c_affixes4_max,
    c_affixes5_code,
    c_affixes5_par,
    c_affixes5_min,
    c_affixes5_max,
    c_affixes6_code,
    c_affixes6_par,
    c_affixes6_min,
    c_affixes6_max,
    c_runeCodes0,
    c_runeCodes1,
    c_runeCodes2,
    c_runeCodes3,
    c_runeCodes4,
    c_runeCodes5,
    c_itype0,
    c_itype1,
    c_itype2,
    c_itype3,
    c_itype4,
    c_itype5,
    c_etype0,
    c_etype1,
    c_etype2,
};

const GeneratedTableSchema& DataRow_runes_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "complete",
            "*rune name",
            "t1code1",
            "t1param1",
            "t1min1",
            "t1max1",
            "t1code2",
            "t1param2",
            "t1min2",
            "t1max2",
            "t1code3",
            "t1param3",
            "t1min3",
            "t1max3",
            "t1code4",
            "t1param4",
            "t1min4",
            "t1max4",
            "t1code5",
            "t1param5",
            "t1min5",
            "t1max5",
            "t1code6",
            "t1param6",
            "t1min6",
            "t1max6",
            "t1code7",
            "t1param7",
            "t1min7",
            "t1max7",
            "rune1",
            "rune2",
            "rune3",
            "rune4",
            "rune5",
            "rune6",
            "itype1",
            "itype2",
            "itype3",
            "itype4",
            "itype5",
            "itype6",
            "etype1",
            "etype2",
            "etype3",
        },
        .aliases = {
            { "rune name", "*rune name" },
        },
    };
    return schema;
}

void DataRow_runes_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_complete, complete);
    reader.read(Column::c_name, name);
    affixes_limit = 0;
    if (reader.available(Column::c_affixes0_code) || reader.available(Column::c_affixes0_par) || reader.available(Column::c_affixes0_min) || reader.available(Column::c_affixes0_max))
        affixes_limit = 1;
    if (reader.available(Column::c_affixes1_code) || reader.available(Column::c_affixes1_par) || reader.available(Column::c_affixes1_min) || reader.available(Column::c_affixes1_max))
        affixes_limit = 2;
    if (reader.available(Column::c_affixes2_code) || reader.available(Column::c_affixes2_par) || reader.available(Column::c_affixes2_min) || reader.available(Column::c_affixes2_max))
        affixes_limit = 3;
    if (reader.available(Column::c_affixes3_code) || reader.available(Column::c_affixes3_par) || reader.available(Column::c_affixes3_min) || reader.available(Column::c_affixes3_max))
        affixes_limit = 4;
    if (reader.available(Column::c_affixes4_code) || reader.available(Column::c_affixes4_par) || reader.available(Column::c_affixes4_min) || reader.available(Column::c_affixes4_max))
        affixes_limit = 5;
    if (reader.available(Column::c_affixes5_code) || reader.available(Column::c_affixes5_par) || reader.available(Column::c_affixes5_min) || reader.available(Column::c_affixes5_max))
        affixes_limit = 6;
    if (reader.available(Column::c_affixes6_code) || reader.available(Column::c_affixes6_par) || reader.available(Column::c_affixes6_min) || reader.available(Column::c_affixes6_max))
        affixes_limit = 7;
    affixes.clear();
    affixes.reserve(7);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes0_code, item.code);
        reader.read(Column::c_affixes0_par, item.par);
        reader.read(Column::c_affixes0_min, item.min);
        reader.read(Column::c_affixes0_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes1_code, item.code);
        reader.read(Column::c_affixes1_par, item.par);
        reader.read(Column::c_affixes1_min, item.min);
        reader.read(Column::c_affixes1_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes2_code, item.code);
        reader.read(Column::c_affixes2_par, item.par);
        reader.read(Column::c_affixes2_min, item.min);
        reader.read(Column::c_affixes2_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes3_code, item.code);
        reader.read(Column::c_affixes3_par, item.par);
        reader.read(Column::c_affixes3_min, item.min);
        reader.read(Column::c_affixes3_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes4_code, item.code);
        reader.read(Column::c_affixes4_par, item.par);
        reader.read(Column::c_affixes4_min, item.min);
        reader.read(Column::c_affixes4_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes5_code, item.code);
        reader.read(Column::c_affixes5_par, item.par);
        reader.read(Column::c_affixes5_min, item.min);
        reader.read(Column::c_affixes5_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes6_code, item.code);
        reader.read(Column::c_affixes6_par, item.par);
        reader.read(Column::c_affixes6_min, item.min);
        reader.read(Column::c_affixes6_max, item.max);
        if (!item.code.empty())
            affixes.push_back(item);
    }
    runeCodes.clear();
    runeCodes.reserve(6);
    {
        std::string value;
        reader.read(Column::c_runeCodes0, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_runeCodes1, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_runeCodes2, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_runeCodes3, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_runeCodes4, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_runeCodes5, value);
        if (!value.empty())
            runeCodes.push_back(value);
    }
    itype.clear();
    itype.reserve(6);
    {
        std::string value;
        reader.read(Column::c_itype0, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype1, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype2, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype3, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype4, value);
        if (!value.empty())
            itype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_itype5, value);
        if (!value.empty())
            itype.push_back(value);
    }
    etype.clear();
    etype.reserve(3);
    {
        std::string value;
        reader.read(Column::c_etype0, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype1, value);
        if (!value.empty())
            etype.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_etype2, value);
        if (!value.empty())
            etype.push_back(value);
    }
}

void DataRow_runes_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_complete, complete);
    writer.write(Column::c_name, name);
    if (affixes.size() > 0)
    {
        writer.write(Column::c_affixes0_code, affixes[0].code);
        writer.write(Column::c_affixes0_par, affixes[0].par);
        writer.write(Column::c_affixes0_min, affixes[0].min);
        writer.write(Column::c_affixes0_max, affixes[0].max);
    }
    if (affixes.size() > 1)
    {
        writer.write(Column::c_affixes1_code, affixes[1].code);
        writer.write(Column::c_affixes1_par, affixes[1].par);
        writer.write(Column::c_affixes1_min, affixes[1].min);
        writer.write(Column::c_affixes1_max, affixes[1].max);
    }
    if (affixes.size() > 2)
    {
        writer.write(Column::c_affixes2_code, affixes[2].code);
        writer.write(Column::c_affixes2_par, affixes[2].par);
        writer.write(Column::c_affixes2_min, affixes[2].min);
        writer.write(Column::c_affixes2_max, affixes[2].max);
    }
    if (affixes.size() > 3)
    {
        writer.write(Column::c_affixes3_code, affixes[3].code);
        writer.write(Column::c_affixes3_par, affixes[3].par);
        writer.write(Column::c_affixes3_min, affixes[3].min);
        writer.write(Column::c_affixes3_max, affixes[3].max);
    }
    if (affixes.size() > 4)
    {
        writer.write(Column::c_affixes4_code, affixes[4].code);
        writer.write(Column::c_affixes4_par, affixes[4].par);
        writer.write(Column::c_affixes4_min, affixes[4].min);
        writer.write(Column::c_affixes4_max, affixes[4].max);
    }
    if (affixes.size() > 5)
    {
        writer.write(Column::c_affixes5_code, affixes[5].code);
        writer.write(Column::c_affixes5_par, affixes[5].par);
        writer.write(Column::c_affixes5_min, affixes[5].min);
        writer.write(Column::c_affixes5_max, affixes[5].max);
    }
    if (affixes.size() > 6)
    {
        writer.write(Column::c_affixes6_code, affixes[6].code);
        writer.write(Column::c_affixes6_par, affixes[6].par);
        writer.write(Column::c_affixes6_min, affixes[6].min);
        writer.write(Column::c_affixes6_max, affixes[6].max);
    }
    if (runeCodes.size() > 0)
    {
        writer.write(Column::c_runeCodes0, runeCodes[0]);
    }
    if (runeCodes.size() > 1)
    {
        writer.write(Column::c_runeCodes1, runeCodes[1]);
    }
    if (runeCodes.size() > 2)
    {
        writer.write(Column::c_runeCodes2, runeCodes[2]);
    }
    if (runeCodes.size() > 3)
    {
        writer.write(Column::c_runeCodes3, runeCodes[3]);
    }
    if (runeCodes.size() > 4)
    {
        writer.write(Column::c_runeCodes4, runeCodes[4]);
    }
    if (runeCodes.size() > 5)
    {
        writer.write(Column::c_runeCodes5, runeCodes[5]);
    }
    if (itype.size() > 0)
    {
        writer.write(Column::c_itype0, itype[0]);
    }
    if (itype.size() > 1)
    {
        writer.write(Column::c_itype1, itype[1]);
    }
    if (itype.size() > 2)
    {
        writer.write(Column::c_itype2, itype[2]);
    }
    if (itype.size() > 3)
    {
        writer.write(Column::c_itype3, itype[3]);
    }
    if (itype.size() > 4)
    {
        writer.write(Column::c_itype4, itype[4]);
    }
    if (itype.size() > 5)
    {
        writer.write(Column::c_itype5, itype[5]);
    }
    if (etype.size() > 0)
    {
        writer.write(Column::c_etype0, etype[0]);
    }
    if (etype.size() > 1)
    {
        writer.write(Column::c_etype1, etype[1]);
    }
    if (etype.size() > 2)
    {
        writer.write(Column::c_etype2, etype[2]);
    }
}

enum DataRow_gems_codegen::Column : std::size_t {
    c_eqipmentCode,
    c_affixes_weapon0_code,
    c_affixes_weapon0_par,
    c_affixes_weapon0_min,
    c_affixes_weapon0_max,
    c_affixes_weapon1_code,
    c_affixes_weapon1_par,
    c_affixes_weapon1_min,
    c_affixes_weapon1_max,
    c_affixes_weapon2_code,
    c_affixes_weapon2_par,
    c_affixes_weapon2_min,
    c_affixes_weapon2_max,
    c_affixes_helm0_code,
    c_affixes_helm0_par,
    c_affixes_helm0_min,
    c_affixes_helm0_max,
    c_affixes_helm1_code,
    c_affixes_helm1_par,
    c_affixes_helm1_min,
    c_affixes_helm1_max,
    c_affixes_helm2_code,
    c_affixes_helm2_par,
    c_affixes_helm2_min,
    c_affixes_helm2_max,
    c_affixes_shield0_code,
    c_affixes_shield0_par,
    c_affixes_shield0_min,
    c_affixes_shield0_max,
    c_affixes_shield1_code,
    c_affixes_shield1_par,
    c_affixes_shield1_min,
    c_affixes_shield1_max,
    c_affixes_shield2_code,
    c_affixes_shield2_par,
    c_affixes_shield2_min,
    c_affixes_shield2_max,
};

const GeneratedTableSchema& DataRow_gems_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
            "code",
            "weaponmod1code",
            "weaponmod1param",
            "weaponmod1min",
            "weaponmod1max",
            "weaponmod2code",
            "weaponmod2param",
            "weaponmod2min",
            "weaponmod2max",
            "weaponmod3code",
            "weaponmod3param",
            "weaponmod3min",
            "weaponmod3max",
            "helmmod1code",
            "helmmod1param",
            "helmmod1min",
            "helmmod1max",
            "helmmod2code",
            "helmmod2param",
            "helmmod2min",
            "helmmod2max",
            "helmmod3code",
            "helmmod3param",
            "helmmod3min",
            "helmmod3max",
            "shieldmod1code",
            "shieldmod1param",
            "shieldmod1min",
            "shieldmod1max",
            "shieldmod2code",
            "shieldmod2param",
            "shieldmod2min",
            "shieldmod2max",
            "shieldmod3code",
            "shieldmod3param",
            "shieldmod3min",
            "shieldmod3max",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_gems_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_eqipmentCode, eqipmentCode);
    affixes_weapon_limit = 0;
    if (reader.available(Column::c_affixes_weapon0_code) || reader.available(Column::c_affixes_weapon0_par) || reader.available(Column::c_affixes_weapon0_min) || reader.available(Column::c_affixes_weapon0_max))
        affixes_weapon_limit = 1;
    if (reader.available(Column::c_affixes_weapon1_code) || reader.available(Column::c_affixes_weapon1_par) || reader.available(Column::c_affixes_weapon1_min) || reader.available(Column::c_affixes_weapon1_max))
        affixes_weapon_limit = 2;
    if (reader.available(Column::c_affixes_weapon2_code) || reader.available(Column::c_affixes_weapon2_par) || reader.available(Column::c_affixes_weapon2_min) || reader.available(Column::c_affixes_weapon2_max))
        affixes_weapon_limit = 3;
    affixes_weapon.clear();
    affixes_weapon.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_weapon0_code, item.code);
        reader.read(Column::c_affixes_weapon0_par, item.par);
        reader.read(Column::c_affixes_weapon0_min, item.min);
        reader.read(Column::c_affixes_weapon0_max, item.max);
        if (!item.code.empty())
            affixes_weapon.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_weapon1_code, item.code);
        reader.read(Column::c_affixes_weapon1_par, item.par);
        reader.read(Column::c_affixes_weapon1_min, item.min);
        reader.read(Column::c_affixes_weapon1_max, item.max);
        if (!item.code.empty())
            affixes_weapon.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_weapon2_code, item.code);
        reader.read(Column::c_affixes_weapon2_par, item.par);
        reader.read(Column::c_affixes_weapon2_min, item.min);
        reader.read(Column::c_affixes_weapon2_max, item.max);
        if (!item.code.empty())
            affixes_weapon.push_back(item);
    }
    affixes_helm_limit = 0;
    if (reader.available(Column::c_affixes_helm0_code) || reader.available(Column::c_affixes_helm0_par) || reader.available(Column::c_affixes_helm0_min) || reader.available(Column::c_affixes_helm0_max))
        affixes_helm_limit = 1;
    if (reader.available(Column::c_affixes_helm1_code) || reader.available(Column::c_affixes_helm1_par) || reader.available(Column::c_affixes_helm1_min) || reader.available(Column::c_affixes_helm1_max))
        affixes_helm_limit = 2;
    if (reader.available(Column::c_affixes_helm2_code) || reader.available(Column::c_affixes_helm2_par) || reader.available(Column::c_affixes_helm2_min) || reader.available(Column::c_affixes_helm2_max))
        affixes_helm_limit = 3;
    affixes_helm.clear();
    affixes_helm.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_helm0_code, item.code);
        reader.read(Column::c_affixes_helm0_par, item.par);
        reader.read(Column::c_affixes_helm0_min, item.min);
        reader.read(Column::c_affixes_helm0_max, item.max);
        if (!item.code.empty())
            affixes_helm.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_helm1_code, item.code);
        reader.read(Column::c_affixes_helm1_par, item.par);
        reader.read(Column::c_affixes_helm1_min, item.min);
        reader.read(Column::c_affixes_helm1_max, item.max);
        if (!item.code.empty())
            affixes_helm.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_helm2_code, item.code);
        reader.read(Column::c_affixes_helm2_par, item.par);
        reader.read(Column::c_affixes_helm2_min, item.min);
        reader.read(Column::c_affixes_helm2_max, item.max);
        if (!item.code.empty())
            affixes_helm.push_back(item);
    }
    affixes_shield_limit = 0;
    if (reader.available(Column::c_affixes_shield0_code) || reader.available(Column::c_affixes_shield0_par) || reader.available(Column::c_affixes_shield0_min) || reader.available(Column::c_affixes_shield0_max))
        affixes_shield_limit = 1;
    if (reader.available(Column::c_affixes_shield1_code) || reader.available(Column::c_affixes_shield1_par) || reader.available(Column::c_affixes_shield1_min) || reader.available(Column::c_affixes_shield1_max))
        affixes_shield_limit = 2;
    if (reader.available(Column::c_affixes_shield2_code) || reader.available(Column::c_affixes_shield2_par) || reader.available(Column::c_affixes_shield2_min) || reader.available(Column::c_affixes_shield2_max))
        affixes_shield_limit = 3;
    affixes_shield.clear();
    affixes_shield.reserve(3);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_shield0_code, item.code);
        reader.read(Column::c_affixes_shield0_par, item.par);
        reader.read(Column::c_affixes_shield0_min, item.min);
        reader.read(Column::c_affixes_shield0_max, item.max);
        if (!item.code.empty())
            affixes_shield.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_shield1_code, item.code);
        reader.read(Column::c_affixes_shield1_par, item.par);
        reader.read(Column::c_affixes_shield1_min, item.min);
        reader.read(Column::c_affixes_shield1_max, item.max);
        if (!item.code.empty())
            affixes_shield.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_shield2_code, item.code);
        reader.read(Column::c_affixes_shield2_par, item.par);
        reader.read(Column::c_affixes_shield2_min, item.min);
        reader.read(Column::c_affixes_shield2_max, item.max);
        if (!item.code.empty())
            affixes_shield.push_back(item);
    }
}

void DataRow_gems_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_eqipmentCode, eqipmentCode);
    if (affixes_weapon.size() > 0)
    {
        writer.write(Column::c_affixes_weapon0_code, affixes_weapon[0].code);
        writer.write(Column::c_affixes_weapon0_par, affixes_weapon[0].par);
        writer.write(Column::c_affixes_weapon0_min, affixes_weapon[0].min);
        writer.write(Column::c_affixes_weapon0_max, affixes_weapon[0].max);
    }
    if (affixes_weapon.size() > 1)
    {
        writer.write(Column::c_affixes_weapon1_code, affixes_weapon[1].code);
        writer.write(Column::c_affixes_weapon1_par, affixes_weapon[1].par);
        writer.write(Column::c_affixes_weapon1_min, affixes_weapon[1].min);
        writer.write(Column::c_affixes_weapon1_max, affixes_weapon[1].max);
    }
    if (affixes_weapon.size() > 2)
    {
        writer.write(Column::c_affixes_weapon2_code, affixes_weapon[2].code);
        writer.write(Column::c_affixes_weapon2_par, affixes_weapon[2].par);
        writer.write(Column::c_affixes_weapon2_min, affixes_weapon[2].min);
        writer.write(Column::c_affixes_weapon2_max, affixes_weapon[2].max);
    }
    if (affixes_helm.size() > 0)
    {
        writer.write(Column::c_affixes_helm0_code, affixes_helm[0].code);
        writer.write(Column::c_affixes_helm0_par, affixes_helm[0].par);
        writer.write(Column::c_affixes_helm0_min, affixes_helm[0].min);
        writer.write(Column::c_affixes_helm0_max, affixes_helm[0].max);
    }
    if (affixes_helm.size() > 1)
    {
        writer.write(Column::c_affixes_helm1_code, affixes_helm[1].code);
        writer.write(Column::c_affixes_helm1_par, affixes_helm[1].par);
        writer.write(Column::c_affixes_helm1_min, affixes_helm[1].min);
        writer.write(Column::c_affixes_helm1_max, affixes_helm[1].max);
    }
    if (affixes_helm.size() > 2)
    {
        writer.write(Column::c_affixes_helm2_code, affixes_helm[2].code);
        writer.write(Column::c_affixes_helm2_par, affixes_helm[2].par);
        writer.write(Column::c_affixes_helm2_min, affixes_helm[2].min);
        writer.write(Column::c_affixes_helm2_max, affixes_helm[2].max);
    }
    if (affixes_shield.size() > 0)
    {
        writer.write(Column::c_affixes_shield0_code, affixes_shield[0].code);
        writer.write(Column::c_affixes_shield0_par, affixes_shield[0].par);
        writer.write(Column::c_affixes_shield0_min, affixes_shield[0].min);
        writer.write(Column::c_affixes_shield0_max, affixes_shield[0].max);
    }
    if (affixes_shield.size() > 1)
    {
        writer.write(Column::c_affixes_shield1_code, affixes_shield[1].code);
        writer.write(Column::c_affixes_shield1_par, affixes_shield[1].par);
        writer.write(Column::c_affixes_shield1_min, affixes_shield[1].min);
        writer.write(Column::c_affixes_shield1_max, affixes_shield[1].max);
    }
    if (affixes_shield.size() > 2)
    {
        writer.write(Column::c_affixes_shield2_code, affixes_shield[2].code);
        writer.write(Column::c_affixes_shield2_par, affixes_shield[2].par);
        writer.write(Column::c_affixes_shield2_min, affixes_shield[2].min);
        writer.write(Column::c_affixes_shield2_max, affixes_shield[2].max);
    }
}

enum DataRow_cubemain_codegen::Column : std::size_t {
    c_description,
    c_enabled,
    c_version,
    c_op,
    c_eol,
    c_numinputs,
    c_output,
    c_ilvl,
    c_plvl,
    c_lvl,
    c_affixes_a0_code,
    c_affixes_a0_par,
    c_affixes_a0_min,
    c_affixes_a0_max,
    c_affixes_a0_chance,
    c_affixes_a1_code,
    c_affixes_a1_par,
    c_affixes_a1_min,
    c_affixes_a1_max,
    c_affixes_a1_chance,
    c_affixes_a2_code,
    c_affixes_a2_par,
    c_affixes_a2_min,
    c_affixes_a2_max,
    c_affixes_a2_chance,
    c_affixes_a3_code,
    c_affixes_a3_par,
    c_affixes_a3_min,
    c_affixes_a3_max,
    c_affixes_a3_chance,
    c_affixes_a4_code,
    c_affixes_a4_par,
    c_affixes_a4_min,
    c_affixes_a4_max,
    c_affixes_a4_chance,
    c_affixes_b0_code,
    c_affixes_b0_par,
    c_affixes_b0_min,
    c_affixes_b0_max,
    c_affixes_b0_chance,
    c_affixes_b1_code,
    c_affixes_b1_par,
    c_affixes_b1_min,
    c_affixes_b1_max,
    c_affixes_b1_chance,
    c_affixes_b2_code,
    c_affixes_b2_par,
    c_affixes_b2_min,
    c_affixes_b2_max,
    c_affixes_b2_chance,
    c_affixes_b3_code,
    c_affixes_b3_par,
    c_affixes_b3_min,
    c_affixes_b3_max,
    c_affixes_b3_chance,
    c_affixes_b4_code,
    c_affixes_b4_par,
    c_affixes_b4_min,
    c_affixes_b4_max,
    c_affixes_b4_chance,
    c_affixes_c0_code,
    c_affixes_c0_par,
    c_affixes_c0_min,
    c_affixes_c0_max,
    c_affixes_c0_chance,
    c_affixes_c1_code,
    c_affixes_c1_par,
    c_affixes_c1_min,
    c_affixes_c1_max,
    c_affixes_c1_chance,
    c_affixes_c2_code,
    c_affixes_c2_par,
    c_affixes_c2_min,
    c_affixes_c2_max,
    c_affixes_c2_chance,
    c_affixes_c3_code,
    c_affixes_c3_par,
    c_affixes_c3_min,
    c_affixes_c3_max,
    c_affixes_c3_chance,
    c_affixes_c4_code,
    c_affixes_c4_par,
    c_affixes_c4_min,
    c_affixes_c4_max,
    c_affixes_c4_chance,
    c_inputs0,
    c_inputs1,
    c_inputs2,
    c_inputs3,
    c_inputs4,
    c_inputs5,
    c_inputs6,
};

const GeneratedTableSchema& DataRow_cubemain_codegen::tableSchema()
{
    static const GeneratedTableSchema schema{
        .columns = {
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
            "mod 1",
            "mod 1 param",
            "mod 1 min",
            "mod 1 max",
            "mod 1 chance",
            "mod 2",
            "mod 2 param",
            "mod 2 min",
            "mod 2 max",
            "mod 2 chance",
            "mod 3",
            "mod 3 param",
            "mod 3 min",
            "mod 3 max",
            "mod 3 chance",
            "mod 4",
            "mod 4 param",
            "mod 4 min",
            "mod 4 max",
            "mod 4 chance",
            "mod 5",
            "mod 5 param",
            "mod 5 min",
            "mod 5 max",
            "mod 5 chance",
            "b mod 1",
            "b mod 1 param",
            "b mod 1 min",
            "b mod 1 max",
            "b mod 1 chance",
            "b mod 2",
            "b mod 2 param",
            "b mod 2 min",
            "b mod 2 max",
            "b mod 2 chance",
            "b mod 3",
            "b mod 3 param",
            "b mod 3 min",
            "b mod 3 max",
            "b mod 3 chance",
            "b mod 4",
            "b mod 4 param",
            "b mod 4 min",
            "b mod 4 max",
            "b mod 4 chance",
            "b mod 5",
            "b mod 5 param",
            "b mod 5 min",
            "b mod 5 max",
            "b mod 5 chance",
            "c mod 1",
            "c mod 1 param",
            "c mod 1 min",
            "c mod 1 max",
            "c mod 1 chance",
            "c mod 2",
            "c mod 2 param",
            "c mod 2 min",
            "c mod 2 max",
            "c mod 2 chance",
            "c mod 3",
            "c mod 3 param",
            "c mod 3 min",
            "c mod 3 max",
            "c mod 3 chance",
            "c mod 4",
            "c mod 4 param",
            "c mod 4 min",
            "c mod 4 max",
            "c mod 4 chance",
            "c mod 5",
            "c mod 5 param",
            "c mod 5 min",
            "c mod 5 max",
            "c mod 5 chance",
            "input 1",
            "input 2",
            "input 3",
            "input 4",
            "input 5",
            "input 6",
            "input 7",
        },
        .aliases = {
        },
    };
    return schema;
}

void DataRow_cubemain_codegen::readData(const CsvRowReader& reader)
{
    reader.read(Column::c_description, description);
    reader.read(Column::c_enabled, enabled);
    reader.read(Column::c_version, version);
    reader.read(Column::c_op, op);
    reader.read(Column::c_eol, eol);
    reader.read(Column::c_numinputs, numinputs);
    reader.read(Column::c_output, output);
    reader.read(Column::c_ilvl, ilvl);
    reader.read(Column::c_plvl, plvl);
    reader.read(Column::c_lvl, lvl);
    affixes_a_limit = 0;
    if (reader.available(Column::c_affixes_a0_code) || reader.available(Column::c_affixes_a0_par) || reader.available(Column::c_affixes_a0_min) || reader.available(Column::c_affixes_a0_max) || reader.available(Column::c_affixes_a0_chance))
        affixes_a_limit = 1;
    if (reader.available(Column::c_affixes_a1_code) || reader.available(Column::c_affixes_a1_par) || reader.available(Column::c_affixes_a1_min) || reader.available(Column::c_affixes_a1_max) || reader.available(Column::c_affixes_a1_chance))
        affixes_a_limit = 2;
    if (reader.available(Column::c_affixes_a2_code) || reader.available(Column::c_affixes_a2_par) || reader.available(Column::c_affixes_a2_min) || reader.available(Column::c_affixes_a2_max) || reader.available(Column::c_affixes_a2_chance))
        affixes_a_limit = 3;
    if (reader.available(Column::c_affixes_a3_code) || reader.available(Column::c_affixes_a3_par) || reader.available(Column::c_affixes_a3_min) || reader.available(Column::c_affixes_a3_max) || reader.available(Column::c_affixes_a3_chance))
        affixes_a_limit = 4;
    if (reader.available(Column::c_affixes_a4_code) || reader.available(Column::c_affixes_a4_par) || reader.available(Column::c_affixes_a4_min) || reader.available(Column::c_affixes_a4_max) || reader.available(Column::c_affixes_a4_chance))
        affixes_a_limit = 5;
    affixes_a.clear();
    affixes_a.reserve(5);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_a0_code, item.code);
        reader.read(Column::c_affixes_a0_par, item.par);
        reader.read(Column::c_affixes_a0_min, item.min);
        reader.read(Column::c_affixes_a0_max, item.max);
        reader.read(Column::c_affixes_a0_chance, item.chance);
        if (!item.code.empty())
            affixes_a.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_a1_code, item.code);
        reader.read(Column::c_affixes_a1_par, item.par);
        reader.read(Column::c_affixes_a1_min, item.min);
        reader.read(Column::c_affixes_a1_max, item.max);
        reader.read(Column::c_affixes_a1_chance, item.chance);
        if (!item.code.empty())
            affixes_a.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_a2_code, item.code);
        reader.read(Column::c_affixes_a2_par, item.par);
        reader.read(Column::c_affixes_a2_min, item.min);
        reader.read(Column::c_affixes_a2_max, item.max);
        reader.read(Column::c_affixes_a2_chance, item.chance);
        if (!item.code.empty())
            affixes_a.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_a3_code, item.code);
        reader.read(Column::c_affixes_a3_par, item.par);
        reader.read(Column::c_affixes_a3_min, item.min);
        reader.read(Column::c_affixes_a3_max, item.max);
        reader.read(Column::c_affixes_a3_chance, item.chance);
        if (!item.code.empty())
            affixes_a.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_a4_code, item.code);
        reader.read(Column::c_affixes_a4_par, item.par);
        reader.read(Column::c_affixes_a4_min, item.min);
        reader.read(Column::c_affixes_a4_max, item.max);
        reader.read(Column::c_affixes_a4_chance, item.chance);
        if (!item.code.empty())
            affixes_a.push_back(item);
    }
    affixes_b_limit = 0;
    if (reader.available(Column::c_affixes_b0_code) || reader.available(Column::c_affixes_b0_par) || reader.available(Column::c_affixes_b0_min) || reader.available(Column::c_affixes_b0_max) || reader.available(Column::c_affixes_b0_chance))
        affixes_b_limit = 1;
    if (reader.available(Column::c_affixes_b1_code) || reader.available(Column::c_affixes_b1_par) || reader.available(Column::c_affixes_b1_min) || reader.available(Column::c_affixes_b1_max) || reader.available(Column::c_affixes_b1_chance))
        affixes_b_limit = 2;
    if (reader.available(Column::c_affixes_b2_code) || reader.available(Column::c_affixes_b2_par) || reader.available(Column::c_affixes_b2_min) || reader.available(Column::c_affixes_b2_max) || reader.available(Column::c_affixes_b2_chance))
        affixes_b_limit = 3;
    if (reader.available(Column::c_affixes_b3_code) || reader.available(Column::c_affixes_b3_par) || reader.available(Column::c_affixes_b3_min) || reader.available(Column::c_affixes_b3_max) || reader.available(Column::c_affixes_b3_chance))
        affixes_b_limit = 4;
    if (reader.available(Column::c_affixes_b4_code) || reader.available(Column::c_affixes_b4_par) || reader.available(Column::c_affixes_b4_min) || reader.available(Column::c_affixes_b4_max) || reader.available(Column::c_affixes_b4_chance))
        affixes_b_limit = 5;
    affixes_b.clear();
    affixes_b.reserve(5);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_b0_code, item.code);
        reader.read(Column::c_affixes_b0_par, item.par);
        reader.read(Column::c_affixes_b0_min, item.min);
        reader.read(Column::c_affixes_b0_max, item.max);
        reader.read(Column::c_affixes_b0_chance, item.chance);
        if (!item.code.empty())
            affixes_b.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_b1_code, item.code);
        reader.read(Column::c_affixes_b1_par, item.par);
        reader.read(Column::c_affixes_b1_min, item.min);
        reader.read(Column::c_affixes_b1_max, item.max);
        reader.read(Column::c_affixes_b1_chance, item.chance);
        if (!item.code.empty())
            affixes_b.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_b2_code, item.code);
        reader.read(Column::c_affixes_b2_par, item.par);
        reader.read(Column::c_affixes_b2_min, item.min);
        reader.read(Column::c_affixes_b2_max, item.max);
        reader.read(Column::c_affixes_b2_chance, item.chance);
        if (!item.code.empty())
            affixes_b.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_b3_code, item.code);
        reader.read(Column::c_affixes_b3_par, item.par);
        reader.read(Column::c_affixes_b3_min, item.min);
        reader.read(Column::c_affixes_b3_max, item.max);
        reader.read(Column::c_affixes_b3_chance, item.chance);
        if (!item.code.empty())
            affixes_b.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_b4_code, item.code);
        reader.read(Column::c_affixes_b4_par, item.par);
        reader.read(Column::c_affixes_b4_min, item.min);
        reader.read(Column::c_affixes_b4_max, item.max);
        reader.read(Column::c_affixes_b4_chance, item.chance);
        if (!item.code.empty())
            affixes_b.push_back(item);
    }
    affixes_c_limit = 0;
    if (reader.available(Column::c_affixes_c0_code) || reader.available(Column::c_affixes_c0_par) || reader.available(Column::c_affixes_c0_min) || reader.available(Column::c_affixes_c0_max) || reader.available(Column::c_affixes_c0_chance))
        affixes_c_limit = 1;
    if (reader.available(Column::c_affixes_c1_code) || reader.available(Column::c_affixes_c1_par) || reader.available(Column::c_affixes_c1_min) || reader.available(Column::c_affixes_c1_max) || reader.available(Column::c_affixes_c1_chance))
        affixes_c_limit = 2;
    if (reader.available(Column::c_affixes_c2_code) || reader.available(Column::c_affixes_c2_par) || reader.available(Column::c_affixes_c2_min) || reader.available(Column::c_affixes_c2_max) || reader.available(Column::c_affixes_c2_chance))
        affixes_c_limit = 3;
    if (reader.available(Column::c_affixes_c3_code) || reader.available(Column::c_affixes_c3_par) || reader.available(Column::c_affixes_c3_min) || reader.available(Column::c_affixes_c3_max) || reader.available(Column::c_affixes_c3_chance))
        affixes_c_limit = 4;
    if (reader.available(Column::c_affixes_c4_code) || reader.available(Column::c_affixes_c4_par) || reader.available(Column::c_affixes_c4_min) || reader.available(Column::c_affixes_c4_max) || reader.available(Column::c_affixes_c4_chance))
        affixes_c_limit = 5;
    affixes_c.clear();
    affixes_c.reserve(5);
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_c0_code, item.code);
        reader.read(Column::c_affixes_c0_par, item.par);
        reader.read(Column::c_affixes_c0_min, item.min);
        reader.read(Column::c_affixes_c0_max, item.max);
        reader.read(Column::c_affixes_c0_chance, item.chance);
        if (!item.code.empty())
            affixes_c.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_c1_code, item.code);
        reader.read(Column::c_affixes_c1_par, item.par);
        reader.read(Column::c_affixes_c1_min, item.min);
        reader.read(Column::c_affixes_c1_max, item.max);
        reader.read(Column::c_affixes_c1_chance, item.chance);
        if (!item.code.empty())
            affixes_c.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_c2_code, item.code);
        reader.read(Column::c_affixes_c2_par, item.par);
        reader.read(Column::c_affixes_c2_min, item.min);
        reader.read(Column::c_affixes_c2_max, item.max);
        reader.read(Column::c_affixes_c2_chance, item.chance);
        if (!item.code.empty())
            affixes_c.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_c3_code, item.code);
        reader.read(Column::c_affixes_c3_par, item.par);
        reader.read(Column::c_affixes_c3_min, item.min);
        reader.read(Column::c_affixes_c3_max, item.max);
        reader.read(Column::c_affixes_c3_chance, item.chance);
        if (!item.code.empty())
            affixes_c.push_back(item);
    }
    {
        AffixProp_codegen item;
        reader.read(Column::c_affixes_c4_code, item.code);
        reader.read(Column::c_affixes_c4_par, item.par);
        reader.read(Column::c_affixes_c4_min, item.min);
        reader.read(Column::c_affixes_c4_max, item.max);
        reader.read(Column::c_affixes_c4_chance, item.chance);
        if (!item.code.empty())
            affixes_c.push_back(item);
    }
    inputs.clear();
    inputs.reserve(7);
    {
        std::string value;
        reader.read(Column::c_inputs0, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs1, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs2, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs3, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs4, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs5, value);
        if (!value.empty())
            inputs.push_back(value);
    }
    {
        std::string value;
        reader.read(Column::c_inputs6, value);
        if (!value.empty())
            inputs.push_back(value);
    }
}

void DataRow_cubemain_codegen::writeData(CsvRowWriter& writer) const
{
    writer.write(Column::c_description, description);
    writer.write(Column::c_enabled, enabled);
    writer.write(Column::c_version, version);
    writer.write(Column::c_op, op);
    writer.write(Column::c_eol, eol);
    writer.write(Column::c_numinputs, numinputs);
    writer.write(Column::c_output, output);
    writer.write(Column::c_ilvl, ilvl);
    writer.write(Column::c_plvl, plvl);
    writer.write(Column::c_lvl, lvl);
    if (affixes_a.size() > 0)
    {
        writer.write(Column::c_affixes_a0_code, affixes_a[0].code);
        writer.write(Column::c_affixes_a0_par, affixes_a[0].par);
        writer.write(Column::c_affixes_a0_min, affixes_a[0].min);
        writer.write(Column::c_affixes_a0_max, affixes_a[0].max);
        writer.write(Column::c_affixes_a0_chance, affixes_a[0].chance);
    }
    if (affixes_a.size() > 1)
    {
        writer.write(Column::c_affixes_a1_code, affixes_a[1].code);
        writer.write(Column::c_affixes_a1_par, affixes_a[1].par);
        writer.write(Column::c_affixes_a1_min, affixes_a[1].min);
        writer.write(Column::c_affixes_a1_max, affixes_a[1].max);
        writer.write(Column::c_affixes_a1_chance, affixes_a[1].chance);
    }
    if (affixes_a.size() > 2)
    {
        writer.write(Column::c_affixes_a2_code, affixes_a[2].code);
        writer.write(Column::c_affixes_a2_par, affixes_a[2].par);
        writer.write(Column::c_affixes_a2_min, affixes_a[2].min);
        writer.write(Column::c_affixes_a2_max, affixes_a[2].max);
        writer.write(Column::c_affixes_a2_chance, affixes_a[2].chance);
    }
    if (affixes_a.size() > 3)
    {
        writer.write(Column::c_affixes_a3_code, affixes_a[3].code);
        writer.write(Column::c_affixes_a3_par, affixes_a[3].par);
        writer.write(Column::c_affixes_a3_min, affixes_a[3].min);
        writer.write(Column::c_affixes_a3_max, affixes_a[3].max);
        writer.write(Column::c_affixes_a3_chance, affixes_a[3].chance);
    }
    if (affixes_a.size() > 4)
    {
        writer.write(Column::c_affixes_a4_code, affixes_a[4].code);
        writer.write(Column::c_affixes_a4_par, affixes_a[4].par);
        writer.write(Column::c_affixes_a4_min, affixes_a[4].min);
        writer.write(Column::c_affixes_a4_max, affixes_a[4].max);
        writer.write(Column::c_affixes_a4_chance, affixes_a[4].chance);
    }
    if (affixes_b.size() > 0)
    {
        writer.write(Column::c_affixes_b0_code, affixes_b[0].code);
        writer.write(Column::c_affixes_b0_par, affixes_b[0].par);
        writer.write(Column::c_affixes_b0_min, affixes_b[0].min);
        writer.write(Column::c_affixes_b0_max, affixes_b[0].max);
        writer.write(Column::c_affixes_b0_chance, affixes_b[0].chance);
    }
    if (affixes_b.size() > 1)
    {
        writer.write(Column::c_affixes_b1_code, affixes_b[1].code);
        writer.write(Column::c_affixes_b1_par, affixes_b[1].par);
        writer.write(Column::c_affixes_b1_min, affixes_b[1].min);
        writer.write(Column::c_affixes_b1_max, affixes_b[1].max);
        writer.write(Column::c_affixes_b1_chance, affixes_b[1].chance);
    }
    if (affixes_b.size() > 2)
    {
        writer.write(Column::c_affixes_b2_code, affixes_b[2].code);
        writer.write(Column::c_affixes_b2_par, affixes_b[2].par);
        writer.write(Column::c_affixes_b2_min, affixes_b[2].min);
        writer.write(Column::c_affixes_b2_max, affixes_b[2].max);
        writer.write(Column::c_affixes_b2_chance, affixes_b[2].chance);
    }
    if (affixes_b.size() > 3)
    {
        writer.write(Column::c_affixes_b3_code, affixes_b[3].code);
        writer.write(Column::c_affixes_b3_par, affixes_b[3].par);
        writer.write(Column::c_affixes_b3_min, affixes_b[3].min);
        writer.write(Column::c_affixes_b3_max, affixes_b[3].max);
        writer.write(Column::c_affixes_b3_chance, affixes_b[3].chance);
    }
    if (affixes_b.size() > 4)
    {
        writer.write(Column::c_affixes_b4_code, affixes_b[4].code);
        writer.write(Column::c_affixes_b4_par, affixes_b[4].par);
        writer.write(Column::c_affixes_b4_min, affixes_b[4].min);
        writer.write(Column::c_affixes_b4_max, affixes_b[4].max);
        writer.write(Column::c_affixes_b4_chance, affixes_b[4].chance);
    }
    if (affixes_c.size() > 0)
    {
        writer.write(Column::c_affixes_c0_code, affixes_c[0].code);
        writer.write(Column::c_affixes_c0_par, affixes_c[0].par);
        writer.write(Column::c_affixes_c0_min, affixes_c[0].min);
        writer.write(Column::c_affixes_c0_max, affixes_c[0].max);
        writer.write(Column::c_affixes_c0_chance, affixes_c[0].chance);
    }
    if (affixes_c.size() > 1)
    {
        writer.write(Column::c_affixes_c1_code, affixes_c[1].code);
        writer.write(Column::c_affixes_c1_par, affixes_c[1].par);
        writer.write(Column::c_affixes_c1_min, affixes_c[1].min);
        writer.write(Column::c_affixes_c1_max, affixes_c[1].max);
        writer.write(Column::c_affixes_c1_chance, affixes_c[1].chance);
    }
    if (affixes_c.size() > 2)
    {
        writer.write(Column::c_affixes_c2_code, affixes_c[2].code);
        writer.write(Column::c_affixes_c2_par, affixes_c[2].par);
        writer.write(Column::c_affixes_c2_min, affixes_c[2].min);
        writer.write(Column::c_affixes_c2_max, affixes_c[2].max);
        writer.write(Column::c_affixes_c2_chance, affixes_c[2].chance);
    }
    if (affixes_c.size() > 3)
    {
        writer.write(Column::c_affixes_c3_code, affixes_c[3].code);
        writer.write(Column::c_affixes_c3_par, affixes_c[3].par);
        writer.write(Column::c_affixes_c3_min, affixes_c[3].min);
        writer.write(Column::c_affixes_c3_max, affixes_c[3].max);
        writer.write(Column::c_affixes_c3_chance, affixes_c[3].chance);
    }
    if (affixes_c.size() > 4)
    {
        writer.write(Column::c_affixes_c4_code, affixes_c[4].code);
        writer.write(Column::c_affixes_c4_par, affixes_c[4].par);
        writer.write(Column::c_affixes_c4_min, affixes_c[4].min);
        writer.write(Column::c_affixes_c4_max, affixes_c[4].max);
        writer.write(Column::c_affixes_c4_chance, affixes_c[4].chance);
    }
    if (inputs.size() > 0)
    {
        writer.write(Column::c_inputs0, inputs[0]);
    }
    if (inputs.size() > 1)
    {
        writer.write(Column::c_inputs1, inputs[1]);
    }
    if (inputs.size() > 2)
    {
        writer.write(Column::c_inputs2, inputs[2]);
    }
    if (inputs.size() > 3)
    {
        writer.write(Column::c_inputs3, inputs[3]);
    }
    if (inputs.size() > 4)
    {
        writer.write(Column::c_inputs4, inputs[4]);
    }
    if (inputs.size() > 5)
    {
        writer.write(Column::c_inputs5, inputs[5]);
    }
    if (inputs.size() > 6)
    {
        writer.write(Column::c_inputs6, inputs[6]);
    }
}

// clang-format on
} // namespace D2ModGen
