/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"
#include "FileIOUtils.hpp"

namespace D2ModGen {

class CascStorage final : public IStorage {
public:
    CascStorage(const Mernel::std_path& storageRoot, const Mernel::std_path& cacheRoot)
        : m_storageRoot(storageRoot)
        , m_cacheRoot(cacheRoot)
    {}

private:
    StoredData readData() const noexcept override;

    void setVerbose(bool verbose) noexcept override { m_verbose = verbose; }

private:
    const Mernel::std_path m_storageRoot;
    const Mernel::std_path m_cacheRoot;

    bool m_verbose = false;
};

}
