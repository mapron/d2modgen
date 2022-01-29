/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"

namespace D2ModGen {

class StorageCache {
public:
    struct Context {
        StorageType m_storage;
        QString     m_root;
        QStringList m_tableIds;
        JsonFileSet m_jsonFiles;

        bool operator==(const Context& rh) const noexcept
        {
            return m_storage == rh.m_storage
                   && m_root == rh.m_root
                   && m_tableIds == rh.m_tableIds
                   && m_jsonFiles == rh.m_jsonFiles;
        }
    };

public:
    DataContextPtr Load(const Context& context)
    {
        if (context == m_prev)
            return CloneCache();

        m_cache = LoadImpl(context);
        m_prev  = context;
        return CloneCache();
    }

    DataContextPtr Load(StorageType        storage,
                        const QString&     root,
                        const QStringList& tableIds,
                        const JsonFileSet& jsonFiles)
    {
        return Load({ storage,
                      root,
                      tableIds,
                      jsonFiles });
    }

private:
    DataContextPtr CloneCache() const
    {
        return m_cache ? std::make_shared<DataContext>(*m_cache) : nullptr;
    }
    static DataContextPtr LoadImpl(const Context& context);

private:
    DataContextPtr m_cache;
    Context        m_prev;
};

}
