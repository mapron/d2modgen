/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMain.hpp"

namespace D2ModGen {

ModuleMain::ModuleMain()
    : PluginModule(Mernel::PropertyTreeMap{
          { "d2legacyPath", Mernel::PropertyTreeScalar{ "" } },
          { "d2rPath", Mernel::PropertyTreeScalar{ "" } },
          { "d2rUseROTW", Mernel::PropertyTreeScalar{ false } },
          { "exportAllTables", Mernel::PropertyTreeScalar{ false } },
          { "isLegacy", Mernel::PropertyTreeScalar{ false } },
          { "modname", Mernel::PropertyTreeScalar{ "rando" } },
          { "outPath", Mernel::PropertyTreeScalar{ "" } },
          { "seed", Mernel::PropertyTreeScalar{ 0 } },
          { "refreshSeed", Mernel::PropertyTreeScalar{ false } },
          { "exportTxtCheck", Mernel::PropertyTreeScalar{ false } },
      })
{
}

void ModuleMain::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
}

}
