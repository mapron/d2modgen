/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

#include "CommonTypes.hpp"

namespace D2ModGen {

class FolderStorage final
    : virtual public IOutputStorage
    , virtual public IInputStorage {
public:
    FolderStorage(const std_path& storageRoot, StorageType storage, const std::string& modname);

    StoredData readData(const RequestInMemoryList& filenames) const noexcept override;

    bool prepareForWrite() const noexcept override;

    bool writeData(const StoredData& data) const noexcept override;

private:
    const StorageType m_storageType;
    const std::string m_modName;
    const std_path    m_root;
};

}
