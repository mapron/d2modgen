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
    if (argc < 4) {
        Logger(Logger::Err) << "Usage: GenTester attr level size";
        return EXIT_FAILURE;
    }
    const int         level      = std::atoi(argv[2]);
    const int         sampleSize = std::atoi(argv[3]);
    const AttributeId attr       = getAttributeId(argv[1]);
    if (attr <= AttributeId::INVALID)
        return EXIT_FAILURE;

    auto precompiled = getPrecompiledData();

    RandomGenerator rng = createRNG(42);

    PropRandomizer randomizer(precompiled, [](int) -> DataRow_skills_ptr { return nullptr; });

    auto            idx = randomizer.createIndexAll();
    BasicProperties props;
    props.levelBracket18 = level / 18;
    props.slot           = SlotType::MainArmor;
    randomizer.indexAnd(idx, props, true);

    auto      gen  = randomizer.getRandomAttributeGenerator(idx, rng);
    if (!gen) {
        Logger(Logger::Err) << "failed to make generator";
        return EXIT_FAILURE;
    }
    MagicProp prop = gen->makeRoll(rng);
    Logger() << prop.toString();

    return EXIT_SUCCESS;
}
