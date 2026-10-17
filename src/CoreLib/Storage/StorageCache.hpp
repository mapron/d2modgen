/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

#include "IStorage.hpp"

namespace D2ModGen {

class StorageCache {
public:
    struct Context {
        StorageType      m_storage;
        Mernel::std_path m_root;
        bool             m_needBaseSubfolder = false;

        bool operator==(const Context& rh) const noexcept = default;
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

private:
    static IStorage::StoredData loadImpl(const Context& input);

private:
    IStorage::StoredData m_cache;
    Context              m_prev;
};

}
