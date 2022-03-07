/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "DyLib.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace D2ModGen {

namespace {

std::string getPlatformFilename(const std::string& baseName)
{
#ifdef _WIN32
    return baseName + ".dll";
#else
#ifdef __APPLE__
    return "lib" + baseName + ".dylib";
#else
    return "lib" + baseName + ".so";
#endif

#endif
}

}

DyLib::DyLib(const std_path& folder, const std::string& baseName)
    : m_path(folder / string2path(getPlatformFilename(baseName)))
{
    std::error_code ec;
    if (!std_fs::exists(m_path, ec))
        throw std::runtime_error("Non-existing path:" + path2string(m_path));
    load();

    if (!m_handle)
        throw std::runtime_error("Failed to load dynamic library into memory:" + path2string(m_path));
}

DyLib::~DyLib()
{
    if (!m_handle)
        return;

    close();
}

void* DyLib::getProc(const std::string& procName) noexcept(false)
{
    auto* addr = getProcImpl(procName.c_str());
    if (!addr)
        throw std::runtime_error("Failed to locate generate() in:" + path2string(m_path));
    return addr;
}

#ifdef _WIN32

void DyLib::load()
{
    HMODULE handle = ::LoadLibraryW(m_path.wstring().c_str());
    m_handle       = reinterpret_cast<void*>(handle);
}

void DyLib::close()
{
    ::FreeLibrary(reinterpret_cast<HMODULE>(m_handle));
}

void* DyLib::getProcImpl(const char* procName) noexcept
{
    return reinterpret_cast<void*>(::GetProcAddress(reinterpret_cast<HMODULE>(m_handle), procName));
}
#else

void DyLib::load()
{
    m_handle = dlopen(path2string(m_path).c_str(), RTLD_NOW);
}

void DyLib::close()
{
    dlclose(m_handle);
}

void* DyLib::getProcImpl(const char* procName) noexcept
{
    return dlsym(m_handle, procName);
}

#endif

}
