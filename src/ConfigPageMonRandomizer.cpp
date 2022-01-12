/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageMonRandomizer.hpp"

namespace {

constexpr const int s_maxIngameLevel = 110;

struct MonLevelTable {
    struct MonRecord {
        QString id;
        int     level;
    };
    QSet<QString>    m_ids;
    QList<MonRecord> m_records;

    QMap<int, int> m_lowerLevelBounds;

    void insert(QString id, int level)
    {
        if (id.isEmpty())
            return;
        if (m_ids.contains(id))
            return;
        m_ids << id;
        m_records << MonRecord{ id, level };
    }

    void sortByLevel()
    {
        std::sort(m_records.begin(), m_records.end(), [](const MonRecord& l, const MonRecord& r) { return l.level < r.level; });
        int lastLevel = -1;
        for (int i = 0; i < m_records.size(); ++i) {
            int level = m_records[i].level;
            for (int j = lastLevel; j <= level; j++) {
                m_lowerLevelBounds[j] = i;
            }
            lastLevel = level + 1;
        }
        for (; lastLevel <= s_maxIngameLevel; ++lastLevel)
            m_lowerLevelBounds[lastLevel] = m_records.size();
    }

    int getUpperBound(int level, int balance) const
    {
        return m_lowerLevelBounds.value(std::clamp(level + balance, 0, s_maxIngameLevel));
    }

    QSet<QString> getRandomMonsterSet(QRandomGenerator& rng, int level, int balance, int count) const
    {
        const auto    upperBound = getUpperBound(level, balance);
        QSet<QString> result;
        if (upperBound <= count) {
            for (int i = 0; i < upperBound; i++) {
                result << m_records[i].id;
            }
            return result;
        }
        for (int i = 0; i < count; i++) {
            const int index = rng.bounded(upperBound);
            result << m_records[index].id;
        }

        return result;
    }
};
}

ConfigPageMonRandomizer::ConfigPageMonRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget("Enable Monster Randomizer", "enable", false, this)
               << new SliderWidgetMinMax("Spawned different types in one area", "spawnedCount", 3, 10, 3, this)
               << new SliderWidgetMinMax("Maximum possible monster types in one area (population variety)", "maxTypes", 6, 25, 25, this));
    closeLayout();
}

KeySet ConfigPageMonRandomizer::generate(TableSet& tableSet, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    if (isAllDefault() || !getWidgetValue("enable"))
        return {};
    KeySet result;
    result << "levels";

    const int spawnedCount = getWidgetValue("spawnedCount");
    const int maxTypes     = getWidgetValue("maxTypes");

    Table&        table = tableSet.tables["levels"];
    TableView     tableView(table);
    MonLevelTable normalTable;
    QSet<QString> hellIds;
    for (auto& row : tableView) {
        const int normalLevel = row["MonLvl"].toInt();
        for (int i = 1; i < 25; ++i) {
            QString& id = row[QString("mon%1").arg(i)];
            if (id.isEmpty())
                break;
            normalTable.insert(id, normalLevel);
        }
        for (int i = 1; i < 25; ++i) {
            QString& id = row[QString("nmon%1").arg(i)];
            if (id.isEmpty())
                break;
            hellIds << id;
        }
    }
    normalTable.sortByLevel();
    QStringList hellIdsList(hellIds.cbegin(), hellIds.cend());
    for (auto& row : tableView) {
        if (row["mon1"].isEmpty())
            continue;
        const int     normalLevel   = row["MonLvl"].toInt();
        const int     normalBalance = 3; // we can afford generating monster +3 level above area;
        QSet<QString> normalSet     = normalTable.getRandomMonsterSet(rng, normalLevel, normalBalance, maxTypes);
        QSet<QString> hellSet;
        for (int i = 0; i < maxTypes; ++i)
            hellSet << hellIdsList[rng.bounded(hellIdsList.size())];
        QSet<QString> unitedSet;
        unitedSet += normalSet;
        unitedSet += hellSet;
        QStringList normalSetList(normalSet.cbegin(), normalSet.cend());
        QStringList hellSetList(hellSet.cbegin(), hellSet.cend());
        const int   unitedSize          = unitedSet.size();
        const int   excessSize          = unitedSize - 25; // we get above 25 limit to generate umon ids. Need to throw away something.
        const int   excessSizeForNormal = normalSet.size() * excessSize / unitedSize + 1;
        const int   excessSizeForHell   = hellSet.size() * excessSize / unitedSize + 1;
        for (int i = 0; i < excessSizeForNormal; ++i)
            normalSetList.removeLast();
        for (int i = 0; i < excessSizeForHell; ++i)
            hellSetList.removeLast();
        QStringList uniques;
        uniques += normalSetList;
        uniques += hellSetList;
        normalSetList = QStringList(normalSet.cbegin(), normalSet.cend());
        hellSetList   = QStringList(hellSet.cbegin(), hellSet.cend());
        for (int i = 1; i < 25; ++i) {
            row[QString("mon%1").arg(i)]  = normalSetList.value(i - 1);
            row[QString("nmon%1").arg(i)] = hellSetList.value(i - 1);
            row[QString("umon%1").arg(i)] = uniques.value(i - 1);
        }
        row["NumMon"] = QString("%1").arg(spawnedCount);
    }

    return result;
}
