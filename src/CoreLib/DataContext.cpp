/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "Logger.hpp"

#include "MernelPlatform/FileFormatJson.hpp"
#include "MernelPlatform/FileFormatCSV.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <algorithm>

#define parseTab(id) \
    if (!parseTable(id, TableId::id, data)) \
    return false

#define serializeTab(id) \
    if (!serializeTable(id, TableId::id, data, true)) \
    return false

#define debugTab(id) \
    serializeTable(id, TableId::id, data, false)

namespace D2ModGen {

DataContext::DataContext() = default;

DataContext::~DataContext() = default;

bool DataContext::parseData(const IStorage::StoredData& data)
{
    dataVersion = data.dataVersion;

    parseTab(weapons);
    parseTab(armor);
    parseTab(automagic);
    parseTab(charstats);
    parseTab(cubemain);
    parseTab(difficultylevels);
    parseTab(experience);
    parseTab(gamble);
    parseTab(gems);
    parseTab(hireling);
    parseTab(itemstatcost);
    parseTab(itemtypes);
    parseTab(levels);
    parseTab(magicprefix);
    parseTab(magicsuffix);
    parseTab(misc);
    parseTab(monlvl);
    parseTab(monstats);
    parseTab(runes);
    parseTab(setitems);
    parseTab(sets);
    parseTab(skilldesc);
    parseTab(skills);
    parseTab(treasureclassex);
    parseTab(uniqueitems);

    Logger() << "Tables have parsed successfully, now doing postprocess to establish links";
    if (!postProcess()) {
        Logger(Logger::Err) << "Postprocess failure";
        return false;
    }

    Logger() << "Postprocess done";

    for (const auto& [id, fileData] : data.jsonFiles) {
        Mernel::PropertyTree doc;
        Logger() << "parse json: " << std::string(getJsonRelPath(id));
        if (!readJsonFromBufferNoexcept(fileData, doc)) {
            Logger(Logger::Warning) << "failed to parse json: " << getJsonRelPath(id);
            return false;
        }
        jsonFiles[id] = std::move(doc);
    }
    blobs = data.blobs;

    return true;
}

bool DataContext::serializeData(IStorage::StoredData& data) const
{
    data.valid       = true;
    data.dataVersion = dataVersion;

    serializeTab(weapons);
    serializeTab(armor);
    serializeTab(automagic);
    serializeTab(charstats);
    serializeTab(cubemain);
    serializeTab(difficultylevels);
    serializeTab(experience);
    serializeTab(gamble);
    serializeTab(gems);
    serializeTab(hireling);
    serializeTab(itemstatcost);
    serializeTab(itemtypes);
    serializeTab(levels);
    serializeTab(magicprefix);
    serializeTab(magicsuffix);
    serializeTab(misc);
    serializeTab(monlvl);
    serializeTab(monstats);
    serializeTab(runes);
    serializeTab(setitems);
    serializeTab(sets);
    serializeTab(skilldesc);
    serializeTab(skills);
    serializeTab(treasureclassex);
    serializeTab(uniqueitems);

    for (const auto& [id, json] : jsonFiles) {
        std::string buffer;
        writeJsonToBufferNoexcept(buffer, json);
        data.jsonFiles[id] = std::move(buffer);
    }

    data.blobs = blobs;
    return true;
}

void DataContext::writeDebug(IStorage::StoredData& data) const
{
    data.valid       = true;
    data.dataVersion = dataVersion;

    debugTab(weapons);
    debugTab(armor);
    debugTab(automagic);
    debugTab(charstats);
    debugTab(cubemain);
    debugTab(difficultylevels);
    debugTab(experience);
    debugTab(gamble);
    debugTab(gems);
    debugTab(hireling);
    debugTab(itemstatcost);
    debugTab(itemtypes);
    debugTab(levels);
    debugTab(magicprefix);
    debugTab(magicsuffix);
    debugTab(misc);
    debugTab(monlvl);
    debugTab(monstats);
    debugTab(runes);
    debugTab(setitems);
    debugTab(sets);
    debugTab(skilldesc);
    debugTab(skills);
    debugTab(treasureclassex);
    debugTab(uniqueitems);
}

bool DataContext::postProcess()
{
    itemtypes.makeIndex(&DataRow_itemtypes::code);
    sets.makeIndex(&DataRow_sets::index);
    armor.makeIndex(&RowBaseEquipment::code);
    weapons.makeIndex(&RowBaseEquipment::code);
    misc.makeIndex(&RowBaseEquipment::code);
    skills.makeIndex(&DataRow_skills::skill);

    const size_t skill_Leap             = 132;
    skills.rows[skill_Leap].canBeOskill = false;

    auto getItemType = [this](std::string code) {
        if (code.size() > 4)
            code.resize(4);
        if (code == "amu")
            code = "amul";
        return itemtypes.index.at(code);
    };

    auto bindEquip = [this, &getItemType](RowBaseEquipment& row) {
        if (!row.isValid())
            return;
        row.itemType = getItemType(row.itemTypeCode);
    };

    for (auto& row : armor.rows)
        bindEquip(row);
    for (auto& row : weapons.rows)
        bindEquip(row);
    for (auto& row : misc.rows)
        bindEquip(row);

    auto findEquip = [this](const std::string& equipCode) -> EquipmentType {
        if (auto it = armor.index.find(equipCode); it != armor.index.cend())
            return it->second;
        if (auto it = weapons.index.find(equipCode); it != weapons.index.cend())
            return it->second;
        if (auto it = misc.index.find(equipCode); it != misc.index.cend())
            return it->second;
        assert(false);
        return nullptr;
    };

    for (auto& row : setitems.rows) {
        if (!row.isValid())
            continue;

        auto* parent_set = sets.index.at(row.setIndex);
        row.eqipmentType = findEquip(row.eqipmentCode);

        const_cast<DataRow_sets*>(parent_set)->level = row.getLevel();
    }
    for (auto& row : uniqueitems.rows) {
        if (!row.isValid())
            continue;

        row.eqipmentType = findEquip(row.eqipmentCode);
    }

    for (auto& row : gems.rows) {
        if (!row.isValid())
            continue;

        row.eqipmentType = findEquip(row.eqipmentCode);
    }

    const std::set<std::string> casterCodes{ "wand", "staf", "orb", "head" };
    const std::set<std::string> brawlCodes{ "h2h", "bow", "xbow", "miss", "thro" };

    for (auto& row : skills.rows) {
        if (!row.charclass.empty())
            row.classSpecific = getClassId(row.charclass);
    }

    for (auto& row : itemtypes.rows) {
        const auto codeStr  = row.code;
        const auto itemType = row.itemType;
        if (codeStr.empty())
            continue;

        const auto parent1 = row.equiv1;
        const auto parent2 = row.equiv2;
        if (!parent1.empty()) {
            auto parent1ptr = const_cast<DataRow_itemtypes*>(itemtypes.index.at(parent1));
            row.parents.insert(parent1ptr);
            parent1ptr->nested.insert(&row);
        }
        if (!parent2.empty()) {
            auto parent2ptr = const_cast<DataRow_itemtypes*>(itemtypes.index.at(parent2));
            row.parents.insert(parent2ptr);
            parent2ptr->nested.insert(&row);
        }

        if (!row.clas.empty())
            row.classSpecific = getClassId(row.clas);

        if (casterCodes.contains(codeStr)) {
            row.idiom = AttributeIdiom::Caster;
        } else if (brawlCodes.contains(codeStr)) {
            row.idiom = AttributeIdiom::Brawler;
        }
    }

    auto expandNested = [this]() -> bool {
        bool change = false;
        for (auto& row : itemtypes.rows) {
            const auto ncopy = row.nested;
            for (const auto& nestedCode : ncopy) {
                row.nested.insert(nestedCode->nested.cbegin(), nestedCode->nested.cend());
            }
            const auto pcopy = row.parents;
            for (const auto& pCode : pcopy) {
                row.parents.insert(pCode->parents.cbegin(), pCode->parents.cend());
            }
            change = change || ncopy != row.nested || pcopy != row.parents;
        }
        return change;
    };
    while (expandNested()) {
        ;
    }

    auto bindAff = [this, &getItemType](auto& row) {
        if (!row.isValid())
            return;
        ItemTypeSet filter;
        for (auto& itype : row.itype) {
            ItemType t = getItemType(itype);
            filter.insert(t);
            filter.insert(t->nested.cbegin(), t->nested.cend());
        }
        for (auto& etype : row.etype) {
            ItemType t = getItemType(etype);
            filter.erase(t);
            for (auto* n : t->nested)
                filter.erase(n);
        }
        row.filter = ItemCodeList(filter);
    };
    for (auto& row : magicprefix.rows)
        bindAff(row);
    for (auto& row : magicsuffix.rows)
        bindAff(row);
    for (auto& row : automagic.rows)
        bindAff(row);
    for (auto& row : runes.rows)
        bindAff(row);

    for (auto& row : runes.rows) {
        row.lvl = 0;
        for (const std::string& rune : row.runeCodes)
            row.lvl = std::max(row.lvl, misc.index.at(rune)->getLevel());
    }
    setstypes.equiv1 = "SET";
    for (auto& row : sets.rows) {
        if (row.isValid())
            row.filter.push_back(&setstypes);
    }

    return true;
}

template<class Row>
bool DataContext::parseTable(DataTable<Row>& table, TableId tableId, const IStorage::StoredData& data)
{
    if (!data.tables.contains(tableId))
        return true;
    const std::string& buffer = data.tables.at(tableId);
    if (!table.parseTable(buffer)) {
        Logger(Logger::Warning) << "failed to parse csv:" << getTableIdString(tableId);
        return false;
    }
    for (size_t i = 0; i < table.indicesUsed.size(); i++) {
        if (table.indicesUsed[i] == size_t(-1)) {
            Logger(Logger::Warning) << "Missing column '" << table.requestedColumns[i] << "' in table '" << getTableIdString(tableId) << "', generation may be incomplete.";
        }
    }

    Logger() << "parse '" << getTableIdString(tableId) << "' buffer[" << buffer.size() << "] => csv[" << table.columnsOriginal.size() << "x" << table.rows.size() << "]";

    return true;
}

template<class Row>
bool DataContext::serializeTable(const DataTable<Row>& table, TableId tableId, IStorage::StoredData& data, bool full) const
{
    if (!table.valid)
        return true;

    std::string& buffer = data.tables[tableId];
    if (!table.serializeTable(buffer, full))
        return false;

    if (full)
        Logger() << "serialize '" << getTableIdString(tableId) << "' csv[" << table.columnsOriginal.size() << "x" << table.rows.size() << "] => buffer[" << buffer.size() << "] ";

    return true;
}

template<class Row>
bool DataTable<Row>::parseTable(const std::string& buffer)
{
    Mernel::CSVTable csv;
    if (!Mernel::readCSVFromBuffer(buffer, csv)) {
        return false;
    }
    columnsOriginal                                  = csv.columns;
    valid                                            = true;
    requestedColumns                                 = Row{}.getColumns();
    const std::map<std::string, std::string> aliases = Row{}.getColumnAliases();
    indicesUsed.resize(requestedColumns.size(), size_t(-1));

    for (size_t i = 0; i < columnsOriginal.size(); i++) {
        std::string col = Mernel::strToLower(columnsOriginal[i]);
        if (auto it = aliases.find(col); it != aliases.cend())
            col = it->second;

        auto it = std::find(requestedColumns.cbegin(), requestedColumns.cend(), col);
        if (it == requestedColumns.cend()) {
            indicesUnused.push_back(i);
        } else {
            auto usedIndex         = std::distance(requestedColumns.cbegin(), it);
            indicesUsed[usedIndex] = i;
            columnsUsed.push_back(col);
        }
    }

    for (auto& rowCsv : csv.rows) {
        Row row;
        row._ser_unusedCells.resize(indicesUnused.size());
        row._ser_usedCells.resize(indicesUsed.size(), RowBase::s_empty);

        for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
            row._ser_unusedCells[i] = rowCsv.data[indicesUnused[i]].str;
        }
        for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
            if (indicesUsed[i] != size_t(-1))
                row._ser_usedCells[i] = rowCsv.data[indicesUsed[i]].str;
        }
        row._ser_read      = true;
        row._ser_cellIndex = 0;
        row.serialize();
        row._ser_usedCells.clear();
        rows.push_back(std::move(row));
    }
    return true;
}

