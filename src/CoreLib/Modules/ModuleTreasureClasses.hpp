/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

namespace D2ModGen {

struct ModuleTreasureClasses : public AbstractModule {
    using AbstractModule::AbstractModule;

    void generate() override;

private:
    void generateItems(DataContext& output, const Config::TreasureClasses::Items& config);
    void generateRunes(DataContext& output, const Config::TreasureClasses::Runes& config);
};

}
