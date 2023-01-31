/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModgenCoreExport.hpp"

#include <string>

#include <ctime>

namespace D2ModGen {
/// Class describes time moment or time interval.
class MODGENCORE_EXPORT ChronoPoint {
    int64_t m_us; //!< microseconds

public:
    static const int64_t ONE_SECOND = 1000000LL;
    /// if now == true, then current ChronoPoint is created.
    ChronoPoint(bool now = false);

    /// Creates ChronoPoint from number of seconds
    ChronoPoint(int seconds);

    /// Creates ChronoPoint from number of seconds
    ChronoPoint(double seconds);

    /// Checks timempoint is valid.
    inline operator bool() const { return m_us != 0; }

    /// Floored number of seconds in ChronoPoint.
    inline int64_t GetSeconds() const { return m_us / ONE_SECOND; }

    /// Get microseconds
    inline int64_t GetUS() const { return m_us; }

    /// Set microseconds
    inline void SetUS(int64_t stamp) { m_us = stamp; }

    /// Microseconds after a whole second.
    inline int64_t GetFractionalUS() const { return m_us % ONE_SECOND; }

    /// Get time struct
    std::tm GetTm() const;

    /// Get time struct
    void SetTm(const std::tm& t, int ms = 0);

    /// Create ChronoPoint from hms
    void SetTime(int hour, int minute, int second, int ms = 0);

    /// Convert to ChronoPoint using system local time (for user interaction).
    ChronoPoint& ToLocal();   // Modify self
    ChronoPoint& FromLocal(); // Modify self

    /// Returns elapsed time from current value.
    inline ChronoPoint GetElapsedTime(const ChronoPoint& to = ChronoPoint(true)) const
    {
        return (to - *this);
    }

    /// Arithmetics
    inline ChronoPoint operator-(const ChronoPoint& another) const
    {
        ChronoPoint ret = *this;
        ret.m_us -= another.m_us;
        return ret;
    }
    inline ChronoPoint operator+(const ChronoPoint& another) const
    {
        ChronoPoint ret = *this;
        ret.m_us += another.m_us;
        return ret;
    }
    inline ChronoPoint operator*(int64_t mul) const
    {
        ChronoPoint ret = *this;
        ret.m_us *= mul;
        return ret;
    }
    inline ChronoPoint operator/(int64_t divd) const
    {
        ChronoPoint ret = *this;
        ret.m_us /= divd;
        return ret;
    }

    inline bool operator>=(const ChronoPoint& another) const
    {
        return this->m_us >= another.m_us;
    }
    inline bool operator<=(const ChronoPoint& another) const
    {
        return this->m_us <= another.m_us;
    }
    inline bool operator>(const ChronoPoint& another) const
    {
        return this->m_us > another.m_us;
    }
    inline bool operator<(const ChronoPoint& another) const
    {
        return this->m_us < another.m_us;
    }
    inline ChronoPoint& operator+=(const ChronoPoint& another)
    {
        this->m_us += another.m_us;
        return *this;
    }
    inline ChronoPoint& operator-=(const ChronoPoint& another)
    {
        this->m_us -= another.m_us;
        return *this;
    }
    inline ChronoPoint& operator*=(int64_t mul)
    {
        m_us /= mul;
        return *this;
    }
    inline ChronoPoint& operator/=(int64_t divd)
    {
        m_us /= divd;
        return *this;
    }

    /// Printable format, like [YYYY-MM-DD] hh:mm:ss[.zzz]
    std::string ToString(bool printMS = true, bool printDate = false) const;
    /// Outputs microseconds, if interval less than two seconds.
    std::string ToProfilingTime() const;

private:
    static int LocalOffsetSeconds();
};
}