template<class Row>
bool DataTable<Row>::serializeTable(std::string& buffer, bool full) const
{
    Mernel::CSVTable csv;
    csv.columns       = full ? columnsOriginal : columnsUsed;
    csv.endsWithNL    = true;
    bool prependedKey = false;

    for (auto row : rows) {
        row._ser_usedCells.resize(indicesUsed.size());
        row._ser_read      = false;
        row._ser_cellIndex = 0;
        row._ser_key.reset();
        row.serialize();
        if (row._ser_key.has_value() && !prependedKey && !full) {
            prependedKey = true;
            csv.columns.insert(csv.columns.begin(), "*pkey");
        }
        Mernel::CSVTableRow rowCsv;
        rowCsv.data.resize(csv.columns.size());

        if (full) {
            for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
                rowCsv.data[indicesUnused[i]].str = row._ser_unusedCells[i];
            }
            for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
                if (indicesUsed[i] != size_t(-1))
                    rowCsv.data[indicesUsed[i]].str = row._ser_usedCells[i];
            }
        } else {
            std::map<size_t, std::string> values;
            for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
                if (indicesUsed[i] != size_t(-1))
                    values[indicesUsed[i]] = row._ser_usedCells[i];
            }

            for (size_t i = 0; auto& [_, value] : values)
                rowCsv.data[i++].str = value;
        }
        if (row._ser_key.has_value() && !full)
            rowCsv.data.insert(rowCsv.data.begin(), row._ser_key.value());
        csv.rows.push_back(std::move(rowCsv));
    }

    return Mernel::writeCSVToBuffer(buffer, csv);
}

}
