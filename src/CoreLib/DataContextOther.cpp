/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextOther.hpp"

#include <cassert>

namespace D2ModGen {

namespace {
const std::vector<std::string> s_modeNames{
    "DT",
    "NU",
    "WL",
    "GH",
    "A1",
    "A2",
    "BL",
    "SC",
    "S1",
    "S2",
    "S3",
    "S4",
    "DD",
    "KB",
    "SQ",
    "RN",
};

}

AnimationMode animationModeFromString(const std::string& mode)
{
    for (size_t i = 0; i < s_modeNames.size(); i++) {
        if (s_modeNames[i] == mode) {
            return AnimationMode(i);
        }
    }
    return AnimationMode::Undefined;
}

std::string animationModeToString(AnimationMode mode)
{
    if (mode == AnimationMode::Undefined)
        return "";
    return s_modeNames[static_cast<int>(mode)];
}

std::vector<std::string> DataRow_charstats::getColumns() const
{
    std::vector<std::string> result{ "class", "statperlevel", "skillsperlevel", "lifeperlevel", "manaperlevel", "healthpotionpercent", "manapotionpercent" };
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
    ser(hpPerLevel);
    ser(manaPerLevel);
    ser(hpPotEfficacy);
    ser(manaPotEfficacy);
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
        "id", "name", "nummon",
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
    ser(id, -1);
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

    ser(level, -1);
}

std::vector<std::string> DataRow_monseq::getColumns() const
{
    return {
        "mode", "sequence", "frame", "dir", "event", "*eol"
    };
}

std::map<std::string, std::string> DataRow_monseq::getColumnAliases() const
{
    std::map<std::string, std::string> result;
    result["eol"] = "*eol";
    return result;
}

void DataRow_monseq::serialize()
{
    serEnum(mode, AnimationMode::Undefined, animationModeFromString, animationModeToString);
    ser(sequence);
    ser(frame);
    ser(dir);
    ser(event);
    ser(eol);
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
        result.push_back("skill" + istr);
        result.push_back("sk" + istr + "mode");
        result.push_back("sk" + istr + "lvl");
    }
    for (size_t i = 0; i < aiParCount; ++i) {
        auto istr = std::to_string(i + 1);
        for (const auto& diffSuffix : diffs) {
            result.push_back("aip" + istr + diffSuffix);
        }
    }

    for (const auto& diffSuffix : diffs) {
        for (const std::string& code : {
                 // clang-format off
                                         "minhp"s, "maxhp"s, "ac"s, "exp"s, "a1mind"s, "a1maxd"s, "a1th"s,
                                         "a2mind"s, "a2maxd"s, "a2th"s, "s1mind"s, "s1maxd"s, "s1th"s,
                                         "el1pct"s, "el1mind"s, "el1maxd"s, "el1dur"s
                 // clang-format on
             }) {
            result.push_back(code + diffSuffix);
        }
    }

