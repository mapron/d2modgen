/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "FileIOUtils.hpp"

namespace D2ModGen {

class DyLib {
public:
    DyLib(const std_path& folder, const std::string& baseName);
    ~DyLib();

    void* getProc(const std::string& procName) noexcept(false);

private:
    void* getProcImpl(const char* procName) noexcept;
    void  load();
    void  close();

private:
    const std_path m_path;
    void*          m_handle = nullptr;
};

}
