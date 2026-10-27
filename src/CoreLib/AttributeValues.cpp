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
class MedianFinder {
private:
    std::vector<int> m_dataLow;
    std::vector<int> m_dataHigh;
    std::vector<int> m_dataPar;

public:
    void add(const NonParValue& val)
    {
        m_dataLow.push_back(val.min);
        m_dataHigh.push_back(val.max);
        m_dataPar.push_back(val.par);
    }
    void calc(NonParValueSummary& stat)
    {
        if (m_dataLow.empty())
            return;
        {
            const auto mid = m_dataLow.begin() + m_dataLow.size() / 2;
            std::ranges::nth_element(m_dataLow, mid);
            stat.low.median = *mid;
        }
        {
            const auto mid = m_dataHigh.begin() + m_dataHigh.size() / 2;
            std::ranges::nth_element(m_dataHigh, mid);
            stat.high.median = *mid;
        }
        {
            const auto mid = m_dataPar.begin() + m_dataPar.size() / 2;
            std::ranges::nth_element(m_dataPar, mid);
            stat.par.median = *mid;
        }
    }
};

}

std::string optionalToString(const std::optional<int>& opt)
{
    return opt.has_value() ? std::to_string(opt.value()) : "{}";
}

void NonParValueSummary::Stat::setMinMaxForCalculation()
{
    min = std::numeric_limits<int>::max();
    max = std::numeric_limits<int>::min();
}

void NonParValueSummary::Stat::cleanMinMax()
{
    if (min == std::numeric_limits<int>::max())
        min = 0;
    if (max == std::numeric_limits<int>::min())
        min = 0;
}

void NonParValueSummary::Stat::makeMonotonic(const Stat& prev)
{
    auto fields = { &NonParValueSummary::Stat::min, &NonParValueSummary::Stat::median, &NonParValueSummary::Stat::max };
    for (auto&& m : fields) {
        if (prev.*m && !(this->*m))
            this->*m = prev.*m;

        if (prev.*m > this->*m)
            this->*m = prev.*m;
    }
}

void NonParValueSummary::setMinMaxForCalculation()
{
    low.setMinMaxForCalculation();
    high.setMinMaxForCalculation();
    par.setMinMaxForCalculation();
}

void NonParValueSummary::cleanMinMax()
{
    low.cleanMinMax();
    high.cleanMinMax();
    par.cleanMinMax();
}

void NonParValueSummary::makeMonotonic(const NonParValueSummary& prev)
{
    low.makeMonotonic(prev.low);
    high.makeMonotonic(prev.high);
    par.makeMonotonic(prev.par);
}

std::string NonParValueSummary::Stat::toString() const
{
    return "{ " + std::to_string(min) + ", " + std::to_string(median) + ", " + std::to_string(max) + " }";
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
    return std::to_string(low.min) + "\t" + std::to_string(low.median) + "\t" + std::to_string(low.max) + "\t"      //
           + std::to_string(high.min) + "\t" + std::to_string(high.median) + "\t" + std::to_string(high.max) + "\t" //
           + std::to_string(par.min) + "\t" + std::to_string(par.median) + "\t" + std::to_string(par.max);
}

void NonParValueSummary::make(const NonParValueList& list)
{
    auto& total = *this;
    total       = {};
    if (list.empty())
        return;

    total.setMinMaxForCalculation();

    MedianFinder averages;
    for (const NonParValue& value : list) {
        total.add(value);
        averages.add(value);
    }
    averages.calc(total);
    total.cleanMinMax();
}

