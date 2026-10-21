/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "DataContext.hpp"
#include "Logger.hpp"
#include "MernelPlatform/FileFormatCSV.hpp"
#include "MernelPlatform/FileFormatJson.hpp"
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

    itemtypes.makeIndex(&DataRow_itemtypes::code);
    sets.makeIndex(&DataRow_sets::index);
    armor.makeIndex(&RowBaseEquipment::code);
    weapons.makeIndex(&RowBaseEquipment::code);
    misc.makeIndex(&RowBaseEquipment::code);

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

        row.parent_set   = sets.index.at(row.setIndex);
        row.eqipmentType = findEquip(row.eqipmentCode);

        const_cast<DataRow_sets*>(row.parent_set)->level = row.getLevel();
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

    const std::map<std::string, Character> classes{

        { "ama", Character::ama },
        { "sor", Character::sor },
        { "nec", Character::nec },
        { "pal", Character::pal },
        { "bar", Character::bar },
        { "dru", Character::dru },
        { "ass", Character::ass },
        { "war", Character::war },
    };

    const std::set<std::string> casterCodes{ "wand", "staf", "orb", "head" };
    const std::set<std::string> brawlCodes{ "h2h", "bow", "xbow", "miss", "thro" };

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

        if (!row.clas.empty()) {
            auto it = classes.find(row.clas);
            if (it != classes.cend())
                row.classSpecific = it->second;
        }

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

    for (const auto& [id, fileData] : data.jsonFiles) {
        Mernel::PropertyTree doc;
        Logger() << "parse json: " << std::string(getJsonRelPath(id));
        if (!readJsonFromBufferNoexcept(fileData, doc)) {
            Logger(Logger::Warning) << "failed to parse json: " << getJsonRelPath(id);
            return false;
        }
        jsonFiles[id] = std::move(doc);
    }

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

template<class Row>
bool DataContext::parseTable(DataTable<Row>& table, TableId tableId, const IStorage::StoredData& data)
{
    if (!data.tables.contains(tableId))
        return true;
    const std::string& buffer = data.tables.at(tableId);

    Mernel::CSVTable csv;
    if (!Mernel::readCSVFromBuffer(buffer, csv)) {
        Logger(Logger::Warning) << "failed to parse csv:" << getTableIdString(tableId);
        return false;
    }
    table.columnsOriginal                                = csv.columns;
    table.valid                                          = true;
    const std::vector<std::string>           usedColumns = Row{}.getColumns();
    const std::map<std::string, std::string> aliases     = Row{}.getColumnAliases();
    table.indicesUsed.resize(usedColumns.size(), size_t(-1));

    for (size_t i = 0; i < table.columnsOriginal.size(); i++) {
        std::string col = Mernel::strToLower(table.columnsOriginal[i]);
        if (auto it = aliases.find(col); it != aliases.cend())
            col = it->second;

        auto it = std::find(usedColumns.cbegin(), usedColumns.cend(), col);
        if (it == usedColumns.cend()) {
            table.indicesUnused.push_back(i);
        } else {
            auto usedIndex               = std::distance(usedColumns.cbegin(), it);
            table.indicesUsed[usedIndex] = i;
            table.columnsUsed.push_back(col);
        }
    }
    for (size_t i = 0; i < table.indicesUsed.size(); i++) {
        if (table.indicesUsed[i] == size_t(-1)) {
            Logger(Logger::Warning) << "Missing column '" << usedColumns[i] << "' in table '" << getTableIdString(tableId) << "', generation may be incomplete.";
        }
    }

    for (auto& rowCsv : csv.rows) {
        Row row;
        row._ser_unusedCells.resize(table.indicesUnused.size());
        row._ser_usedCells.resize(table.indicesUsed.size());

        for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
            row._ser_unusedCells[i] = rowCsv.data[table.indicesUnused[i]].str;
        }
        for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
            if (table.indicesUsed[i] != size_t(-1))
                row._ser_usedCells[i] = rowCsv.data[table.indicesUsed[i]].str;
        }
        row._ser_read      = true;
        row._ser_cellIndex = 0;
        row.serialize();
        row._ser_usedCells.clear();
        table.rows.push_back(std::move(row));
    }

    Logger() << "parse '" << getTableIdString(tableId) << "' buffer[" << buffer.size() << "] => csv[" << csv.columns.size() << "x" << csv.rows.size() << "]";

    return true;
}

