/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMain.hpp"

namespace D2ModGen {

ModuleMain::ModuleMain()
    : PluginModule(Mernel::PropertyTreeMap{
          { "inputPath", Mernel::PropertyTreeScalar{ "" } },
          { "outputPath", Mernel::PropertyTreeScalar{ "" } },
          { "version", Mernel::PropertyTreeScalar{ 2 } },
          { "inputMode", Mernel::PropertyTreeScalar{ 0 } },
          { "outputMode", Mernel::PropertyTreeScalar{ 0 } },
          { "modname", Mernel::PropertyTreeScalar{ "rando" } },
          { "seed", Mernel::PropertyTreeScalar{ 0 } },
          { "refreshSeed", Mernel::PropertyTreeScalar{ false } },
          { "exportAll", Mernel::PropertyTreeScalar{ false } },
      })
{
}

void ModuleMain::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
}

}
