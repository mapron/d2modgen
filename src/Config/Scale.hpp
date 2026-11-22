/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <algorithm>
#include <compare>
#include <string>
#include <cstdint>

namespace D2ModGen {

template<int Denom>
struct Scaled {
    int64_t raw = 0;

    constexpr Scaled operator-() const { return { -raw }; }
    constexpr Scaled operator+() const { return *this; }

    constexpr Scaled operator+(Scaled rhs) const { return { raw + rhs.raw }; }
    constexpr Scaled operator-(Scaled rhs) const { return { raw - rhs.raw }; }

    template<int OtherDenom>
    constexpr auto operator*(Scaled<OtherDenom> rhs) const
    {
        constexpr int     OutDenom = std::max(Denom, OtherDenom);
        constexpr int64_t Divisor  = std::min(Denom, OtherDenom);
        return Scaled<OutDenom>{ ((raw * rhs.raw) / Divisor) };
    }

    template<int OtherDenom>
    constexpr Scaled& operator*=(Scaled<OtherDenom> rhs)
    {
        raw = raw * rhs;
        return *this;
    }

    friend constexpr int operator*(int val, Scaled s)
    {
        return static_cast<int>((val * s.raw) / Denom);
    }
    friend constexpr int operator*(Scaled s, int val)
    {
        return val * s;
    }
    friend constexpr int64_t operator*(int64_t val, Scaled s)
    {
        return ((val * s.raw) / Denom);
    }
    friend constexpr int64_t operator*(Scaled s, int64_t val)
    {
        return val * s;
    }

    template<int TargetDenom>
    constexpr Scaled<TargetDenom> as() const
    {
        return { static_cast<int64_t>((raw * TargetDenom) / Denom) };
    }

    auto operator<=>(const Scaled&) const = default;

    std::string toString() const
    {
        std::string result = std::to_string(raw);
        if (Denom == 100)
            result += "_pct";
        else if (Denom == 1000)
            result += "_perm";
        else
            result += "/" + std::to_string(Denom);
        return result;
    }
};

using Percent  = Scaled<100>;
using Permille = Scaled<1000>;

constexpr Percent operator""_pct(unsigned long long v)
{
    return { static_cast<int64_t>(v) };
}
constexpr Permille operator""_perm(unsigned long long v)
{
    return { static_cast<int64_t>(v) };
}

constexpr const inline Percent  g_identityPct  = 100_pct;
constexpr const inline Permille g_identityPerm = 1000_perm;

inline constexpr Percent inverse(Percent value)
{
    //100 => 100
    // 75 => 133
    // 150 => 66
    return { 100LL / value.raw };
}

}
