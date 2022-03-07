/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "PlatformPathUtils.hpp"

#include "RAIIUtils.hpp"
#include "Logger.hpp"
#include "FileIOUtils.hpp"

#include <csignal>
#include <cstring>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN32
#include <unistd.h>
#else
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <direct.h>
#include <winioctl.h>
#include <objbase.h>
#include <shlobj.h>
#include <accctrl.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

namespace D2ModGen {

#ifdef _WIN32
bool createShortCut(const std::string& from, const std::string& to, const std::string& extraArgs)
{
    Logger() << "Creating shortcut '" << from << "' -> '" << to << "'";
    const auto linkPath = string2path(from + ".lnk");
    const auto toPath   = string2path(to);

    const std::wstring extraArgsW(extraArgs.cbegin(), extraArgs.cend());

    IShellLink* psl          = nullptr;
    bool        neededCoInit = false;

    HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**) &psl);

    if (hres == CO_E_NOTINITIALIZED) { // COM was not initialized
        neededCoInit = true;
        CoInitialize(NULL);
        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**) &psl);
    }
    MODGEN_SCOPE_EXIT([&psl, neededCoInit] {
        if (psl)
            psl->Release();
        if (neededCoInit)
            CoUninitialize();
    });
    if (!SUCCEEDED(hres))
        return false;
    if (!SUCCEEDED(psl->SetPath(toPath.wstring().c_str())))
        return false;
    if (!SUCCEEDED(psl->SetWorkingDirectory(toPath.parent_path().wstring().c_str())))
        return false;

    if (!extraArgsW.empty() && !SUCCEEDED(psl->SetArguments(extraArgsW.c_str())))
        return false;

    IPersistFile* ppf = nullptr;
    hres              = psl->QueryInterface(IID_IPersistFile, (void**) &ppf);
    MODGEN_SCOPE_EXIT([&ppf] {
        if (ppf)
            ppf->Release();
    });
    if (!SUCCEEDED(hres))
        return false;

    hres     = ppf->Save(linkPath.wstring().c_str(), TRUE);
    auto err = GetLastError();
    if (!SUCCEEDED(hres)) {
        Logger(Logger::Warning) << "Creating shortcut '" << from << "' failed with code=" << err;
        return false;
    }

    return true;
}
#else
bool createShortCut(const std::string& from, const std::string& to, const std::string& extraArgs)
{
    return false;
}
#endif

std::string getExecutableRootFolder()
{
#ifdef __APPLE__
    char     path[4096], actualpath[4096];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
        return path;
    char* abspath = realpath(path, actualpath);
    if (abspath)
        return string2path(abspath).parent_path().string();
    return "./";
#elif !defined(_WIN32)
    int  len;
    char path[1024];

    // Read symbolic link /proc/self/exe

    len = readlink("/proc/self/exe", path, sizeof(path));
    if (len == -1)
        return std::string("./");

    path[len] = '\0';
    return string2path(abspath).parent_path().string();
#else

    WCHAR szFileName[MAX_PATH];

    GetModuleFileNameW(nullptr, szFileName, MAX_PATH);
    std::wstring w(szFileName);
    return path2string(std_path(w).parent_path());
#endif
}

}
