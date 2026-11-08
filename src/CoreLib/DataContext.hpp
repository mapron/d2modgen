/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#pragma once

#include "Storage/IStorage.hpp"
#include "MernelPlatform/PropertyTree.hpp"

#include "DataContextBase.hpp"
#include "DataContextEquipment.hpp"
#include "DataContextOther.hpp"

namespace D2ModGen {

struct Bitmap {
    std::vector<uint8_t> rgba;
    size_t               width  = 0;
    size_t               height = 0;

    struct Color {
        uint8_t r{};
        uint8_t g{};
        uint8_t b{};
        uint8_t a{};
    };

    Color brush;

    void           resize(size_t w, size_t h);
    uint8_t*       getPixel(size_t x, size_t y);
    const uint8_t* getPixel(size_t x, size_t y) const;
    Color          getColor(size_t x, size_t y) const;

    void putDot(size_t x, size_t y);

    void dumpToBuffer(std::string& buffer) const;
    bool readFromSprite(const std::string& spriteBuffer);
    void writeToSprite(std::string& spriteBuffer);
    void paintLineH(size_t x1, size_t x2, size_t y);
    void paintLineV(size_t x, size_t y1, size_t y2);
    void fillRectPoints(size_t x1, size_t x2, size_t y1, size_t y2);
    void fill();

    void fillRect(size_t x, size_t y, size_t w, size_t h);
    void linedRect(size_t x, size_t y, size_t w, size_t h);

    static void blit(const Bitmap& src, int srcX, int srcY, Bitmap& dest, int destX, int destY, int width, int height);
};

struct DataContext {
    DataTable<DataRow_itemtypes> itemtypes;

    DataTable<DataRow_weapons> weapons;
    DataTable<DataRow_armor>   armor;
    DataTable<DataRow_misc>    misc;

    DataTable<DataRow_automagic>   automagic;
    DataTable<DataRow_magicprefix> magicprefix;
    DataTable<DataRow_magicsuffix> magicsuffix;

    DataTable<DataRow_uniqueitems> uniqueitems;
    DataTable<DataRow_setitems>    setitems;

    DataTable<DataRow_sets>     sets;
    DataTable<DataRow_runes>    runes;
    DataTable<DataRow_gems>     gems;
    DataTable<DataRow_cubemain> cubemain;

    DataTable<DataRow_charstats>        charstats;
    DataTable<DataRow_difficultylevels> difficultylevels;
    DataTable<DataRow_experience>       experience;
    DataTable<DataRow_gamble>           gamble;
    DataTable<DataRow_hireling>         hireling;
    DataTable<DataRow_itemstatcost>     itemstatcost;
    DataTable<DataRow_levels>           levels;
    DataTable<DataRow_monlvl>           monlvl;
    DataTable<DataRow_monseq>           monseq;
    DataTable<DataRow_monstats>         monstats;
    DataTable<DataRow_monstatsEx>       monstatsEx;
    DataTable<DataRow_skilldesc>        skilldesc;
    DataTable<DataRow_skills>           skills;
    DataTable<DataRow_treasureclassex>  treasureclassex;
    DataTable<DataRow_missiles>         missiles;
    DataTable<DataRow_inventory>        inventory;

    static auto allTables()
    {
        return std::tuple{
            &DataContext::itemtypes,

            &DataContext::weapons,
            &DataContext::armor,
            &DataContext::misc,

            &DataContext::automagic,
            &DataContext::magicprefix,
            &DataContext::magicsuffix,

            &DataContext::uniqueitems,
            &DataContext::setitems,

            &DataContext::sets,
            &DataContext::runes,
            &DataContext::gems,
            &DataContext::cubemain,

            &DataContext::charstats,
            &DataContext::difficultylevels,
            &DataContext::experience,
            &DataContext::gamble,
            &DataContext::hireling,
            &DataContext::itemstatcost,
            &DataContext::levels,
            &DataContext::monseq,
            &DataContext::monlvl,
            &DataContext::monstats,
            &DataContext::monstatsEx,
            &DataContext::skilldesc,
            &DataContext::skills,
            &DataContext::treasureclassex,
            &DataContext::missiles,
            &DataContext::inventory,
        };
    }

    DataRow_itemtypes setstypes; // special marker for full sets so they have own fake item type.

    std::map<JsonId, Mernel::PropertyTree> jsonFiles;
    std::map<BlobId, std::string>          blobs;
    std::map<std::string, Bitmap>          bitmaps;

    int64_t dataVersion = 0;
    bool    silent      = true;

    struct IdClone {
        std::string from;
        std::string to;
    };
    using IdCloneList = std::vector<IdClone>;
    struct {
        IdCloneList monstats;
        IdCloneList missiles;
        IdCloneList skills;
    } idClones;

    DataContext();
    ~DataContext();

    bool parseData(const IStorage::StoredData& data, bool doPostprocess = true);
    bool serializeData(IStorage::StoredData& data) const;

    IStorage::StoredData writeDebug() const;

    DataContext calculateDiff(const DataContext& older) const;

    int getClassesCount() const;

    DataContext makeSilent() const
    {
        DataContext result = *this;
        result.silent      = true;
        return result;
    }

private:
    bool postProcess();

    template<class Row>
    bool parseTable(DataTable<Row>& table, const IStorage::StoredData& data);

    template<class Row>
    bool serializeTable(const DataTable<Row>& table, IStorage::StoredData& data, bool full) const;

    template<class Row>
    void diffTable(const DataTable<Row>& tableOlder, const DataTable<Row>& tableNewer, DataTable<Row>& result) const;
};

}
