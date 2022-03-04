/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "StorageCache.hpp"

#include "FileIOUtils.hpp"

#include "CascStorage.hpp"
#include "StormStorage.hpp"

#include <cassert>

namespace D2ModGen {

IStorage::StoredData StorageCache::loadImpl(const Context& input)
{
    IInputStorage::Ptr storage;
    if (input.m_storage == StorageType::D2ResurrectedInternal)
        storage = std::make_shared<CascStorage>(input.m_root);
    else if (input.m_storage == StorageType::D2LegacyInternal)
        storage = std::make_shared<StormStorage>(input.m_root);
    else {
        assert(!"TODO");
        return {};
    }
    return storage->readData(input.m_inMemoryFiles);
}

}
