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

namespace D2ModGen {

DataContext::DataContext() = default;

DataContext::~DataContext() = default;

bool DataContext::parseData(const IStorage::StoredData& data)
{
    dataVersion = data.dataVersion;

    auto parseTab = [&data, this](auto m) -> bool {
        auto& table = this->*m;
        return parseTable(table, data);
    };

    const auto result = std::apply([&parseTab](const auto&... args) -> bool {
        return (parseTab(std::forward<decltype(args)>(args)) && ...);
    },
                                   allTables());

    if (!result)
        return true;

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

    auto serializeTab = [&data, this](auto m) -> bool {
        const auto& table = this->*m;
        return serializeTable(table, data, true);
    };

    const auto result = std::apply([&serializeTab](const auto&... args) -> bool {
        return (serializeTab(std::forward<decltype(args)>(args)) && ...);
    },
                                   allTables());

    if (!result)
        return true;

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

    auto serializeTab = [&data, this](auto m) -> bool {
        const auto& table = this->*m;
        return serializeTable(table, data, false);
    };

    std::ignore = std::apply([&serializeTab](const auto&... args) -> bool {
        return (serializeTab(std::forward<decltype(args)>(args)) && ...);
    },
                             allTables());
}

DataContext DataContext::calculateDiff(const DataContext& older) const
{
    DataContext result;
    result.dataVersion = dataVersion;

    auto diffTab = [&older, &result, this](auto m) -> bool {
        const auto& tableOlder  = older.*m;
        const auto& tableNewer  = this->*m;
        auto&       tableResult = result.*m;
        tableResult             = tableNewer;
        tableResult.rows.clear();

        diffTable(tableOlder, tableNewer, tableResult);
        return true;
    };

    std::ignore = std::apply([&diffTab](const auto&... args) -> bool {
        return (diffTab(std::forward<decltype(args)>(args)) && ...);
    },
                             allTables());

    return result;
}

int DataContext::getClassesCount() const
{
    return charstats.rows.size() - 1;
}

bool DataContext::postProcess()
{
    itemtypes.makeIndex(&DataRow_itemtypes::code);
    sets.makeIndex(&DataRow_sets::index);
    armor.makeIndex(&RowBaseEquipment::code);
    weapons.makeIndex(&RowBaseEquipment::code);
    misc.makeIndex(&RowBaseEquipment::code);
    skills.makeIndex(&DataRow_skills::skill);
    monstatsEx.makeIndex(&DataRow_monstatsEx::id);
    monstats.makeIndex(&DataRow_monstats::id);
    skilldesc.makeIndex(&DataRow_skilldesc::skilldesc);
    missiles.makeIndex(&DataRow_missiles::name);

    const size_t skill_Leap             = 132;
    skills.rows[skill_Leap].canBeOskill = false;

    auto getItemType = [this](std::string code) -> ItemType {
        if (code.size() > 4)
            code.resize(4);
        if (code == "amu")
            code = "amul";
        return itemtypes.find(code);
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
        if (auto it = armor.find(equipCode))
            return it;
        if (auto it = weapons.find(equipCode))
            return it;
        if (auto it = misc.find(equipCode))
            return it;
        assert(false);
        return nullptr;
    };

    for (auto& row : setitems.rows) {
        if (!row.isValid())
            continue;

        auto* parent_set = sets.index.at(Mernel::strToLower(row.setIndex));
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

    for (auto& row : cubemain.rows) {
        if (!row.isCrafted() || !row.inputs.size())
            continue;

        auto input1 = row.inputs[0];
        if (input1.starts_with('"'))
            input1 = input1.substr(1);
        auto pos = input1.find(',');
        if (pos != std::string::npos)
            input1 = input1.substr(0, pos);
        input1 = Mernel::strToLower(input1);

        row.itemType = getItemType(input1);
        if (!row.itemType) {
            row.eqipmentType = findEquip(input1);

        } else {
            auto slot = row.itemType->determineSlot();
            assert(slot != SlotType::None);
        }
    }
    for (auto& row : levels.rows) {
        if (!row.name.starts_with("Act "))
            continue;
        if (row.name == "Act 5 - Mountain Top")
            continue;
        if (row.name.starts_with("Act 5 - Pandemonium"))
            continue;
        if (row.mon.empty() || row.umon.empty() || !row.numMon.has_value())
            continue;
        if (!row.hasSpawns())
            continue;
        row.useForRando = true;
        for (auto& id : row.mon) {
            if (auto idx = monstats.findN(id); idx != size_t(-1))
                monstats.rows[idx].spawnedAsNormal = true;
        }
        for (auto& id : row.nmon) {
            if (auto idx = monstats.findN(id); idx != size_t(-1))
                monstats.rows[idx].spawnedAsNormal = true;
        }
        for (auto& id : row.umon) {
            if (auto idx = monstats.findN(id); idx != size_t(-1))
                monstats.rows[idx].spawnedAsElite = true;
        }
    }

    bool hasRegular = false;
    for (auto& row : monstats.rows) {
        if (row.spawnedAsNormal) {
            for (auto& minionId : row.minionIds) {
                if (minionId.empty() || minionId == row.id)
                    continue;

                auto n = monstats.findN(minionId);
                if (n != size_t(-1))
                    monstats.rows[n].spawnedAsNormal = true;
            }
        }
    }
    for (auto& row : monstats.rows) {
        row.isRegular = row.isEnabled() && row.isSpawn() && row.isKillable() && !row.isNpc() && !row.isBoss() && row.level.has_value();
        row.isRegular = row.isRegular && monstatsEx.index.contains(row.monStatsEx);

        row.useForRando = row.isRegular && row.spawnedAsNormal;
        hasRegular      = hasRegular || row.isRegular;
    }
    assert(hasRegular);

    return true;
}

template<class Row>
bool DataContext::parseTable(DataTable<Row>& table, const IStorage::StoredData& data)
{
    const TableId tableId = Row::s_tableId;
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
bool DataContext::serializeTable(const DataTable<Row>& table, IStorage::StoredData& data, bool full) const
{
    if (!table.valid)
        return true;

    const TableId tableId = Row::s_tableId;
    std::string&  buffer  = data.tables[tableId];
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
    if (csv.rows.empty()) {
        buffer = {};
        return true;
    }

    return Mernel::writeCSVToBuffer(buffer, csv);
}

template<class Row>
void DataContext::diffTable(const DataTable<Row>& tableOlder, const DataTable<Row>& tableNewer, DataTable<Row>& result) const
{
    for (size_t i = 0, cnt = tableNewer.rows.size(); i < cnt; ++i) {
        if (i >= tableOlder.rows.size()) {
            result.rows.push_back(tableNewer.rows[i]);
            continue;
        }

        auto olderRow = tableOlder.rows[i];
        auto newerRow = tableNewer.rows[i];
        olderRow._ser_usedCells.resize(tableOlder.indicesUsed.size());
        olderRow._ser_read      = false;
        olderRow._ser_cellIndex = 0;
        olderRow._ser_key.reset();
        olderRow.serialize();

        newerRow._ser_usedCells.resize(tableOlder.indicesUsed.size());
        newerRow._ser_read      = false;
        newerRow._ser_cellIndex = 0;
        newerRow._ser_key.reset();
        newerRow.serialize();

        if (olderRow._ser_usedCells == newerRow._ser_usedCells)
            continue;
        result.rows.push_back(newerRow);
    }
}

}
