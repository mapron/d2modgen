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
    CascStorage(const std::string& storageRoot, bool needBaseSubfolder)
        : m_storageRoot(storageRoot)
        , m_needBaseSubfolder(needBaseSubfolder)
    {}

private:
    StoredData readData(const RequestInMemoryList& filenames) const noexcept override;

private:
    const std::string m_storageRoot;
    const bool        m_needBaseSubfolder;
};

}
