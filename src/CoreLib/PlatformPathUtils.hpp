/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <string>

#include "CoreExport.hpp"

namespace D2ModGen {

CORE_EXPORT bool createShortCut(const std::string& from, const std::string& to, const std::string& extraArgs);

CORE_EXPORT std::string getExecutableRootFolder();
}
