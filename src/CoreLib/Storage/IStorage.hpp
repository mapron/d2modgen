/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include <QByteArray>
#include <QStringList>

#include <memory>

namespace D2ModGen {

class IStorage {
public:
    static QString makeTableRelativePath(const QString& id, bool backslash);

    struct StoredFileTable {
        std::string data;
        QString     id;
    };
    using StoredFileTableList = std::vector<StoredFileTable>;

    struct StoredFileMemory {
        std::string data;
        QString     relFilepath;
    };
    using StoredFileMemoryList = std::vector<StoredFileMemory>;

    struct StoredFileRef {
        QString absSrcFilepath;
        QString relFilepath;
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

    using RequestInMemoryList = QSet<QString>;

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
