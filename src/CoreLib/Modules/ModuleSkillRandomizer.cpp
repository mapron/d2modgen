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

struct SpriteCfg {
    Character cha;
    BlobId    high;
    BlobId    low;
};
using SpriteCfgList         = std::vector<SpriteCfg>;
const SpriteCfgList s_unite = {
    SpriteCfg{ Character::Ama, BlobId::IconAmaHighre, BlobId::IconAmaLowres },
    SpriteCfg{ Character::Sor, BlobId::IconSorHighre, BlobId::IconSorLowres },
    SpriteCfg{ Character::Nec, BlobId::IconNecHighre, BlobId::IconNecLowres },
    SpriteCfg{ Character::Pal, BlobId::IconPalHighre, BlobId::IconPalLowres },
    SpriteCfg{ Character::Bar, BlobId::IconBarHighre, BlobId::IconBarLowres },
    SpriteCfg{ Character::Dru, BlobId::IconDruHighre, BlobId::IconDruLowres },
    SpriteCfg{ Character::Ass, BlobId::IconAssHighre, BlobId::IconAssLowres },
    SpriteCfg{ Character::War, BlobId::IconWarHighre, BlobId::IconWarLowres },
};

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

    "fists of fire",
    "claws of thunder",
    "blades of ice",
    "dragon claw",
};

const StringSet s_noPoison{};

const StringSet s_skippedSkills{
    "static field",
    "vengeance",

};

struct ElemDesc {
    std::string descId;
    std::string minattr;
    std::string maxattr;
    std::string lenattr;
    int         compensate = 0;
    int         dur        = 0;
};

const auto s_knownElements = std::vector{ Element::Ltng, Element::Fire, Element::Cold, Element::Mag, Element::Pois };

// Magic damage is unresisted, so the skill needs to be weaker to compensate
// Poison compensate 2^5==32 times for poison as we do 100 frames duration and it needs 32x damage reduction (total damage is 3x higher)
const std::map<Element, ElemDesc> s_elemDesc{
    { Element::Ltng, ElemDesc{ .descId = "StrSkill7", .minattr = "lightmindam", .maxattr = "lightmaxdam" } },
    { Element::Fire, ElemDesc{ .descId = "StrSkill5", .minattr = "firemindam", .maxattr = "firemaxdam" } },
    { Element::Cold, ElemDesc{ .descId = "StrSkill6", .minattr = "coldmindam", .maxattr = "coldmaxdam", .dur = 50 } },
    { Element::Mag, ElemDesc{ .descId = "StrSkill39", .minattr = "magicmindam", .maxattr = "magicmaxdam", .compensate = -1 } },
    { Element::Pois, ElemDesc{ .descId = "StrSkill8", .minattr = "poisonmindam", .maxattr = "poisonmaxdam", .lenattr = "poisonlength", .compensate = -5, .dur = 100 } },

    { Element::Phys, ElemDesc{ .descId = "StrSkill4" } },
    { Element::Frze, ElemDesc{ .descId = "StrSkill29" } },
    { Element::Burn, ElemDesc{ .descId = "Skillld226" } },
};

void dump(std::string indent, DataContext& output, const DataRow_missiles& row)
{
    std::string                       idsStr = "";
    std::set<const DataRow_missiles*> children;
    for (const std::string& child : row.childmissile) {
        if (child.empty())
            continue;
        if (child == row.name)
            continue;
        if (auto* c = output.missiles.find(child))
            children.insert(c);
        idsStr += child + ", ";
    }
    Logger() << indent << row.name << " E: '" << row.eType << "' parent skill: '"
             << row.skill << "' missileSkill: " << row.missileSkill.value_or(0)
             << ", children missiles: " << idsStr;
    for (auto* row : children)
        dump(indent + "- ", output, *row);
}

void dump(std::string indent, DataContext& output, const DataRow_skills& row)
{
    std::string                       idsStr = "";
    std::set<const DataRow_missiles*> children;
    for (const std::string& child : row.missile) {
        if (child.empty())
            continue;
        if (auto* c = output.missiles.find(child))
            children.insert(c);
        idsStr += child + ", ";
    }
    Logger() << indent << "SKILL '" << row.skill << "' E: '" << row.eType << "' children missiles: " << idsStr;
    for (auto* row : children)
        dump(indent + "- ", output, *row);
}

}

