/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once
#include "IModule.hpp"

namespace D2ModGen {

enum SType
{
    ExistingHidden, // this is kept in main range and not updated
    ExistingActive, // this is kept in main range and not updated, but used as pool
    GenHidden,      // this is kept in main range and UPDATED
    GenActive,      // this added to the end with NEW DATA
    Filler,         // this added to the end with ID ONLY
};
std::string enumToString(SType stype, bool asEnum = false);

struct ModuleSkillRandomizer : public AbstractModule {
    using AbstractModule::AbstractModule;

    void generate() override;

private:
    struct SkillInfo {
        SType stype = SType::Filler;

        Element eType = Element::None;

        int rowIdx      = -1;
        int descIdx     = -1;
        int changeInRow = 0;

        std::string originalId;

        DataRow_skills    row;
        DataRow_skilldesc desc;

        std::string toString() const;

        bool operator==(const SkillInfo&) const noexcept = default;

        void resetIdx()
        {
            rowIdx  = -1;
            descIdx = -1;
        }

        void setNewElement(Element element, bool extraTrace);
    };
    using Ptr     = SkillInfo*;
    using PtrList = std::vector<Ptr>;
    using PtrSet  = std::set<Ptr>;

    struct CharacterData {
        std::vector<SkillInfo> skills;
        bool                   extraTrace = false;

        int rowIdxStart  = -1;
        int descIdxStart = -1;

        Character clas = Character::None;

        std::vector<size_t> getIndexesByType(SType stype, SType stype2 = SType::Filler) const;
        PtrList             getActive();

        bool readExisting(DataContext& output);
        void writeOut(DataContext& output) const;

        void makeInactive(RandomGenerator& rng, int keepCnt);

        void generateAdditionalSkills(RandomGenerator& rng, const CharacterData& someOtherClass, int generateCount);
        void createIdx(DataContext& output);

        void randomizePositions(RandomGenerator& rng);

        void updateSkillReq(const Config::SkillRandomizer& module, bool recalDeps);
        void updateSkillNameRef(const DataContext& output, RandomGenerator& rng);
        void updateSkillPower(int globalHitShift);

        void randomizeElements(RandomGenerator& rng, int maximumVariants, int percentToChange);

        void insertRows(DataContext& output, int rowsPerClass);
        void updateMissiles(DataContext& output);

        void updateMissile(DataContext& output, const SkillInfo& parent, std::string& missileId, std::map<std::string, std::string>& scope);
    };

    using AllCharacterData = std::map<Character, CharacterData>;

    AllCharacterData existingData;

    AllCharacterData generatedData;
};

}
