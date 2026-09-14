/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

namespace D2ModGen {

class StormStorage final : public IInputStorage {
public:
    StormStorage(const Mernel::std_path& storageRoot)
        : m_storageRoot(storageRoot)
    {}

private:
    StoredData readData(const RequestInMemoryList& filenames) const noexcept override;

private:
    const Mernel::std_path m_storageRoot;
};

}
