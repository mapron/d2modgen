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
    using Ptr = std::shared_ptr<const IStorage>;

    struct RequestFile {
        QString relFilepath;
        QString id; // if present, will be kept in StoredFile
    };
    using RequestFileList = QList<RequestFile>;

    struct StoredFile {
        QByteArray data; // can be empty for enumeration.
        QString    relFilepath;
        QString    id;
    };

    struct Result {
        bool              success = false;
        QList<StoredFile> files;
    };

public:
    virtual ~IStorage() = default;

    virtual Result ReadData(const QString& storageRoot, const RequestFileList& filenames) const noexcept = 0;

    virtual Result ListContents(const QString& storageRoot) const noexcept = 0;
};

}
