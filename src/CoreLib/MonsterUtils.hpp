/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "DataContext.hpp"
#include "GenerationEnvironment.hpp"

namespace D2ModGen {
struct MonResist {
    std::map<Element, int> m_resist;

    void readFromRow(const DataRow_monstats& row, size_t i);

    void writeToRow(DataRow_monstats& row, size_t i) const;

    void shuffle(RandomGenerator& rng);
};
}
