/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once
#include "IModule.hpp"

namespace D2ModGen {

struct ModuleMonRandomizer : public AbstractModule {
    using AbstractModule::AbstractModule;

    void generate() override;

private:
    void updateLevelDensity();

private:
    struct TCTable {
        struct TCGroup {
            struct TCRecord {
                std::string id;
                int         level;
            };
            std::vector<TCRecord> records;
        };
        std::map<int, TCGroup>     groups;
        std::map<std::string, int> tc2group;

        void        add(const std::string& id, int group, int level);
        std::string promote(const std::string& tc, const int oldLevel, const int newLevel) const;
        ;
    };

    struct MonResist {
        std::map<Element, int> m_resist;

        void readFromRow(const DataRow_monstats& row, size_t i);

        void writeToRow(DataRow_monstats& row, size_t i) const;

        void shuffle(RandomGenerator& rng);
    };

    struct MonsterTask {
        const DataRow_monstats* m_source = nullptr;

        DataRow_monstats   m_dest;
        DataRow_monstatsEx m_destEx;

        bool m_isGuest     = false;
        bool m_makeChimera = false;
        bool m_rollStat    = false;
        bool m_rollRes     = false;

        bool m_skip = false;

        int m_targetNormalLevel = 0;

        size_t m_areaId = 0;

        bool hasAnyMods() const
        {
            return m_isGuest || m_makeChimera || m_rollStat || m_rollRes;
        }

        std::string toString() const;
    };
    using MonsterTaskList = std::vector<MonsterTask>;

private:
    void prepareChimeraPools();
    void prepareGuestPools();

    void generateChimera(DataRow_monstats& chimera, DataRow_monstatsEx& chimeraEx);
    bool compatibleMonSeq(const std::string& sequenceId, const DataRow_monstatsEx& recipient);

    void processTask(MonsterTask& task, MonsterTaskList& extraMinions);

private:
    TCTable tcTable;

    std::vector<const DataRow_monstats*> validGuestIndexes;

    std::vector<std::pair<std::string, std::string>> idCopies;
    int                                              hcIdx = 0;
};

}
