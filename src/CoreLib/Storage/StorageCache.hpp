/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include "IStorage.hpp"

namespace D2ModGen {

class StorageCache {
public:
    using RequestList = IInputStorage::RequestInMemoryList;
    struct Context {
        StorageType m_storage;
        QString     m_root;
        RequestList m_inMemoryFiles;

        bool operator==(const Context& rh) const noexcept
        {
            return m_storage == rh.m_storage
                   && m_root == rh.m_root
                   && m_inMemoryFiles == rh.m_inMemoryFiles;
        }
    };

public:
    IStorage::StoredData load(const Context& input)
    {
        if (input == m_prev)
            return m_cache;

        m_cache = loadImpl(input);
        m_prev  = input;
        return m_cache;
    }

    IStorage::StoredData load(StorageType        storage,
                              const QString&     root,
                              const RequestList& inMemoryFiles)
    {
        return load({ storage,
                      root,
                      inMemoryFiles });
    }

private:
    static IStorage::StoredData loadImpl(const Context& input);

private:
    IStorage::StoredData m_cache;
    Context              m_prev;
};

}