template<class Row>
bool DataContext::serializeTable(const DataTable<Row>& table, TableId tableId, IStorage::StoredData& data, bool full) const
{
    if (!table.valid)
        return true;

    std::string& buffer = data.tables[tableId];

    Mernel::CSVTable csv;
    csv.columns    = full ? table.columnsOriginal : table.columnsUsed;
    csv.endsWithNL = true;

    for (auto row : table.rows) {
        row._ser_usedCells.resize(table.indicesUsed.size());
        row._ser_read      = false;
        row._ser_cellIndex = 0;
        row.serialize();
        Mernel::CSVTableRow rowCsv;
        rowCsv.data.resize(csv.columns.size());

        if (full) {
            for (size_t i = 0; i < row._ser_unusedCells.size(); ++i) {
                rowCsv.data[table.indicesUnused[i]].str = row._ser_unusedCells[i];
            }
            for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
                if (table.indicesUsed[i] != size_t(-1))
                    rowCsv.data[table.indicesUsed[i]].str = row._ser_usedCells[i];
            }
        } else {
            std::map<size_t, std::string> values;
            for (size_t i = 0; i < row._ser_usedCells.size(); ++i) {
                if (table.indicesUsed[i] != size_t(-1))
                    values[table.indicesUsed[i]] = row._ser_usedCells[i];
            }

            for (size_t i = 0; auto& [_, value] : values)
                rowCsv.data[i++].str = value;
        }
        csv.rows.push_back(std::move(rowCsv));
    }

    if (!Mernel::writeCSVToBuffer(buffer, csv))
        return false;

    if (full)
        Logger() << "serialize '" << getTableIdString(tableId) << "' csv[" << csv.columns.size() << "x" << csv.rows.size() << "] => buffer[" << buffer.size() << "] ";

    return true;
}

std::vector<std::string> RowBaseAffixes::getColumns() const
{
    std::vector<std::string> result{ "name", "level", "maxlevel", "levelreq", "spawnable" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("mod" + istr + "code");
        result.push_back("mod" + istr + "param");
        result.push_back("mod" + istr + "min");
        result.push_back("mod" + istr + "max");
    }
    for (size_t i = 0; i < 7; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("itype" + istr);
    }
    for (size_t i = 0; i < 5; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("etype" + istr);
    }

    return result;
}

void RowBaseAffixes::serialize()
{
    ser(name);
    ser(lvl);
    ser(maxlevel);
    ser(lvlreq);
    ser(spawnable);
    ser(affixes);
    ser(itype, 7);
    ser(etype, 5);
}

std::vector<std::string> DataRow_weapons::getColumns() const
{
    // clang-format off
    return { "level", "levelreq", "reqstr", "reqdex", "showlevel", "code", "type",
            "mindam", "maxdam",  };
    // clang-format on
}

void DataRow_weapons::serialize()
{
    ser(lvl);
    ser(lvlreq);
    ser(reqstr);
    ser(reqdex);
    ser(showLevel);
    ser(code);
    ser(itemTypeCode);

    ser(mindam);
    ser(maxdam);
}

std::vector<std::string> DataRow_armor::getColumns() const
{
    // clang-format off
    return { "level", "levelreq", "reqstr", "reqdex", "showlevel", "code", "type",
            "minac", "maxac",  };
    // clang-format on
}