    std::vector<std::string> rest{
        // clang-format off
        "velocity", "run", "toblock", "el1mode", "el1type",
        "spawn", "minion1", "minion2",
        "enabled", "isspawn", "killable", "npc", "boss",
        "rangedtype", "ismelee", "flying",
        "level", "mingrp", "maxgrp",
        "id", "baseid", "*hcidx",
        "monstatsex", "ai",
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
    serArray(skills, skCount, [this](Skill& item) {
        ser(item.name);
        ser(item.mode);
        ser(item.level); }, [](Skill& item) { return !item.name.empty(); });

    ser(aipar);
    for (size_t i = 0; i < 3; ++i) {
        ser(battleStats[i].minHP);
        ser(battleStats[i].maxHP);
        ser(battleStats[i].AC);
        ser(battleStats[i].exp);
        ser(battleStats[i].A1minD);
        ser(battleStats[i].A1maxD);
        ser(battleStats[i].A1TH);
        ser(battleStats[i].A2minD);
        ser(battleStats[i].A2maxD);
        ser(battleStats[i].A2TH);
        ser(battleStats[i].S1minD);
        ser(battleStats[i].S1maxD);
        ser(battleStats[i].S1TH);
        ser(battleStats[i].el1Pct);
        ser(battleStats[i].el1MinD);
        ser(battleStats[i].el1MaxD);
        ser(battleStats[i].el1Dur);
    }
    ser(walkSpeed);
    ser(runSpeed);
    ser(toBlock);
    ser(el1Mode);
    ser(el1Type);

    ser(minionIds);

    ser(enabled);
    ser(isspawn);
    ser(killable);
    ser(npc);
    ser(boss);

    ser(rangedtype);
    ser(melee);
    ser(flying);

    ser(level);
    ser(minGrp);
    ser(maxGrp);

    ser(id);
    ser(baseId);
    ser(hcIdx, -1);

    ser(monStatsEx);
    ser(AI);
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

bool DataRow_monstatsEx::hasMode(AnimationMode mode) const
{
    if (mode == AnimationMode::Undefined)
        return false;
    return modes[static_cast<int>(mode)].value_or(false);
}

AnimationMode DataRow_monstatsEx::getSafeSupportedMode() const
{
    for (AnimationMode safeMode : { AnimationMode::A1, AnimationMode::A2, AnimationMode::NU }) {
        if (hasMode(safeMode)) {
            return safeMode;
        }
    }
    assert(0);
    return AnimationMode::Undefined;
}

std::vector<std::string> DataRow_monstatsEx::getColumns() const
{
    // clang-format off
    std::vector<std::string> result{
        "id", "hitclass", "*hcidx",
        "mdt", "mnu", // D2R
        "mwl", "mgh",
        "ma1", "ma2", "mbl", "msc",
        "ms1", "ms2", "ms3", "ms4",
        "mdd", "mkb", "msq", "mrn",
    };
    // clang-format on

    return result;
}

void DataRow_monstatsEx::serialize()
{
    ser(id);
    ser(hitClass);
    ser(hcIdx, -1);
    for (size_t i = 0; i < 16; ++i)
        ser(modes[i], true);
}

std::map<std::string, std::string> DataRow_monstatsEx::getColumnAliases() const
{
    std::map<std::string, std::string> result;
    result["hcidx"] = "*hcidx";
    return result;
}

std::vector<std::string> DataRow_skilldesc::getColumns() const
{
    std::vector<std::string> result{
        "skilldesc",
        "skillpage",
        "skillrow",
        "skillcolumn",
        "listrow",
        "str name",
    };
    for (size_t i = 0; i < maxDesc1; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("desccalca" + istr);
        result.push_back("desccalcb" + istr);
    }
    for (size_t i = 0; i < maxDesc2; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("dsc2calca" + istr);
        result.push_back("dsc2calcb" + istr);
    }
    for (size_t i = 0; i < maxDesc3; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("dsc3calca" + istr);
        result.push_back("dsc3calcb" + istr);
    }
    for (size_t i = 0; i < maxDesc3; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("dsc3texta" + istr);
        result.push_back("dsc3textb" + istr);
    }

    return result;
}

void DataRow_skilldesc::serialize()
{
    ser(skilldesc);
    ser(skillPage);
    ser(skillRow);
    ser(skillColumn);
    ser(listRow);
    ser(strName);

    ser(calc);
    ser(desc3);
}

std::vector<std::string> DataRow_skills::getColumns() const
{
    std::vector<std::string> result{
        // clang-format off
        "skill", "*id", "skilldesc", "maxlvl", "reqlevel", "intown", "cost mult", "cost add" ,
        "elen", "hitshift", "etype", "charclass",
        "itypea1",
        "reqskill1", "reqskill2", "reqskill3",
        
        "tohitcalc",
        "dmgsympercalc",
        "edmgsympercalc",
        "elensympercalc",
        "auralencalc",
        "aurarangecalc",
        // clang-format on
    };
    for (size_t i = 0; i < 6; ++i)
        result.push_back("aurastatcalc" + std::to_string(i + 1));
    for (size_t i = 0; i < 14; ++i)
        result.push_back("passivecalc" + std::to_string(i + 1));
    for (size_t i = 0; i < 3; ++i)
        result.push_back("prgcalc" + std::to_string(i + 1));
    for (size_t i = 0; i < 5; ++i)
        result.push_back("sumsk" + std::to_string(i + 1) + "calc");
    for (size_t i = 0; i < 3; ++i)
        result.push_back("cltcalc" + std::to_string(i + 1));
    for (size_t i = 0; i < 10; ++i)
        result.push_back("calc" + std::to_string(i + 1));

    return result;
}

void DataRow_skills::serialize()
{
    ser(skill);
    ser(id, -1);
    ser(skilldesc);
    ser(maxlvl);
    ser(lvlreq);
    ser(inTown);
    ser(costMult);
    ser(costAdd);

    ser(eLen);
    ser(hitShift);
    ser(eType);
    ser(charclass);
    ser(itypea1);

    ser(reqskills, 3);

    ser(calc);
}

std::map<std::string, std::string> DataRow_skills::getColumnAliases() const
{
    return { { "id", "*id" } };
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
