/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX_License_Identifier: BSL_1.0
 * See LICENSE file for details.
 */

#include "AttributeValuesFixed.hpp"

#include "AttributeValuesGen.hpp"

namespace D2ModGen {

AllAttributeData getPrecompiledData()
{
    AllAttributeData result;
    for (auto& rec : g_generatedNonParRecordsGrouped)
        result.groupMap[rec.group] = rec;
    result.refreshIds();
    return result;
}

}