void DataRow_armor::serialize()
{
    ser(lvl);
    ser(lvlreq);
    ser(reqstr);
    ser(reqdex);
    ser(showLevel);
    ser(code);
    ser(itemTypeCode);

    ser(minac);
    ser(maxac);
}

std::vector<std::string> DataRow_charstats::getColumns() const
{
    std::vector<std::string> result{ "class", "statperlevel", "skillsperlevel" };
    for (size_t i = 0; i < maxItems; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("item" + istr + "count");
        result.push_back("item" + istr);
        result.push_back("item" + istr + "loc");
    }
    for (size_t i = 0; i < maxSkills; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("skill " + istr);
    }

    return result;
}

void DataRow_charstats::serialize()
{
    ser(clas);
    ser(statPerLevel);
    ser(skillsPerLevel);
    serArray(items, maxItems, [this](Item& item) {
        ser(item.count);
        ser(item.code);
        ser(item.loc); }, [](Item& item) { return !item.code.empty(); });

    ser(skills, maxSkills);
}

std::vector<std::string> DataRow_cubemain::getColumns() const
{
    std::vector<std::string> result{
        "description",
        "enabled",
        "version",
        "op",
        "*eol",
        "numinputs",
        "output",
        "ilvl",
        "plvl",
        "lvl",
    };
    using namespace std::literals;
    for (const auto& a : { ""s, "b "s, "c "s }) {
        for (size_t i = 0; i < affixes_a.getLimit(); ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(a + "mod " + istr);
            result.push_back(a + "mod " + istr + " param");
            result.push_back(a + "mod " + istr + " min");
            result.push_back(a + "mod " + istr + " max");
            result.push_back(a + "mod " + istr + " chance");
        }
    }

    for (size_t i = 0; i < maxInputs; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("input " + istr);
    }

    return result;
}

void DataRow_cubemain::serialize()
{
    ser(description);
    ser(enabled);
    ser(version);
    ser(op);
    ser(eol);
    ser(numinputs);
    ser(output);
    ser(ilvl);
    ser(plvl);
    ser(lvl);

    for (AffixPropList* list : affixLists()) {
        ser(*list, true);
    }
    ser(inputs, maxInputs);
}

std::vector<std::string> DataRow_difficultylevels::getColumns() const
{
    return {
        "name",
        "resistpenalty",
        "gamblerare",
        "gambleset",
        "gambleunique",
        "gambleuber",
        "gambleultra",
    };
}

void DataRow_difficultylevels::serialize()
{
    ser(name);
    ser(resistPenalty);

    ser(gambleRare);
    ser(gambleSet);
    ser(gambleUnique);
    ser(gambleUber);
    ser(gambleUltra);
}

std::vector<std::string> DataRow_experience::getColumns() const
{
    return { "expratio" };
}

void DataRow_experience::serialize()
{
    ser(expRatio);
}

std::vector<std::string> DataRow_gamble::getColumns() const
{
    return { "name", "code" };
}

void DataRow_gamble::serialize()
{
    ser(name);
    ser(code);
}

std::vector<std::string> DataRow_gems::getColumns() const
{
    std::vector<std::string> result{ "code" };

    using namespace std::literals;
    for (const auto& a : { "weapon"s, "helm"s, "shield"s }) {
        for (size_t i = 0; i < affixes_weapon.getLimit(); ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(a + "mod" + istr + "code");
            result.push_back(a + "mod" + istr + "param");
            result.push_back(a + "mod" + istr + "min");
            result.push_back(a + "mod" + istr + "max");
        }
    }

    return result;
}

void DataRow_gems::serialize()
{
    ser(eqipmentCode);
    for (AffixPropList* list : affixLists()) {
        ser(*list);
    }
}

std::vector<std::string> DataRow_hireling::getColumns() const
{
    return { "hp", "hp/lvl", "dmg-min", "dmg-max", "dmg/lvl" };
}

void DataRow_hireling::serialize()
{
    ser(hp);
    ser(hp_lvl);
    ser(dmg_min);
    ser(dmg_max);
    ser(dmg_lvl);
}

