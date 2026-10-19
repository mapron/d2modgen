/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "AttributeValues.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <cassert>
#include <cmath>

namespace D2ModGen {

namespace {
void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string printEnum(AttributeId code)
{
    auto str = idToString(code);
    replaceAll(str, "/lvl", "_perlvl");
    replaceAll(str, "%", "_percent");
    replaceAll(str, "-", "_");
    return "AttributeId::" + str;
}
}

std::string ApproximationResults::toString() const
{
    if (!frequency)
        return "{}";
    std::string result = "{ " + std::to_string(frequency) + ", { { " + results[0].toString();
    for (size_t i = 1; i < s_segments; ++i)
        result += ", " + results[i].toString();
    result += " } } }";
    return result;
}

void ApproximationResults::makeMonotonic(bool positive)
{
    for (size_t i = 1; i < s_segments; ++i) {
        const ApproximationResult& prev = results[i - 1];
        ApproximationResult&       curr = results[i];
        if (prev.avg && !curr.avg)
            curr.avg = prev.avg;

        if (positive && prev.avg > curr.avg)
            curr.avg = prev.avg;
        if (!positive && prev.avg < curr.avg)
            curr.avg = prev.avg;

        const int prevMax = prev.avg + prev.plus;
        const int prevMin = prev.avg + prev.minus;
        const int currMax = curr.avg + curr.plus;
        const int currMin = curr.avg + curr.minus;
        const int incMin  = currMin - prevMin;
        const int incMax  = currMax - prevMax;

        if (positive && incMin < 0 || !positive && incMin > 0) {
            // currMin - prevMin < 0
            // currMin - prevMin => 0
            // curr.avg + curr.minus - prevMin => 0
            // curr.minus => prevMin - curr.avg
            curr.minus = prevMin - curr.avg;
        }
        if (positive && incMax < 0 || !positive && incMax > 0) {
            curr.plus = prevMax - curr.avg;
        }
    }
}
void ApproximationResults::linearApprox(const LevelValueList& values)
{
    const std::array<std::pair<int, int>, s_segments> limits{ { { -20, 20 }, { 0, 40 }, { 20, 60 }, { 40, 80 }, { 60, 300 } } };
    std::array<int, s_segments>                       sizes{ {} };

    const size_t n = values.size();
    frequency      = n;
    for (const LevelValue& value : values) {
        assert(value.level >= -1 && value.level < 260);
        size_t hit = 0;
        for (size_t s = 0; s < ApproximationResults::s_segments; ++s) {
            if (limits[s].first <= value.level && value.level < limits[s].second) {
                auto& r = results[s];
                if (!sizes[s]) {
                    r.minus = value.min;
                    r.plus  = value.max;
                }
                hit++;
                sizes[s]++;
                r.avg += value.min;
                r.avg += value.max;
                r.minus = std::min(r.minus, value.min);
                r.plus  = std::max(r.plus, value.max);
            }
        }
        assert(hit);
    }
    size_t hit = 0;
    for (size_t s = 0; s < s_segments; ++s) {
        auto& r = results[s];
        if (!sizes[s])
            continue;
        hit++;
        r.avg /= sizes[s] * 2;
        r.minus = r.minus - r.avg;
        r.plus  = r.plus - r.avg;
        r.avg   = smartRound(r.avg, smartRoundDetect(r.avg));
        r.minus = smartRound(r.minus, smartRoundDetect(r.minus));
        r.plus  = smartRound(r.plus, smartRoundDetect(r.plus));
    }
    assert(hit);
}

std::string slotTypeName(SlotType slotType)
{
    switch (slotType) {
        case SlotType::Undefined:
            return "?";
        case SlotType::MainArmor:
            return "MainArmor";
        case SlotType::OtherArmor:
            return "OtherArmor";
        case SlotType::Ring:
            return "Ring";
        case SlotType::Amulet:
            return "Amulet";
        case SlotType::Weapon:
            return "Weapon";
        case SlotType::Charm:
            return "Charm";
        case SlotType::Socketable:
            return "Socketable";
        case SlotType::Set:
            return "Set";
    }
    return "?";
}

std::string ApproximationResult::toString() const
{
    if (!avg && !minus && !plus)
        return "{}";
    if (!minus && !plus) {
        return "{ " + std::to_string(avg) + " }";
    }
    return "{ " + std::to_string(avg) + " , +" + std::to_string(plus) + ", " + std::to_string(minus) + " }";
}

int buildResultsSimple(const ApproximationResults& slotApprox, const ApproximationResults& globalApprox)
{
    int result = 0;
    int cnt    = 0;
    for (size_t i = 0; i < ApproximationResults::s_segments; ++i) {
        auto&     s   = slotApprox.results[i];
        const int avg = s.avg;
        if (!avg)
            continue;

        auto& g = globalApprox.results[i];
        cnt++;
        result += std::abs(g.avg) < 10 ? 100 : avg * 100 / g.avg;
    }
    result /= cnt;
    if (result >= 90 && result <= 110)
        result = 100;
    return result;
}

std::string NonPropAttributeRecord::toString() const
{
    std::string result = "    NonPropAttributeRecord{\n";
    result += "        .code       = " + printEnum(code) + ",\n";
    result += "        .frequency  = " + std::to_string(frequency) + ",\n";
    if (goodValues.frequency) {
        result += "        .goodValues = " + goodValues.toString() + ",\n";
    }
    if (badValues.frequency) {
        result += "        .badValues  = " + badValues.toString() + ",\n";
    }
    result += "    },\n";
    return result;
}

std::string LevelValue::toString() const
{
    return "{ " + std::to_string(level) + ", " + std::to_string(min) + ", " + std::to_string(max) + " }";
}

std::string AffixPropLeveled::toString() const
{
    if (!hasRange())
        return "{ " + std::to_string(level) + ", \"" + par + "\" }";
    return "{ " + std::to_string(level) + ", \"" + par + "\", " + std::to_string(min.value_or(0)) + ", " + std::to_string(max.value_or(0)) + " }";
}

std::string PropAttributeRecord::toString() const
{
    std::string result = "    PropAttributeRecord{\n";
    result += "        .code       = " + printEnum(code) + ",\n";
    result += "        .slotValues = {\n";
    for (const auto& [slotType, data] : slotValues) {
        result += "            { SlotType::" + slotTypeName(slotType) + ",\n";
        result += "                {\n";
        for (const auto& dataRec : data) {
            result += "                    " + dataRec.toString() + ",\n";
        }
        result += "                },\n";
        result += "            },\n";
    }
    result += "        },\n";
    result += "    },\n";
    return result;
}

std::string printMap(const ApproximationResultsSimpleMap& data)
{
    std::string result = "{\n";
    for (const auto& [slotType, data2] : data) {
        result += "            { SlotType::" + slotTypeName(slotType) + ",\n";
        result += "                {\n";
        for (const auto& [code, power] : data2) {
            result += "                    { " + printEnum(code) + ", " + std::to_string(power) + " },\n";
        }
        result += "                },\n";
        result += "            },\n";
    }
    result += "}";
    return result;
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

}