std::string enumToString(SType stype, bool asEnum)
{
    using enum SType;
    switch (stype) {
        case ExistingHidden:
            return "ExistingHidden";
        case ExistingActive:
            return "ExistingActive";
        case GenHidden:
            return "GenHidden";
        case GenActive:
            return "GenActive";
        case Filler:
            return "Filler";
    };
    return "?";
}

void ModuleSkillRandomizer::generate()
{
    const auto& module = config.m_skillRandomizer;
    if (!module.m_enabled)
        return;

    if (0) {
        Bitmap bmpUnite;
        for (const auto& cfg : s_unite) {
            auto& spriteBlob = output.blobs[cfg.high];

            Bitmap& bmp = output.bitmaps[enumToString(cfg.cha)];

            bmp.readFromSprite(spriteBlob);
            bmpUnite.height = bmp.height;
            bmpUnite.width  = bmp.width;
            break;
        }
        bmpUnite.resize(bmpUnite.width * 2, bmpUnite.height);
        int posX = 0;
        int cnt  = 0;
        for (const auto& cfg : s_unite) {
            auto& spriteBlob = output.blobs[cfg.high];

            Bitmap& bmp = output.bitmaps[enumToString(cfg.cha)];

            bmp.readFromSprite(spriteBlob);
            Bitmap::blit(bmp, 0, 0, bmpUnite, posX, 0, bmp.width, bmp.height);
            posX += bmp.width;
            cnt++;
            if (cnt == 2)
                break;
        }
        for (const auto& cfg : s_unite) {
            auto& spriteBlob = output.blobs[cfg.high];

            Bitmap& bmp = output.bitmaps[enumToString(cfg.cha)];
            bmp         = bmpUnite;
            bmp.writeToSprite(spriteBlob);
        }
    }

    const int totalNewSkillsPerClass = 60;

    const int keepSkillCount          = module.m_randoMode == 1 ? 0 : module.m_keepOriginal;
    const int generateCountSuggestion = module.m_generate;

    const bool extraTrace = env.extraTrace.contains("skill_rando");

    for (Character clas : getClassTypes()) {
        CharacterData data;
        data.extraTrace = extraTrace;
        data.clas       = clas;
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

    generatedData           = existingData;
    const bool randoEnabled = module.m_randoMode > 0;

    for (auto& [key, cha] : generatedData) {
        if (randoEnabled) {
            cha.makeInactive(rng, keepSkillCount);
        }
        const int keptActve           = cha.getActive().size();
        const int skillLimit          = 54;
        const int availableToGenerate = skillLimit - keptActve;
        const int generateCount       = std::min(generateCountSuggestion, availableToGenerate);
        const int desirableTotal      = generateCount + keptActve;

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
            [[maybe_unused]] const size_t activeSize = cha.getActive().size();
            assert(activeSize == desirableTotal);
        }
        if (module.m_randoMode == 3) {
            // pick from any class
            std::vector<int> weights;

            for (auto& [sourceKey, sourceData] : existingData) {
                int w = getClassWeight(sourceKey);
                if (sourceKey == cha.clas && keptActve > 0)
                    w = 0;
                weights.push_back(w);
            }
            std::vector<int> counts;
            splitWeights(generateCount, weights, counts);
            for (size_t i = 0; auto& [sourceKey, sourceData] : existingData) {
                const int characterGenerateCount = counts[i++];
                cha.generateAdditionalSkills(rng, sourceData, characterGenerateCount);
            }
            [[maybe_unused]] const size_t activeSize = cha.getActive().size();
            assert(activeSize == desirableTotal);
        }

        cha.insertRows(output, totalNewSkillsPerClass);
        cha.createIdx(output);
        output.skills.updateIndex();
        output.skilldesc.updateIndex();
        if (randoEnabled) {
            cha.randomizePositions(rng);
            cha.updateSkillNameRef(output, rng);
        }
        cha.updateSkillReq(module, randoEnabled);
        cha.updateSkillPower(module.m_globalHitShift - 2);

        cha.randomizeElements(rng, module.m_maximumElements, module.m_chanceElemChange);
        cha.updateMissiles(output);
    }

    for (auto& [key, cha] : generatedData) {
        cha.writeOut(output);
    }

    output.missiles.commitExtraRows();

    for (auto& skill : output.skills.rows) {
        if (skill.classSpecific != Character::Nec)
            continue;
        if (extraTrace)
            dump("", output, skill);
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
        skill.eType = getElementId(row.eType);
        skills.push_back(skill);
    }
    return validCnt > 0;
}