std::vector<std::string> DataRow_itemstatcost::getColumns() const
{
    return { "multiply", "add" };
}

void DataRow_itemstatcost::serialize()
{
    ser(multiply);
    ser(add);
}

SlotType DataRow_itemtypes::determineSlot() const
{
    if (code == "shld" || code == "pala")
        return SlotType::MainArmor;
    if (code == "weap")
        return SlotType::Weapon;
    if (code == "armo")
        return SlotType::OtherArmor;
    if (bodyLoc1 == "head" || bodyLoc1 == "tors")
        return SlotType::MainArmor;
    if (bodyLoc1 == "feet" || bodyLoc1 == "glov" || bodyLoc1 == "belt")
        return SlotType::OtherArmor;
    if (bodyLoc1 == "rrin")
        return SlotType::Ring;
    if (bodyLoc1 == "neck")
        return SlotType::Amulet;
    if (bodyLoc1 == "rarm")
        return SlotType::Weapon;

    if (equiv1 == "char")
        return SlotType::Charm;
    if (equiv1 == "gem" || equiv1 == "sock")
        return SlotType::Socketable;
    if (equiv1 == "SET")
        return SlotType::Set;

    for (auto* parent : parents) {
        SlotType parentType = parent->determineSlot();
        if (parentType != SlotType::Undefined)
            return parentType;
    }

    return SlotType::Undefined;
}

std::vector<std::string> DataRow_itemtypes::getColumns() const
{
    return { "code", "class", "itemtype", "equiv1", "equiv2", "bodyloc1", "throwable", "repair", "shoots", "maxsockets1", "maxsockets2", "maxsockets3" };
}

std::map<std::string, std::string> DataRow_itemtypes::getColumnAliases() const
{
    return {
        { "maxsock1", "maxsockets1" },
        { "maxsock25", "maxsockets2" },
        { "maxsock40", "maxsockets3" },
    };
}

void DataRow_itemtypes::serialize()
{
    ser(code);
    ser(clas);
    ser(itemType);
    ser(equiv1);
    ser(equiv2);
    ser(bodyLoc1);

    ser(throwable);
    ser(repair);
    ser(shoots);
    ser(sockets);

    if (_ser_read) {
        flags.clear();
        if (throwable.value_or(false))
            flags.insert(AttributeFlag::Quantity);
        else if (repair.value_or(false))
            flags.insert(AttributeFlag::Durability);

        if (sockets[0].value_or(0))
            flags.insert(AttributeFlag::Sockets);
        if (!shoots.empty())
            flags.insert(AttributeFlag::Missile);
        if (itemType.starts_with("Map"))
            flags.insert(AttributeFlag::PD2Map);
    }
}

std::vector<std::string> DataRow_levels::getColumns() const
{
    std::vector<std::string> result{
        // clang-format off
        "name", "nummon",
        "monumin"  , "monumin(n)"  , "monumin(h)",
        "monumax"  , "monumax(n)"  , "monumax(h)",
        "monden"   , "monden(n)"   , "monden(h)" ,
        "monlvlex" , "monlvlex(n)" , "monlvlex(h)",
        // clang-format on
    };
    using namespace std::literals;
    for (const auto& colTpl : { "mon"s, "nmon"s, "umon"s }) {
        for (size_t i = 0; i < maxTypes; ++i) {
            auto istr = std::to_string(i + 1);
            result.push_back(colTpl + istr);
        }
    }
    return result;
}

std::map<std::string, std::string> DataRow_levels::getColumnAliases() const
{
    return {
        { "monlvl1ex", "monlvlex" },
        { "monlvl2ex", "monlvlex(n)" },
        { "monlvl3ex", "monlvlex(h)" },
    };
}

