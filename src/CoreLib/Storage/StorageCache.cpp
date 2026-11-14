/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "StorageCache.hpp"

#include "CascStorage.hpp"
#include "StormStorage.hpp"
#include "FolderStorage.hpp"
#include "Logger.hpp"

namespace D2ModGen {

IStorage::StoredData StorageCache::load(const Context& input, bool verbose)
{
    std::string buildInfo;
    const bool  canCache = input.m_storage != StorageType::D2ResurrectedInternal
                          || Mernel::readFileIntoBufferNoexcept(input.m_root / ".build.info", buildInfo);
    if (canCache && m_cache.valid && input == m_prev && buildInfo == m_buildInfo) {
        Logger(Logger::Info) << "Using cached data for " << path2string(input.m_root);
        return m_cache;
    }

    m_cache     = loadImpl(input, verbose);
    m_prev      = input;
    m_buildInfo = std::move(buildInfo);
    return m_cache;
}

IStorage::StoredData StorageCache::loadImpl(const Context& input, bool verbose)
{
    std::unique_ptr<IStorage> storage;
    if (input.m_storage == StorageType::D2ResurrectedInternal)
        storage = std::make_unique<CascStorage>(input.m_root, input.m_cascCache);
    else if (input.m_storage == StorageType::D2LegacyInternal)
        storage = std::make_unique<StormStorage>(input.m_root);
    else {
        storage = std::make_unique<FolderStorage>(input.m_root, input.m_storage);
    }
    storage->setVerbose(verbose);
    return storage->readData();
}

}
