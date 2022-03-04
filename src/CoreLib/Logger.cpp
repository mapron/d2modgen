/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "Logger_details.hpp"
#include "Logger.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <chrono>
#include <sstream>

#include <cstdio>

namespace D2ModGen {

std::unique_ptr<ILoggerBackend> g_loggerBackend(new LoggerBackendConsole(Logger::Debug, true, true, false, LoggerBackendConsole::Type::Cout));

void Logger::SetLoggerBackend(std::unique_ptr<ILoggerBackend>&& backend)
{
    g_loggerBackend = std::move(backend);
}

bool Logger::IsLogLevelEnabled(int logLevel)
{
    return g_loggerBackend->LogEnabled(logLevel);
}

Logger::Logger(int logLevel)
    : m_logLevel(logLevel)
{
    if (g_loggerBackend->LogEnabled(logLevel))
        m_stream = std::make_unique<std::ostringstream>();
}

Logger::Logger(const std::string& context, int logLevel)
{
    if (g_loggerBackend->LogEnabled(logLevel)) {
        m_stream = std::make_unique<std::ostringstream>();
        if (!context.empty())
            *m_stream << "{" << context << "} ";
    }
}

Logger::~Logger()
{
    if (m_stream)
        g_loggerBackend->FlushMessage(m_stream->str(), m_logLevel);
}

Logger::operator bool() const
{
    return !!m_stream;
}

Logger& Logger::operator<<(const char* str)
{
    if (m_stream) {
        *m_stream << str;
    }
    return *this;
}
Logger& Logger::operator<<(const Binary& SizeHolder)
{
    if (m_stream) {
        *m_stream << "[" << SizeHolder.m_size << "] ";
        *m_stream << std::hex << std::setfill('0');
        const size_t outputSize = std::min(SizeHolder.m_size, SizeHolder.m_outputMax);

        if (SizeHolder.m_size <= SizeHolder.m_outputMax) {
            for (size_t i = 0; i < outputSize; ++i)
                *m_stream << std::setw(2) << int(SizeHolder.m_data[i]) << ' ';
        } else {
            for (size_t i = 0; i < outputSize / 2; ++i)
                *m_stream << std::setw(2) << int(SizeHolder.m_data[i]) << ' ';

            *m_stream << "... ";

            for (size_t i = 0; i < outputSize / 2; ++i)
                *m_stream << std::setw(2) << int(SizeHolder.m_data[SizeHolder.m_size - outputSize / 2 + i]) << ' ';
        }
        *m_stream << std::dec;
    }
    return *this;
}

Logger::Binary::Binary(const void* data, size_t size, size_t outputMax)
    : m_data((const unsigned char*) data)
    , m_size(size)
    , m_outputMax(outputMax)
{
}

Logger::Binary::Binary(const char* data, size_t size, size_t outputMax)
    : m_data((const unsigned char*) data)
    , m_size(size)
    , m_outputMax(outputMax)
{
}

Logger::Binary::Binary(const unsigned char* data, size_t size, size_t outputMax)
    : m_data((const unsigned char*) data)
    , m_size(size)
    , m_outputMax(outputMax)
{
}

LoggerBackendFiles::LoggerBackendFiles(int      maxLogLevel,
                                       bool     duplicateInStderr,
                                       bool     outputLoglevel,
                                       bool     outputTimestamp,
                                       bool     outputTimeoffsets,
                                       std_path filename)
    : AbstractLoggerBackend(maxLogLevel, outputLoglevel, outputTimestamp, outputTimeoffsets, true)
    , m_duplicateInStderr(duplicateInStderr)
    , m_filename(std::move(filename))
{
}

LoggerBackendFiles::~LoggerBackendFiles()
{
    CloseFile();
}

void LoggerBackendFiles::FlushMessageInternal(const std::string& message, int) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_currentFile.is_open())
        OpenNextFile();

    if (!m_currentFile.is_open())
        return;

    if (m_duplicateInStderr) {
        std::cerr << message << std::flush;
    }
    m_currentFile.write(message.c_str(), message.size());
    if (!m_currentFile)
        std::cerr << "write() to file of \'" << message << "\' failed! \n";

    m_currentFile.flush();
}

void LoggerBackendFiles::OpenNextFile() const
{
    CloseFile();

    m_currentFile.open(m_filename, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    if (!m_currentFile)
        std::cerr << "Failed to open:" << m_filename << std::endl;
}

void LoggerBackendFiles::CloseFile() const
{
    if (m_currentFile.is_open())
        m_currentFile.close();
}

}
