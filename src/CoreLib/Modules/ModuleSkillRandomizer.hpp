/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once
#include "IModule.hpp"

namespace D2ModGen {

struct ModuleSkillRandomizer : public AbstractModule {
    using AbstractModule::AbstractModule;

    void generate() override;

private:
    struct EType {
        Element e = Element::None;

        std::string extra;
        void        set(const std::string& str);

        std::string toString() const;

        bool operator==(const EType&) const = default;
    };

    struct SkillLocation {
        int skillPage   = 0;
        int skillRow    = 0;
        int skillColumn = 0;
        int listRow     = 0;
        int reqlevel    = 0;

        bool operator==(const SkillLocation&) const = default;

        std::string toString() const;
    };

    struct SkillDamageInfo {
        EType              eType;
        std::optional<int> eLen;
        std::optional<int> eLenLev;
        std::optional<int> eLenSyn;

        int hitShift = 0;

        bool operator==(const SkillDamageInfo&) const = default;

        std::string toString() const;
    };

    struct SkillInfo {
        std::string     uppercaseKey;
        std::string     lowercaseKey;
        Character       classSpecific; // ass|bar|...
        SkillLocation   loc;
        SkillDamageInfo dmg;
        StringVector    reqSkills;

        void readFromRowMain(const DataRow_skills& row);
        void readFromRowDesc(const DataRow_skilldesc& row);
        void writeToRowMain(DataRow_skills& row);
        void writeToRowDesc(DataRow_skilldesc& row);

        std::string toString() const;

        bool operator==(const SkillInfo&) const = default;
    };

    using SkillParsedData = std::map<std::string, SkillInfo>;

    struct SkillTree {
        struct CharacterData {
            struct Skill {
                SkillLocation    loc;
                int              skillId;
                std::vector<int> reqSkillIds;
            };
            std::vector<Skill> skills;
            Character          classSpecific;
            StringVector       skillIds;

            int addSkillId(const std::string& skill);
        };
        std::map<Character, CharacterData> chars;

        void parse(const SkillParsedData& data);

        void randomizeTabs(RandomGenerator& rng);

        void randomizeDmg(SkillParsedData& data, RandomGenerator& rng, const bool ensureDifferent);

        void writeOutput(SkillParsedData& data) const;
    };
};

}
