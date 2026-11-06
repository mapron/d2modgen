/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IModule.hpp"

namespace D2ModGen {

struct ModuleGlobal : public AbstractModule {
    ModuleGlobal(DataContext& output_, RandomGenerator& rng_, const Config& config_, const GenerationEnvironment& env_);

    std::string getErr() const { return m_err; }

    void reset();

    void generate() override;

private:
    struct ModuleRecord {
        std::string              m_id;
        std::unique_ptr<IModule> m_gen;
    };
    template<class T>
    void create(const std::string& id);

private:
    std::string m_err;

    std::vector<ModuleRecord> m_modules;
};

}
