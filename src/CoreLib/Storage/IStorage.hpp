/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <map>
#include <string>
#include <array>

namespace D2ModGen {

enum class TableId
{
    weapons,
    armor,
    automagic,
    charstats,
    cubemain,
    difficultylevels,
    experience,
    gamble,
    gems,
    hireling,
    itemstatcost,
    itemtypes,
    levels,
    magicprefix,
    magicsuffix,
    misc,
    monlvl,
    monstats,
    runes,
    setitems,
    sets,
    skilldesc,
    skills,
    treasureclassex,
    uniqueitems,

    count__,
};

inline std::array<TableId, size_t(TableId::count__)> getTableIds()
{
    std::array<TableId, size_t(TableId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<TableId>(i);
    return result;
}

std::string_view getTableIdString(TableId id);

enum class JsonId
{
    ItemNames,
    ItemNameAffixes,
    Monsters,

    count__,
};

inline auto getJsonIds()
{
    std::array<JsonId, size_t(JsonId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<JsonId>(i);
    return result;
}

std::string_view getJsonRelPath(JsonId id);

enum class BlobId
{
    PatchStringTbl,

    count__,
};

inline auto getBlobIds()
{
    std::array<BlobId, size_t(BlobId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<BlobId>(i);
    return result;
}

std::string_view getBlobRelPath(BlobId id);

class IStorage {
public:
    static std::string makeTableRelativePath(TableId id, bool baseSub);

    static std::string_view s_dataVersionPath;

    struct StoredData {
        bool                           valid = false;
        std::map<TableId, std::string> tables;
        std::map<JsonId, std::string>  jsonFiles;
        std::map<BlobId, std::string>  blobs;

        int64_t dataVersion = 0;

        bool operator==(const StoredData&) const = default;
    };

public:
    virtual ~IStorage() = default;

    virtual StoredData readData() const noexcept = 0;
};

class IWriteableStorage : public IStorage {
public:
    virtual bool prepareForWrite() const noexcept                 = 0;
    virtual bool writeData(const StoredData& data) const noexcept = 0;
};

}
