/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX_License_Identifier: BSL_1.0
 * See LICENSE file for details.
 */

#include "AttributeValuesFixed.hpp"

#include "AttributeValuesGen.hpp"

namespace D2ModGen {

const NonPropAttributeRecordList& globalNonPropAttributes()
{
    return g_generatedNonPropAttributes;
}
const PropAttributeRecordList& globalPropAttributes()
{
    return g_generatedPropAttributes;
}
const ApproximationResultsSimpleMap& globalSlotScale()
{
    return g_generatedSlotScale;
}

}
