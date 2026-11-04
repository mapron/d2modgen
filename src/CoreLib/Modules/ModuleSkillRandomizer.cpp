/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleSkillRandomizer.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {

const StringSet s_skillsOkToRandomizeDamage{
    "fire bolt",
    "charged bolt",
    "ice bolt",
    "inferno",
    "static field",
    "frost nova",
    "ice blast",
    "blaze",
    "nova",
    "lightning",
    "fire wall",
    "chain lightning",
    "thunder storm",
    "blizzard",
    "hydra",
    "frozen orb",

    "fire arrow",
    "cold arrow",
    "ice arrow",

    "firestorm",
    "hurricane",
    "molten boulder",
    "arctic blast",
    "eruption",

    "blessed hammer",
    "fist of the heavens",

    "teeth",
    "bone spear",
    "bone spirit",
    "poison dagger",
    "poison nova",
    "poison explosion",

    "fire trauma",
    "shock field",
    "fists of fire",
    "charged bolt sentry",
    "wake of fire sentry",
    "claws of thunder",
    "lightning sentry",
    "inferno sentry",
    "blades of ice",
    "death sentry",
};

}

void ModuleSkillRandomizer::generate()
{
    if (!config.m_skillRandomizer.m_enabled)
        return;
    const auto& module = config.m_skillRandomizer;

    const bool skillTreeRandomize   = module.m_skillTree;
    const bool skillDamageRandomize = module.m_skillDamage;
    const bool ensureDifferent      = module.m_ensureDifferent;

    SkillParsedData skillParsedData;
    SkillTree       charTree;
    for (auto& row : output.skills.rows) {
        SkillInfo info;
        info.readFromRowMain(row);
        if (info.classSpecific == Character::None)
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
    const auto oldCopy = skillParsedData;

    charTree.parse(skillParsedData);

    if (skillTreeRandomize)
        charTree.randomizeTabs(rng);

    charTree.writeOutput(skillParsedData);

    if (skillDamageRandomize)
        charTree.randomizeDmg(skillParsedData, rng, ensureDifferent);

    {
        std::ostringstream os;
        for (const auto& [key, value] : skillParsedData) {
            const SkillInfo& oldValue = oldCopy.at(key);
            if (oldValue == value) {
                os << "    " << key << ": NOT_CHANGED\n";
            } else {
                os << "    " << key << ":\n"
                   << "        " << oldValue.toString() << "\n"
                   << "        " << value.toString() << "\n";
            }
        }

        Logger() << "changes in skill data: \n"
                 << os.str();
    }

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

void ModuleSkillRandomizer::SkillInfo::readFromRowMain(const DataRow_skills& row)
{
    uppercaseKey  = row.skill;
    lowercaseKey  = Mernel::strToLower(row.skill);
    classSpecific = row.classSpecific;
    loc.reqlevel  = row.lvlreq.value_or(0);
    reqSkills     = row.reqskills;
    dmg.eLen      = row.eLen;
    dmg.hitShift  = row.hitShift.value_or(0);
    dmg.eType.set(row.eType);
}

void ModuleSkillRandomizer::SkillInfo::readFromRowDesc(const DataRow_skilldesc& row)
{
    loc.skillPage   = row.skillPage.value_or(0);
    loc.skillRow    = row.skillRow.value_or(0);
    loc.skillColumn = row.skillColumn.value_or(0);
    loc.listRow     = row.listRow.value_or(0);
}

void ModuleSkillRandomizer::SkillInfo::writeToRowMain(DataRow_skills& row)
{
    row.lvlreq    = loc.reqlevel;
    row.reqskills = reqSkills;
    row.eLen      = dmg.eLen;
    row.hitShift  = dmg.hitShift;
    row.eType     = dmg.eType.toString();
}

void ModuleSkillRandomizer::SkillInfo::writeToRowDesc(DataRow_skilldesc& row)
{
    row.skillPage   = loc.skillPage;
    row.skillRow    = loc.skillRow;
    row.skillColumn = loc.skillColumn;
    row.listRow     = loc.listRow;
}

std::string ModuleSkillRandomizer::SkillInfo::toString() const
{
    std::ostringstream os;
    os
        << "{"
        //<< " key=" << lowercaseKey
        << " class=" << enumToString(classSpecific)
        << ", loc=" << loc.toString()
        << ", dmg=" << dmg.toString()
        << ", req=[";

    for (auto& req : reqSkills)
        os << req << ", ";

    os << "] }";
    return os.str();
}

int ModuleSkillRandomizer::SkillTree::CharacterData::addSkillId(const std::string& skill)
{
    auto it = std::find(skillIds.cbegin(), skillIds.cend(), skill);

    int idx = it == skillIds.cend() ? -1 : it - skillIds.cbegin();
    if (idx != -1)
        return idx;
    idx = skillIds.size();
    skillIds << skill;
    return idx;
}

void ModuleSkillRandomizer::SkillTree::parse(const SkillParsedData& data)
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

void ModuleSkillRandomizer::SkillTree::randomizeTabs(RandomGenerator& rng)
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

void ModuleSkillRandomizer::SkillTree::randomizeDmg(SkillParsedData& data, RandomGenerator& rng, const bool ensureDifferent)
{
    // Physical IS NOT ALLOWED in skills.
    const auto                   knownElements = std::vector{ Element::Ltng, Element::Fire, Element::Cold, Element::Mag, Element::Pois };
    const std::map<Element, int> compensateBitShift{
        { Element::Ltng, 0 },
        { Element::Fire, 0 },
        { Element::Cold, 0 },
        { Element::Mag, -2 }, // Magic damage is unresisted, so the skill needs to be weaker to compensate
        { Element::Pois, 2 }, // compensate 4 times for poison
    };
    const std::map<Element, int> defaultDuration{
        { Element::Ltng, 0 },
        { Element::Fire, 0 },
        { Element::Cold, 50 },
        { Element::Mag, 0 },
        { Element::Pois, 100 },
    };

    for (auto& p : data) {
        SkillInfo& skillInfo = p.second;
        if (!s_skillsOkToRandomizeDamage.contains(skillInfo.lowercaseKey))
            continue;

        const auto oldE            = skillInfo.dmg.eType.e;
        const int  oldCompensation = compensateBitShift.at(oldE);

        const int neutralhitShift = skillInfo.dmg.hitShift - oldCompensation;

        // shuffle
        {
            auto elementsTmp = knownElements;
            if (ensureDifferent) {
                auto it = std::find(elementsTmp.begin(), elementsTmp.end(), skillInfo.dmg.eType.e);
                if (it != elementsTmp.end())
                    elementsTmp.erase(it);
            }
            skillInfo.dmg.eType.e = elementsTmp[rng((int) elementsTmp.size())];
        }

        const auto newE            = skillInfo.dmg.eType.e;
        const int  newCompensation = compensateBitShift.at(newE);

        skillInfo.dmg.hitShift = std::clamp(neutralhitShift + newCompensation, 0, 15);

        const int duration = defaultDuration.at(newE);
        if (duration > 0) {
            if (!skillInfo.dmg.eLen.has_value())
                skillInfo.dmg.eLen = duration;
        } else {
            skillInfo.dmg.eLen.reset();
        }
    }
}

void ModuleSkillRandomizer::SkillTree::writeOutput(SkillParsedData& data) const
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

std::string ModuleSkillRandomizer::SkillLocation::toString() const
{
    std::ostringstream os;
    os
        << "{ pag=" << skillPage
        << ", row=" << skillRow
        << ", col=" << skillColumn
        << ", lro=" << listRow
        << ", req=" << reqlevel << " }";
    return os.str();
}

std::string ModuleSkillRandomizer::SkillDamageInfo::toString() const
{
    std::ostringstream os;
    os
        << "{ eType=" << eType.toString()
        << ", eLen=" << eLen.value_or(-1)
        << ", hitShift=" << hitShift << "}";
    return os.str();
}

void ModuleSkillRandomizer::EType::set(const std::string& str)
{
    if (str.empty()) {
        extra = "";
        e     = Element::None;
    } else {
        e = getElementId(str);
        if (e == Element::None)
            extra = str; // to preserve unknown elem like "stam" damage
    }
}

std::string ModuleSkillRandomizer::EType::toString() const
{
    if (e != Element::None) {
        return enumToString(e);
    }
    return extra;
}

}
