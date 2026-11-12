/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "StormStorage.hpp"

#include "MernelPlatform/ScopeExit.hpp"

#include "Logger.hpp"

#include <StormLib.h>

namespace D2ModGen {

IStorage::StoredData StormStorage::readData() const noexcept
{
    using namespace Mernel;
    std_path              mainMpqPath;
    std::vector<std_path> patches;
    {
        const bool     isPD2       = std_fs::exists(m_storageRoot / "pd2data.mpq");
        const std_path vanillaRoot = isPD2 ? m_storageRoot / ".." : m_storageRoot;

        mainMpqPath = vanillaRoot / "d2data.mpq";

        patches = { vanillaRoot / "d2exp.mpq", vanillaRoot / "patch_d2.mpq" };

        if (isPD2) {
            patches.push_back(m_storageRoot / "pd2data.mpq");
            patches.push_back(m_storageRoot / "pd2assets.mpq");
        }

        if (!std_fs::exists(mainMpqPath)) {
            Logger(Logger::Err) << "Base game data file not found: " << path2string(mainMpqPath);
            return {};
        }
    }

    HANDLE mpq;
    if (!SFileOpenArchive(mainMpqPath.native().c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
        Logger(Logger::Err) << "Failed to load main mpq: " << path2string(mainMpqPath);
        return {};
    } else {
        Logger() << "Opened main mpq: " << path2string(mainMpqPath);
    }

    MERNEL_SCOPE_EXIT([mpq] { SFileCloseArchive(mpq); });

    for (auto& patchPath : patches) {
        if (!SFileOpenPatchArchive(mpq, patchPath.native().c_str(), nullptr, STREAM_FLAG_READ_ONLY)) {
            Logger(Logger::Err) << "Failed to load patch mpq: " << path2string(patchPath);
            return {};
        } else {
            Logger() << "Opened patch mpq: " << path2string(patchPath);
        }
    }

    auto readStormFile = [mpq](std::string& data, const std::string& filename) -> bool {
        const std::string fullId = filename;
        if (!SFileHasFile(mpq, fullId.c_str())) {
            Logger() << "no such file:" << filename;
            return false;
        }

        HANDLE fileHandle;
        if (!SFileOpenFileEx(mpq, fullId.c_str(), SFILE_OPEN_FROM_MPQ, &fileHandle)) {
            Logger(Logger::Warning) << "failed to open:" << fullId;
            return false;
        }
        MERNEL_SCOPE_EXIT([fileHandle] { SFileCloseFile(fileHandle); });

        auto dataSize = SFileGetFileSize(fileHandle, nullptr);
        data.resize(dataSize);
        DWORD wread;
        if (!SFileReadFile(fileHandle, data.data(), dataSize, &wread, nullptr)) {
            Logger(Logger::Warning) << "failed to read:" << fullId.c_str();
            return false;
        }

        Logger() << "Read: " << filename << " [size: " << data.size() << "]";

        return true;
    };

    StoredData result{ true };
    for (TableId id : getTableIdsRead()) {
        std::string buffer;
        if (!readStormFile(buffer, IStorage::makeTableRelativePath(id, false)))
            continue;

        result.tablesMain[id] = std::move(buffer);
    }
    for (BlobId id : getBlobIdsLegacy()) {
        std::string buffer;
        if (!readStormFile(buffer, std::string(getBlobRelPath(id))))
            continue;

        result.blobs[id] = std::move(buffer);
    }
    // hmm probably never will have json in mpq.
    result.valid = !result.tablesMain.empty();

    return result;
}

}
