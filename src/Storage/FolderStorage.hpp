/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

#include "CommonTypes.hpp"

namespace D2ModGen {

class FolderStorage : public IOutputStorage {
public:
    FolderStorage(const QString& storageRoot, StorageType storage, const QString& modname);

    bool prepareForWrite() const noexcept override;

    bool writeData(const OutFileList& files) const noexcept override;

private:
    const StorageType m_storage;
    const QString     m_modName;
    const QString     m_modRoot;
};

}
