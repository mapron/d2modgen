/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"
#include "FileIOUtils.hpp"

#include <memory>

namespace D2ModGen {

class IStorage {
public:
    static std::string makeTableRelativePath(const std::string& id, bool backslash, bool baseSub);

    struct StoredFileTable {
        std::string data;
        std::string id;
    };
    using StoredFileTableList = std::vector<StoredFileTable>;

    struct StoredFileMemory {
        std::string data;
        std_path    relFilepath;
    };
    using StoredFileMemoryList = std::vector<StoredFileMemory>;

    struct StoredFileRef {
        std_path absSrcFilepath;
        std_path relFilepath;
    };
    using StoredFileRefList = std::vector<StoredFileRef>;

    struct StoredData {
        bool                 valid = false;
        StoredFileTableList  tables;
        StoredFileMemoryList inMemoryFiles;
        StoredFileRefList    refFiles;
    };

public:
    virtual ~IStorage() = default;
};

class IInputStorage : virtual public IStorage {
public:
    using Ptr = std::shared_ptr<const IInputStorage>;

    using RequestInMemoryList = StringSet;

public:
    virtual ~IInputStorage() = default;

    virtual StoredData readData(const RequestInMemoryList& filenames) const noexcept = 0;
};

class IOutputStorage : virtual public IStorage {
public:
    using Ptr = std::shared_ptr<const IOutputStorage>;

public:
    virtual ~IOutputStorage() = default;

    virtual bool prepareForWrite() const noexcept = 0;

    virtual bool writeData(const StoredData& data) const noexcept = 0;
};

}
