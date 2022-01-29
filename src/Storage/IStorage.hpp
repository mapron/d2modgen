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
public:
    virtual ~IStorage() = default;
};

class IInputStorage : public IStorage {
public:
    using Ptr = std::shared_ptr<const IInputStorage>;

    struct StoredFile {
        QByteArray data; // can be empty for enumeration.
        QString    relFilepath;
        QString    id;
    };
    using StoredFileList = QList<StoredFile>;

    struct RequestFile {
        QString relFilepath;
        QString id; // if present, will be kept in StoredFile
    };
    using RequestFileList = QList<RequestFile>;

    struct Result {
        bool           success = false;
        StoredFileList files;
    };

public:
    virtual Result readData(const QString& storageRoot, const RequestFileList& filenames) const noexcept = 0;

    virtual Result listContents(const QString& storageRoot) const noexcept = 0;
};

class IOutputStorage : public IStorage {
public:
    using Ptr = std::shared_ptr<const IOutputStorage>;

    struct OutFile {
        QByteArray data; // can be empty for enumeration.
        QString    relFilepath;
        QString    srcAbsFilepath;
    };
    using OutFileList = QList<OutFile>;

public:
    virtual bool prepareForWrite() const noexcept = 0;

    virtual bool writeData(const OutFileList& files) const noexcept = 0;
};

}
