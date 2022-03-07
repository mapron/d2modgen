/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "FileIOUtils.hpp"
#include "PropertyTree.hpp"

#include "CoreExport.hpp"

namespace D2ModGen {

CORE_EXPORT bool readJsonFromBuffer(const std::string& buffer, PropertyTree& data);
CORE_EXPORT bool writeJsonToBuffer(std::string& buffer, const PropertyTree& data);

}
