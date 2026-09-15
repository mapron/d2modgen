/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleMain.hpp"

namespace D2ModGen {

ModuleMain::ModuleMain()
    : PluginModule(Mernel::PropertyTreeMap{
          { "inputPath", Mernel::PropertyTreeScalar{ "" } },
          { "outputPath", Mernel::PropertyTreeScalar{ "" } },
          { "versionMajor", Mernel::PropertyTreeScalar{ 1 } }, // 0 = LoD, 1 = D2R
          { "versionD2R", Mernel::PropertyTreeScalar{ 1 } },   // 0 D2R-LoD, 1=D2R-RotW
          { "inputMode", Mernel::PropertyTreeScalar{ 0 } },
          { "outputMode", Mernel::PropertyTreeScalar{ 0 } },
          { "modname", Mernel::PropertyTreeScalar{ "rando" } },
          { "seed", Mernel::PropertyTreeScalar{ 0 } },
          { "refreshSeed", Mernel::PropertyTreeScalar{ true } },
          { "exportAll", Mernel::PropertyTreeScalar{ false } },
      })
{
}

void ModuleMain::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
}

}
