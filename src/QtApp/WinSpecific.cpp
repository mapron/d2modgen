/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#include "WinSpecific.hpp"
#include "RAIIUtils.hpp"

#include <QDebug>
#include <QFileInfo>

#include <qt_windows.h>

#include <sys/types.h>
#include <direct.h>
#include <winioctl.h>
#include <objbase.h>
#include <shlobj.h>
#include <accctrl.h>
#include <cassert>

namespace D2ModGen {

namespace {

std::wstring winFilename(QString filename)
{
    return filename.replace(QLatin1Char('/'), QLatin1Char('\\')).toStdWString();
}

}

bool createShortCut(const QString& from, const QString& to, const QString& extraArgs)
{
    qDebug() << "Creating shortcut '" << from << "' -> '" << to << "'";
    auto linkPath = from + ".lnk";

    QString destDir = QFileInfo(to).absolutePath();

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
    if (!SUCCEEDED(psl->SetPath(winFilename(to).c_str())))
        return false;
    if (!SUCCEEDED(psl->SetWorkingDirectory(winFilename(destDir).c_str())))
        return false;

    if (!extraArgs.isEmpty() && !SUCCEEDED(psl->SetArguments(winFilename(extraArgs).c_str())))
        return false;

    IPersistFile* ppf = nullptr;
    hres              = psl->QueryInterface(IID_IPersistFile, (void**) &ppf);
    MODGEN_SCOPE_EXIT([&ppf] {
        if (ppf)
            ppf->Release();
    });
    if (!SUCCEEDED(hres))
        return false;

    hres     = ppf->Save(linkPath.toStdWString().c_str(), TRUE);
    auto err = GetLastError();
    if (!SUCCEEDED(hres)) {
        qInfo() << "Creating shortcut '" << from << "' failed with code=" << err;
        return false;
    }

    return true;
}

}
