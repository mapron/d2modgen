/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleSkillRandomizer.hpp"
#include "MernelPlatform/StringUtils.hpp"

namespace D2ModGen {

namespace {

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
    Character       classSpecific; // ass|bar|...
    SkillLocation   loc;
    SkillDamageInfo dmg;
    StringVector    reqSkills;

    void readFromRowMain(const DataRow_skills& row)
    {
        uppercaseKey  = row.skill;
        lowercaseKey  = Mernel::strToLower(row.skill);
        classSpecific = row.classSpecific;
        loc.reqlevel  = row.lvlreq.value_or(0);
        reqSkills     = row.reqskills;
        dmg.ELen      = row.eLen.value_or(0);
        dmg.HitShift  = row.hitShift.value_or(0);
        dmg.EType     = row.eType;
    }
    void readFromRowDesc(const DataRow_skilldesc& row)
    {
        loc.skillPage   = row.skillPage.value_or(0);
        loc.skillRow    = row.skillRow.value_or(0);
        loc.skillColumn = row.skillColumn.value_or(0);
        loc.listRow     = row.listRow.value_or(0);
    }
    void writeToRowMain(DataRow_skills& row)
    {
        row.lvlreq    = loc.reqlevel;
        row.reqskills = reqSkills;
        row.eLen      = dmg.ELen;
        if (!row.eLen.value())
            row.eLen.reset();
        row.hitShift = dmg.HitShift;
        row.eType    = dmg.EType;
    }
    void writeToRowDesc(DataRow_skilldesc& row)
    {
        row.skillPage   = loc.skillPage;
        row.skillRow    = loc.skillRow;
        row.skillColumn = loc.skillColumn;
        row.listRow     = loc.listRow;
    }
};

using SkillParsedData = std::map<std::string, SkillInfo>;

struct SkillTree {
    struct CharacterData {
        struct Skill {
            SkillLocation    loc;
            int              skillId;
            std::vector<int> reqSkillIds;
        };
        std::vector<Skill> skills;
        Character          classSpecific;
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
    std::map<Character, CharacterData> chars;

    void parse(const SkillParsedData& data)
    {
        for (const auto& p : data) {
            const SkillInfo& skillInfo = p.second;
            CharacterData&   c         = chars[skillInfo.classSpecific];
            c.classSpecific            = skillInfo.classSpecific;

            CharacterData::Skill s;
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
            CharacterData& c      = p.second;
            StringVector   source = c.skillIds;
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
            const CharacterData& c = p.second;
            for (const CharacterData::Skill& s : c.skills) {
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

ModuleSkillRandomizer::ModuleSkillRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "ensureDifferent", Mernel::PropertyTreeScalar{ true } },
          { "skillDamage", Mernel::PropertyTreeScalar{ true } },
          { "skillTree", Mernel::PropertyTreeScalar{ true } },
      })
{}

void ModuleSkillRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool skillTreeRandomize   = input.getInt("skillTree");
    const bool skillDamageRandomize = input.getInt("skillDamage");
    const bool ensureDifferent      = input.getInt("ensureDifferent");

    SkillParsedData skillParsedData;
    SkillTree       charTree;
    for (auto& row : output.skills.rows) {
        SkillInfo info;
        info.readFromRowMain(row);
        if (info.classSpecific != Character::none)
            continue;

        skillParsedData[info.lowercaseKey] = std::move(info);
    }
    for (auto& row : output.skilldesc.rows) {
        const auto& skilldescKey = Mernel::strToLower(row.skilldesc);
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
    for (auto& row : output.skills.rows) {
        const auto& skilldescKey = Mernel::strToLower(row.skill);
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowMain(row);
    }
    for (auto& row : output.skilldesc.rows) {
        const auto& skilldescKey = Mernel::strToLower(row.skilldesc);
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowDesc(row);
    }
}

}
