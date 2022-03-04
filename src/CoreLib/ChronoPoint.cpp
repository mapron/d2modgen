/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ChronoPoint.hpp"

#include <chrono>
#include <sstream>
#include <iomanip>

#include <cstdio>

namespace {

template<class To, class Rep, class Period>
To round_down(const std::chrono::duration<Rep, Period>& d)
{
    To t = std::chrono::duration_cast<To>(d);
    if (t > d)
        --t;
    return t;
}

#ifndef _WIN32
#include <sys/time.h>
int getTimeOffset()
{
    time_t    now1 = time(nullptr);
    struct tm lcl  = *localtime(&now1);
    struct tm gmt  = *gmtime(&now1);
    int       h    = lcl.tm_hour - gmt.tm_hour;
    if (h > 12)
        h -= 24;
    if (h < -12)
        h += 24;
    return h * 3600;
}
#else
#include <windows.h>
int getTimeOffset()
{
    struct _TIME_ZONE_INFORMATION tz;
    GetTimeZoneInformation(&tz);
    return -tz.Bias * 60;
    // UTC = local time + bias
}

#endif

// Returns number of days since civil 1970-01-01.  Negative values indicate
//    days prior to 1970-01-01.
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//                 m is in [1, 12]
//                 d is in [1, last_day_of_month(y, m)]
//                 y is "approximately" in
//                   [numeric_limits<Int>::min()/366, numeric_limits<Int>::max()/366]
//                 Exact range of validity is:
//                 [civil_from_days(numeric_limits<Int>::min()),
//                  civil_from_days(numeric_limits<Int>::max()-719468)]
struct civil {
    int      y;
    unsigned m;
    unsigned d;
    civil(int _y, unsigned _m, unsigned _d)
        : y(_y)
        , m(_m)
        , d(_d)
    {}
};

int days_from_civil(civil c)
{
    // static_assert(std::numeric_limits<unsigned>::digits >= 18,
    //          "This algorithm has not been ported to a 16 bit unsigned integer");
    //  static_assert(std::numeric_limits<Int>::digits >= 20,
    //          "This algorithm has not been ported to a 16 bit signed integer");
    c.y -= c.m <= 2;
    const int      era = (c.y >= 0 ? c.y : c.y - 399) / 400;
    const auto     yoe = static_cast<unsigned>(c.y - era * 400);               // [0, 399]
    const unsigned doy = (153 * (c.m + (c.m > 2 ? -3 : 9)) + 2) / 5 + c.d - 1; // [0, 365]
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                // [0, 146096]
    return era * 146097 + static_cast<int>(doe) - 719468;
}

// Returns year/month/day triple in civil calendar
// Preconditions:  z is number of days since 1970-01-01 and is in the range:
//                   [numeric_limits<Int>::min(), numeric_limits<Int>::max()-719468].

civil civil_from_days(int z)
{
    // static_assert(std::numeric_limits<unsigned>::digits >= 18,
    //          "This algorithm has not been ported to a 16 bit unsigned integer");
    // static_assert(std::numeric_limits<Int>::digits >= 20,
    //          "This algorithm has not been ported to a 16 bit signed integer");
    z += 719468;
    const int      era = (z >= 0 ? z : z - 146096) / 146097;
    const auto     doe = static_cast<unsigned>(z - era * 146097);               // [0, 146096]
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
    const int      y   = static_cast<int>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
    const unsigned mp  = (5 * doy + 2) / 153;                     // [0, 11]
    const unsigned d   = doy - (153 * mp + 2) / 5 + 1;            // [1, 31]
    const unsigned m   = mp + (mp < 10 ? 3 : -9);                 // [1, 12]
    return civil(y + (m <= 2), m, d);
}

unsigned weekday_from_days(int z)
{
    return static_cast<unsigned>(z >= -4 ? (z + 4) % 7 : (z + 5) % 7 + 6);
}

}

using namespace std::chrono;
using days = duration<int, std::ratio<24 * 3600>>;

