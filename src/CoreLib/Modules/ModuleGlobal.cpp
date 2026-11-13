/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleGlobal.hpp"

#include "ModuleRandoItems.hpp"
#include "ModuleRandoSkills.hpp"
#include "ModuleRandoMonsters.hpp"

#include "ModuleBalanceCharacter.hpp"
#include "ModuleBalanceGambling.hpp"
#include "ModuleBalanceItems.hpp"
#include "ModuleBalanceMaps.hpp"
#include "ModuleBalanceMonsters.hpp"
#include "ModuleBalanceSkills.hpp"

#include "ModuleDroprate.hpp"

#include "ModuleOtherExpandedInventory.hpp"
#include "ModuleOtherHoradricCube.hpp"
#include "ModuleOtherQualityOfLife.hpp"

#include "TblParser.hpp"

#include "Version.hpp"

#include "Logger.hpp"

namespace D2ModGen {

ModuleGlobal::ModuleGlobal(DataContext& output_, const Config& config_, const GenerationEnvironment& env_)
    : AbstractModule(output_, config_, env_)
{
    reset();
}

void ModuleGlobal::reset()
{
    // light modules
    create<ModuleBalanceCharacter>("BalanceCharacter");
    create<ModuleBalanceGambling>("BalanceGambling");
    create<ModuleBalanceMonsters>("BalanceMonsters");
    create<ModuleBalanceMaps>("BalanceMaps");

    create<ModuleDroprate>("Droprate");
    create<ModuleOtherQualityOfLife>("OtherQualityOfLife");
    create<ModuleOtherExpandedInventory>("OtherExpandedInventory");

    // modules that can add more rows, thus in the end
    create<ModuleOtherHoradricCube>("OtherHoradricCube");
    create<ModuleRandoItems>("RandoItems");
    create<ModuleRandoMonsters>("RandoMonsters");
    create<ModuleRandoSkills>("RandoSkills");

    // item balance affect possible input (item stat ranges) for randomizer, that's why it is in the end.
    create<ModuleBalanceItems>("BalanceItems");
    // same for this coming after rando.
    create<ModuleBalanceSkills>("BalanceSkills");
}

void ModuleGlobal::generate()
{
    for (auto& module : m_modules) {
        Logger() << "start module: " << module.m_id;
        try {
            module.m_gen->generate();
        }
        catch (const std::exception& ex) {
            std::string msg("module '" + module.m_id + "': " + std::string(ex.what()));

            throw std::runtime_error(msg);
        }
    }

    auto replaceKey = [](Mernel::PropertyTreeMap& jsonObject, const std::string& sourceId, const std::string& newId) -> bool {
        if (!jsonObject.contains(sourceId))
            return false;
        const std::string modelName = jsonObject[sourceId].getScalar().toString();
        assert(!modelName.empty());
        jsonObject[newId] = Mernel::PropertyTreeScalar{ modelName };
        return true;
    };
    auto underscoreBeforeLast = [](std::string s) {
        char last = s.back();
        s.pop_back();
        s += "_";
        s += last;
        return s;
    };
    auto underscored = [](std::string s) {
        std::replace(s.begin(), s.end(), ' ', '_');
        std::replace(s.begin(), s.end(), '-', '_');
        return s;
    };

    auto doReplace = [&replaceKey, &underscoreBeforeLast, &underscored](const DataContext::IdCloneList& cloneList, Mernel::PropertyTree& jsonDoc, std::string title) {
        Mernel::PropertyTreeMap& jsonObject = jsonDoc.getMap();
        Logger() << "Start updating " << title << ", existing keys: " << jsonObject.size() << ", clone list size:" << cloneList.size();
        for (const auto& [sourceId, newId] : cloneList) {
            if (replaceKey(jsonObject, sourceId, newId))
                continue;

            auto safeId   = underscored(sourceId);
            auto safeDest = underscored(newId);
            if (replaceKey(jsonObject, safeId, safeDest))
                continue;

            safeId   = underscoreBeforeLast(safeId);
            safeDest = underscoreBeforeLast(safeDest);
            if (replaceKey(jsonObject, safeId, safeDest)) {
                continue;
            }
            Logger(Logger::Err) << "Not found replacement for=" << sourceId << " / " << safeId;
        }
        Logger() << "Finish  updating " << title << ", new size: " << jsonObject.size();
    };

    if (output.jsonFiles.contains(JsonId::Monsters)) {
        auto& jsonDoc = output.jsonFiles[JsonId::Monsters];
        doReplace(output.idClones.monstats, jsonDoc, "monster.json");
    }

    if (output.jsonFiles.contains(JsonId::Missiles)) {
        auto& jsonDoc = output.jsonFiles[JsonId::Missiles];
        doReplace(output.idClones.missiles, jsonDoc, "missiles.json");
    }

    if (output.newLoc.items.size() > 0) {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const auto& loc : output.newLoc.items) {
            totalKeySize += loc.key.size() + 1;
            totalValSize += loc.value.size() + 1;
        }
        Logger(Logger::Notice) << "Adding new " << output.newLoc.items.size() << " translations, totalKeySize=" << totalKeySize << ", totalValSize=" << totalValSize;
        if (output.jsonFiles.contains(JsonId::ItemNames)) {
            auto& jsonDoc = output.jsonFiles[JsonId::ItemNames];
            updateD2Rtranslation(jsonDoc, env.d2rLocaleId);
        }
        if (output.blobs.contains(BlobId::PatchStringTbl)) {
            auto& blob = output.blobs[BlobId::PatchStringTbl];
            updateLoDtranslation(blob);
        }
    }

