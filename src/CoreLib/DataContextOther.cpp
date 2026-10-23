/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextOther.hpp"

namespace D2ModGen {

std::vector<std::string> DataRow_charstats::getColumns() const
{
    std::vector<std::string> result{ "class", "statperlevel", "skillsperlevel" };
    for (size_t i = 0; i < maxItems; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("item" + istr + "count");
        result.push_back("item" + istr);
        result.push_back("item" + istr + "loc");
    }
    for (size_t i = 0; i < maxSkills; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("skill " + istr);
    }

    return result;
}

void DataRow_charstats::serialize()
{
    ser(clas);
    ser(statPerLevel);
    ser(skillsPerLevel);
    serArray(items, maxItems, [this](Item& item) {
        ser(item.count);
        ser(item.code);
        ser(item.loc); }, [](Item& item) { return !item.code.empty() && item.code != "0"; });

    ser(skills, maxSkills);
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

void DataRow_difficultylevels::serialize()
{
    ser(name);
    ser(resistPenalty);

    ser(gambleRare);
    ser(gambleSet);
    ser(gambleUnique);
    ser(gambleUber);
    ser(gambleUltra);
}

std::vector<std::string> DataRow_experience::getColumns() const
{
    return { "expratio" };
}

void DataRow_experience::serialize()
{
    ser(expRatio);
}

std::vector<std::string> DataRow_gamble::getColumns() const
{
    return { "name", "code" };
}

void DataRow_gamble::serialize()
{
    ser(name);
    ser(code);
}

std::vector<std::string> DataRow_hireling::getColumns() const
{
    return { "hp", "hp/lvl", "dmg-min", "dmg-max", "dmg/lvl" };
}

void DataRow_hireling::serialize()
{
    ser(hp);
    ser(hp_lvl);
    ser(dmg_min);
    ser(dmg_max);
    ser(dmg_lvl);
}

std::vector<std::string> DataRow_itemstatcost::getColumns() const
{
    return { "multiply", "add" };
}

void DataRow_itemstatcost::serialize()
{
    ser(multiply);
    ser(add);
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
    using namespace std::literals;
    for (const auto& colTpl : { "mon"s, "nmon"s, "umon"s }) {
        for (size_t i = 0; i < maxTypes; ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(colTpl + istr);
        }
    }
    return result;
}

std::map<std::string, std::string> DataRow_levels::getColumnAliases() const
{
    return {
        { "monlvl1ex", "monlvlex" },
        { "monlvl2ex", "monlvlex(n)" },
        { "monlvl3ex", "monlvlex(h)" },
    };
}

void DataRow_levels::serialize()
{
    ser(name);
    ser(numMon);
    ser(monUMin);
    ser(monUMax);
    ser(monDen);
    ser(monLvlEx);

    ser(mon, maxTypes);
    ser(nmon, maxTypes);
    ser(umon, maxTypes);
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

void DataRow_monlvl::serialize()
{
    ser(TH);
    ser(AC);
    ser(HP);
    ser(DM);
    ser(XP);

    ser(level);
}

std::vector<std::string> DataRow_monstats::getColumns() const
{
    using namespace std::literals;
    std::vector<std::string>         result;
    const std::array<std::string, 3> diffs{ ""s, "(n)"s, "(h)"s };

    for (const std::string& code : { "resfi"s, "resli"s, "resco"s, "respo"s, "resma"s, "resdm"s }) {
        for (const auto& diffSuffix : diffs) {
            result.push_back(code + diffSuffix);
        }
    }

    for (const std::string& code : {
             // clang-format off
             "treasureclass"s, "treasureclasschamp"s , "treasureclassunique"s , "treasureclassquest"s,
             // "treasureclassdesecrated"s, "treasureclassdesecratedchamp"s , "treasureclassdesecratedunique"s , "treasureclassherald"s,
             // clang-format on
         }) {
        for (const auto& diffSuffix : diffs) {
            result.push_back(code + diffSuffix);
        }
    }

    for (size_t i = 0; i < skCount; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("sk" + istr + "lvl");
    }

    std::vector<std::string> rest{
        // clang-format off
        "spawn", "minion1", "minion2",
        "enabled", "isspawn", "killable", "npc", "boss",
        "level", "mingrp", "maxgrp",
        "id", "baseid", "*hcidx",
        // clang-format on
    };
    result.insert(result.end(), rest.cbegin(), rest.cend());

    return result;
}

void DataRow_monstats::serialize()
{
    ser(resFi);
    ser(resLi);
    ser(resCo);
    ser(resPo);
    ser(resMa);
    ser(resDm);

    ser(tc);
    ser(skLevels, skCount);
    ser(minionIds);

    ser(enabled);
    ser(isspawn);
    ser(killable);
    ser(npc);
    ser(boss);

    ser(level);
    ser(minGrp);
    ser(maxGrp);

    ser(id);
    ser(baseId);
    ser(hcIdx);
}

std::map<std::string, std::string> DataRow_monstats::getColumnAliases() const
{
    using namespace std::literals;
    std::map<std::string, std::string> result;
    const std::array<std::string, 3>   diffs{ ""s, "(n)"s, "(h)"s };
    std::vector<std::string>           tcs{ "treasureclass"s, "treasureclasschamp"s, "treasureclassunique"s, "treasureclassquest"s };
    for (size_t i = 0; i < 4; ++i) {
        for (auto& diff : diffs) {
            std::string src  = "treasureclass"s + std::to_string(i + 1) + diff;
            std::string dest = tcs[i] + diff;
            result[src]      = dest;
        }
    }
    result["hcidx"] = "*hcidx";
    return result;
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

void DataRow_skilldesc::serialize()
{
    ser(skilldesc);
    ser(skillPage);
    ser(skillRow);
    ser(skillColumn);
    ser(listRow);
}

std::vector<std::string> DataRow_skills::getColumns() const
{
    // clang-format off
    return { "skill", "maxlvl", "reqlevel", "intown", "cost mult", "cost add" ,
             "elen", "hitshift", "etype", "charclass",
             "reqskill1", "reqskill2", "reqskill3"};
    // clang-format on
}

void DataRow_skills::serialize()
{
    ser(skill);
    ser(maxlvl);
    ser(lvlreq);
    ser(inTown);
    ser(costMult);
    ser(costAdd);

    ser(eLen);
    ser(hitShift);
    ser(eType);
    ser(charclass);

    ser(reqskills, 3);
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

void DataRow_treasureclassex::serialize()
{
    ser(name);
    ser(k_unique);
    ser(k_set);
    ser(k_rare);
    ser(k_magic);
    ser(noDrop);
    ser(group);
    ser(picks);
    ser(level);

    serArray(items, maxItems, [this](Item& item) {
        ser(item.prob);
        ser(item.tc); }, [](Item& item) { return !item.tc.empty(); });
}

int DataRow_treasureclassex::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : items)
        totalOther += item.prob.value_or(0);
    return totalOther;
}

}
