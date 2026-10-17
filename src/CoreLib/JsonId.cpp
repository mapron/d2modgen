/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "JsonId.hpp"

#include <array>

namespace D2ModGen {

namespace {
const std::array<std::string_view, size_t(JsonId::count__)> s_paths{
    "data\\local\\lng\\strings\\item-names.json",
    "data\\local\\lng\\strings\\item-nameaffixes.json",
    "data\\hd\\character\\monsters.json",
};

}

std::string_view getJsonRelPath(JsonId id)
{
    return s_paths[static_cast<size_t>(id)];
}

}