    auto replaceJson = [this](const std::map<std::string, std::string>& replacements, const JsonId& name) {
        if (replacements.empty() || !output.jsonFiles.contains(name))
            return;
        auto& jsonDoc   = output.jsonFiles[name];
        auto& jsonArray = jsonDoc.getList();
        for (Mernel::PropertyTree& lang : jsonArray) {
            const std::string key = lang["Key"].getScalar().toString();
            if (replacements.contains(key)) {
                auto val              = replacements.at(key);
                lang[env.d2rLocaleId] = Mernel::PropertyTreeScalar{ val };
            }
        }
    };
    replaceJson(output.locReplacement.itemNames, JsonId::ItemNames);
    replaceJson(output.locReplacement.itemAffixes, JsonId::ItemNameAffixes);

    std::string configCrc = "A0B1C2D3"; /// @todo:

    if (output.jsonFiles.contains(JsonId::ItemNames)) {
        auto& jsonDoc = output.jsonFiles[JsonId::ItemNames];
        for (auto& itemDesc : jsonDoc.getList()) {
            const std::string key = itemDesc["Key"].getScalar().toString();
            if (key == "box") {
                itemDesc[env.d2rLocaleId] = Mernel::PropertyTreeScalar("d2modgen " + std::string(g_version) + " CRC:" + configCrc);
            }
        }
    }
}

template<class T>
void ModuleGlobal::create(const std::string& id)
{
    m_modules.push_back(ModuleRecord{ id, std::make_unique<T>(output, config, env) });
}

void ModuleGlobal::updateD2Rtranslation(Mernel::PropertyTree& jsonDoc, const std::string& localeId)
{
    {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const auto& itemDesc : jsonDoc.getList()) {
            const std::string key = itemDesc["Key"].getScalar().toString();
            const std::string val = itemDesc[localeId].getScalar().toString();
            totalKeySize += key.size() + 1;
            totalValSize += val.size() + 1;
        }
        Logger(Logger::Notice) << "Existing: totalKeySize=" << totalKeySize << " totalValSize=" << totalValSize;
    }

    int id = 1000000;
    for (const DataContext::NewLoc& loc : output.newLoc.items) {
        Mernel::PropertyTree itemDesc;
        itemDesc["Key"]    = Mernel::PropertyTreeScalar(loc.key);
        itemDesc["id"]     = Mernel::PropertyTreeScalar(id++);
        itemDesc[localeId] = Mernel::PropertyTreeScalar(loc.value);

        jsonDoc.append(itemDesc);
    }
}

void ModuleGlobal::updateLoDtranslation(std::string& patchstringTlb)
{
    TblStructure tbl;
    tbl.readFromBuffer(patchstringTlb);

    {
        size_t totalKeySize = 0;
        size_t totalValSize = 0;
        for (const auto& itemDesc : tbl.m_rows) {
            totalKeySize += itemDesc.Key.size() + 1;
            totalValSize += itemDesc.Val.size() + 1;
        }
        Logger(Logger::Notice) << "Existing: totalKeySize=" << totalKeySize << " totalValSize=" << totalValSize;
    }

    for (const DataContext::NewLoc& loc : output.newLoc.items) {
        tbl.m_rows.push_back({ .Key = loc.key, .Val = loc.value });
    }
    tbl.writeToBuffer(patchstringTlb);
}

}
