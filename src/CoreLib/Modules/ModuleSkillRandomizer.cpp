/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleSkillRandomizer.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleSkillRandomizer>();

const StringSet s_skillsOkToRandomizeDamage{
    "Fire Bolt",
    "Charged Bolt",
    "Ice Bolt",
    "Inferno",
    "Static Field",
    "Frost Nova",
    "Ice Blast",
    "Blaze",
    "Nova",
    "Lightning",
    "Fire Wall",
    "Chain Lightning",
    "Thunder Storm",
    "Blizzard",
    "Hydra",
    "Frozen Orb",

    "Fire Arrow",
    "Cold Arrow",
    "Ice Arrow",

    "Firestorm",
    "Hurricane",
    "Molten Boulder",
    "Arctic Blast",
    "Eruption",

    "Blessed Hammer",
    "Fist of the Heavens",

    "Teeth",
    "Bone Spear",
    "Bone Spirit",
    "Poison Dagger",
    "Poison Nova",
    "Poison Explosion",

    "Fire Trauma",
    "Shock Field",
    "Fists of Fire",
    "Charged Bolt Sentry",
    "Wake of Fire Sentry",
    "Claws of Thunder",
    "Lightning Sentry",
    "Inferno Sentry",
    "Blades of Ice",
    "Death Sentry",
};

struct SkillLocation {
    int skillPage   = 0;
    int skillRow    = 0;
    int skillColumn = 0;
    int listRow     = 0;
    int reqlevel    = 0;
};

struct SkillDamageInfo {
    std::string EType;
    int         ELen;
    int         HitShift;
};

struct SkillInfo {
    std::string     uppercaseKey;
    std::string     lowercaseKey;
    std::string     charClass; // ass|bar|...
    SkillLocation   loc;
    SkillDamageInfo dmg;
    StringVector    reqSkills;

    void readFromRowMain(const TableView::RowView& row)
    {
        uppercaseKey = row["skill"].str;
        lowercaseKey = row["skill"].toLower();
        charClass    = row["charclass"].str;
        loc.reqlevel = row["reqlevel"].toInt();
        for (int i = 1; i <= 3; ++i) {
            const auto& req = row[argCompat("reqskill%1", i)];
            if (!req.isEmpty())
                reqSkills << req.str;
        }
        dmg.ELen     = row["ELen"].toInt();
        dmg.HitShift = row["HitShift"].toInt();
        dmg.EType    = row["EType"].str;
    }
    void readFromRowDesc(const TableView::RowView& row)
    {
        loc.skillPage   = row["SkillPage"].toInt();
        loc.skillRow    = row["SkillRow"].toInt();
        loc.skillColumn = row["SkillColumn"].toInt();
        loc.listRow     = row["ListRow"].toInt();
    }
    void writeToRowMain(TableView::RowView& row)
    {
        row["reqlevel"] = argCompat("%1", loc.reqlevel);
        for (int i = 1; i <= 3; ++i) {
            auto& req = row[argCompat("reqskill%1", i)];
            req.str   = reqSkills.size() >= i ? reqSkills[i - 1] : "";
        }
        row["ELen"]     = dmg.ELen ? argCompat("%1", dmg.ELen) : "";
        row["HitShift"] = argCompat("%1", dmg.HitShift);
        row["EType"]    = dmg.EType;
    }
    void writeToRowDesc(TableView::RowView& row)
    {
        row["SkillPage"]   = argCompat("%1", loc.skillPage);
        row["SkillRow"]    = argCompat("%1", loc.skillRow);
        row["SkillColumn"] = argCompat("%1", loc.skillColumn);
        row["ListRow"]     = argCompat("%1", loc.listRow);
    }
};

using SkillParsedData = std::map<std::string, SkillInfo>;

struct SkillTree {
    struct Character {
        struct Skill {
            SkillLocation    loc;
            int              skillId;
            std::vector<int> reqSkillIds;
        };
        std::vector<Skill> skills;
        std::string        charClass;
        StringVector       skillIds;

        int addSkillId(const std::string& skill)
        {
            auto it = std::find(skillIds.cbegin(), skillIds.cend(), skill);

            int idx = it == skillIds.cend() ? -1 : it - skillIds.cbegin();
            if (idx != -1)
                return idx;
            idx = skillIds.size();
            skillIds << skill;
            return idx;
        }
    };
    std::map<std::string, Character> chars;

