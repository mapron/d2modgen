/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "ModgenCoreExport.hpp"

#include "MernelPlatform/FsUtils.hpp"

#include <optional>
#include <string>

namespace D2ModGen {

/// Bytes of the first PNG frame in the exe's first RT_GROUP_ICON group, or nullopt.
MODGENCORE_EXPORT std::optional<std::string> extractExePngIcon(const Mernel::std_path& exe);

}
