/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "AttributeValues.hpp"
#include "Logger.hpp"

#include <cassert>
#include <cmath>
#include <algorithm>

namespace D2ModGen {

namespace {

struct MedianFinder {
    std::vector<int> m_data;

    void add(int val)
    {
        m_data.push_back(val);
    }
    int calcPercentile(int percentile)
    {
        const auto mid = m_data.begin() + m_data.size() * percentile / 100;
        return *mid;
    }

    void calc(NonParValueSummary::Stat& stat)
    {
        if (m_data.empty()) {
            stat = {};
            return;
        }
        std::sort(m_data.begin(), m_data.end());

        stat.min = m_data.front();
        stat.max = m_data.back();
        stat.p50 = calcPercentile(50);
        if (m_data.size() > 9) {
            stat.p10 = calcPercentile(10);
            stat.p90 = calcPercentile(90);
        } else {
            stat.p10 = stat.p90 = stat.p50;
        }
    }
};

struct MedianFinderGroup {
    MedianFinder low;
    MedianFinder high;
    MedianFinder par;

    void add(const NonParValue& val)
    {
        low.add(val.min);
        high.add(val.max);
        par.add(val.par);
    }
    void calc(NonParValueSummary& stat)
    {
        low.calc(stat.low);
        high.calc(stat.high);
        par.calc(stat.par);
        stat.freq = FREQ_PER_ONE_PROP * low.m_data.size();
    }
};

}

std::string optionalToString(const std::optional<int>& opt)
{
    return opt.has_value() ? std::to_string(opt.value()) : "{}";
}

void NonParValueSummary::Stat::makeMonotonic(const Stat& prev)
{
    auto fields = { &NonParValueSummary::Stat::min, &NonParValueSummary::Stat::p10, &NonParValueSummary::Stat::p50, &NonParValueSummary::Stat::p90, &NonParValueSummary::Stat::max };
    for (auto&& m : fields) {
        if (prev.*m && !(this->*m))
            this->*m = prev.*m;

        if (prev.*m > this->*m)
            this->*m = prev.*m;
    }
}

void NonParValueSummary::Stat::rescaleValuesOnTotal(const Stat& total)
{
    if (!total.p90) {
        return;
    }
    min = min * 1000 / total.p90;
    max = max * 1000 / total.p90;
    p10 = p10 * 1000 / total.p90;
    p50 = p50 * 1000 / total.p90;
    p90 = p90 * 1000 / total.p90;
}

void NonParValueSummary::makeMonotonic(const NonParValueSummary& prev)
{
    low.makeMonotonic(prev.low);
    high.makeMonotonic(prev.high);
    par.makeMonotonic(prev.par);
}

void NonParValueSummary::rescaleValuesOnTotal(const NonParValueSummary& total)
{
    low.rescaleValuesOnTotal(total.low);
    high.rescaleValuesOnTotal(total.high);
    par.rescaleValuesOnTotal(total.par);
}

std::string NonParValueSummary::Stat::toString() const
{
    return "{ " + std::to_string(min) + ", " + std::to_string(p10) + ", " + std::to_string(p50) + ", " + std::to_string(p90) + ", " + std::to_string(max) + " }";
}

std::string NonParValueSummary::toString() const
{
    if (!freq)
        return "{}";

    std::string result = "{ .freq = " + std::to_string(freq);
    if (!low.isEmpty())
        result += ", .low = " + low.toString();
    if (!high.isEmpty())
        result += ", .high = " + high.toString();
    if (!par.isEmpty())
        result += ", .par = " + par.toString();

    return result + " }";
}

std::string NonParValueSummary::toStringTabbed() const
{
    return std::to_string(low.min) + "\t" + std::to_string(low.p50) + "\t" + std::to_string(low.max) + "\t"      //
           + std::to_string(high.min) + "\t" + std::to_string(high.p50) + "\t" + std::to_string(high.max) + "\t" //
           + std::to_string(par.min) + "\t" + std::to_string(par.p50) + "\t" + std::to_string(par.max);
}

void NonParValueSummary::make(const NonParValueList& list)
{
    auto& total = *this;
    total       = {};
    if (list.empty())
        return;

    MedianFinderGroup averages;
    for (const NonParValue& value : list) {
        averages.add(value);
    }
    averages.calc(total);
}

std::string NonParValueByLevel::toString(bool printLevels) const
{
    if (!total.freq)
        return "{}";
    std::string result = "NonParValueByLevel{\n";
    result += "            .total = " + total.toString() + ",\n";
    if (printLevels) {
        result += "            .levels{ {\n";
        for (size_t i = 0; i < s_segments; ++i) {
            result += "                " + levels[i].toString() + ",\n";
        }
        result += "            } },\n";
    }
    result += "        }";
    return result;
}

void NonParValueByLevel::makeMonotonic()
{
    for (size_t i = 1; i < s_segments; ++i) {
        const NonParValueSummary& prev = levels[i - 1];
        NonParValueSummary&       curr = levels[i];
        if (!prev.freq)
            continue;
        curr.makeMonotonic(prev);
        if (!curr.freq) {
            curr.freq = 1;
            total.freq++;
        }
    }
}
void NonParValueByLevel::makeApprox(const NonParValueList& list, NonParValueByLevel* prev)
{
    total = {};

    for (auto& summary : levels) {
        summary = {};
    }
    if (list.empty())
        return;

    MedianFinderGroup                         averages;
    std::array<MedianFinderGroup, s_segments> levelAverages;

    for (const NonParValue& value : list) {
        auto levelTier = makeTier(value.level);
        averages.add(value);

        auto& byLevel = levels[levelTier];
        levelAverages[levelTier].add(value);
    }
    averages.calc(total);
    for (size_t i = 0; i < s_segments; ++i) {
        levelAverages[i].calc(levels[i]);
    }
}

void NonParValueByLevel::rescaleFreq(Percent percent, int denum)
{
    auto scaler = [percent, denum](int& val) {
        if (!val)
            return;
        val = std::max(1, (val * percent) / denum);
    };
    scaler(total.freq);
    for (size_t i = 0; i < s_segments; ++i)
        scaler(levels[i].freq);
}

void NonParValueByLevel::rescaleValuesOnTotal()
{
    for (size_t i = 0; i < s_segments; ++i)
        levels[i].rescaleValuesOnTotal(total);
}

size_t NonParValueByLevel::estimateSize() const
{
    size_t result = 0;
    for (size_t i = 0; i < s_segments; ++i) {
        if (levels[i].freq)
            result++;
    }
    return result;
}

void NonParRecord::addScale(SlotType slot, const NonParValueSummary& slotApprox, const NonParValueSummary& globalApprox)
{
    if (!slotApprox.freq || !globalApprox.freq)
        return;

    const int global = std::abs(globalApprox.high.max);
    const int local  = std::abs(slotApprox.high.max);
    const int prev   = slotTypesScale.get(slot).value_or(0);

    int result = global < 10 ? 100 : local * 100 / global;
    result     = std::max(prev, result);

    slotTypesScale.insert(slot, result);
    slotTypes.insert(slot);
}

void NonParRecord::cleanupScale()
{
    auto old       = slotTypesScale;
    slotTypesScale = {};
    for (const auto& [slot, percent] : old.asVector()) {
        int result = std::clamp(percent, 20, 100);
        if (result != 100)
            slotTypesScale.insert(slot, result);
    }
}

size_t NonParRecord::estimateSize() const
{
    size_t result = 0;
    result += comm.estimateSize();
    return result * slotTypes.estimateSize();
}

void NonParRecord::rescaleFreq(Percent percent, int denum)
{
    comm.rescaleFreq(percent, denum);
}

std::string NonParRecord::toString() const
{
    std::string result = "    NonParRecord{\n";
    result += "        .code           = " + enumToString(code, true) + ",\n";
    result += "        .freqFactor     = " + std::to_string(freqFactor) + ",\n";
    result += "        .slotTypes      = { ";
    for (size_t i = 0; const auto& slotType : slotTypes.asVector()) {
        if (i++)
            result += ", ";
        result += enumToString(slotType, true);
    }
    result += " },\n";
    /*
    result += "        .slotTypesScale = {";
    const bool slotTypesScaleEmpty = slotTypesScale.empty();
    if (!slotTypesScaleEmpty)
        result += " ";
    for (size_t i = 0; const auto& [slotType, scale] : slotTypesScale.asVector()) {
        if (i++)
            result += ", ";
        result += "{ " + enumToString(slotType, true) + ", " + std::to_string(scale) + " }";
    }
    if (!slotTypesScaleEmpty)
        result += " ";
    result += "},\n";*/

    std::string pad = "          ";
    if (comm.total.freq) {
        result += "        .comm " + pad + "= " + comm.toString() + ",\n";
        pad.clear();
    }

    result += "    },\n";
    return result;
}

std::string PropWithRange::toString() const
{
    if (!hasRange())
        return "{ " + optionalToString(par) + " }";
    return "{ " + optionalToString(par) + ", " + std::to_string(min.value_or(0)) + ", " + std::to_string(max.value_or(0)) + " }";
}

std::string PropListByLevel::toString() const
{
    if (!freq)
        return "{}";
    std::string result = "PropListByLevel{\n";
    result += "            .freq = " + std::to_string(freq) + ",\n";
    result += "            .levels{ {\n";
    for (size_t i = 0; i < s_segments; ++i) {
        if (levels[i].empty()) {
            result += "                {},\n";
        } else {
            result += "                {\n";
            for (auto& rec : levels[i])
                result += "                    " + rec.toString() + ",\n";
            result += "                },\n";
        }
    }
    result += "            } },\n";
    result += "        }";
    return result;
}

size_t PropListByLevel::estimateSize() const
{
    size_t result = 0;
    for (size_t i = 0; i < s_segments; ++i) {
        result += levels[i].size();
    }
    return result;
}

std::string ParRecord::toString() const
{
    std::string result = "    ParRecord{\n";
    result += "        .code       = " + enumToString(code, true) + ",\n";
    result += "        .freqFactor = " + std::to_string(freqFactor) + ",\n";

    result += "        .slotTypes  = { ";
    for (size_t i = 0; const auto& slotType : slotTypes.asVector()) {
        if (i++)
            result += ", ";
        result += enumToString(slotType, true);
    }
    result += " },\n";
    std::string pad = "      ";

    if (comm.freq) {
        result += "        .comm " + pad + "= " + comm.toString() + ",\n";
        pad.clear();
    }

    result += "    },\n";
    return result;
}

size_t ParRecord::estimateSize() const
{
    size_t result = 0;
    result += comm.estimateSize();
    return result * slotTypes.estimateSize();
}

void ParRecord::rescaleFreq(Percent percent, int denum)
{
    freqFactor = (freqFactor * percent) / denum;
}

int smartRound(int value, int rounding)
{
    return rounding <= 1 ? value : static_cast<int>(std::round(double(value) / rounding) * rounding);
}

int smartRoundDetect(int value)
{
    int a = std::abs(value);
    if (a < 12)
        return 1;
    if (a < 30)
        return 2;
    if (a < 100)
        return 5;
    return 10;
}

void AllAttributeData::refreshIds()
{
    ids.clear();
    for (auto& rec : par)
        ids.insert(rec.code);
    for (auto& rec : nonPar)
        ids.insert(rec.code);
    for (auto& [group, rec] : groupMap)
        for (auto id : rec.ids)
            ids.insert(id);
    for (auto& [id, _] : idMap)
        ids.insert(id);
}

/*
void AllAttributeData::insert(const AllAttributeData& other)
{
    par.reserve(par.size() + other.par.size());
    nonPar.reserve(nonPar.size() + other.nonPar.size());
    for (auto& rec : other.par) {
        if (ids.contains(rec.code))
            continue;
        ids.insert(rec.code);
        par.push_back(rec);
    }
    for (auto& rec : other.nonPar) {
        if (ids.contains(rec.code))
            continue;
        ids.insert(rec.code);
        nonPar.push_back(rec);
    }
}*/

void AllAttributeData::erase(AttributeId id)
{
    ids.erase(id);
    std::erase_if(par, [id](const auto& rec) { return rec.code == id; });
    std::erase_if(nonPar, [id](const auto& rec) { return rec.code == id; });
}

void AllAttributeData::add(ParRecord rec)
{
    ids.insert(rec.code);
    par.push_back(std::move(rec));
}

void AllAttributeData::add(NonParRecord rec)
{
    ids.insert(rec.code);
    nonPar.push_back(std::move(rec));
}

void AllAttributeData::addGrouped(NonParRecordNew rec)
{
    ids.insert_range(rec.ids);
    groupMap[rec.group] = std::move(rec);
}

void AllAttributeData::addIdd(NonParRecordNew rec)
{
    ids.insert_range(rec.ids);
    idMap[*rec.ids.begin()] = std::move(rec);
}

ParRecord* AllAttributeData::findExistingPar(AttributeId id)
{
    if (!ids.contains(id))
        return nullptr;
    for (auto& rec : par) {
        if (id != rec.code)
            continue;

        return &rec;
    }
    return nullptr;
}

NonParRecord* AllAttributeData::findExistingNonPar(AttributeId id)
{
    if (!ids.contains(id))
        return nullptr;
    for (auto& rec : nonPar) {
        if (id != rec.code)
            continue;

        return &rec;
    }
    return nullptr;
}

void StatisticsTable::addSnapshot(const AllAttributeData& allData, int ver)
{
    versions.insert(ver);
    for (auto& [id, rec] : allData.statistics) {
        const NonParValueSummary& row  = rec;
        const int                 freq = rec.freq;

        rows[id][ver].freq = freq;
        rows[id][ver].data = rec.toStringTabbed();
    }
}

void StatisticsTable::traceStatistics() const
{
    Logger() << "---------------- " << "Post-generation summary" << " ----------------";

    Logger() << "Attribute\tVer(0=pre-1=post)\tFreq\tFreq-diff%\tLow-min\tLow-med\tLow-max\tHigh-min\tHigh-med\tHigh-max\tPar-min\tPar-med\tPar-max";

    for (const auto& [code, row_] : rows) {
        int prevFreq = -1;
        for (int ver : versions) {
            if (row_.contains(ver)) {
                auto&       row = row_.at(ver);
                std::string diffstr;
                if (prevFreq != -1) {
                    int min    = std::min(prevFreq, row.freq);
                    int max    = std::max(prevFreq, row.freq);
                    int diff   = max - min;
                    int change = (diff * 100 / min);
                    if (change > 20)
                        diffstr = std::to_string(change);
                }
                prevFreq = row.freq;
                Logger() << enumToString(code) << "\t" << ver << "\t" << std::to_string(row.freq) << "\t" << diffstr << "\t" << row.data;
            } else {
                Logger() << enumToString(code) << "\t" << ver << "\t" << -1;
            }
        }
    }

    Logger() << "---------------- " << "END" << " ----------------";
}

std::string NonParRecordNew::toString() const
{
    std::string result = "    NonParRecordNew{\n";
    result += "        .freq      = " + std::to_string(freq) + ",\n";
    result += "        .minMedian = " + std::to_string(minMedian) + ",\n";
    result += "        .maxMedian = " + std::to_string(maxMedian) + ",\n";
    if (penMax)
        result += "        .penMax    = " + std::to_string(penMax) + ",\n";
    result += "        .minLevel  = " + std::to_string(minLevel) + ",\n";

    result += "        .group     = " + enumToString(group, true) + ",\n";

    result += "        .ids       = { ";
    for (size_t i = 0; const auto& id : ids) {
        if (i++)
            result += ", ";
        result += enumToString(id, true);
    }
    result += " },\n";

    result += "        .slotTypes = { ";
    for (size_t i = 0; const auto& slotType : slotTypes.asVector()) {
        if (i++)
            result += ", ";
        result += enumToString(slotType, true);
    }
    result += " },\n";

    if (!slotScale.empty()) {
        result += "        .slotScale = { ";
        for (size_t i = 0; const auto& [slotType, scale] : slotScale.asVector()) {
            if (i++)
                result += ", ";
            result += "{ " + enumToString(slotType, true) + ", " + scale.toString() + " }";
        }
        result += " },\n";
    }

    auto printCurve = [&result](const PermilleCurve& curve, const std::string& name) {
        if (curve != s_defaultCurve) {
            result += "        ." + name + " = { ";
            for (size_t i = 0; auto val : curve) {
                if (i++)
                    result += ", ";
                result += val.toString();
            }
            result += " },\n";
        }
    };
    printCurve(medCurve, "medCurve ");
    printCurve(ratioP000, "ratioP000");
    printCurve(ratioP010, "ratioP010");
    printCurve(ratioP090, "ratioP090");
    printCurve(ratioP100, "ratioP100");

    result += "    },\n";
    return result;
}

}
