/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageSkillRandomizer.hpp"

namespace D2ModGen {

namespace {

struct SkillLocation {
    int skillPage   = 0;
    int skillRow    = 0;
    int skillColumn = 0;
    int listRow     = 0;
    int reqlevel    = 0;
};

struct SkillInfo {
    QString       uppercaseKey;
    QString       lowercaseKey;
    QString       charClass; // ass|bar|...
    SkillLocation loc;
    QStringList   reqSkills;

    void readFromRowMain(const TableView::RowView& row)
    {
        uppercaseKey = row["skill"];
        lowercaseKey = uppercaseKey.toLower();
        charClass    = row["charclass"];
        loc.reqlevel = row["reqlevel"].toInt();
        for (int i = 1; i <= 3; ++i) {
            const QString& req = row[QString("reqskill%1").arg(i)];
            if (!req.isEmpty())
                reqSkills << req;
        }
    }
    void readFromRowDesc(const TableView::RowView& row)
    {
        loc.skillPage   = row["SkillPage"].toInt();
        loc.skillRow    = row["SkillRow"].toInt();
        loc.skillColumn = row["SkillColumn"].toInt();
        loc.listRow     = row["ListRow"].toInt();
    }
    void writeToRowMain(TableView::RowView& row)
    {
        row["reqlevel"] = QString("%1").arg(loc.reqlevel);
        for (int i = 1; i <= 3; ++i) {
            QString& req = row[QString("reqskill%1").arg(i)];
            req          = reqSkills.value(i - 1);
        }
    }
    void writeToRowDesc(TableView::RowView& row)
    {
        row["SkillPage"]   = QString("%1").arg(loc.skillPage);
        row["SkillRow"]    = QString("%1").arg(loc.skillRow);
        row["SkillColumn"] = QString("%1").arg(loc.skillColumn);
        row["ListRow"]     = QString("%1").arg(loc.listRow);
    }
};

using SkillParsedData = QMap<QString, SkillInfo>;

struct SkillTree {
    struct Character {
        struct Skill {
            SkillLocation loc;
            int           skillId;
            QList<int>    reqSkillIds;
        };
        QList<Skill> skills;
        QString      charClass;
        QStringList  skillIds;

        int addSkillId(const QString& skill)
        {
            int idx = skillIds.indexOf(skill);
            if (idx != -1)
                return idx;
            idx = skillIds.size();
            skillIds << skill;
            return idx;
        }
    };
    QMap<QString, Character> chars;

    void parse(const SkillParsedData& data)
    {
        for (const SkillInfo& skillInfo : data) {
            Character& c = chars[skillInfo.charClass];
            c.charClass  = skillInfo.charClass;

            Character::Skill s;
            s.loc     = skillInfo.loc;
            s.skillId = c.addSkillId(skillInfo.uppercaseKey);
            for (const QString& req : skillInfo.reqSkills)
                s.reqSkillIds << c.addSkillId(req);
            c.skills << s;
        }
    }

    void randomizeTabs(QRandomGenerator& rng)
    {
        for (Character& c : chars) {
            QStringList source = c.skillIds;
            c.skillIds.clear();
            while (source.size() > 1) {
                c.skillIds << source.takeAt(rng.bounded(source.size()));
            }
            c.skillIds << source;
        }
    }

    void writeOutput(SkillParsedData& data) const
    {
        for (const Character& c : chars) {
            for (const Character::Skill& s : c.skills) {
                auto       skillId   = c.skillIds[s.skillId];
                SkillInfo& skillInfo = data[skillId.toLower()];
                skillInfo.loc        = s.loc;
                skillInfo.reqSkills.clear();
                for (int req : s.reqSkillIds)
                    skillInfo.reqSkills << c.skillIds[req];
            }
        }
    }
};

}

ConfigPageSkillRandomizer::ConfigPageSkillRandomizer(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Randomize skill tabs within each character"), "skillTree", true, this));
    closeLayout();
}

QString ConfigPageSkillRandomizer::pageHelp() const
{
    return tr("This allow to bring some randomization into character skills.");
}

void ConfigPageSkillRandomizer::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    const bool skillTreeRandomize = getWidgetValue("skillTree");

    Table&          skillsTable = output.tableSet.tables["skills"];
    TableView       skillsTableView(skillsTable, true);
    Table&          descTable = output.tableSet.tables["skilldesc"];
    TableView       descTableView(descTable, true);
    SkillParsedData skillParsedData;
    SkillTree       charTree;
    for (auto& row : skillsTableView) {
        SkillInfo info;
        info.readFromRowMain(row);
        if (info.charClass.isEmpty())
            continue;

        skillParsedData[info.lowercaseKey] = std::move(info);
    }
    for (auto& row : descTableView) {
        auto& skilldescKey = row["skilldesc"];
        if (!skillParsedData.contains(skilldescKey))
            continue;
        skillParsedData[skilldescKey].readFromRowDesc(row);
    }
    // transfrom data
    charTree.parse(skillParsedData);
    if (skillTreeRandomize)
        charTree.randomizeTabs(rng);
    charTree.writeOutput(skillParsedData);
    // write out
    for (auto& row : skillsTableView) {
        const auto& skilldescKey = row["skill"].toLower();
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowMain(row);
    }
    for (auto& row : descTableView) {
        const auto& skilldescKey = row["skilldesc"].toLower();
        if (!skillParsedData.contains(skilldescKey))
            continue;

        skillParsedData[skilldescKey].writeToRowDesc(row);
    }
}

}
