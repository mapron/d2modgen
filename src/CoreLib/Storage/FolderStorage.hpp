/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "IStorage.hpp"

#include "FileIOUtils.hpp"
#include "GenerationEnvironment.hpp"

namespace D2ModGen {

class FolderStorage final
    : public IWriteableStorage {
public:
    FolderStorage(const std_path& storageRoot, StorageType storage = StorageType::CsvFolder);

    StoredData readData() const noexcept override;

    bool create() const noexcept override;

    bool writeMeta(const StoredData& data) const noexcept override;
    bool writeMainData(const StoredData& data) const noexcept override;
    bool writeBitmaps(const StoredData& data) const noexcept override;

    void setSilentTrace(bool silent) noexcept override { m_silent = silent; }
    void setFileSuffix(const std::string& suffix) noexcept override { m_suffix = suffix; }
    void setUseBaseSubolder(bool useBaseSubfolder) noexcept override { m_needBaseSubfolder = useBaseSubfolder; }
    void setModName(const std::string& modname) noexcept override { m_modname = modname; }

private:
    std_path getRoot() const;
    bool     writeFile(const std::string& data, const std_path& relPath) const;
    bool     readFile(std::string& data, const std_path& relPath, bool warnNonExistent = false) const;

    const StorageType m_storageType;
    std_path          m_root;
    bool              m_needBaseSubfolder;
    bool              m_silent = true;
    std::string       m_suffix;
    std::string       m_modname;
};

}
