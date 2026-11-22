/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

#include "RandoUtils.hpp"

namespace D2ModGen {

struct ModuleRandoItems : public AbstractModule {
    using AbstractModule::AbstractModule;

    void generate() override;

    struct Loc {
        std::string key;
        std::string value;
    };

    using ExtraLoc = std::vector<Loc>;

    struct SlotPropsVal {
        std::map<int, MagicPropList> propsByLevel;
    };

    struct AllPropsVal {
        std::map<SlotType, SlotPropsVal> propsBySlot;
    };

    struct AllProps {
        std::map<AttributeId, AllPropsVal> propsById;
    };

    AllProps grabAllProps(bool                   onlySetUniques = true,
                          std::set<std::string>* unknownList    = nullptr,
                          std::set<std::string>* skippedList    = nullptr);

    void createNewItems(int repeatUniques, int repeatSets);

    AllAttributeData    analyzeAll(const AllProps& allProps) const;
    AttributeStatistics makeStatistics(const AllProps& allProps) const;

    void analyzeNonPar(const AllPropsVal& allPropsVal, NonParRecord& record) const;
    void analyzePar(const SlotPropsVal& allPropsVal, ParRecordParam& record) const;

    void dumpAttributes(const Mernel::std_path& dumpFile, const AllAttributeData& attributeData) const;

private:
    struct MagicContext {
        size_t precompiledSize = 0; // exposed for tracing.

        AllAttributeData actual;

        void loadPrecompiled();
        void addExtra(const Config& config, const int classCount);
        void scaleSettings(Percent scalePercent, const std::vector<AttributeId>& attrs);
    };

    SkillCodeResolver m_codeResolver;
    SkillGetter       m_skillGetter;
    RandomGenerator   rng;

    bool m_verbose = true;
};

}
