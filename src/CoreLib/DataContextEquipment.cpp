/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

#include "DataContextEquipment.hpp"

namespace D2ModGen {

// =========================================================

SlotType DataRow_itemtypes::determineSlot() const
{
    if (code == "shld" || code == "pala")
        return SlotType::MainArmor;
    if (code == "weap")
        return SlotType::Weapon;
    if (code == "armo")
        return SlotType::OtherArmor;
    if (code == "pvpm" || code == "map" || code == "t4m" || code == "t5m")
        return SlotType::PD2Map;
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
        if (parentType != SlotType::None)
            return parentType;
    }

    return SlotType::None;
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
        flags = {};
        if (throwable.value_or(false))
            flags.insert(AttributeFlag::Throwable), flags.insert(AttributeFlag::ThrowableOrBow);
        else if (repair.value_or(false))
            flags.insert(AttributeFlag::Durability);

        if (sockets[0].value_or(0))
            flags.insert(AttributeFlag::Sockets);
        if (!shoots.empty())
            flags.insert(AttributeFlag::Bow), flags.insert(AttributeFlag::ThrowableOrBow);
        if (itemType.starts_with("Map"))
            flags.insert(AttributeFlag::PD2Map);
    }
}

// =========================================================

std::vector<std::string> DataRow_weapons::getColumns() const
{
    // clang-format off
    return { "name", "level", "levelreq", "reqstr", "reqdex", "showlevel", "code", "type",
        "normcode", "ubercode", "ultracode",  "mindam", "maxdam",  };
    // clang-format on
}

void DataRow_weapons::serialize()
{
    ser(name);
    ser(lvl);
    ser(lvlreq);
    ser(reqstr);
    ser(reqdex);
    ser(showLevel);
    ser(code);
    ser(itemTypeCode);

    ser(normcode);
    ser(ubercode);
    ser(ultracode);

    ser(mindam);
    ser(maxdam);
    category = EquipmentCategory::Weapon;
}

std::vector<std::string> DataRow_armor::getColumns() const
{
    // clang-format off
    return { "name", "level", "levelreq", "reqstr", "reqdex", "showlevel", "code", "type",
        "normcode", "ubercode", "ultracode", "minac", "maxac",  };
    // clang-format on
}
void DataRow_armor::serialize()
{
    ser(name);
    ser(lvl);
    ser(lvlreq);
    ser(reqstr);
    ser(reqdex);
    ser(showLevel);
    ser(code);
    ser(itemTypeCode);

    ser(normcode);
    ser(ubercode);
    ser(ultracode);

    ser(minac);
    ser(maxac);
    category = EquipmentCategory::Armor;
}

std::vector<std::string> DataRow_misc::getColumns() const
{
    // clang-format off
    return { "name", "code", "type", "levelreq", "level",
             "maxstack" };
    // clang-format on
}

void DataRow_misc::serialize()
{
    ser(name);
    ser(code);
    ser(itemTypeCode);
    ser(lvlreq);
    ser(lvl);

    ser(maxstack);
}

// =========================================================

std::map<std::string, std::string> RowBaseUniqueSet::getColumnAliases() const
{
    return { { "enabled", "spawnable" }, { "level", "lvl" }, { "item", "code" } };
}

std::vector<std::string> RowBaseUniqueSet::getColumns() const
{
    std::vector<std::string> result{
        "index",
        "code",

        "rarity",
        "lvl",
        "lvl req",
        "spawnable",
        "disablechronicle",

        "cost mult",
        "cost add",

        "*id",
        "*itemname",
        "chrtransform",
        "invtransform",
        "flippyfile",

        "invfile",
        "dropsound",
        "dropsfxframe",
        "usesound",
        "*eol",
    };
    return result;
}

void RowBaseUniqueSet::serialize()
{
    ser(index);
    ser(eqipmentCode);

    ser(rarity);
    ser(lvl);
    ser(lvlreq);
    ser(spawnable, true);
    ser(disableChronicle);

    ser(cost_mul);
    ser(cost_add);

    ser(ux.id);
    ser(ux.itemName);
    ser(ux.chrtransform);
    ser(ux.invtransform);
    ser(ux.flippyfile);

    ser(ux.invfile);
    ser(ux.dropsound);
    ser(ux.dropsfxframe);
    ser(ux.usesound);
    ser(ux.eol);

    if (_ser_read) {
        displayName = index;
    }
    _ser_key = displayName;
}

std::vector<std::string> DataRow_uniqueitems::getColumns() const
{
    std::vector<std::string> result = RowBaseUniqueSet::getColumns();
    result.push_back("carry1");
    result.push_back("version");

    for (size_t i = 0; i < affixes.getLimit(); ++i) {
        auto istr = std::to_string(i + 1);
        result.push_back("prop" + istr);
        result.push_back("par" + istr);
        result.push_back("min" + istr);
        result.push_back("max" + istr);
    }
    return result;
}

void DataRow_uniqueitems::serialize()
{
    RowBaseUniqueSet::serialize();

    ser(carry1);
    ser(version);

    ser(affixes);
}

std::vector<std::string> DataRow_setitems::getColumns() const
{
    std::vector<std::string> result = RowBaseUniqueSet::getColumns();
    result.push_back("add func");
    result.push_back("set");

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
void DataRow_setitems::serialize()
{
    RowBaseUniqueSet::serialize();
    ser(addfunc);
    ser(setIndex);

    for (auto* affixList : affixLists())
        ser(*affixList);
}

// =========================================================

std::vector<std::string> DataRow_sets::getColumns() const
{
    std::vector<std::string> result{ "index", "name", "uiclass", "version" };
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
    ser(uiClass);
    ser(version);
    for (auto* affixList : affixLists())
        ser(*affixList);
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

}
