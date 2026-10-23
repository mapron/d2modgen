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
    DataTable<DataRow_monstats>         monstats;
    DataTable<DataRow_skilldesc>        skilldesc;
    DataTable<DataRow_skills>           skills;
    DataTable<DataRow_treasureclassex>  treasureclassex;

    DataRow_itemtypes setstypes; // special marker for full sets so they have own fake item type.

    std::map<JsonId, Mernel::PropertyTree> jsonFiles;
    std::map<BlobId, std::string>          blobs;

    int64_t dataVersion = 0;

    DataContext();
    ~DataContext();

    bool parseData(const IStorage::StoredData& data);
    bool serializeData(IStorage::StoredData& data) const;

    void writeDebug(IStorage::StoredData& data) const;

private:
    bool postProcess();

    template<class Row>
    bool parseTable(DataTable<Row>& table, TableId tableId, const IStorage::StoredData& data);

    template<class Row>
    bool serializeTable(const DataTable<Row>& table, TableId tableId, IStorage::StoredData& data, bool full) const;
};

}
