/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleMonRandomizer.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleMonRandomizer>();

constexpr const int s_maxIngameLevel = 110;
const QString       s_monstersJson   = "data\\hd\\character\\monsters.json";

struct MotTypeTable {
    struct MonRecord {
        QString id;
        int     level;
    };
    struct MonType {
        QString          baseId;
        QList<MonRecord> children;
        int              count = 0;
    };
    QMap<QString, MonType> types;
    QMap<QString, QString> id2base;
    QMap<QString, int>     id2index;
    struct MonCopy {
        int     sourceIndex;
        QString sourceId;
        QString newId;
        int     oldLevel;
        int     newLevel;
    };
    using MonCopyList = QList<MonCopy>;
    MonCopyList newCopies;

    void add(QString id, QString baseId, int level, int index)
    {
        MonType& mt = types[baseId];
        mt.baseId   = baseId;
        MonRecord r{ id, level };
        mt.children << r;
        mt.count++;
        id2base[id]  = baseId;
        id2index[id] = index;
    }

    QString pickRandomId(QRandomGenerator& rng, QString baseId) const
    {
        const MonType&   mt = types[baseId];
        const MonRecord& mr = mt.children[rng.bounded(mt.count)];
        return mr.id;
    }

    QString pickRandomId(QRandomGenerator& rng, QString baseId, int targetLevel, int maxDistance = 3)
    {
        assert(types.contains(baseId));
        MonType& mt = types[baseId];
        assert(mt.count > 0);
        QList<int> okIndexes;
        for (int i = 0; i < mt.children.size(); ++i) {
            const MonRecord& mr = mt.children[i];
            if (std::abs(mr.level - targetLevel) <= maxDistance)
                okIndexes << i;
        }
        if (okIndexes.empty()) {
            okIndexes << mt.children.size();
            const MonRecord& source = mt.children[rng.bounded(mt.count)];

            QString newId = QString("%1_%2").arg(source.id).arg(targetLevel);
            mt.children << MonRecord{ newId, targetLevel };
            newCopies << MonCopy{ id2index[source.id], source.id, newId, source.level, targetLevel };
        }
        const int        index = okIndexes[rng.bounded(okIndexes.size())];
        const MonRecord& mr    = mt.children[index];
        return mr.id;
    }

    void dump()
    {
        for (auto& type : types) {
            QString s = type.baseId + ": ";
            for (auto& c : type.children)
                s += c.id + "(" + QString::number(c.level) + "), ";
            qDebug() << s;
        }
    }
};

struct TCTable {
    struct TCGroup {
        struct TCRecord {
            QString id;
            int     level;
        };
        QList<TCRecord> records;
    };
    QMap<int, TCGroup> groups;
    QMap<QString, int> tc2group;

    void add(const QString& id, const QString& group, const QString& level)
    {
        if (group.isEmpty() || level.isEmpty())
            return;
        TCGroup& g = groups[group.toInt()];
        g.records << TCGroup::TCRecord{ id, level.toInt() };
        tc2group[id] = group.toInt();
    }
    QString promote(const QString& tc, const int oldLevel, const int newLevel) const
    {
        if (!tc2group.contains(tc) || oldLevel == newLevel)
            return tc;
        const TCGroup& g = groups[tc2group[tc]];

        QString res = tc;
        for (auto& rec : g.records) {
            if (rec.level > newLevel)
                break;
            res = rec.id;
        }
        return res;
    };
};

}

PropertyTreeScalarMap ModuleMonRandomizer::defaultValues() const
{
    return {
        { "spawnedCount", 5 },
        { "maxTypes", 25 },
    };
}

IModule::UiControlHintMap ModuleMonRandomizer::uiHints() const
{
    return {
        { "spawnedCount", UiControlHintSliderMinMax(2, 10) },
        { "maxTypes", UiControlHintSliderMinMax(3, 25) },
    };
}

void ModuleMonRandomizer::gatherInfo(PreGenerationContext& output, const InputContext& input) const
{
    if (input.m_env.isLegacy)
        return;

    output.m_extraJson << s_monstersJson;
}