void DataRow_levels::serialize()
{
    ser(name);
    ser(numMon);
    ser(monUMin);
    ser(monUMax);
    ser(monDen);
    ser(monLvlEx);

    ser(mon, maxTypes);
    ser(nmon, maxTypes);
    ser(umon, maxTypes);
}

std::vector<std::string> DataRow_misc::getColumns() const
{
    // clang-format off
    return { "code", "type", "levelreq", "level",
             "maxstack" };
    // clang-format on
}

void DataRow_misc::serialize()
{
    ser(code);
    ser(itemTypeCode);
    ser(lvlreq);
    ser(lvl);

    ser(maxstack);
}

std::vector<std::string> DataRow_monlvl::getColumns() const
{
    return {
        // clang-format off
         "th", "th(n)", "th(h)", "l-th", "l-th(n)", "l-th(h)",
         "ac", "ac(n)", "ac(h)", "l-ac", "l-ac(n)", "l-ac(h)",
         "hp", "hp(n)", "hp(h)", "l-hp", "l-hp(n)", "l-hp(h)",
         "dm", "dm(n)", "dm(h)", "l-dm", "l-dm(n)", "l-dm(h)",
         "xp", "xp(n)", "xp(h)", "l-xp", "l-xp(n)", "l-xp(h)",
        // clang-format on
        "level",
    };
}

void DataRow_monlvl::serialize()
{
    ser(TH);
    ser(AC);
    ser(HP);
    ser(DM);
    ser(XP);

    ser(level);
}

std::vector<std::string> DataRow_monstats::getColumns() const
{
    using namespace std::literals;
    std::vector<std::string>         result;
    const std::array<std::string, 3> diffs{ ""s, "(n)"s, "(h)"s };

    for (const std::string& code : { "resfi"s, "resli"s, "resco"s, "respo"s, "resma"s, "resdm"s }) {
        for (const auto& diffSuffix : diffs) {
            result.push_back(code + diffSuffix);
        }
    }

    for (const std::string& code : {
             // clang-format off
             "treasureclass"s, "treasureclasschamp"s , "treasureclassunique"s , "treasureclassquest"s,
            // "treasureclassdesecrated"s, "treasureclassdesecratedchamp"s , "treasureclassdesecratedunique"s , "treasureclassherald"s,
             // clang-format on
         }) {
        for (const auto& diffSuffix : diffs) {
            result.push_back(code + diffSuffix);
        }
    }

    for (size_t i = 0; i < skCount; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("sk" + istr + "lvl");
    }

    std::vector<std::string> rest{
        // clang-format off
        "spawn", "minion1", "minion2",
        "isspawn", "killable", "npc", "boss",
        "level", "mingrp", "maxgrp",
        "id", "baseid", "*hcidx",
        // clang-format on
    };
    result.insert(result.end(), rest.cbegin(), rest.cend());

    return result;
}

void DataRow_monstats::serialize()
{
    ser(resFi);
    ser(resLi);
    ser(resCo);
    ser(resPo);
    ser(resMa);
    ser(resDm);

    ser(tc);
    ser(skLevels, skCount);
    ser(minionIds, minionCount);

    ser(spawn);
    ser(killable);
    ser(npc);
    ser(boss);

    ser(level);
    ser(minGrp);
    ser(maxGrp);

    ser(id);
    ser(baseId);
    ser(hcIdx);
}

std::map<std::string, std::string> DataRow_monstats::getColumnAliases() const
{
    using namespace std::literals;
    std::map<std::string, std::string> result;
    const std::array<std::string, 3>   diffs{ ""s, "(n)"s, "(h)"s };
    std::vector<std::string>           tcs{ "treasureclass"s, "treasureclasschamp"s, "treasureclassunique"s, "treasureclassquest"s };
    for (size_t i = 0; i < 4; ++i) {
        for (auto& diff : diffs) {
            std::string src  = "treasureclass"s + std::to_string(i + 1) + diff;
            std::string dest = tcs[i] + diff;
            result[src]      = dest;
        }
    }
    result["hcidx"] = "*hcidx";
    return result;
}

