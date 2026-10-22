/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

namespace D2ModGen {
struct MagicContext;

class ModuleItemRandomizer : public PluginModule {
public:
    static constexpr const std::string_view configKey{ "randomizer" };
    static constexpr const std::string_view moduleKey{ "appModuleItemRandomizer" };

    struct Loc {
        std::string key;
        std::string value;
    };

    using ExtraLoc = std::vector<Loc>;

    ModuleItemRandomizer();

    void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const override;

    using AllPropsVal = std::map<SlotType, std::map<int, AffixPropList>>;
    using AllProps    = std::map<AttributeId, AllPropsVal>;

    void grabAllProps(AllProps& allProps, DataContext& output, MagicContext& context) const;

    void analyzeAllPropsForDump(
        AttributeId                    code,
        const AllPropsVal&             allPropsVal,
        NonPropAttributeRecordList&    nonPropList,
        PropAttributeRecordList&       propList,
        ApproximationResultsSimpleMap& approxMapSimple) const;

    void dumpAttributes(
        const NonPropAttributeRecordList&    nonPropList,
        const PropAttributeRecordList&       propList,
        const ApproximationResultsSimpleMap& approxMapSimple) const;

    void updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const ExtraLoc& extraLoc) const;
    void updateLoDtranslation(std::string& patchstringTlb, const ExtraLoc& extraLoc) const;
};

}
