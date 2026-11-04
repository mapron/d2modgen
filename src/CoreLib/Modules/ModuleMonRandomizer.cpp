/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMonRandomizer.hpp"

#include "Logger.hpp"
#include "RandoUtils.hpp"

#include <algorithm>

namespace D2ModGen {

namespace {
constexpr const int s_maxIngameLevel = 110;

std::string printStringSetSummary(const std::vector<std::string>& idsSource, int limit = 5)
{
    std::vector<std::string> idsUnique;

    for (const auto& id : idsSource) {
        if (std::find(idsUnique.cbegin(), idsUnique.cend(), id) == idsUnique.cend())
            idsUnique.push_back(id);
    }
    std::ostringstream os;
    for (int i = 0; std::string& ai : idsUnique) {
        i++;
        if (i > limit) {
            os << ", ...";
            break;
        }
        if (i > 1)
            os << ", ";
        os << ai;
    }
    os << " (unique: " << idsUnique.size() << ", total: " << idsSource.size() << ")";
    return os.str();
}

}

void ModuleMonRandomizer::generate()
{
    if (!config.m_monsterRandomizer.m_enabled)
        return;

    hcIdx = output.monstats.rows.back().hcIdx + 1;

    for (auto& row : output.treasureclassex.rows)
        tcTable.add(row.name, row.group.value_or(0), row.level.value_or(0));

    updateLevelDensity();

    prepareChimeraPools();
    prepareGuestPools();

    const auto& module = config.m_monsterRandomizer;

    for (size_t areaIndex = 0; auto& row : output.levels.rows) {
        areaIndex++;
        if (!row.useForRando)
            continue;

        auto prevRow = row;

        auto& monIds = row.nmon;

        StringSet monSet;

        MonsterTaskList                   tasks;
        int                               targetLevel = 0;
        std::set<const DataRow_monstats*> existingIndexes;
        for (auto& id : monIds) {
            MonsterTask task;
            task.m_areaIndex = areaIndex - 1;
            task.m_source    = output.monstats.find(id);
            if (!task.m_source->useForRando)
                task.m_skip = true;
            if (existingIndexes.contains(task.m_source))
                continue;

            existingIndexes.insert(task.m_source);
            targetLevel = std::max(targetLevel, task.m_source->level.value_or(0));
            tasks.push_back(task);
        }

        const int originalSize = tasks.size();
        const int guestAmount  = genRandomScaledValue(rng, originalSize, module.m_guestsMin, module.m_guestsMax);
        const int keepAmount   = scaleValue(originalSize, module.m_spawnKeepOriginal, 0);

        if (keepAmount > 0) {
            while (tasks.size() > keepAmount) {
                auto r = rng(static_cast<int>(monIds.size()));
                tasks.erase(tasks.begin() + r);
            }
        } else {
            tasks.clear();
        }

        std::set<const DataRow_monstats*> guestIndexes = existingIndexes;
        for (int i = 0, target = guestAmount + existingIndexes.size(); i < guestAmount + 5; ++i) {
            if (guestIndexes.size() >= target)
                break;
            guestIndexes.insert(validGuestIndexes[rng(validGuestIndexes.size())]);
        }
        for (const DataRow_monstats* guestIdx : guestIndexes) {
            if (existingIndexes.contains(guestIdx))
                continue;
            MonsterTask task;
            task.m_areaIndex         = areaIndex - 1;
            task.m_source            = guestIdx;
            task.m_isGuest           = true;
            task.m_targetNormalLevel = targetLevel;
            tasks.push_back(task);
        }

        const int totalAmount    = tasks.size();
        const int chimeraAmount  = genRandomScaledValue(rng, totalAmount, module.m_chimeraAIMin, module.m_chimeraAIMax);
        const int rollStatAmount = genRandomScaledValue(rng, totalAmount, module.m_rollStatMin, module.m_rollStatMax);
        const int rollResAmount  = genRandomScaledValue(rng, totalAmount, module.m_rollResistancesMin, module.m_rollResistancesMax);

        for (auto index : genRandomIndexes(rng, totalAmount, chimeraAmount))
            tasks[index].m_makeChimera = true;
        for (auto index : genRandomIndexes(rng, totalAmount, rollStatAmount))
            tasks[index].m_rollStat = true;
        for (auto index : genRandomIndexes(rng, totalAmount, rollResAmount))
            tasks[index].m_rollRes = true;

        Logger() << "Area '" << row.name << "' > total=" << totalAmount << ", guests=" << guestAmount << ", chimera=" << chimeraAmount << ", rollStat=" << rollStatAmount << ", rollRes=" << rollResAmount << ", targetLevel=" << targetLevel;

        for (auto& task : tasks) {
            if (!task.hasAnyMods())
                task.m_skip = true;

            MonsterTaskList extraMinions;
            processTask(task, extraMinions);

            if (0) {
                Logger() << "        " << task.toString();
                for (auto& minionTask : extraMinions)
                    Logger() << "         minion: " << minionTask.toString();
            }
        }
        row.mon.clear();
        row.nmon.clear();
        row.umon.clear();

        for (auto& task : tasks) {
            row.mon.push_back(task.m_dest.id);
        }
        for (auto& task : tasks) {
            row.nmon.push_back(task.m_dest.id);
        }
        for (auto& task : tasks) {
            if (task.m_source->spawnedAsElite)
                row.umon.push_back(task.m_dest.id);
        }
    }

    output.monstats.commitExtraRows();
    output.monstatsEx.commitExtraRows();

    if (module.m_hellResistances) {
        for (auto& row : output.monstats.rows) {
            if (!row.isRegular)
                continue;
            size_t                   hell = 2;
            std::array<MonResist, 3> r;
            for (size_t i = 0; i < 3; ++i)
                r[i].readFromRow(row, hell);

            for (size_t i = 0; i < 3; ++i)
                r[i].writeToRow(row, i);
        }
    }

    if (output.jsonFiles.contains(JsonId::Monsters)) {
        // update D2R 3D model for new records.
        auto& jsonDoc    = output.jsonFiles[JsonId::Monsters];
        auto& jsonObject = jsonDoc.getMap();
        for (const auto& [sourceId, newId] : idCopies) {
            const std::string modelName = jsonObject[sourceId].getScalar().toString();
            assert(!modelName.empty());
            jsonObject[newId] = Mernel::PropertyTreeScalar{ modelName };
        }
    }
}

void ModuleMonRandomizer::updateLevelDensity()
{
    const auto& module = config.m_monsterRandomizer;

    if (const int groupIncrease = module.m_monGroupIncreaseSize; groupIncrease > 0) {
        for (auto& row : output.monstats.rows) {
            if (!row.isRegular)
                continue;
            auto& valueMin = row.minGrp;
            auto& valueMax = row.maxGrp;
            if (!valueMin.has_value() || !valueMax.has_value())
                continue;
            if (valueMin == valueMax && valueMax == 1)
                continue;
            for (auto* value : { &valueMin, &valueMax }) {
                const int prev = value->value();
                const int next = prev + groupIncrease;
                *value         = std::clamp(next, 1, 99);
            }
        }
    }

    auto densityAdjust = [&module, this](std::optional<int>& value) {
        if (module.m_densityMin == 100 && module.m_densityMax == 100)
            return;
        if (!value.has_value())
            return;

        const int prev  = value.value_or(600);
        const int scale = genBetweenRng(rng, module.m_densityMin, module.m_densityMax);
        const int next  = scaleValue(prev, scale);
        value           = std::clamp(next, 200, 9900);
    };
    auto packsAdjust = [&module, this](std::optional<int>& value) {
        if (module.m_elitePacksMin == 100 && module.m_elitePacksMax == 100)
            return;
        const int prev  = value.value_or(0);
        const int scale = genBetweenRng(rng, module.m_elitePacksMin, module.m_elitePacksMax);
        const int next  = scaleValue(prev, scale);
        value           = (std::min(next, 255));
    };

    const int spawnedCount = module.m_numMonExtra;
    for (auto& row : output.levels.rows) {
        if (!row.useForRando)
            continue;

        if (module.m_hellPacks) {
            row.monUMin[0] = row.monUMin[1] = row.monUMin[2];
            row.monUMax[0] = row.monUMax[1] = row.monUMax[2];
        }

        for (size_t i = 0; i < 3; ++i) {
            packsAdjust(row.monUMin[i]);
            packsAdjust(row.monUMax[i]);
            if (row.monUMax[i].has_value()) {
                row.monUMax[i] = std::max(row.monUMin[i].value_or(0), row.monUMax[i].value_or(0));
                if (module.m_maxPacks)
                    row.monUMin[i] = row.monUMax[i];
            }
        }

        for (auto& val : row.monDen)
            densityAdjust(val);

        row.numMon = std::max(2, row.numMon.value() + spawnedCount);
    }
}

bool ModuleMonRandomizer::compatibleMonSeq(const std::string& sequenceId, const DataRow_monstatsEx& recipient)
{
    for (const auto& row : output.monseq.rows) {
        if (row.sequence == sequenceId) {
            if (!recipient.hasMode(row.mode)) {
                return false;
            }
        }
    }

    return true;
}

void ModuleMonRandomizer::processTask(MonsterTask& task, MonsterTaskList& extraMinions)
{
    DataRow_monstats& row = task.m_dest;
    row                   = *task.m_source;

    if (task.m_skip) {
        return;
    }

    DataRow_monstatsEx& rowEx = task.m_destEx;
    rowEx                     = *output.monstatsEx.index.at(row.monStatsEx);

    const std::string origId = row.id;
    row.useForRando          = false;
    row.id                   = "gen_" + row.id + "_" + std::to_string(task.m_areaIndex);

    const int guestOrigLevel = row.level.value_or(0);
    row.level                = task.m_targetNormalLevel;

    row.monStatsEx = row.id;
    rowEx.id       = row.id;

    row.hcIdx   = hcIdx;
    rowEx.hcIdx = hcIdx;
    hcIdx++;

    if (0) {
        output.monstats.extra.push_back(row);
        output.monstatsEx.extra.push_back(rowEx);
        idCopies.push_back({ origId, row.id });
        return;
    }

    if (task.m_isGuest) {
        if (guestOrigLevel > task.m_targetNormalLevel) {
            for (auto& skill : row.skills) {
                const int newLevel = skill.level.value_or(0) * task.m_targetNormalLevel / guestOrigLevel;
                skill.level        = (std::max(1, newLevel));
            }
        }
        for (auto* tc : row.nonEmptyTCs()) {
            auto promoted = tcTable.promote(*tc, guestOrigLevel, task.m_targetNormalLevel);
            *tc           = promoted;
        }
    }

    if (task.m_makeChimera) {
        generateChimera(row, rowEx);
    }

    if (task.m_rollRes) {
        std::array<MonResist, 3> r;
        for (size_t i = 0; i < 3; ++i)
            r[i].readFromRow(row, i);

        for (size_t i = 0; i < 3; ++i)
            r[i].shuffle(rng);

        for (size_t i = 0; i < 3; ++i)
            r[i].writeToRow(row, i);
    }
    if (task.m_rollStat) {
        auto genTriangle = [this](int steps) -> int {
            int totalF = 0;
            for (int i = steps; i >= 1; --i)
                totalF += i;
            int number = rng(totalF);
            totalF     = 0;
            for (int i = steps, result = 0; i >= 1; --i, result++) {
                totalF += i;
                if (totalF > number)
                    return result;
            }
            return 0;
        };

        auto genStat = [this, &genTriangle](std::optional<int>& val, int min, int med, int max, int steps = 4) {
            if (!val.has_value())
                return;
            int branch = rng(3);
            if (branch == 0) {
                val = med;
                return;
            }
            const int roll = genTriangle(steps) + 1; // [1..steps]
            const int diff = (branch == 1 ? max : min) - med;
            val            = med + roll * diff / steps;
        };
        genStat(row.walkSpeed, 2, 6, 10);
        genStat(row.runSpeed, 6, 10, 14);
        genStat(row.toBlock, 5, 25, 45);
        for (auto& s : row.battleStats) {
            auto old = s.maxHP;
            genStat(s.maxHP, 40, 120, 280);
            if (s.maxHP.has_value())
                s.minHP = s.maxHP.value() * 3 / 4;

            genStat(s.AC, 70, 100, 150);
            genStat(s.exp, 50, 100, 180);

            s.A1minD = 0;
            s.A1maxD = 0;
            s.A1TH   = 0;
            s.A2minD = 0;
            s.A2maxD = 0;
            s.A2TH   = 0;
            s.S1minD = 0;
            s.S1maxD = 0;
            s.S1TH   = 0;

            genStat(s.A1minD, 50, 90, 160);
            genStat(s.A1maxD, 110, 160, 250);
            genStat(s.A2minD, 50, 90, 160);
            genStat(s.A2maxD, 110, 160, 250);
            genStat(s.S1minD, 50, 90, 160);
            genStat(s.S1maxD, 110, 160, 250);

            genStat(s.A1TH, 70, 100, 220);
            genStat(s.A2TH, 70, 100, 220);
            genStat(s.S1TH, 70, 100, 220);

            if (!s.el1Dur.has_value())
                s.el1Dur = 100;

            genStat(s.el1Pct, 25, 50, 75);
            genStat(s.el1MinD, 10, 30, 60);
            genStat(s.el1MaxD, 30, 60, 100);

            Element ran;
        }
        if (rowEx.hasMode(AnimationMode::A1) && row.el1Mode.empty()) {
            row.el1Mode = "A1";
        }
        if (rowEx.hasMode(AnimationMode::A2) && row.el1Mode.empty()) {
            row.el1Mode = "A2";
        }
        const auto elements     = std::array{ Element::Ltng, Element::Fire, Element::Cold, Element::Pois };
        Element    randoElement = elements[rng(elements.size())];
        row.el1Type             = enumToString(randoElement);
    }

    for (auto& minionId : row.minionIds) {
        if (minionId.empty() || minionId == task.m_source->id)
            continue;
        MonsterTask minionTask = task;
        minionTask.m_source    = output.monstats.index.at(minionId);
        if (!minionTask.m_source->useForRando) {
            minionTask.m_skip = true;
        }
        processTask(minionTask, extraMinions);
        minionId = minionTask.m_dest.id;
        extraMinions.push_back(minionTask);
    }

    output.monstats.extra.push_back(row);
    output.monstatsEx.extra.push_back(rowEx);
    idCopies.push_back({ origId, row.id });
}

void ModuleMonRandomizer::prepareChimeraPools()
{
    std::vector<std::vector<size_t>> pools;

    std::vector<std::function<bool(const DataRow_monstats&)>> filters;
    filters.push_back([](const DataRow_monstats& row) -> bool {
        return !row.isRanged() && !row.isFlying(); // melee walkers
    });
    filters.push_back([](const DataRow_monstats& row) -> bool {
        return row.isRanged() && !row.isFlying(); // ranged walkers
    });

    filters.push_back([](const DataRow_monstats& row) -> bool {
        return !row.isRanged() && row.isFlying(); // melee flyers
    });
    filters.push_back([](const DataRow_monstats& row) -> bool {
        return row.isRanged() && row.isFlying(); // ranged flyers
    });

    pools.resize(filters.size());
    std::set<std::string> allGuestAiIds;

    for (size_t p = 0; p < pools.size(); ++p) {
        auto& pool = pools[p];
        auto& f    = filters[p];
        for (size_t i = 0; i < output.monstats.rows.size(); ++i) {
            auto& row = output.monstats.rows[i];
            if (row.useForRando && f(row)) {
                pool.push_back(i);
                row.usedAsGuest = true;
                allGuestAiIds.insert(row.AI);
            }
        }
    }

    std::vector<std::string> nonRando;
    for (size_t i = 0; i < output.monstats.rows.size() && i < 700; ++i) {
        auto& row = output.monstats.rows[i];
        if (!row.useForRando && !row.usedAsGuest && !allGuestAiIds.contains(row.AI)) {
            nonRando.push_back(row.AI);
            continue;
        }
    }
    Logger() << "Unused: " << printStringSetSummary(nonRando, 20);

    for (auto& pool : pools) {
        if (pool.size() < 2)
            return;
        std::vector<std::string> ais;

        for (size_t i = 0; i < pool.size(); ++i) {
            const size_t recipientIdx = pool[i];
            auto         ai           = output.monstats.rows[recipientIdx].AI;
            ais.push_back(ai);
        }
        Logger() << "AI pool: " << printStringSetSummary(ais, 10);

        for (size_t i = 0; i < pool.size(); ++i) {
            const size_t shiftedI = pool.size() > 10 ? rng(pool.size()) : ((i + 1) % pool.size());

            const size_t recipientIdx = pool[i];
            const size_t donorIdx     = pool[shiftedI];

            DataRow_monstats&       recipient = output.monstats.rows[recipientIdx];
            const DataRow_monstats& donor     = output.monstats.rows[donorIdx];

            recipient.donorIdx = donorIdx;
        }
    }
}

void ModuleMonRandomizer::prepareGuestPools()
{
    StringSet monSet;
    for (auto& row : output.monstats.rows) {
        if (!row.useForRando || !row.spawnedAsNormal || row.hasMinions)
            continue;

        validGuestIndexes.push_back(&row);
    }
}

void ModuleMonRandomizer::generateChimera(DataRow_monstats& chimera, DataRow_monstatsEx& chimeraEx)
{
    if (chimera.donorIdx == size_t(-1)) {
        chimera.donorId = "FAIL";
        Logger() << "Failed to generate chimera for " << chimera.id << " - no donor";
        return;
    }

    const DataRow_monstats&   donor   = output.monstats.rows[chimera.donorIdx];
    const DataRow_monstatsEx& donorEx = *output.monstatsEx.index.at(donor.monStatsEx);

    chimera.donorId = donor.id;

    // behavioral brain components from the AI donor row
    chimera.AI     = donor.AI;
    chimera.aipar  = donor.aipar;
    chimera.skills = donor.skills;

    // AI skill frames
    chimeraEx.hitClass = donorEx.hitClass;

    bool result = true;
    // Handle skill mappings / AI sequence
    for (auto& skill : chimera.skills) {
        std::string& skillName = skill.name;
        std::string& mode      = skill.mode;

        // Check if the donor mode string represents a custom sequence hook
        if (mode.starts_with("seq_")) {
            if (!compatibleMonSeq(mode, chimeraEx)) {
                const auto newMode = chimeraEx.getSafeSupportedMode();
                skill.mode         = animationModeToString(newMode);
            }
        } else {
            AnimationMode directMode = animationModeFromString(mode);
            assert(directMode != AnimationMode::Undefined);
            const auto newMode = chimeraEx.hasMode(directMode) ? directMode : chimeraEx.getSafeSupportedMode();
            skill.mode         = animationModeToString(newMode);
        }
    }
}

void ModuleMonRandomizer::TCTable::add(const std::string& id, int group, int level)
{
    if (!group || !level)
        return;
    TCGroup& g = groups[group];
    g.records.push_back(TCGroup::TCRecord{ id, level });
    tc2group[id] = group;
}

std::string ModuleMonRandomizer::TCTable::promote(const std::string& tc, const int oldLevel, const int newLevel) const
{
    if (!tc2group.contains(tc) || oldLevel == newLevel)
        return tc;
    const TCGroup& g = groups.at(tc2group.at(tc));
    std::string    res;
    for (int i = g.records.size() - 1; i >= 0; i--) {
        if (g.records[i].level > newLevel)
            res = g.records[i].id;
        else
            break;
    }
    return res.empty() ? tc : res;
}

void ModuleMonRandomizer::MonResist::readFromRow(const DataRow_monstats& row, size_t i)
{
    m_resist[Element::Phys] = row.resDm[i].value_or(0);
    m_resist[Element::Mag]  = row.resMa[i].value_or(0);
    m_resist[Element::Fire] = row.resFi[i].value_or(0);
    m_resist[Element::Ltng] = row.resLi[i].value_or(0);
    m_resist[Element::Cold] = row.resCo[i].value_or(0);
    m_resist[Element::Pois] = row.resPo[i].value_or(0);
}

void ModuleMonRandomizer::MonResist::writeToRow(DataRow_monstats& row, size_t i) const
{
    row.resDm[i] = m_resist.at(Element::Phys);
    row.resMa[i] = m_resist.at(Element::Mag);
    row.resFi[i] = m_resist.at(Element::Fire);
    row.resLi[i] = m_resist.at(Element::Ltng);
    row.resCo[i] = m_resist.at(Element::Cold);
    row.resPo[i] = m_resist.at(Element::Pois);
}

void ModuleMonRandomizer::MonResist::shuffle(RandomGenerator& rng)
{
    std::map<Element, Element> permutation;
    const std::vector<Element> commonElems{ Element::Fire, Element::Ltng, Element::Cold, Element::Pois };
    std::set<Element>          remaining{ Element::Phys, Element::Mag, Element::Fire, Element::Ltng, Element::Cold, Element::Pois };
    // first, Physical and Magic have 20% chance to move to elemental, 20% to swap or 60% to stay the same.
    if (rng(5) == 0) {
        permutation[Element::Phys] = Element::Mag;
        permutation[Element::Mag]  = Element::Phys;
        remaining.erase(Element::Phys);
        remaining.erase(Element::Mag);
    } else {
        auto remainingCopy = remaining;
        remainingCopy.erase(Element::Phys);
        remainingCopy.erase(Element::Mag);
        for (Element elem : { Element::Phys, Element::Mag }) {
            if (rng(5) == 0) {
                auto it = remainingCopy.begin();
                if (remainingCopy.size() > 1)
                    std::advance(it, rng(remainingCopy.size()));
                Element dest      = *it;
                permutation[elem] = dest;
                remainingCopy.erase(dest);
                remaining.erase(dest);
            } else {
                permutation[elem] = elem;
                remaining.erase(elem);
            }
        }
    }
    // second, common element permute with the rest
    for (Element elem : commonElems) {
        auto it = remaining.begin();
        if (remaining.size() > 1)
            std::advance(it, rng(remaining.size()));
        Element dest      = *it;
        permutation[elem] = dest;
        remaining.erase(dest);
    }
    assert(remaining.size() == 0);
    assert(permutation.size() == 6);
    assert(permutation.count(Element::Phys));
    assert(permutation.count(Element::Mag));
    assert(permutation.count(Element::Fire));
    assert(permutation.count(Element::Ltng));
    assert(permutation.count(Element::Cold));
    assert(permutation.count(Element::Pois));
    std::map<Element, int> newResist;
    for (const auto& [src, dest] : permutation)
        newResist[dest] = m_resist[src];

    m_resist = newResist;
}

std::string ModuleMonRandomizer::MonsterTask::toString() const
{
    std::ostringstream os;
    os << "{ srcId:" << m_source->id;
    if (m_skip) {
        os << ", SKIP }";
        return os.str();
    }
    os << ", dstId:" << m_dest.id;
    os << ", oldAI:" << m_source->AI;
    os << ", newAI:" << m_dest.AI;
    os << ", makeChimera:" << m_makeChimera;
    if (m_makeChimera)
        os << ", donor:" << m_dest.donorId;

    os << ", rollStat:" << m_rollStat;
    os << ", rollRes:" << m_rollRes;
    os << ", tgtNorm:" << m_targetNormalLevel;
    os << ", lvl:" << m_source->level.value_or(0);

    os << "}";
    return os.str();
}

}