std::vector<std::string> DataRow_runes::getColumns() const
{
    std::vector<std::string> result{ "complete", "*rune name" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("t1code" + istr);
        result.push_back("t1param" + istr);
        result.push_back("t1min" + istr);
        result.push_back("t1max" + istr);
    }
    for (size_t i = 0; i < maxRunes; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("rune" + istr);
    }
    for (size_t i = 0; i < maxItype; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("itype" + istr);
    }
    for (size_t i = 0; i < maxEtype; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("etype" + istr);
    }
    return result;
}

std::map<std::string, std::string> DataRow_runes::getColumnAliases() const
{
    return { { "rune name", "*rune name" } };
}

void DataRow_runes::serialize()
{
    ser(complete);
    ser(name);
    ser(affixes);
    ser(runeCodes, maxRunes);
    ser(itype, maxItype);
    ser(etype, maxEtype);
}

std::vector<std::string> DataRow_setitems::getColumns() const
{
    std::vector<std::string> result{ "lvl", "lvl req", "index", "set", "item", "add func", "disablechronicle" };
    for (size_t i = 0; i < affixes_base.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prop" + istr);
        result.push_back("par" + istr);
        result.push_back("min" + istr);
        result.push_back("max" + istr);
    }
    for (size_t i = 0; i < affixes_partial1.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("aprop" + istr + "a");
        result.push_back("apar" + istr + "a");
        result.push_back("amin" + istr + "a");
        result.push_back("amax" + istr + "a");
    }
    for (size_t i = 0; i < affixes_partial2.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("aprop" + istr + "b");
        result.push_back("apar" + istr + "b");
        result.push_back("amin" + istr + "b");
        result.push_back("amax" + istr + "b");
    }

    return result;
}

std::map<std::string, std::string> DataRow_setitems::getColumnAliases() const
{
    return { { "level", "lvl" } };
}

void DataRow_setitems::serialize()
{
    ser(lvl);
    ser(lvlreq);

    ser(index);
    ser(setIndex);
    ser(eqipmentCode);
    ser(addfunc);
    ser(disableChronicle);

    for (auto* affixList : affixLists())
        ser(*affixList);
}

std::vector<std::string> DataRow_sets::getColumns() const
{
    std::vector<std::string> result{ "index", "name" };
    for (size_t i = 0; i < affixes_full.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("fcode" + istr);
        result.push_back("fparam" + istr);
        result.push_back("fmin" + istr);
        result.push_back("fmax" + istr);
    }
    for (size_t i = 0; i < affixes_partial1.getLimit(); ++i) {
        auto istr = std::to_string(i + 2);
        result.push_back("pcode" + istr + "a");
        result.push_back("pparam" + istr + "a");
        result.push_back("pmin" + istr + "a");
        result.push_back("pmax" + istr + "a");
    }
    for (size_t i = 0; i < affixes_partial2.getLimit(); ++i) {
        auto istr = std::to_string(i + 2);
        result.push_back("pcode" + istr + "b");
        result.push_back("pparam" + istr + "b");
        result.push_back("pmin" + istr + "b");
        result.push_back("pmax" + istr + "b");
    }

    return result;
}

void DataRow_sets::serialize()
{
    ser(index);
    ser(name);
    for (auto* affixList : affixLists())
        ser(*affixList);
}

std::vector<std::string> DataRow_skilldesc::getColumns() const
{
    return {
        "skilldesc",
        "skillpage",
        "skillrow",
        "skillcolumn",
        "listrow",
    };
}

void DataRow_skilldesc::serialize()
{
    ser(skilldesc);
    ser(skillPage);
    ser(skillRow);
    ser(skillColumn);
    ser(listRow);
}

