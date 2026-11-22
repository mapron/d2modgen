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
    for (auto& [group, rec] : groupMap)
        for (auto id : rec.ids)
            ids.insert(id);
    for (auto& [id, _] : idMap)
        ids.insert(id);
    for (auto& [id, _] : parMap)
        ids.insert(id);
}

std::vector<ParRecord*> AllAttributeData::findExistingGroupPar(AttributeGroup id)
{
    std::vector<ParRecord*> result;
    for (auto& [_, rec] : parMap) {
        auto desc = getAttributeDesc(rec.id);
        if (desc.group == id) {
            result.push_back(&rec);
        }
    }
    return result;
}

void StatisticsTable::addSnapshot(const AttributeStatistics& allData, int ver)
{
    versions.insert(ver);
    for (auto& [id, rec] : allData) {
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

std::string NonParRecord::toString() const
{
    std::string result = "    NonParRecord{\n";
    result += "        .freq      = " + std::to_string(freq) + ",\n";
    result += "        .freqMult  = " + freqMult.toString() + ",\n";
    result += "        .powerMult = " + powerMult.toString() + ",\n";
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
    result += "        .minLevel  = " + std::to_string(minLevel) + ",\n";

    /// model VVV
    result += "        .minMedian = " + std::to_string(minMedian) + ",\n";
    result += "        .maxMedian = " + std::to_string(maxMedian) + ",\n";
    result += "        .limit     = " + std::to_string(limit) + ",\n";
    if (penMax)
        result += "        .penMax    = " + std::to_string(penMax) + ",\n";

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

std::string ParRecordParam::toString() const
{
    std::string result = "            ParRecordParam{\n";

    result += "                .freq      = " + std::to_string(freq) + ",\n";
    result += "                .par       = " + std::to_string(par) + ",\n";
    result += "                .minLevel  = " + std::to_string(minLevel) + ",\n";

    /// model VVV
    result += "                .minMedian = " + std::to_string(minMedian) + ",\n";
    result += "                .maxMedian = " + std::to_string(maxMedian) + ",\n";

    auto printCurve = [&result](const PermilleCurve& curve, const std::string& name) {
        if (curve != s_defaultCurve) {
            result += "                ." + name + " = { ";
            for (size_t i = 0; auto val : curve) {
                if (i++)
                    result += ", ";
                result += val.toString();
            }
            result += " },\n";
        }
    };
    printCurve(minCurve, "minCurve ");
    printCurve(maxCurve, "maxCurve ");

    result += "            },\n";
    return result;
}

std::string ParRecord::toString() const
{
    std::string result = "    ParRecord{\n";
    result += "        .id        = " + enumToString(id, true) + ",\n";

    result += "        .freqMult  = " + freqMult.toString() + ",\n";
    result += "        .powerMult = " + powerMult.toString() + ",\n";

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

    result += "        .params    = {\n";
    for (const auto& param : params) {
        result += param.toString();
    }
    result += "        },\n";

    result += "    },\n";
    return result;
}

}
