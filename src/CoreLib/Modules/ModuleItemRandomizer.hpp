/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "RandoUtils.hpp"

namespace D2ModGen {

struct ModuleItemRandomizer : public IModule {
    struct Loc {
        std::string key;
        std::string value;
    };

    using ExtraLoc = std::vector<Loc>;

    void generate(DataContext& output, RandomGenerator& rng, const Config& config, const GenerationEnvironment& env) override;

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
                          std::set<std::string>* skippedList    = nullptr);

    void createNewItems(DataContext&                 output,
                        RandomGenerator&             rng,
                        const Config&                config,
                        const GenerationEnvironment& env,
                        int                          repeatUniques,
                        int                          repeatSets);

    AllAttributeData analyzeAll(const AllProps& allProps);

    void analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecord& record, NonParValueSummary& stat);
    void analyzePar(const AllPropsVal& allPropsVal, ParRecord& record, NonParValueSummary& stat);

    void dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData, const std::set<AttributeId>& skip);

    void updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const std::string& localeId);
    void updateLoDtranslation(std::string& patchstringTlb);

private:
    struct MagicContext {
        MagicContext(AllAttributeData gameData_);

        const AllAttributeData precompiled;
        const AllAttributeData gameData;
        AllAttributeData       actual;

        AllAttributeData precompiledSkipped;
        AllAttributeData gameDataSkipped;

        void addExtra(const Config& config, const GenerationEnvironment& env);
        void homogenizeNonPar(int64_t scalePercent, const std::vector<AttributeId>& attrs);
        void scaleSettings(int64_t scalePercent, const std::vector<AttributeId>& attrs);
    };

    SkillCodeResolver m_codeResolver;
    SkillGetter       m_skillGetter;
    ExtraLoc          m_extraLoc;
};

}
