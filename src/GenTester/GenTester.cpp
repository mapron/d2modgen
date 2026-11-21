/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "AttributeValues.hpp"
#include "PropRandomizer.hpp"
#include "GenerationEnvironment.hpp"

#include "Logger.hpp"
#include "MernelPlatform/Logger_details.hpp"

int main(int argc, char* argv[])
{
    using namespace D2ModGen;

    Logger::SetLoggerBackend(std::make_unique<Mernel::LoggerBackendConsole>(Logger::Debug, false, false, false, Mernel::LoggerBackendConsole::Type::Cout));
    if (argc < 3) {
        Logger(Logger::Err) << "Usage: GenTester attr level";
        return EXIT_FAILURE;
    }
    const int         levelTier  = makeTier(std::atoi(argv[2]));
    const int         sampleSize = 20;
    const AttributeId attr       = getAttributeId(argv[1]);
    if (attr <= AttributeId::INVALID)
        return EXIT_FAILURE;
    Logger() << "generate attr=" << enumToString(attr) << ", tier=" << levelTier << ", sampleSize=" << sampleSize;

    auto precompiled = getPrecompiledData();

    RandomGenerator rng = createRNG(42);

    PropRandomizer randomizer(precompiled, [](int) -> DataRow_skills_ptr { return nullptr; });

    auto            idx = randomizer.createIndexAll();
    BasicProperties props;
    props.levelTier = levelTier;
    randomizer.indexAnd(idx, props, true);
    randomizer.indexAnd(idx, attr, 0);

    auto gen = randomizer.getRandomAttributeGenerator(idx, rng);
    if (!gen) {
        Logger(Logger::Err) << "failed to make generator";
        return EXIT_FAILURE;
    }

    std::vector<Percent> extremeChances{ 0_pct, g_identityPct };
    for (const Percent& extreme : extremeChances) {
        for (SlotType slot : getSlotTypes()) {
            if (!gen->slotTypes.contains(slot))
                continue;

            props.slot = slot;
            for (int i = 0; i < sampleSize; ++i) {
                MagicProp prop = gen->makeRoll2(rng, props, extreme, extreme);
                Logger() << prop.toString();
            }
        }
    }

    return EXIT_SUCCESS;
}
