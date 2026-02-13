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
        std::string m_root;
        RequestList m_inMemoryFiles;
        bool        m_needBaseSubfolder = false;

        bool operator==(const Context& rh) const noexcept
        {
            return m_storage == rh.m_storage
                   && m_root == rh.m_root
                   && m_inMemoryFiles == rh.m_inMemoryFiles
                   && m_needBaseSubfolder == rh.m_needBaseSubfolder;
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
                              const std::string& root,
                              const RequestList& inMemoryFiles,
                              bool               needBaseSubfolder)
    {
        return load({ storage,
                      root,
                      inMemoryFiles,
                      needBaseSubfolder });
    }

private:
    static IStorage::StoredData loadImpl(const Context& input);

private:
    IStorage::StoredData m_cache;
    Context              m_prev;
};

}
