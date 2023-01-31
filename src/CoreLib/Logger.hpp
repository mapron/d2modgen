/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "ModgenCoreExport.hpp"

#include <string>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>
#include <map>

namespace D2ModGen {
/// Logging implementation
class ILoggerBackend {
public:
    virtual ~ILoggerBackend() = default;

    /// Returns true if log should be flushed. Logger::Emerg <= loglevel <=  Logger::Debug
    virtual bool LogEnabled(int logLevel) const = 0;

    /// Outputs log message to log backend.
    virtual void FlushMessage(const std::string& message, int logLevel) const = 0;
};

/// Logger to standard output or another backend.
class MODGENCORE_EXPORT Logger {
public:
    enum LogLevel
    {
        Emerg,
        Alert,
        Crit,
        Err,
        Warning,
        Notice,
        Info,
        Debug
    };

    /// Change default logging behaviour.
    static void SetLoggerBackend(std::unique_ptr<ILoggerBackend>&& backend);
    static bool IsLogLevelEnabled(int logLevel);

    /// Convenience wrapper for blobs. When outputting to Logger, output will be HEX-formatted.
    class Binary {
        friend class Logger;
        const unsigned char* m_data;
        const size_t         m_size;
        const size_t         m_outputMax;

    public:
        Binary(const void* data, size_t size, size_t outputMax = size_t(-1));
        Binary(const char* data, size_t size, size_t outputMax = size_t(-1));
        Binary(const unsigned char* data, size_t size, size_t outputMax = size_t(-1));
    };
    /// Creates logger. If loglevel is flushable by backend, stream will be created.
    Logger(int logLevel = Logger::Debug);

    /// Convenience constructor for outputting context. Context string will be enclosed in braces, if not empty.
    Logger(const std::string& context, int logLevel = Logger::Debug);

    /// Flushes all output to backend.
    ~Logger();

    /// Returns true if logger is active and Stream exists.
    operator bool() const;

    Logger& operator<<(const char* str);
    Logger& operator<<(const Binary& SizeHolder);
    template<class Data>
    Logger& operator<<(const Data& D)
    {
        if (m_stream)
            *m_stream << D;
        return *this;
    }
    template<class Data>
    Logger& operator<<(const std::vector<Data>& D);
    template<class Data>
    Logger& operator<<(const std::deque<Data>& D);

private:
    std::unique_ptr<std::ostringstream> m_stream; //!< Stream used for buffer formatting
    int                                 m_logLevel{};

    void flush();

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
    // move operations doesn't look evil.
};

template<class Data>
Logger& Logger::operator<<(const std::vector<Data>& D)
{
    if (m_stream) {
        for (const auto& d : D)
            *m_stream << d << ' ';
    }
    return *this;
}

template<class Data>
Logger& Logger::operator<<(const std::deque<Data>& D)
{
    if (m_stream) {
        for (const auto& d : D)
            *m_stream << d << ' ';
    }
    return *this;
}

}