void ModuleSkillRandomizer::CharacterData::writeOut(DataContext& output) const
{
    for (int i = -1; const SkillInfo& skill : skills) {
        i++;
        if (skill.stype < SType::ExistingActive) {
            if (extraTrace)
                Logger() << "WRITE-SKIP [" << i << "] " << enumToString(clas) << " " << skill.toString();
            continue;
        }

        auto& row                            = output.skills.rows[skill.rowIdx];
        row                                  = skill.row;
        output.skilldesc.rows[skill.descIdx] = skill.desc;
        if (skill.eType != Element::None)
            row.eType = enumToString(skill.eType);
        if (extraTrace)
            Logger() << "WRITE      [" << i << "] " << enumToString(clas) << " " << skill.toString();
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
    [[maybe_unused]] const size_t activeSize = getActive().size();
    assert(activeSize <= 18 * 3);
    if (extraTrace)
        Logger() << "char '" << enumToString(clas) << "' generateCount=" << generateCount << ", generated="
                 << generated << " from '" << enumToString(someOtherClass.clas) << "' with incomingSize=" << incomingSize;
}

void ModuleSkillRandomizer::CharacterData::createIdx(DataContext& output)
{
    if (extraTrace)
        Logger() << "char " << enumToString(clas) << " createIdx start=" << rowIdxStart;
    size_t i = 0;
    for (SkillInfo& skill : skills) {
        //Logger() << "    i=" << i << ", skill " << skill.row.skill << ", stype=" << int(skill.stype) << " skill.rowIdx=" << skill.rowIdx;

        if (skill.rowIdx >= 0)
            continue;
        auto& row  = skill.row;
        auto& desc = skill.desc;

        int& rowIdx  = skill.rowIdx;
        int& descIdx = skill.descIdx;
        rowIdx       = rowIdxStart + i;
        descIdx      = descIdxStart + i;

        row.setNewClass(clas);

        row.skilldesc = "gen_" + enumToString(clas) + "_" + std::to_string(i);
        output.idClones.skills.push_back({ .from = row.skill, .to = row.skilldesc });

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

    // std::vector indexToRow{ 1, 6, 2, 5, 3, 4, 1, 3, 5, 2, 4, 6, 1, 2, 3, 4, 5, 6 };

    std::vector indexToRow{ 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6 };

    std::vector<std::vector<Ptr>> megacolumns;
    megacolumns.resize(9);

    int total = activeSkills.size();
    int i     = 0;
    for (i = 0; i < total / 2; ++i) {
        megacolumns[i % 9].push_back(activeSkills[i]);
    }
    for (; i < total; ++i) {
        int col = rng(9);

        while (megacolumns[col].size() == 6) {
            col = (col + 1) % 9;
        }

        megacolumns[col].push_back(activeSkills[i]);
    }

    for (int megacol = 0; megacol < 9; megacol++) {
        const int        page            = (megacol / 3) + 1;
        const int        col             = (megacol % 3) + 1;
        std::vector<int> allowedRowsLeft = { 1, 2, 3, 4, 5, 6 };

        for (int megarow = 0; megarow < megacolumns[megacol].size(); megarow++) {
            Ptr       skill  = megacolumns[megacol][megarow];
            auto&     desc   = skill->desc;
            const int oldRow = desc.skillRow.value_or(0);

            desc.listRow = desc.skillPage = page;
            desc.skillColumn              = col;

            if (megarow == 0 && col == 1) {
                desc.skillRow = 1;
                allowedRowsLeft.erase(allowedRowsLeft.begin());
            } else if (megarow == 0 && col == 3) {
                desc.skillRow = 6;
                allowedRowsLeft.pop_back();
            } else {
                int idx       = rng(allowedRowsLeft.size());
                desc.skillRow = allowedRowsLeft[idx];
                allowedRowsLeft.erase(allowedRowsLeft.begin() + idx);
            }

            skill->changeInRow = desc.skillRow.value() - oldRow;
        }
    }
}

void ModuleSkillRandomizer::CharacterData::updateSkillReq(const Config::SkillRandomizer& module, bool recalDeps)
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

    PtrList              activeSkills = getActive();
    std::vector<PtrList> columns(9);
    for (Ptr skill : activeSkills) {
        skill->row.lvlreq = getLevelFromRow(skill->desc.skillRow.value_or(0));
        int col           = skill->desc.skillColumn.value() - 1;
        int page          = skill->desc.skillPage.value() - 1;
        columns[col + page * 3].push_back(skill);
    }
    if (recalDeps) {
        for (PtrList& columns1 : columns) {
            std::sort(columns1.begin(), columns1.end(), [](Ptr l, Ptr r) { return l->desc.skillRow.value() < r->desc.skillRow.value(); });
            Ptr prev = nullptr;
            for (Ptr skill : columns1) {
                skill->row.reqskills.clear();
                if (prev) {
                    skill->row.reqskills.push_back(prev->row.skill);
                }
                prev = skill;
            }
        }
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
        for (auto& str : skill->row.aurastatcalc)
            replaceTokensInplace(str, replaceSkillId);
    }
    for (Ptr skill : activeSkills) {
        for (auto& str : skill->desc.desc) {
            if (str == skill->desc.strName)
                continue;

            auto it = replacementBySkillTranslationKey.find(str);
            if (it != replacementBySkillTranslationKey.cend()) {
                str = it->second->desc.strName;
            }
        }
    }
}

void ModuleSkillRandomizer::CharacterData::updateSkillPower(int globalHitShift)
{
    for (Ptr skill : getActive()) {
        int rowCompensate = 0;
        if (skill->changeInRow > 1) {
            rowCompensate = ((skill->changeInRow - 0) / 2);
        } else if (skill->changeInRow < -1) {
            rowCompensate = ((skill->changeInRow + 1) / 2);
        }
        skill->row.hitShift  = std::clamp(skill->row.hitShift.value_or(0) + globalHitShift + rowCompensate, 0, 16);
        skill->row.manaShift = std::clamp(skill->row.manaShift.value_or(0) + rowCompensate, 0, 16);
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
    std::set<std::string> passiveMinMax;
    for (auto& [key, desc] : s_elemDesc) {
        if (!desc.minattr.empty())
            passiveMinMax.insert(desc.minattr);
        if (!desc.maxattr.empty())
            passiveMinMax.insert(desc.maxattr);
    }

    for (auto& skill : skills) {
        if (skill.stype != SType::ExistingActive && skill.stype != SType::GenActive)
            continue;

        if (skill.eType == Element::None) {
            if (extraTrace)
                Logger() << "'" << skill.row.skill << "' (og='" << skill.originalId << "') " << " elem=None, SKIP";
            continue;
        }

        if (s_skippedSkills.contains(Mernel::strToLower(skill.originalId))) {
            if (extraTrace)
                Logger() << "'" << skill.row.skill << "' (og='" << skill.originalId << "') " << " ignoredlist, SKIP";
            continue;
        }
        if (percentToChange < 100) {
            if (rng(100) > percentToChange)
                continue;
        }
        bool noPoison = s_noPoison.contains(Mernel::strToLower(skill.originalId));
        for (auto& pass : skill.row.passivestat) {
            if (passiveMinMax.contains(pass))
                noPoison = true;
        }

        auto elementsTmp = elementsTmp1;
        if (noPoison) {
            if (auto it = std::find(elementsTmp.begin(), elementsTmp.end(), Element::Pois); it != elementsTmp.end())
                elementsTmp.erase(it);
        }

        if (auto it = std::find(elementsTmp.begin(), elementsTmp.end(), skill.eType); it != elementsTmp.end())
            elementsTmp.erase(it);

        if (elementsTmp.empty())
            continue;

        skill.setNewElement(elementsTmp[rng((int) elementsTmp.size())], extraTrace);
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

void ModuleSkillRandomizer::CharacterData::updateMissiles(DataContext& output)
{
    for (auto& skill : skills) {
        if (skill.stype != SType::ExistingActive && skill.stype != SType::GenActive)
            continue;

        if (skill.eType == Element::None)
            continue;

        std::map<std::string, std::string> skillScope;
        for (std::string& child : skill.row.missile)
            updateMissile(output, skill, child, skillScope);

        for (std::string& child : skill.desc.missile)
            updateMissile(output, skill, child, skillScope);
    }
}

void ModuleSkillRandomizer::CharacterData::updateMissile(DataContext& output, const SkillInfo& parent, std::string& missileId, std::map<std::string, std::string>& scope)
{
    if (missileId.empty())
        return;
    if (auto it = scope.find(missileId); it != scope.cend()) {
        missileId = it->second;
        return;
    }

    auto* miss = output.missiles.find(missileId);
    if (!miss) {
        Logger(Logger::Warning) << "Unknown missileId: " << missileId;
        return;
    }
    DataRow_missiles newMiss = *miss;
    std::string      newName = parent.row.skill + "_" + newMiss.name;
    for (std::string& child : newMiss.childmissile) {
        if (child == newMiss.name) {
            child = newName;
            continue;
        }
        updateMissile(output, parent, child, scope);
    }

    auto origId  = newMiss.name;
    newMiss.name = parent.row.skill + "_" + newMiss.name;
    newMiss.id   = output.missiles.rows.size() + output.missiles.extra.size();
    missileId    = newMiss.name;

    if (getElementId(newMiss.eType) != Element::None) // will skip freeze/bleed
        newMiss.eType = enumToString(parent.eType);

    if (!newMiss.skill.empty())
        newMiss.skill = parent.row.skill;

    output.missiles.extra.push_back(newMiss);
    output.idClones.missiles.push_back({ .from = origId, .to = newMiss.name });
    scope[origId] = newMiss.name;
}

std::string ModuleSkillRandomizer::SkillInfo::toString() const
{
    std::ostringstream os;
    os << " '" << row.skill << "' OG='" << originalId << "' idx=" << descIdx << " stype=" << enumToString(stype);
    os << " pos: (" << desc.skillPage.value() << ", " << desc.skillRow.value() << ", " << desc.skillColumn.value() << ")";
    return os.str();
}

void ModuleSkillRandomizer::SkillInfo::setNewElement(Element element, bool extraTrace)
{
    if (eType == element)
        return;
    const auto      old     = eType;
    const ElemDesc& oldDesc = s_elemDesc.at(old);
    const ElemDesc& newDesc = s_elemDesc.at(element);

    const int  oldCompensation = oldDesc.compensate;
    const auto oldDescText     = oldDesc.descId;

    const int  newCompensation = newDesc.compensate;
    const auto newDescText     = newDesc.descId;

    eType = element;

    row.hitShift = std::clamp(row.hitShift.value_or(0) - oldCompensation + newCompensation, 0, 15);

    if (extraTrace)
        Logger() << "'" << row.skill << "' (og='" << originalId << "') " << " element change " << enumToString(old) << " -> " << enumToString(element) << " (hs=" << row.hitShift.value_or(0) << ")";

    const int duration = newDesc.dur;
    if (duration > 0) {
        row.eLen = duration;
    } else {
        row.eLen.reset();
    }

    for (auto& str : desc.desc) {
        if (str == oldDescText)
            str = newDescText;
    }
    for (auto& pass : row.passivestat) {
        if (pass == oldDesc.minattr)
            pass = newDesc.minattr;
        if (pass == oldDesc.maxattr)
            pass = newDesc.maxattr;
    }
    bool hasReplacedAura = false;
    for (auto& pass : row.aurastat) {
        if (pass == oldDesc.minattr) {
            pass            = newDesc.minattr;
            hasReplacedAura = true;
        }
        if (pass == oldDesc.maxattr) {
            pass            = newDesc.maxattr;
            hasReplacedAura = true;
        }
    }

    if (element == Element::Pois) {
        for (auto& str : desc.calc) {
            if (str == "enma" || str == "exma") {
                str = str == "enma" ? "enms" : "exms";
                str += "*edln/256";
            }
        }
        for (auto& str : row.aurastatcalc) {
            if (str == "enma" || str == "exma") {
                str = str == "enma" ? "enms" : "exms";
            }
        }
        if (hasReplacedAura) {
            row.aurastat.push_back(newDesc.lenattr);
            row.aurastatcalc.push_back("100");
        }
    }
    if (old == Element::Pois) {
        for (auto& str : desc.calc) {
            if (str == "StrSkillPoisonLengthSingular")
                str = "StrSkill0";
            if (str.ends_with("*edln/256")) {
                str = str.substr(0, 4);
                if (str == "enms")
                    str = "enma";
                if (str == "exms")
                    str = "exma";
            }
        }
    }
}

}