    void parse(const SkillParsedData& data)
    {
        for (const auto& p : data) {
            const SkillInfo& skillInfo = p.second;
            Character&       c         = chars[skillInfo.charClass];
            c.charClass                = skillInfo.charClass;

            Character::Skill s;
            s.loc     = skillInfo.loc;
            s.skillId = c.addSkillId(skillInfo.uppercaseKey);
            for (const std::string& req : skillInfo.reqSkills)
                s.reqSkillIds.push_back(c.addSkillId(req));
            c.skills.push_back(std::move(s));
        }
    }

    void randomizeTabs(RandomGenerator& rng)
    {
        for (auto& p : chars) {
            Character&   c      = p.second;
            StringVector source = c.skillIds;
            c.skillIds.clear();
            while (source.size() > 1) {
                auto i = rng(static_cast<int>(source.size()));
                c.skillIds << source[i];
                source.erase(source.begin() + i);
            }
            c.skillIds << source;
        }
    }

    void randomizeDmg(SkillParsedData& data, RandomGenerator& rng, const bool ensureDifferent)
    {
        const StringVector elements{ "ltng", "fire", "cold", "mag", "pois" };

        for (auto& p : data) {
            SkillInfo& skillInfo = p.second;
            if (!s_skillsOkToRandomizeDamage.contains(skillInfo.uppercaseKey))
                continue;
            const bool wasPoison = skillInfo.dmg.EType == "pois";
            {
                StringVector elementsTmp = elements;
                if (ensureDifferent) {
                    auto it = std::find(elementsTmp.begin(), elementsTmp.end(), skillInfo.dmg.EType);
                    if (it != elementsTmp.end())
                        elementsTmp.erase(it);
                }
                skillInfo.dmg.EType = elementsTmp[rng((int) elementsTmp.size())];
            }
            const bool isPoison = skillInfo.dmg.EType == "pois";
            if (isPoison) {
                skillInfo.dmg.ELen = 50;
                skillInfo.dmg.HitShift -= 4;
                skillInfo.dmg.HitShift = std::max(0, skillInfo.dmg.HitShift);
            }
            if (wasPoison) {
                skillInfo.dmg.HitShift += 4;
                skillInfo.dmg.HitShift = std::min(8, skillInfo.dmg.HitShift);
            }
        }
    }

    void writeOutput(SkillParsedData& data) const
    {
        for (const auto& p : chars) {
            const Character& c = p.second;
            for (const Character::Skill& s : c.skills) {
                auto       skillId   = c.skillIds[s.skillId];
                SkillInfo& skillInfo = data[toLower(skillId)];
                skillInfo.loc        = s.loc;
                skillInfo.reqSkills.clear();
                for (int req : s.reqSkillIds)
                    skillInfo.reqSkills << c.skillIds[req];
            }
        }
    }
};

}

PropertyTreeScalarMap ModuleSkillRandomizer::defaultValues() const
{
    return {
        { "skillTree", true },
        { "skillDamage", true },
        { "ensureDifferent", true },
    };
}

void ModuleSkillRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool skillTreeRandomize   = input.getInt("skillTree");
    const bool skillDamageRandomize = input.getInt("skillDamage");
    const bool ensureDifferent      = input.getInt("ensureDifferent");

    Table&          skillsTable = output.tableSet.tables["skills"];
    TableView       skillsTableView(skillsTable, true);
    Table&          descTable = output.tableSet.tables["skilldesc"];
    TableView       descTableView(descTable, true);
    SkillParsedData skillParsedData;
    SkillTree       charTree;
    for (auto& row : skillsTableView) {
        SkillInfo info;
        info.readFromRowMain(row);
        if (info.charClass.empty())
            continue;

        skillParsedData[info.lowercaseKey] = std::move(info);
    }
    for (auto& row : descTableView) {
        auto& skilldescKey = row["skilldesc"].str;
        if (!skillParsedData.contains(skilldescKey))
            continue;
        skillParsedData[skilldescKey].readFromRowDesc(row);
    }
    // transfrom data
    charTree.parse(skillParsedData);
    if (skillTreeRandomize)
        charTree.randomizeTabs(rng);
    charTree.writeOutput(skillParsedData);
    if (skillDamageRandomize)
        charTree.randomizeDmg(skillParsedData, rng, ensureDifferent);
    // write out
    for (auto& row : skillsTableView) {
        const auto& skilldescKey = row["skill"].toLower();
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowMain(row);
    }
    for (auto& row : descTableView) {
        const auto& skilldescKey = row["skilldesc"].toLower();
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowDesc(row);
    }
}

}
