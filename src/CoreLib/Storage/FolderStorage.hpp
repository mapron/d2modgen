/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

#include "CommonTypes.hpp"
#include "FileIOUtils.hpp"

namespace D2ModGen {

class FolderStorage final
    : public IWriteableStorage {
public:
    FolderStorage(const std_path& storageRoot, StorageType storage, const std::string& modname, bool needBaseSubfolder, std::string suffix = "");

    StoredData readData() const noexcept override;

    bool prepareForWrite() const noexcept override;

    bool writeData(const StoredData& data) const noexcept override;

private:
    const StorageType m_storageType;
    const std::string m_modName;
    const std_path    m_root;
    const bool        m_needBaseSubfolder;
    const std::string m_suffix;
};

}