void ModuleMonRandomizer::generate(DataContext& output, QRandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    auto& tableSet = output.tableSet;

    MotTypeTable typeTable;
    TCTable      tcTable;
    {
        Table&    table = tableSet.tables["monstats"];
        TableView tableView(table);
        for (auto& row : tableView) {
            const int normalLevel = row["Level"].toInt();
            if (normalLevel <= 0)
                continue;
            typeTable.add(row["Id"], row["BaseId"], normalLevel, row.index());
        }
    }
    {
        Table&    table = tableSet.tables["treasureclassex"];
        TableView tableView(table);
        for (auto& row : tableView)
            tcTable.add(row["Treasure Class"], row["group"], row["level"]);
    }
    {
        Table&        table = tableSet.tables["levels"];
        TableView     tableView(table, true);
        QSet<QString> baseIds, ubaseIds, nonUniqueBaseIds;
        int           cols = 25;
        for (const char* colTpl : { "mon%1", "nmon%1", "umon%1" }) {
            for (int i = 1; i <= cols; ++i)
                if (!tableView.hasColumn(QString(colTpl).arg(i))) {
                    cols = i - 1;
                    break;
                }
        }
        const int spawnedCount = input.getInt("spawnedCount");
        const int maxTypes     = std::min(input.getInt("maxTypes"), cols);
        for (auto& row : tableView) {
            if (row["Name"] == "Act 5 - Mountain Top")
                continue;
            for (int i = 1; i <= cols; ++i) {
                QString& id = row[QString("mon%1").arg(i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id);
                if (!base.isEmpty())
                    baseIds << base;
            }
            for (int i = 1; i <= cols; ++i) {
                QString& id = row[QString("nmon%1").arg(i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id);
                if (!base.isEmpty())
                    baseIds << base;
            }
            for (int i = 1; i <= cols; ++i) {
                QString& id = row[QString("umon%1").arg(i)];
                if (id.isEmpty())
                    break;
                auto base = typeTable.id2base.value(id);
                if (!base.isEmpty())
                    ubaseIds << base;
            }
        }
        nonUniqueBaseIds = baseIds;
        nonUniqueBaseIds -= ubaseIds;
        //qDebug() << "non uniques:" << nonUniqueBaseIds;
        //typeTable.removeUnused(baseIds);

        QStringList idsList(baseIds.cbegin(), baseIds.cend());
        for (auto& row : tableView) {
            if (row["mon1"].isEmpty())
                continue;
            if (row["Name"] == "Act 5 - Mountain Top")
                continue;
            const int     normalLevel = row.hasColumn("MonLvlEx") ? row["MonLvlEx"].toInt() : row["MonLvl1Ex"].toInt();
            QSet<QString> normalSet;
            for (int i = 0; i < maxTypes * 2; ++i) {
                normalSet << idsList[rng.bounded(idsList.size())];
                if (normalSet.size() >= maxTypes)
                    break;
            }

            QStringList normalSetList;
            QStringList uniques;
            for (auto baseId : normalSet) {
                auto id = typeTable.pickRandomId(rng, baseId, normalLevel);
                normalSetList << id;
                if (!nonUniqueBaseIds.contains(baseId))
                    uniques << id;
            }

            for (int i = 1; i <= cols; ++i) {
                row[QString("mon%1").arg(i)]  = normalSetList.value(i - 1);
                row[QString("nmon%1").arg(i)] = normalSetList.value(i - 1);
                row[QString("umon%1").arg(i)] = uniques.value(i - 1);
            }
            row["NumMon"] = QString("%1").arg(spawnedCount);
        }
    }
    {
        Table& table   = tableSet.tables["monstats"];
        table.modified = true;
        MotTypeTable::MonCopyList newCopies;
        auto                      insertNewRows = [&table, &rng, &newCopies, &typeTable, &tcTable]() {
            MotTypeTable::MonCopyList tmpList = std::move(typeTable.newCopies);
            assert(!typeTable.types.isEmpty());
            newCopies << tmpList;
            int        idCol    = 0;
            int        uidCol   = 1;
            int        levelCol = table.columns.indexOf("Level");
            QList<int> minionCols;
            QList<int> skillCols;
            QList<int> tcCols;
            for (const char* col : { "spawn", "minion1", "minion2" }) {
                int colIndex = table.columns.indexOf(col);
                assert(colIndex > -1);
                minionCols << colIndex;
            }
            for (int i = 1; i <= 8; ++i) {
                int colIndex = table.columns.indexOf(QString("Sk%1lvl").arg(i));
                assert(colIndex > -1);
                skillCols << colIndex;
            }
            for (int i = 1; i <= 4; ++i) {
                int colIndex = table.columns.indexOf(QString("TreasureClass%1").arg(i));
                assert(colIndex > -1);
                tcCols << colIndex;
            }

            for (const auto& copy : tmpList) {
                auto newRow           = table.rows[copy.sourceIndex];
                newRow.data[idCol]    = copy.newId;
                newRow.data[levelCol] = QString("%1").arg(copy.newLevel);
                newRow.data[uidCol]   = QString("%1").arg(table.rows.size());
                for (int minionCol : minionCols) {
                    QString& minion = newRow.data[minionCol];
                    if (minion.isEmpty())
                        continue;
                    const auto minionBase = typeTable.id2base.value(minion);
                    if (minionBase.isEmpty()) {
                        minion = "";
                        continue;
                    }
                    auto id                = typeTable.pickRandomId(rng, minionBase, copy.newLevel);
                    newRow.data[minionCol] = id;
                }
                if (copy.oldLevel > copy.newLevel) {
                    for (int skillCol : skillCols) {
                        QString& cell = newRow.data[skillCol];
                        if (cell.isEmpty())
                            break;
                        const int newLevel = copy.newLevel * cell.toInt() / copy.oldLevel;
                        cell               = QString("%1").arg(std::max(1, newLevel));
                    }
                }
                for (int tcCol : tcCols) {
                    QString& cell = newRow.data[tcCol];
                    if (cell.isEmpty())
                        continue;
                    cell = tcTable.promote(cell, copy.oldLevel, copy.newLevel);
                }

                table.rows << newRow;
            }
        };
        assert(!typeTable.types.isEmpty());
        insertNewRows();
        insertNewRows();
        insertNewRows();
        typeTable.newCopies = std::move(newCopies);
    }
    if (output.jsonFiles.contains(s_monstersJson)) {
        auto& jsonDoc    = output.jsonFiles[s_monstersJson];
        auto& jsonObject = jsonDoc.getMap();
        for (const auto& copy : typeTable.newCopies) {
            const std::string sourceId  = copy.sourceId.toStdString();
            const std::string newId     = copy.newId.toStdString();
            const std::string modelName = jsonObject[sourceId].toString();
            assert(!modelName.empty());
            jsonObject[newId] = PropertyTreeScalar{ modelName };
        }
    }
}

}