/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "RAIIUtils.hpp"

#include <CascLib.h>

#include <QDebug>

namespace D2ModGen {

IInputStorage::Result CascStorage::readData(const QString& storageRoot, const RequestFileList& filenames) const noexcept
{
    const std::string utf8path = storageRoot.toStdString();
    HANDLE            storage;
    if (!CascOpenStorage(utf8path.c_str(), 0, &storage)) {
        qWarning() << "failed to open storage:" << utf8path.c_str();
        return {};
    }
    MODGEN_SCOPE_EXIT([storage] { CascCloseStorage(storage); });

    auto readCascFile = [storage](QByteArray& data, const QString& filename) -> bool {
        const std::string fullId = QString("data:%1").arg(filename).toStdString();
        HANDLE            fileHandle;

        if (!CascOpenFile(storage, fullId.c_str(), CASC_LOCALE_ALL, 0, &fileHandle)) {
            qWarning() << "failed to open:" << fullId.c_str();
            return false;
        }
        MODGEN_SCOPE_EXIT([fileHandle] { CascCloseFile(fileHandle); });

        auto dataSize = CascGetFileSize(fileHandle, nullptr);

        data.resize(dataSize);
        DWORD wread;
        if (!CascReadFile(fileHandle, data.data(), dataSize, &wread)) {
            qWarning() << "failed to read:" << fullId.c_str();
            return false;
        }
        return true;
    };

    IInputStorage::Result result{ true };

    for (const auto& filename : filenames) {
        QByteArray buffer;
        if (!readCascFile(buffer, filename.relFilepath))
            return {};
        result.files << StoredFile{ buffer, filename.relFilepath, filename.id };
    }
    return result;
}

}
