/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "GenerationEnvironment.hpp"

#include "Logger.hpp"
#include "AppConfig.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <random>

namespace D2ModGen {

bool GenerationEnvironment::init(const AppConfig& config)
{
    auto& main = config;

    auto versionMajor     = GenerationEnvironment::VersionMajor(main.m_versionMajor);
    auto versionD2R       = GenerationEnvironment::VersionD2R(main.m_versionD2R);
    auto configInputMode  = GenerationEnvironment::InputMode(main.m_inputMode);
    auto configOutputMode = GenerationEnvironment::OutputMode(main.m_outputMode);

    std::vector<std::string> localeIds{ "enUS", "deDE", "esES", "esMX", "frFR", "itIT", "jaJP", "koKR", "plPL", "ptBR", "ruRU", "zhCN", "zhTW" };
    int                      localeIdIndex = main.m_d2rLocaleId;

    d2rLocaleId = localeIds.at(localeIdIndex);
    modName     = main.m_modname;
    inputPath   = main.m_inputPath;
    outputPath  = main.m_outputPath;
    refreshSeed = main.m_refreshSeed;

    if (outputPath.empty()) {
        if (configInputMode != GenerationEnvironment::InputMode::Game) {
            Logger(Logger::Err) << "Cannot detect output path.";
            return false;
        }
        outputPath = inputPath;
    }

    if (inputPath.empty()) {
        Logger(Logger::Err) << "Input path is empty!";
        return false;
    }

    if (!Mernel::isExistingDirectory(inputPath)) {
        Logger(Logger::Err) << "Input path is not a valid dir:" + Mernel::path2string(inputPath);
        return false;
    }

    isLegacy          = versionMajor == GenerationEnvironment::VersionMajor::Legacy;
    needDataSubfolder = versionD2R == GenerationEnvironment::VersionD2R::LoD;
    isRotW            = !isLegacy && versionD2R == GenerationEnvironment::VersionD2R::RotW;
    if (configInputMode == GenerationEnvironment::InputMode::Game) {
        if (isLegacy) {
            inputMode = StorageType::D2LegacyInternal;
        }
    } else if (configInputMode == GenerationEnvironment::InputMode::FullFolders) {
        if (isLegacy) {
            inputMode = StorageType::D2LegacyFolder;
        } else {
            inputMode = StorageType::D2LegacyFolder;
        }
    } else {
        inputMode = StorageType::CsvFolder;
    }

    if (configOutputMode == GenerationEnvironment::OutputMode::D2RMod) {
        if (isLegacy) {
            Logger(Logger::Err) << "You must select another output mode!";
            return false;
        }
    } else if (configOutputMode == GenerationEnvironment::OutputMode::FullFolders) {
        outputMode = StorageType::D2LegacyFolder;
    } else if (configOutputMode == GenerationEnvironment::OutputMode::Txt) {
        outputMode = StorageType::CsvFolder;
    } else {
        outputMode = StorageType::None;
    }

    auto* extraFileNameEnv = getenv("D2MODGEN_EXTRA_FILE");
    auto* extraTraceEnv    = getenv("D2MODGEN_EXTRA_TRACE");
    if (extraFileNameEnv) {
        extraFile = Mernel::string2path(extraFileNameEnv);
    }
    if (extraTraceEnv) {
        auto stringList = Mernel::splitLine(std::string(extraTraceEnv), ',', true);
        for (auto& str : stringList)
            extraTrace.insert(str);
    }

    {
        auto stringList = Mernel::splitLine(main.m_extraTrace, ',', true);
        for (auto& str : stringList)
            extraTrace.insert(str);
    }

    return true;
}

RandomGenerator createRNG(uint32_t seed)
{
    using Distribution32 = std::uniform_int_distribution<int32_t>;
    std::mt19937_64 engine;
    // Really we need A LOT of bits to safely seed mt engine!
    // But for our purpose 32 bits more than enough.
    // don't use that approach for cryptography, read the docs!
    engine.seed(seed);
    std::function<int(int)> rngWrapper = [engine](int bound) mutable {
        if (bound <= 1)
            return 0;
        return Distribution32(0, bound - 1)(engine);
    };
    return rngWrapper;
}

}