namespace D2ModGen {

ChronoPoint::ChronoPoint(bool now)
    : m_us(0)
{
    if (now) {
        auto d = std::chrono::system_clock::now().time_since_epoch();
        m_us   = round_down<std::chrono::microseconds>(d).count();
    }
}

ChronoPoint::ChronoPoint(int seconds)
    : m_us(seconds * ONE_SECOND)
{
}

ChronoPoint::ChronoPoint(double seconds)
    : m_us(static_cast<int64_t>(seconds * ONE_SECOND))
{
}

std::tm ChronoPoint::GetTm() const
{
    // return make_utc_tm(*this);

    // t is time duration since 1970-01-01
    std::chrono::system_clock::duration t(0);
    int64_t                             seconds = GetSeconds();

    t += std::chrono::seconds(seconds); //seconds( (int)system_clock::to_time_t(tp) );
    // d is days since 1970-01-01
    auto d = round_down<::days>(t);
    // t is now time duration since midnight of day d
    t -= d;
    // break d down into year/month/day

    civil c = civil_from_days(d.count());
    // start filling in the tm with calendar info
    std::tm tm{};
    tm.tm_year = c.y - 1900;
    tm.tm_mon  = c.m - 1;
    tm.tm_mday = c.d;
    tm.tm_wday = weekday_from_days(d.count());
    tm.tm_yday = d.count() - days_from_civil(civil(c.y, 1, 1));
    // Fill in the time
    tm.tm_hour = duration_cast<hours>(t).count();
    t -= hours(tm.tm_hour);
    tm.tm_min = duration_cast<minutes>(t).count();
    t -= minutes(tm.tm_min);
    tm.tm_sec = (int) duration_cast<std::chrono::seconds>(t).count();
    return tm;
}

void ChronoPoint::SetTm(const std::tm& t, int ms)
{
    std::chrono::system_clock::duration d;
    civil                               c(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
    d += ::days(days_from_civil(c));
    d += hours(t.tm_hour);
    d += minutes(t.tm_min);
    d += std::chrono::seconds(t.tm_sec);
    d += std::chrono::milliseconds(ms);
    m_us = duration_cast<std::chrono::microseconds>(d).count();
}

void ChronoPoint::SetTime(int hour, int minute, int second, int ms)
{
    std::tm t = GetTm();
    t.tm_hour = hour;
    t.tm_min  = minute;
    t.tm_sec  = second;
    SetTm(t, ms);

    ChronoPoint now(true);

    if (*this > now) {
        if (*this - now >= ChronoPoint(12 * 3600)) {
            *this -= ChronoPoint(24 * 3600);
        }
    } else if (*this < now) {
        if (now - *this >= ChronoPoint(12 * 3600)) {
            *this += ChronoPoint(24 * 3600);
        }
    }
}

ChronoPoint& ChronoPoint::ToLocal()
{
    m_us += ONE_SECOND * (LocalOffsetSeconds());
    return *this;
}

ChronoPoint& ChronoPoint::FromLocal()
{
    m_us -= ONE_SECOND * (LocalOffsetSeconds());
    return *this;
}

std::string ChronoPoint::ToString(bool printMS, bool printDate) const
{
    std::ostringstream os;
    std::tm            timeinfo = this->GetTm();

    timeinfo.tm_year = timeinfo.tm_year + 1900;
    timeinfo.tm_mon  = timeinfo.tm_mon + 1;
    os << std::setfill('0');

    if (printDate)
        os << std::setw(4) << timeinfo.tm_year << '-' << std::setw(2) << timeinfo.tm_mon << '-' << std::setw(2) << timeinfo.tm_mday << ' ';

    if (printDate || timeinfo.tm_hour) {
        os << std::setw(2) << timeinfo.tm_hour << ':';
    }

    os << std::setw(2) << timeinfo.tm_min << ':' << std::setw(2) << timeinfo.tm_sec;
    if (printMS) {
        int64_t ms = GetFractionalUS() / 1000;
        os << '.' << std::setw(3) << ms;
    }
    return os.str();
}

std::string ChronoPoint::ToProfilingTime() const
{
    return m_us > 2 * ONE_SECOND ? this->ToString(false) : std::to_string(m_us) + " us.";
}

int ChronoPoint::LocalOffsetSeconds()
{
    static int sec = getTimeOffset();
    return sec;
}

}
