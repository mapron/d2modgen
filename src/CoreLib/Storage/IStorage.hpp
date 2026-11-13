/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include <map>
#include <string>
#include <array>
#include <vector>

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
    inventory,
    itemstatcost,
    itemtypes,
    levels,
    lvlmaze,
    magicprefix,
    magicsuffix,
    misc,
    missiles,
    monlvl,
    monseq,
    monstats,
    monstats2,
    runes,
    setitems,
    sets,
    skilldesc,
    skills,
    superuniques,
    treasureclassex,
    uniqueitems,

    count__,
};

inline auto getTableIdsRead()
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
    Missiles,

    ProfileHd,
    ProfileLv,

    LayoutMnkbClassic,
    LayoutMnkbExpansn,

    LayoutCtrlClassic,
    LayoutCtrlExpansn,

    count__,
};

inline auto getJsonIdsRead()
{
    std::array<JsonId, size_t(JsonId::count__)> result;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = static_cast<JsonId>(i);
    return result;
}

std::string_view getJsonRelPath(JsonId id);
bool             isPrettyPrintable(JsonId id);

enum class BlobId
{
    PatchStringTbl,

    IconAmaDC6,
    IconSorDC6,
    IconNecDC6,
    IconPalDC6,
    IconBarDC6,
    IconDruDC6,
    IconAssDC6,

    // Mnkb = Mouse and keyboard
    // Orign = unpatched sprite
    // Lowend = 1/2 resolution
    OrignMnkbInventoryClassicSpriteHighre,
    OrignMnkbInventoryClassicSpriteLowres,
    OrignMnkbInventoryExpansnSpriteHighre,
    OrignMnkbInventoryExpansnSpriteLowend,

    // Ctrl = controller
    OrignCtrlInventoryClassicSpriteHighre,
    OrignCtrlInventoryClassicSpriteLowres,
    OrignCtrlInventoryExpansnSpriteHighre,
    OrignCtrlInventoryExpansnSpriteLowend,

    // fullscreen controller panel background, used as base for all expanded inventory backgrounds
    OrignCtrlMainPanelBgSpriteHighre,
    OrignCtrlMainPanelBgSpriteLowend,

    IconAmaHighre,
    IconAmaLowres,
    IconSorHighre,
    IconSorLowres,
    IconNecHighre,
    IconNecLowres,
    IconPalHighre,
    IconPalLowres,

    IconBarHighre,
    IconBarLowres,
    IconDruHighre,
    IconDruLowres,
    IconAssHighre,
    IconAssLowres,
    IconWarHighre,
    IconWarLowres,

    // Patched version of sprite with different name
    PatchMnkbInventoryClassicSpriteHighre,
    PatchMnkbInventoryClassicSpriteLowres,
    PatchMnkbInventoryExpansnSpriteHighre,
    PatchMnkbInventoryExpansnSpriteLowend,

    PatchCtrlInventoryClassicSpriteHighre,
    PatchCtrlInventoryClassicSpriteLowres,
    PatchCtrlInventoryExpansnSpriteHighre,
    PatchCtrlInventoryExpansnSpriteLowend,

    count__,
    LegacyFirst = PatchStringTbl,
    LegacyLast  = IconAssDC6,

    ResurrectedFirst = OrignMnkbInventoryClassicSpriteHighre,
    ResurrectedLast  = IconWarLowres,

    SkipWriteFirst = OrignMnkbInventoryClassicSpriteHighre,
    SkipWriteLast  = OrignCtrlMainPanelBgSpriteLowend,
};

inline auto getBlobIdsLegacy()
{
    using enum BlobId;
    const size_t                        start = size_t(LegacyFirst);
    const size_t                        end   = size_t(LegacyLast);
    std::array<BlobId, 1 + end - start> result;
    for (size_t i = start; i <= end; ++i)
        result[i - start] = static_cast<BlobId>(i);
    return result;
}

inline auto getBlobIdsResurrected()
{
    using enum BlobId;
    const size_t                        start = size_t(ResurrectedFirst);
    const size_t                        end   = size_t(ResurrectedLast);
    std::array<BlobId, 1 + end - start> result;
    for (size_t i = start; i <= end; ++i)
        result[i - start] = static_cast<BlobId>(i);
    return result;
}

std::string_view getBlobRelPath(BlobId id);

class IStorage {
public:
    static std::string makeTableRelativePath(TableId id, bool baseSub);

    static std::string_view s_dataVersionPath;
    using TableMap = std::map<TableId, std::string>;

    struct StoredData {
        bool                               valid = false;
        TableMap                           tablesMain;
        TableMap                           tablesBase; // D2R - LoD
        std::map<JsonId, std::string>      jsonFiles;
        std::map<BlobId, std::string>      blobs;
        std::map<std::string, std::string> bitmaps;

        int64_t dataVersion = 0;

        bool operator==(const StoredData&) const = default;
    };

public:
    virtual ~IStorage() = default;

    virtual StoredData readData() const noexcept = 0;

    virtual void setSilentTrace(bool silent) noexcept {}
    virtual void setFileSuffix(const std::string& suffix) noexcept {}
};

class IWriteableStorage : public IStorage {
public:
    virtual void setModName(const std::string& modname) noexcept = 0;

    virtual bool create() const noexcept = 0;

    virtual bool writeMeta(const StoredData& data) const noexcept     = 0;
    virtual bool writeMainData(const StoredData& data) const noexcept = 0;
    virtual bool writeBitmaps(const StoredData& data) const noexcept  = 0;
};

}