std::string NonParValueByLevel::toString() const
{
    if (!total.freq)
        return "{}";
    std::string result = "NonParValueByLevel{\n";
    result += "            .total = " + total.toString() + ",\n";
    result += "            .levels{ {\n";
    for (size_t i = 0; i < s_segments; ++i) {
        result += "                " + levels[i].toString() + ",\n";
    }
    result += "            } },\n";
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

    total.setMinMaxForCalculation();

    for (auto& summary : levels)
        summary.setMinMaxForCalculation();

    MedianFinder                         averages;
    std::array<MedianFinder, s_segments> levelAverages;

    for (const NonParValue& value : list) {
        size_t levelBracket20 = std::clamp(value.level, 0, 99) / 20;

        if (prev) {
            auto& r = prev->levels[levelBracket20];
            if (std::abs(value.max) <= std::abs(r.high.max)) {
                r.add(value);
                prev->total.add(value);
                continue;
            }
        }
        total.add(value);
        averages.add(value);

        auto& byLevel = levels[levelBracket20];
        byLevel.add(value);
        levelAverages[levelBracket20].add(value);
    }
    averages.calc(total);
    total.cleanMinMax();
    for (size_t i = 0; i < s_segments; ++i) {
        levelAverages[i].calc(levels[i]);
        levels[i].cleanMinMax();
    }
}

void NonParValueByLevel::add(const NonParValueByLevel& other)
{
    total.add(other.total);
    for (size_t i = 0; i < s_segments; ++i)
        levels[i].add(other.levels[i]);
}

void NonParValueByLevel::rescaleFreq(int percent, int denum)
{
    auto scaler = [percent, denum](int& val) {
        if (!val)
            return;
        val = std::max(1, ((val * percent) / 100) / denum);
    };
    scaler(total.freq);
    for (size_t i = 0; i < s_segments; ++i)
        scaler(levels[i].freq);
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
    result += badd.estimateSize();
    result += comm.estimateSize();
    result += rare.estimateSize();
    result += epic.estimateSize();
    return result * slotTypes.estimateSize();
}

void NonParRecord::doHomogen(const std::vector<const NonParRecord*>& sources)
{
    for (const NonParRecord* source : sources) {
        for (auto slot : source->slotTypes.asVector()) {
            this->slotTypes.insert(slot);
            int powerOld = slotTypesScale.get(slot).value_or(100);
            int powerNew = source->slotTypesScale.get(slot).value_or(100);
            powerNew     = std::max(powerNew, powerOld);
            slotTypesScale.insert(slot, powerNew);
        }
        comm.add(source->comm);
        rare.add(source->rare);
        epic.add(source->epic);
    }

    for (auto slot : this->slotTypes.asVector()) {
        int power = slotTypesScale.get(slot).value_or(100);
        if (power == 100)
            slotTypesScale.erase(slot);
    }
}

void NonParRecord::rescaleFreq(int percent, int denum)
{
    comm.rescaleFreq(percent, denum);
    rare.rescaleFreq(percent, denum);
    epic.rescaleFreq(percent, denum);
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
    result += "},\n";

    std::string pad = "          ";
    if (badd.total.freq) {
        result += "        .badd " + pad + "= " + badd.toString() + ",\n";
        pad.clear();
    }
    if (comm.total.freq) {
        result += "        .comm " + pad + "= " + comm.toString() + ",\n";
        pad.clear();
    }
    if (rare.total.freq) {
        result += "        .rare " + pad + "= " + rare.toString() + ",\n";
        pad.clear();
    }
    if (epic.total.freq) {
        result += "        .epic " + pad + "= " + epic.toString() + ",\n";
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
    if (badd.freq) {
        result += "        .badd " + pad + "= " + badd.toString() + ",\n";
        pad.clear();
    }
    if (comm.freq) {
        result += "        .comm " + pad + "= " + comm.toString() + ",\n";
        pad.clear();
    }
    if (rare.freq) {
        result += "        .rare " + pad + "= " + rare.toString() + ",\n";
        pad.clear();
    }
    if (epic.freq) {
        result += "        .epic " + pad + "= " + epic.toString() + ",\n";
        pad.clear();
    }
    result += "    },\n";
    return result;
}

size_t ParRecord::estimateSize() const
{
    size_t result = 0;
    result += badd.estimateSize();
    result += comm.estimateSize();
    result += rare.estimateSize();
    result += epic.estimateSize();
    return result * slotTypes.estimateSize();
}

void ParRecord::rescaleFreq(int percent, int denum)
{
    freqFactor = ((freqFactor * percent) / 100) / denum;
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
}

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
}

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

}
