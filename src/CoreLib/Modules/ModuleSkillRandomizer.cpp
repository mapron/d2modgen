/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleSkillRandomizer.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include "Logger.hpp"
#include <numeric>

namespace D2ModGen {

namespace {

using Replacer = std::function<std::string(std::string)>;

void replaceTokensInplace(std::string&       str,
                          Replacer&&         replacer,
                          const std::string& start_token = "skill('",
                          const std::string& end_token   = "'.")
{
    if (str.empty() || start_token.empty() || end_token.empty()) {
        return;
    }

    size_t current_pos = 0;

    while (true) {
        const size_t start_pos = str.find(start_token, current_pos);
        if (start_pos == std::string::npos)
            break;

        const size_t content_start = start_pos + start_token.length();
        const size_t end_pos       = str.find(end_token, content_start);
        if (end_pos == std::string::npos)
            break;

        const size_t      key_length         = end_pos - content_start;
        const std::string extracted_key      = str.substr(content_start, key_length);
        const size_t      total_block_length = (end_pos + end_token.length()) - start_pos;

        const std::string newValue = replacer(extracted_key);

        const auto value = start_token + newValue + end_token;
        str.replace(start_pos, total_block_length, value);
        current_pos = start_pos + value.length();
    }
}

void splitWeights(int originalValue, const std::vector<int>& weights, std::vector<int>& result)
{
    assert(!weights.empty());
    if (originalValue == 0) {
        result.resize(weights.size(), 0);
        return;
    }

    const int sumWeights = std::accumulate(weights.begin(), weights.end(), 0);

    if (sumWeights == 0) {
        const std::vector<int> weights2(weights.size(), 1);
        splitWeights(originalValue, weights2, result);
        return;
    }

    result.resize(weights.size(), 0);

    std::vector<std::pair<double, int*>> remainders;
    remainders.reserve(weights.size());

    int currentSum = 0;

    for (size_t i = 0; i < weights.size(); ++i) {
        const double exact = static_cast<double>(originalValue) * weights[i] / sumWeights;

        const int baseVal = static_cast<int>(exact);

        result[i] = baseVal;
        currentSum += baseVal;

        const double remainder = exact - baseVal;
        remainders.push_back({ remainder, &result[i] });
    }

    const int remainderToDistribute = originalValue - currentSum;

    std::sort(remainders.begin(), remainders.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    const int limit = std::min(remainderToDistribute, static_cast<int>(weights.size()));
    for (int i = 0; i < limit; ++i)
        (*remainders[i].second)++;

    const int finalSum = std::accumulate(result.begin(), result.end(), 0);
    if (finalSum != originalValue && !result.empty()) {
        result[0] += (originalValue - finalSum);
    }
}

const StringSet s_bannedRandoPickSkills{
    "fire mastery",
    "cold mastery",
    "lightning mastery",
    "frenzy",
    "double swing",
    "double throw",
};

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

const StringSet s_skippedSkills{
    //ama
    "charged strike",
    "exploding arrow",
    "freezing arrow",
    "immolation arrow",
    "lightning bolt",
    "lightning fury",
    "lightning strike",
    "plague javelin",
    "poison javelin",
    "power strike",

    //sor
    "chilling armor",
    "enchant",
    "fire ball",
    "fire mastery",
    "glacial spike",
    "meteor",
    "shiver armor",
    "telekinesis",
    "warmth",

    //nec
    "corpse explosion",
    "firegolem",

    //pal
    "holy bolt",
    "holy fire",
    "holy freeze",
    "holy shock",
    "sanctuary",
    "vengeance",

    //bar
    "berserk",
    "concentrate",
    "frenzy",

    //dru
    "armageddon",
    "fire claws",
    "plague poppy",
    "rabies",
    "summon spirit wolf",
    "volcano",

    //ass
    "dragon tail",
    "psychic hammer",
    "venom",

    //war
    "abyss",
    "apocalypse",
    "blade warp",
    "blood boil",
    "eldritch blast",
    "flame wave",
    "hex bane",
    "hex purge",
    "miasma bolt",
    "miasma chains",
    "ring of fire",
    "sigil death",
    "summon tainted",
};

const StringSet s_skippedSkills_PD2{

    //ama
    "dopplezon",
    "guided arrow",
    "inner sight",
    "magic arrow",
    "shattering arrow",
    "slow movement",
    "valkyrie",

    //sor
    "cold enchant",
    "cold mastery",
    "combustion",
    "energy shield",
    "ice barrage",
    "lesser hydra",
    "lightning mastery",
    "teleport",

    //nec
    "ampdmg",
    "attract",
    "bone armor",
    "bone prison",
    "bone wall",
    "clay golem",
    "confuse",
    "dark pact",
    "decrepify",
    "desecrate",
    "dim vision",
    "golem mastery",
    "iron maiden",
    "irongolem",
    "life tap",
    "lowres",
    "raise skeletal mage",
    "raise skeleton",
    "raise skeleton archer",
    "revive",
    "skeleton mastery",
    "terror",
    "weaken",

    //pal
    "cleansing",
    "holy light",
    "holy nova",
    "holy shield",
    "holy sword",
    "meditation",
    "resist cold",
    "resist fire",
    "resist lightning",

    //dru
    "gust",
    "raven",
    "vines",

    //ass
    "chain lightning sentry",
    "cobra strike",
};

const auto s_knownElements = std::vector{ Element::Ltng, Element::Fire, Element::Cold, Element::Mag, Element::Pois };

const std::map<Element, int> s_compensateBitShift{
    { Element::Ltng, 0 },
    { Element::Fire, 0 },
    { Element::Cold, 0 },
    { Element::Mag, -1 }, // Magic damage is unresisted, so the skill needs to be weaker to compensate
    { Element::Pois, 2 }, // compensate 4 times for poison
};
const std::map<Element, int> s_defaultDuration{
    { Element::Ltng, 0 },
    { Element::Fire, 0 },
    { Element::Cold, 50 },
    { Element::Mag, 0 },
    { Element::Pois, 100 },
};

}

void ModuleSkillRandomizer::generate()
{
    const auto& module = config.m_skillRandomizer;
    if (!module.m_enabled)
        return;

    const int totalNewSkillsPerClass = 60;

    const int keepSkillCount = module.m_randoMode == 1 ? 0 : module.m_keepOriginal;
    const int generateCount  = module.m_generate;

    for (Character clas : getClassTypes()) {
        CharacterData data;
        data.clas = clas;
        if (data.readExisting(output))
            existingData[clas] = std::move(data);
    }

    auto getClassWeight = [&module](Character clas) -> int {
        using enum Character;
        switch (clas) {
                // clang-format off
            case Ama: return module.m_weightAma;
            case Sor: return module.m_weightSor;
            case Nec: return module.m_weightNec;
            case Pal: return module.m_weightPal;
            case Bar: return module.m_weightBar;
            case Dru: return module.m_weightDru;
            case Ass: return module.m_weightAss;
            case War: return module.m_weightWar;
                // clang-format on
            case None:
                break;
        }
        return 0;
    };

    generatedData = existingData;

    for (auto& [key, cha] : generatedData) {
        if (module.m_randoMode > 0) {
            cha.makeInactive(rng, keepSkillCount);
        }
        if (module.m_randoMode == 1) {
            // same class shuffle
            for (auto& [sourceKey, sourceData] : existingData) {
                if (sourceKey != key)
                    continue;
                cha.generateAdditionalSkills(rng, sourceData, generateCount);
            }
        }
        if (module.m_randoMode == 2) {
            // pick from class
            for (auto& [sourceKey, sourceData] : existingData) {
                if ((int) sourceKey != module.m_specificClass)
                    continue;
                const int characterGenerateCount = generateCount;
                cha.generateAdditionalSkills(rng, sourceData, characterGenerateCount);
            }
        }
        if (module.m_randoMode == 3) {
            // pick from any class
            std::vector<int> weights;

            for (auto& [sourceKey, sourceData] : existingData) {
                weights.push_back(getClassWeight(sourceKey));
            }
            std::vector<int> counts;
            splitWeights(generateCount, weights, counts);
            for (size_t i = 0; auto& [sourceKey, sourceData] : existingData) {
                const int characterGenerateCount = counts[i++];
                cha.generateAdditionalSkills(rng, sourceData, characterGenerateCount);
            }
        }

        cha.insertRows(output, totalNewSkillsPerClass);
        cha.createIdx();
        output.skills.updateIndex();
        output.skilldesc.updateIndex();
        if (module.m_randoMode > 0) {
            cha.randomizePositions(rng);
            cha.updateSkillReq(module);
            cha.updateSkillNameRef(output, rng);
        }
        cha.randomizeElements(rng, module.m_maximumElements, module.m_chanceElemChange);
    }

    for (auto& [key, cha] : generatedData) {
        cha.writeOut(output);
    }
}

std::vector<size_t> ModuleSkillRandomizer::CharacterData::getIndexesByType(SType stype, SType stype2) const
{
    std::vector<size_t> result;
    for (size_t i = 0; i < skills.size(); ++i) {
        bool matchFilter = false;
        if (skills[i].stype == stype)
            matchFilter = true;
        if (stype2 != SType::Filler && skills[i].stype == stype2)
            matchFilter = true;
        if (matchFilter)
            result.push_back(i);
    }
    return result;
}

ModuleSkillRandomizer::PtrList ModuleSkillRandomizer::CharacterData::getActive()
{
    PtrList activeSkills;
    for (SkillInfo& skill : skills) {
        if (skill.stype == SType::ExistingActive || skill.stype == SType::GenActive) {
            activeSkills.push_back(&skill);
        }
    }
    return activeSkills;
}

bool ModuleSkillRandomizer::CharacterData::readExisting(DataContext& output)
{
    int validCnt = 0;
    for (int rowIdx = -1; auto& row : output.skills.rows) {
        rowIdx++;
        if (row.classSpecific != clas)
            continue;

        validCnt++;
        SkillInfo skill;
        skill.rowIdx     = rowIdx;
        skill.row        = row;
        skill.descIdx    = output.skilldesc.indexN.at(Mernel::strToLower(row.skilldesc));
        skill.desc       = output.skilldesc.rows[skill.descIdx];
        skill.originalId = row.skill;

        if (!skill.desc.isValid()) {
            skill.stype = SType::ExistingHidden;
        } else {
            skill.stype = SType::ExistingActive;
        }
        skills.push_back(skill);
    }
    return validCnt > 0;
}

void ModuleSkillRandomizer::CharacterData::writeOut(DataContext& output) const
{
    for (const SkillInfo& skill : skills) {
        if (skill.stype < SType::GenHidden)
            continue;
        output.skills.rows[skill.rowIdx]     = skill.row;
        output.skilldesc.rows[skill.descIdx] = skill.desc;
    }
}

void ModuleSkillRandomizer::CharacterData::makeInactive(RandomGenerator& rng, int keepCnt)
{
    auto exisitingActive = getIndexesByType(SType::ExistingActive);

    while (exisitingActive.size() > keepCnt) {
        auto  r = rng(static_cast<int>(exisitingActive.size()));
        auto  i = exisitingActive[r];
        auto& s = skills[i];
        s.desc.makeHidden();
        s.stype = SType::GenHidden;

        exisitingActive.erase(exisitingActive.begin() + r);
    }
}

void ModuleSkillRandomizer::CharacterData::generateAdditionalSkills(RandomGenerator& rng, const CharacterData& someOtherClass, int generateCount)
{
    auto incoming = someOtherClass.getIndexesByType(SType::ExistingActive);
    for (int i = int(incoming.size()) - 1; i >= 0; i--) {
        auto skill = someOtherClass.skills[incoming[i]];
        if (s_bannedRandoPickSkills.contains(Mernel::strToLower(skill.row.skill)))
            incoming.erase(incoming.begin() + i);
    }

    size_t incomingSize = incoming.size();
    size_t generated    = 0;
    for (int i = 0; i < generateCount; ++i) {
        if (incoming.empty())
            break;

        auto r     = rng(static_cast<int>(incoming.size()));
        auto skill = someOtherClass.skills[incoming[r]];
        skill.resetIdx();
        skill.stype = SType::GenActive;
        if (skill.row.itypea1 == "h2h")
            skill.row.itypea1 = "mele";
        skills.push_back(skill);
        generated++;
        incoming.erase(incoming.begin() + r);
    }
    Logger() << "char '" << enumToString(clas) << "' generateCount=" << generateCount << ", generated="
             << generated << " from '" << enumToString(someOtherClass.clas) << "' with incomingSize=" << incomingSize;
}

void ModuleSkillRandomizer::CharacterData::createIdx()
{
    Logger() << "char " << enumToString(clas) << " createIdx: rowIdxStart=" << rowIdxStart;
    size_t i = 0;
    for (SkillInfo& skill : skills) {
        Logger() << "    i=" << i << ", skill " << skill.row.skill << ", stype=" << int(skill.stype) << " skill.rowIdx=" << skill.rowIdx;

        if (skill.rowIdx >= 0)
            continue;
        auto& row  = skill.row;
        auto& desc = skill.desc;

        int& rowIdx  = skill.rowIdx;
        int& descIdx = skill.descIdx;
        rowIdx       = rowIdxStart + i;
        descIdx      = descIdxStart + i;

        row.classSpecific = clas;
        row.charclass     = enumToString(clas);

        row.skilldesc  = "gen_" + enumToString(clas) + "_" + std::to_string(i);
        row.skill      = row.skilldesc;
        row.id         = rowIdx;
        desc.skilldesc = row.skilldesc;

        i++;
    }
}

void ModuleSkillRandomizer::CharacterData::randomizePositions(RandomGenerator& rng)
{
    PtrList                           activeSkills = getActive();
    std::map<int, std::map<int, int>> pageAndRowCounter;
    const int                         tabSize = (activeSkills.size() + 2) / 3;
    assert(tabSize <= 18);

    std::vector indexToRow{ 1, 6, 2, 5, 3, 4, 1, 3, 5, 2, 4, 6, 1, 2, 3, 4, 5, 6 };

    for (int i = -1; Ptr skill : activeSkills) {
        i++;
        auto& desc = skill->desc;

        const int page            = (i / tabSize) + 1;
        const int indexInsidePage = i % tabSize;
        desc.listRow = desc.skillPage = page;
        desc.skillRow                 = indexToRow[indexInsidePage];

        auto& cols       = pageAndRowCounter[page][desc.skillRow.value()];
        desc.skillColumn = (++cols);
    }
    for (Ptr skill : activeSkills) {
        auto&     desc = skill->desc;
        const int page = desc.skillPage.value();

        const int rowTotal = pageAndRowCounter[page][desc.skillRow.value()];

        if (rowTotal == 1) {
            desc.skillColumn = rng(3) + 1;
        } else if (rowTotal == 2 && desc.skillColumn == 1) {
            if (rng(2))
                desc.skillColumn = 3;
        }
    }
}

void ModuleSkillRandomizer::CharacterData::updateSkillReq(const Config::SkillRandomizer& module)
{
    auto getLevelFromRow = [&module](int row) -> int {
        switch (row) {
                // clang-format off
            case 1: return module.m_lvlreqRow1;
            case 2: return module.m_lvlreqRow2;
            case 3: return module.m_lvlreqRow3;
            case 4: return module.m_lvlreqRow4;
            case 5: return module.m_lvlreqRow5;
            case 6: return module.m_lvlreqRow6;
                // clang-format on
        }
        return 0;
    };

    PtrList activeSkills = getActive();
    for (Ptr skill : activeSkills) {
        skill->row.lvlreq    = getLevelFromRow(skill->desc.skillRow.value_or(0));
        skill->row.reqskills = {};
    }
}

void ModuleSkillRandomizer::CharacterData::updateSkillNameRef(const DataContext& output, RandomGenerator& rng)
{
    PtrList activeSkills = getActive();
    if (activeSkills.size() <= 2)
        return;

    /*
Source:
Amazon
  Cold Arrow     skilldesc11      synergyA calc: skill('Ice Arrow'.blvl)  synergyA desc: skilldesc21
  Ice  Arrow     skilldesc21
  Fire Arrow     skilldesc31
Sorceress
  Blizzard      skilldesc100     synergyA calc: skill('Ice bolt'.blvl)   synergyA desc: skilldesc101
                                 synergyB calc: skill('Ice blast'.blvl)  synergyB desc: skilldesc102
  Ice bolt      skilldesc101    
  Ice blast     skilldesc102

Dest:
Necromancer
  gen_nec_1  og: Blizzard    skilldesc100
                                          synergyA calc: skill('gen_nec_3'.blvl)  synergyA desc: skilldesc11 = synergy from Cold Arrow
                                          synergyB calc: skill('gen_nec_4'.blvl)  synergyB desc: skilldesc21
  gen_nec_2  og: Cold Arrow  skilldesc11
                                          synergyA calc: skill('gen_nec_1'.blvl)  synergyA desc: skilldesc100 
  gen_nec_3  og: Fire Arrow  skilldesc31
  gen_nec_4  og: Ice  Arrow  skilldesc21

  replace 
*/

    PtrSet allExistingSkills;
    for (Ptr skill : activeSkills) {
        allExistingSkills.insert(skill);
    }

    PtrSet currentWorkingSet;
    Ptr    currentSkillId;
    auto   refreshWS = [&currentWorkingSet, &allExistingSkills, &currentSkillId]() -> PtrList {
        if (currentWorkingSet.size() <= 1)
            currentWorkingSet = allExistingSkills;

        PtrSet copyWOcurrentSkill = currentWorkingSet;
        copyWOcurrentSkill.erase(currentSkillId);
        PtrList result(copyWOcurrentSkill.cbegin(), copyWOcurrentSkill.cend());
        return result;
    };

    PtrList allExistingButCurrent;

    std::map<std::string, Ptr> replacementBySkillId;
    std::map<std::string, Ptr> replacementBySkillTranslationKey;

    auto replaceSkillId = [&replacementBySkillId, &replacementBySkillTranslationKey, &refreshWS, &currentWorkingSet, &output, &rng](const std::string& str) -> std::string {
        if (auto it = replacementBySkillId.find(str); it != replacementBySkillId.cend())
            return it->second->row.skill;

        PtrList pool = refreshWS();

        Ptr replacementId = pool[rng(pool.size())];

        replacementBySkillId[str] = replacementId;
        currentWorkingSet.erase(replacementId);

        if (auto* sourceSkill = output.skills.find(Mernel::strToLower(str))) {
            if (auto* sourceDesc = output.skilldesc.find(Mernel::strToLower(sourceSkill->skilldesc))) {
                replacementBySkillTranslationKey[sourceDesc->strName] = replacementId;
            }
        }

        return replacementId->row.skill;
    };

    for (Ptr skill : activeSkills) {
        for (auto& str : skill->desc.calc)
            replaceTokensInplace(str, replaceSkillId);
        for (auto& str : skill->row.calc)
            replaceTokensInplace(str, replaceSkillId);
    }
    for (Ptr skill : activeSkills) {
        for (auto& str : skill->desc.desc3) {
            auto it = replacementBySkillTranslationKey.find(str);
            if (it != replacementBySkillTranslationKey.cend()) {
                str = it->second->desc.strName;
            }
        }
    }
}

void ModuleSkillRandomizer::CharacterData::randomizeElements(RandomGenerator& rng, int maximumVariants, int percentToChange)
{
    if (percentToChange == 0)
        return;

    auto elementsTmp1 = s_knownElements;
    while (elementsTmp1.size() > maximumVariants) {
        auto i = rng(static_cast<int>(elementsTmp1.size()));
        elementsTmp1.erase(elementsTmp1.begin() + i);
    }

    for (auto& skill : skills) {
        if (skill.eType == Element::None)
            continue;
        if (!s_skillsOkToRandomizeDamage.contains(Mernel::strToLower(skill.row.skill))) {
            //if (!s_skippedSkills.contains(skillInfo.lowercaseKey) && !s_skippedSkills_PD2.contains(skillInfo.lowercaseKey))
            //   skipped[skillInfo.classSpecific].push_back("\"" + skillInfo.lowercaseKey + "\"");
            //Logger() << "Skipping \"" << skillInfo.lowercaseKey << "\" with E=" << enumToString(oldE);
            continue;
        }
        if (percentToChange < 100) {
            if (rng(100) > percentToChange)
                continue;
        }
        auto elementsTmp = elementsTmp1;

        auto it = std::find(elementsTmp.begin(), elementsTmp.end(), skill.eType);
        if (it != elementsTmp.end() && !elementsTmp.empty())
            elementsTmp.erase(it);

        if (!elementsTmp.empty())
            skill.setNewElement(elementsTmp[rng((int) elementsTmp.size())]);
    }
}

void ModuleSkillRandomizer::CharacterData::insertRows(DataContext& output, int rowsPerClass)
{
    size_t activeSize = getIndexesByType(SType::GenActive).size();

    size_t sizeOld = skills.size();
    skills.resize(skills.size() + rowsPerClass - activeSize);
    for (size_t i = sizeOld; i < skills.size(); ++i)
        skills[i].desc.makeHidden();

    rowIdxStart  = output.skills.rows.size();
    descIdxStart = output.skilldesc.rows.size();

    output.skills.rows.resize(output.skills.rows.size() + rowsPerClass);
    output.skilldesc.rows.resize(output.skilldesc.rows.size() + rowsPerClass);
}

void ModuleSkillRandomizer::SkillInfo::setNewElement(Element element)
{
    const int oldCompensation = s_compensateBitShift.at(eType);

    eType = element;

    const int newCompensation = s_compensateBitShift.at(eType);

    row.hitShift = std::clamp(row.hitShift.value_or(0) - oldCompensation + newCompensation, 0, 15);

    const int duration = s_defaultDuration.at(eType);
    if (duration > 0) {
        if (!row.eLen.has_value())
            row.eLen = duration;
    } else {
        row.eLen.reset();
    }
}

}
