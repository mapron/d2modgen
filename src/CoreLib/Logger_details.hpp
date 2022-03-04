/*
 * Copyright (C) 2020 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "Logger.hpp"
#include "ChronoPoint.hpp"
#include "FileIOUtils.hpp"

#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>

namespace D2ModGen {

class AbstractLoggerBackend : public ILoggerBackend {
public:
    AbstractLoggerBackend(int  maxLogLevel,
                          bool outputLoglevel,
                          bool outputTimestamp,
                          bool outputTimeoffsets,
                          bool appendEndl)
        : m_maxLogLevel(maxLogLevel)
        , m_outputLoglevel(outputLoglevel)
        , m_outputTimestamp(outputTimestamp)
        , m_outputTimeoffsets(outputTimeoffsets)
        , m_appendEndl(appendEndl)
    {
        m_startTime = ChronoPoint(true);
        m_prevTime  = m_startTime;
    }

    bool LogEnabled(int logLevel) const override { return logLevel <= m_maxLogLevel; }

    void FlushMessage(const std::string& message, int logLevel) const override
    {
        std::ostringstream os;
        if (m_outputLoglevel) {
            os << LogLevelString(logLevel) << " ";
        }
        if (m_outputTimestamp) {
            os << "[" << ChronoPoint(true).ToString() << "] ";
        }

        if (m_outputTimeoffsets) {
            ChronoPoint now(true);
            const auto  fromStart = (now - m_startTime).GetUS();
            const auto  fromPrev  = (now - m_prevTime).GetUS();
            os << "[" << fromStart << ", +" << fromPrev << "] ";
            m_prevTime = now;
        }
        os << message;
        if (m_appendEndl)
            os << std::endl;
        FlushMessageInternal(os.str(), logLevel);
    }

    virtual void FlushMessageInternal(const std::string& message, int logLevel) const = 0;

protected:
    std::string LogLevelString(int loglevel) const
    {
        switch (loglevel) {
            case Logger::Emerg:
                return "EMERG";
            case Logger::Alert:
                return "ALERT";
            case Logger::Crit:
                return "CRIT ";
            case Logger::Err:
                return "ERROR";
            case Logger::Warning:
                return "WARNI";
            case Logger::Notice:
                return "NOTIC";
            case Logger::Info:
                return "INFO ";
            case Logger::Debug:
                return "DEBUG";
            default:
                return "?????";
        }
    }

private:
    ChronoPoint         m_startTime;
    mutable ChronoPoint m_prevTime;
    const int           m_maxLogLevel;
    const bool          m_outputLoglevel;
    const bool          m_outputTimestamp;
    const bool          m_outputTimeoffsets;
    const bool          m_appendEndl;
};

class LoggerBackendConsole : public AbstractLoggerBackend {
public:
    enum class Type
    {
        Cout,
        Cerr,
        Printf
    };

public:
    LoggerBackendConsole(int  maxLogLevel,
                         bool outputLoglevel,
                         bool outputTimestamp,
                         bool outputTimeoffsets,
                         Type type)
        : AbstractLoggerBackend(maxLogLevel, outputLoglevel, outputTimestamp, outputTimeoffsets, false)
        , m_type(type)
    {}
    void FlushMessageInternal(const std::string& message, int) const override
    {
        if (m_type == Type::Printf)
            printf("%s\n", message.c_str());
        else if (m_type == Type::Cerr)
            std::cerr << message << std::endl
                      << std::flush;
        else
            std::cout << message << std::endl
                      << std::flush;
    }

private:
    const Type m_type;
};

class CORE_EXPORT LoggerBackendFiles : public AbstractLoggerBackend {
public:
    LoggerBackendFiles(int      maxLogLevel,
                       bool     duplicateInStderr,
                       bool     outputLoglevel,
                       bool     outputTimestamp,
                       bool     outputTimeoffsets,
                       std_path filename);
    ~LoggerBackendFiles();
    void FlushMessageInternal(const std::string& message, int) const override;

private:
    void OpenNextFile() const;
    void CloseFile() const;

    const bool            m_duplicateInStderr;
    const std_path        m_filename;
    mutable std::ofstream m_currentFile;
    mutable std::mutex    m_mutex;
};

}
