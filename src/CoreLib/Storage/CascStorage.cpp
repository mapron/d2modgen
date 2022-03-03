/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "CascStorage.hpp"

#include "RAIIUtils.hpp"
#include "StorageConstants.hpp"

#include <CascLib.h>

#include <QDebug>

namespace D2ModGen {

IStorage::StoredData CascStorage::readData(const RequestInMemoryList& filenames) const noexcept
{
    const std::string utf8path = m_storageRoot.toStdString();
    HANDLE            storage;
    if (!CascOpenStorage(utf8path.c_str(), 0, &storage)) {
        qWarning() << "failed to open storage:" << utf8path.c_str();
        return {};
    }
    MODGEN_SCOPE_EXIT([storage] { CascCloseStorage(storage); });

    auto readCascFile = [storage](std::string& data, const QString& filename) -> bool {
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

    IStorage::StoredData result{ true };

    for (const QString& id : g_tableNames) {
        std::string buffer;
        if (!readCascFile(buffer, IStorage::makeTableRelativePath(id, true)))
            continue;

        result.tables.push_back(StoredFileTable{ std::move(buffer), id });
    }
    for (const QString& relativePath : filenames) {
        std::string buffer;
        if (!readCascFile(buffer, relativePath))
            continue;

        result.inMemoryFiles.push_back(StoredFileMemory{ std::move(buffer), relativePath });
    }
    return result;
}

}
