/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

namespace D2ModGen {

class CascStorage final : virtual public IInputStorage {
public:
    CascStorage(const QString& storageRoot)
        : m_storageRoot(storageRoot)
    {}

private:
    StoredData readData(const RequestInMemoryList& filenames) const noexcept override;

private:
    const QString m_storageRoot;
};

}
