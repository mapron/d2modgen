/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

namespace D2ModGen {

class CascStorage final : public IInputStorage {
public:
    CascStorage(const Mernel::std_path& storageRoot, bool needBaseSubfolder)
        : m_storageRoot(storageRoot)
        , m_needBaseSubfolder(needBaseSubfolder)
    {}

private:
    StoredData readData(const RequestInMemoryList& filenames) const noexcept override;

private:
    const Mernel::std_path m_storageRoot;
    const bool             m_needBaseSubfolder;
};

}
