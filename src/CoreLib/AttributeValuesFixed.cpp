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
    result.nonPar = NonParRecordList(std::cbegin(g_generatedNonParRecordList), std::cend(g_generatedNonParRecordList));
    result.par    = ParRecordList(std::cbegin(g_generatedParRecordList), std::cend(g_generatedParRecordList));
    result.refreshIds();
    result.erase(AttributeId::INVALID);
    return result;
}

}
