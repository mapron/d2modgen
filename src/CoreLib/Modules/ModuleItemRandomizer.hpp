/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "PluginModule.hpp"

#include "RandoUtils.hpp"

namespace D2ModGen {

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

    struct SlotPropsVal {
        std::map<int, MagicPropList> propsByLevel;
    };

    struct RollPropsVal {
        std::map<SlotType, SlotPropsVal> propsBySlot;
    };

    struct AllPropsVal {
        std::map<RollType, RollPropsVal> propsByRoll;
    };
    struct AllProps {
        std::map<AttributeId, AllPropsVal> propsById;
    };

    AllProps grabAllProps(const DataContext&     output,
                          bool                   onlySetUniques = true,
                          std::set<std::string>* unknownList    = nullptr,
                          std::set<std::string>* skippedList    = nullptr) const;

    void createNewItems(DataContext&        output,
                        RandomGenerator&    rng,
                        const InputContext& input,
                        int                 repeatUniques,
                        int                 repeatSets) const;

    AllAttributeData analyzeAll(const AllProps& allProps) const;

    void analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecord& record, NonParValueSummary& stat) const;
    void analyzePar(const AllPropsVal& allPropsVal, ParRecord& record, NonParValueSummary& stat) const;

    void dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData, const std::set<AttributeId>& skip) const;

    void updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const std::string& localeId) const;
    void updateLoDtranslation(std::string& patchstringTlb) const;

private:
    struct MagicContext {
        MagicContext(AllAttributeData gameData_);

        const AllAttributeData precompiled;
        const AllAttributeData gameData;
        AllAttributeData       actual;

        AllAttributeData precompiledSkipped;
        AllAttributeData gameDataSkipped;

        void addExtra(const InputContext& input);
        void homogenizeNonPar(int scalePercent, const std::vector<AttributeId>& attrs);
        void scaleSettings(const InputContext& input, const std::string& key, const std::vector<AttributeId>& attrs);
    };

    mutable SkillCodeResolver m_codeResolver;
    mutable SkillGetter       m_skillGetter;
    mutable ExtraLoc          m_extraLoc;
};

}
