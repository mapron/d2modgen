/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleItemRandomizer.hpp"
#include "RandoUtils.hpp"
#include "AttributeHelper.hpp"

#include "Logger.hpp"
#include "MernelPlatform/ChronoPoint.hpp"

namespace D2ModGen {

namespace {
constexpr const int s_maxUnbalanceLevel = 100;

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return ch < 0 || !std::isspace(ch);
            }));
}
inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return ch < 0 || !std::isspace(ch);
            }).base(),
            s.end());
}

inline StringVector splitLine(const std::string& line, char sep, bool skipEmpty = false)
{
    std::vector<std::string> result;
    std::string              token;
    std::istringstream       ss(line);
    while (std::getline(ss, token, sep)) {
        ltrim(token);
        rtrim(token);
        if (!token.empty() || !skipEmpty)
            result.push_back(token);
    }
    return result;
}

}

ModuleItemRandomizer::ModuleItemRandomizer()
    : PluginModule(Mernel::PropertyTreeMap{
          { "affixRandom", Mernel::PropertyTreeScalar{ true } },
          { "crazyLevel", Mernel::PropertyTreeScalar{ 20 } },
          { "extraKnown", Mernel::PropertyTreeScalar{} },
          { "gemsRandom", Mernel::PropertyTreeScalar{ false } },
          { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 70 } },
          { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
          { "removeKnock", Mernel::PropertyTreeScalar{ true } },
          { "repeat_sets", Mernel::PropertyTreeScalar{ 5 } },
          { "repeat_uniques", Mernel::PropertyTreeScalar{ 10 } },
          { "replaceCharges", Mernel::PropertyTreeScalar{ false } },
          { "replaceSkills", Mernel::PropertyTreeScalar{ false } },
          { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
          { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
      })
{
    setPresets({
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "crazyLevel", Mernel::PropertyTreeScalar{ 100 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 0 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "gemsRandom", Mernel::PropertyTreeScalar{ true } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "crazyLevel", Mernel::PropertyTreeScalar{ 30 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 50 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 20 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 1000 } },
            { "affixRandom", Mernel::PropertyTreeScalar{ true } },
            { "replaceSkills", Mernel::PropertyTreeScalar{ true } },
            { "replaceCharges", Mernel::PropertyTreeScalar{ true } },
        } },
        Mernel::PropertyTree{ Mernel::PropertyTreeMap{
            { "crazyLevel", Mernel::PropertyTreeScalar{ 10 } },
            { "itemFitPercent", Mernel::PropertyTreeScalar{ 70 } },
            { "keepOriginalPercent", Mernel::PropertyTreeScalar{ 50 } },
            { "relativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "relativeCountMax", Mernel::PropertyTreeScalar{ 120 } },
            { "setRelativeCountMin", Mernel::PropertyTreeScalar{ 100 } },
            { "setRelativeCountMax", Mernel::PropertyTreeScalar{ 150 } },
        } },
    });
}

void ModuleItemRandomizer::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    MagicPropUniverse props;

    std::vector<std::string> setItemsKeys;
    std::vector<std::string> allItemsKeys;

    const bool replaceSkills  = input.getInt("replaceSkills");
    const bool replaceCharges = input.getInt("replaceCharges");
    const bool removeKnock    = input.getInt("removeKnock");

    const int repeatUniques = input.getInt("repeat_uniques");
    const int repeatSets    = input.m_env.isLegacy ? 0 : input.getInt("repeat_sets");

    auto postProcessRawList = [replaceSkills, replaceCharges, removeKnock](MagicPropRawList& rawList) {
        rawList.postProcess(replaceSkills, replaceCharges, removeKnock);
    };
    auto grabProps = [&props, &postProcessRawList]<typename Row>(const DataTable<Row>& table) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int level = row.getLevel();

            ItemCodeFilter types = row.getItemFilter();

            for (const AffixPropList* affixList : row.affixLists()) {
                MagicPropRawList rawList;
                rawList.readFromRow(*affixList);
                postProcessRawList(rawList);
                props.add(std::move(rawList), types, level);
            }
        }
    };

    {
        grabProps(output.uniqueitems);
        std::vector<DataRow_uniqueitems> validRows;
        for (auto& row : output.uniqueitems.rows) {
            if (!row.isValid())
                continue;
            allItemsKeys.push_back(row.index);
            validRows.push_back(row);
        }
        if (repeatUniques > 1) {
            for (int i = 2; i <= repeatUniques; ++i)
                output.uniqueitems.rows.insert(output.uniqueitems.rows.end(), validRows.cbegin(), validRows.cend());
        }
    }

    {
        grabProps(output.runes);
    }
    {
        grabProps(output.setitems);
        std::vector<DataRow_setitems> validRows;
        for (auto& row : output.setitems.rows) {
            if (!row.isValid())
                continue;
            setItemsKeys.push_back(row.index);
            allItemsKeys.push_back(row.index);
            validRows.push_back(row);
        }

        for (int i = 2; i <= repeatSets; ++i) {
            auto setSuffix = " " + std::to_string(i);
            auto rowsCopy  = validRows;
            for (auto& row : rowsCopy) {
                row.index += setSuffix;
                row.setIndex += setSuffix;
            }
            output.setitems.rows.insert(output.setitems.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }
    {
        grabProps(output.gems);
    }
    {
        grabProps(output.magicprefix);
        grabProps(output.magicsuffix);
    }
    {
        grabProps(output.sets);
        std::vector<DataRow_sets> validRows;
        for (auto& row : output.sets.rows) {
            if (!row.isValid())
                continue;
            setItemsKeys.push_back(row.index);
            allItemsKeys.push_back(row.name);
            validRows.push_back(row);
        }

        const auto rowsOriginal = output.sets.rows;
        for (int i = 2; i <= repeatSets; ++i) {
            auto setSuffix = " " + std::to_string(i);
            auto rowsCopy  = validRows;
            for (auto& row : rowsCopy) {
                row.index += setSuffix;
                row.name += setSuffix;
            }
            output.sets.rows.insert(output.sets.rows.end(), rowsCopy.cbegin(), rowsCopy.cend());
        }
    }

    const int unbalance               = input.getInt("crazyLevel");
    const int itemFitPercent          = input.getInt("itemFitPercent");
    const int keepOriginalPercent     = input.getInt("keepOriginalPercent");
    const int relativeGeneralCountMin = input.getInt("relativeCountMin");
    const int relativeGeneralCountMax = input.getInt("relativeCountMax");
    const int relativeSetCountMin     = input.getInt("setRelativeCountMin");
    const int relativeSetCountMax     = input.getInt("setRelativeCountMax");

    auto calcNewCount = [&rng,
                         relativeGeneralCountMin,
                         relativeGeneralCountMax,
                         relativeSetCountMin,
                         relativeSetCountMax,
                         keepOriginalPercent](const int originalCount, bool isSet) -> std::pair<int, int> {
        const auto relativeCountMin = isSet ? relativeSetCountMin : relativeGeneralCountMin;
        const auto relativeCountMax = isSet ? relativeSetCountMax : relativeGeneralCountMax;
        if (relativeCountMin == 100 && relativeCountMax == 100) {
            if (keepOriginalPercent == 100)
                return { originalCount, 0 };
            if (keepOriginalPercent == 0)
                return { 0, originalCount };
        }

        const int relativeCountBase     = std::min(relativeCountMin, relativeCountMax);
        const int relativeCountBonusRng = std::max(relativeCountMin, relativeCountMax) - relativeCountBase;
        const int relativeCount         = relativeCountBase + (relativeCountBonusRng > 0 ? rng(relativeCountBonusRng) : 0);
        const int absCountInHundreds    = originalCount * relativeCount;
        if (keepOriginalPercent == 0)
            return { 0, absCountInHundreds / 100 };
        const int  keepCountInHundreds = keepOriginalPercent * originalCount;
        const auto rngFraction         = [&rng](const int hundredsCount) -> int {
            const int fraction = hundredsCount % 100;
            const int whole    = hundredsCount / 100;
            if (fraction < 2)
                return whole;
            return whole + (0 == rng(fraction));
        };
        const int keepCount          = rngFraction(keepCountInHundreds);
        const int genCountInHundreds = absCountInHundreds - keepCount * 100;
        const int generatedCount     = rngFraction(genCountInHundreds);
        return { keepCount, generatedCount };
    };

    auto fillProps = [&props,
                      &postProcessRawList,
                      replaceSkills,
                      &rng,
                      &calcNewCount,
                      itemFitPercent,
                      unbalance]<typename Row>(DataTable<Row>& table,
                                               const bool      skipEmptyList,
                                               const bool      supportMinMax,
                                               const bool      isSet = false) {
        for (auto& row : table.rows) {
            if (!row.isValid())
                continue;
            const int              level               = row.getLevel();
            const AttributeFlagSet supportedAttributes = row.getAttributes();

            const ItemCodeFilter types = row.getItemFilter();

            int genCountPrev = 0;

            for (AffixPropList* affixList : row.affixLists()) {
                MagicPropRawList rawList;
                rawList.readFromRow(*affixList);
                if (skipEmptyList && rawList.parsedProps.empty())
                    continue;

                postProcessRawList(rawList);

                const int originalCount  = rawList.getTotalSize();
                auto [keepCnt, genCount] = calcNewCount(originalCount, isSet);

                if (keepCnt + genCount == 0) {
                    genCount = 1;
                }
                const int maxGenCount = affixList->getLimit() - keepCnt;
                genCount += genCountPrev;
                if (genCount > maxGenCount) {
                    genCountPrev = genCount - maxGenCount;
                    genCount     = maxGenCount;
                }

                if (keepCnt < originalCount)
                    rawList.truncateRandom(rng, keepCnt);

                int existingClass = types.determineClass();
                if (existingClass == -1)
                    existingClass = rawList.determineClass(!replaceSkills);
                auto newProps = props.generate(rng,
                                               rawList.getAllCodes(),
                                               existingClass,
                                               supportedAttributes,
                                               types,
                                               itemFitPercent,
                                               genCount,
                                               level,
                                               unbalance == s_maxUnbalanceLevel ? -1 : unbalance);

                rawList.append(std::move(newProps));
                if (rawList.getTotalSize() > affixList->getLimit())
                    rawList.truncateRandom(rng, affixList->getLimit());

                if (!supportMinMax)
                    rawList.makePerfect();

                rawList.writeToRow(*affixList);
            }
        }
    };
    auto startFillProps = Mernel::ChronoPoint(true);

    fillProps(output.uniqueitems, true, true);

    fillProps(output.runes, true, false);

    fillProps(output.setitems, false, false, true);

    if (input.getInt("gemsRandom"))
        fillProps(output.gems, true, false);

    if (input.getInt("affixRandom")) {
        fillProps(output.magicprefix, true, true);
        fillProps(output.magicsuffix, true, true);
    }

    fillProps(output.sets, false, false);

    auto endFillProps = startFillProps.GetElapsedTime().ToProfilingTime();

    if (repeatSets > 1 && output.jsonFiles.contains(JsonId::ItemNames)) {
        auto&                                       jsonDoc = output.jsonFiles[JsonId::ItemNames];
        std::map<std::string, Mernel::PropertyTree> itemNamesByKey;
        for (const auto& itemDesc : jsonDoc.getList()) {
            const std::string key = itemDesc["Key"].getScalar().toString();
            itemNamesByKey[key]   = itemDesc;
        }
        int id = 1000000;
        for (const auto& key : allItemsKeys) {
            if (itemNamesByKey.contains(key))
                continue;
            Mernel::PropertyTree itemDesc;
            itemDesc["Key"]  = Mernel::PropertyTreeScalar(key);
            itemDesc["enUS"] = Mernel::PropertyTreeScalar(key);
            itemDesc["id"]   = Mernel::PropertyTreeScalar(id++);
            jsonDoc.append(itemDesc);
            itemNamesByKey[key] = itemDesc;
        }
        for (int i = 2; i <= repeatSets; ++i) {
            for (const auto& key : setItemsKeys) {
                const std::string    keyCopy  = key + " " + std::to_string(i);
                Mernel::PropertyTree itemDesc = itemNamesByKey[key];
                itemDesc["Key"]               = Mernel::PropertyTreeScalar(keyCopy);
                itemDesc["id"]                = Mernel::PropertyTreeScalar(id++);
                for (auto& [locKey, locValue] : itemDesc.getMap()) {
                    if (locKey == "Key" || locKey == "id")
                        continue;
                    const auto str = locValue.getScalar().toString();
                    locValue       = Mernel::PropertyTreeScalar(str + " " + std::to_string(i));
                }
                jsonDoc.append(itemDesc);
            }
        }
    }

    Logger() << "  randomizer::generate fillprops done in: " << endFillProps;
}

}