std::vector<std::string> DataRow_skills::getColumns() const
{
    // clang-format off
    return { "skill", "maxlvl", "reqlevel", "intown", "cost mult", "cost add" ,
             "elen", "hitshift", "etype", "charclass",
             "reqskill1", "reqskill2", "reqskill3"};
    // clang-format on
}

void DataRow_skills::serialize()
{
    ser(skill);
    ser(maxlvl);
    ser(lvlreq);
    ser(inTown);
    ser(costMult);
    ser(costAdd);

    ser(eLen);
    ser(hitShift);
    ser(eType);
    ser(charclass);

    ser(reqskills, 3);
}

std::vector<std::string> DataRow_treasureclassex::getColumns() const
{
    std::vector<std::string> result{
        "treasure class",
        "unique",
        "set",
        "rare",
        "magic",
        "nodrop",
        "group",
        "picks",
        "level",
    };
    for (size_t i = 0; i < maxItems; ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prob" + istr);
        result.push_back("item" + istr);
    }

    return result;
}

void DataRow_treasureclassex::serialize()
{
    ser(name);
    ser(k_unique);
    ser(k_set);
    ser(k_rare);
    ser(k_magic);
    ser(noDrop);
    ser(group);
    ser(picks);
    ser(level);

    serArray(items, maxItems, [this](Item& item) {
        ser(item.prob);
        ser(item.tc); }, [](Item& item) { return !item.tc.empty(); });
}

int DataRow_treasureclassex::getDropSomethingProb() const
{
    int totalOther = 0;
    for (const auto& item : items)
        totalOther += item.prob.value_or(0);
    return totalOther;
}

std::vector<std::string> DataRow_uniqueitems::getColumns() const
{
    std::vector<std::string> result{ "rarity", "lvl", "lvl req", "carry1", "spawnable", "disablechronicle", "code", "index" };
    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prop" + istr);
        result.push_back("par" + istr);
        result.push_back("min" + istr);
        result.push_back("max" + istr);
    }
    return result;
}

std::map<std::string, std::string> DataRow_uniqueitems::getColumnAliases() const
{
    return { { "enabled", "spawnable" } };
}

void DataRow_uniqueitems::serialize()
{
    ser(rarity);
    ser(lvl);
    ser(lvlreq);
    ser(carry1);
    ser(spawnable);
    ser(disableChronicle);
    ser(eqipmentCode);
    ser(index);
    ser(affixes);
}

void RowBase::ser(int& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data = 0;
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        str = std::to_string(data);
    }
}

void RowBase::ser(std::optional<int>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string(data.value());
        else
            str = {};
    }
}

void RowBase::ser(std::optional<bool>& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        if (str.empty()) {
            data.reset();
        } else {
            data = std::atoi(str.c_str());
        }
    } else {
        if (data.has_value())
            str = std::to_string((int) data.value());
        else
            str = {};
    }
}

void RowBase::ser(std::string& data)
{
    auto& str = _ser_usedCells[_ser_cellIndex++];
    if (_ser_read) {
        data = str;
    } else {
        str = data;
    }
}

AttributeFlagSet RowBaseEquipment::getAttributes() const
{
    return itemType ? itemType->flags : AttributeFlagSet{};
}

Character ItemCodeList::determineClass() const
{
    for (ItemType type : *this)
        if (type->classSpecific != Character::none)
            return type->classSpecific;

    return Character::none;
}

SlotType ItemCodeList::determineSlot() const
{
    for (ItemType type : *this) {
        auto slot = type->determineSlot();
        if (slot != SlotType::Undefined)
            return slot;
    }

    return SlotType::Undefined;
}

AttributeIdiom ItemCodeList::determineIdiom() const
{
    for (ItemType type : *this) {
        auto slot = type->determineIdiom();
        if (slot != AttributeIdiom::Any)
            return slot;
    }

    return AttributeIdiom::Any;
}

std::string ItemCodeList::toString() const
{
    std::string res = "";
    for (ItemType type : *this) {
        res += type->code + ",";
    }
    return res;
}

}
