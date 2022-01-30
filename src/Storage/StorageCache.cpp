/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StorageCache.hpp"

#include "FileIOUtils.hpp"

#include "CascStorage.hpp"
#include "StormStorage.hpp"

namespace D2ModGen {

IStorage::StoredData StorageCache::loadImpl(const Context& context)
{
    IInputStorage::Ptr storage;
    if (context.m_storage == StorageType::D2ResurrectedInternal)
        storage = std::make_shared<CascStorage>(context.m_root);
    else if (context.m_storage == StorageType::D2LegacyInternal)
        storage = std::make_shared<StormStorage>(context.m_root);
    else {
        assert(!"TODO");
        return {};
    }
    return storage->readData(context.m_inMemoryFiles);
}

}
