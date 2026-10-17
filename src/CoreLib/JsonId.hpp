/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <array>
#include <string>

namespace D2ModGen {

enum class JsonId
{
    ItemNames,
    ItemNameAffixes,
    Monsters,

    count__,
};

inline auto getJsonIds()
{
    std::array<JsonId, size_t(JsonId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<JsonId>(i);
    return result;
}

std::string_view getJsonRelPath(JsonId id);

}
