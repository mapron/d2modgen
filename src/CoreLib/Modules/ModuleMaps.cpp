/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMaps.hpp"

namespace D2ModGen {

namespace {

const std::set s_buggy{ 113, 115, 118 }; // ice caverns don't like 1
}

void ModuleMaps::generate()
{
    const auto& module = config.m_maps;
    if (module.m_mazeMode > 0) {
        for (auto& row : output.lvlmaze.rows) {
            if (!row.isValid())
                continue;

            if (module.m_mazeMode == 1) {
                int id           = row.levelId.value_or(0);
                int compactCount = s_buggy.contains(id) ? 4 : 1;
                for (auto& room : row.rooms) {
                    room = compactCount;
                }
            } else if (module.m_mazeMode == 2) {
                // all to normal
                row.rooms[2] = row.rooms[0];
                row.rooms[1] = row.rooms[0];
            } else if (module.m_mazeMode == 3) {
                row.rooms[0] = row.rooms[2];
                row.rooms[1] = row.rooms[2];
            }
        }
    }
}

}
