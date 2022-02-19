/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageSkillRandomizer.hpp"

namespace D2ModGen {

namespace {

const QSet<QString> s_skillsOkToRandomizeDamage{
    "Fire Bolt",
    "Charged Bolt",
    "Ice Bolt",
    "Inferno",
    "Static Field",
    "Frost Nova",
    "Ice Blast",
    "Blaze",
    "Nova",
    "Lightning",
    "Fire Wall",
    "Chain Lightning",
    "Thunder Storm",
    "Blizzard",
    "Hydra",
    "Frozen Orb",

    "Fire Arrow",
    "Cold Arrow",
    "Ice Arrow",

    "Firestorm",
    "Hurricane",
    "Molten Boulder",
    "Arctic Blast",
    "Eruption",

    "Blessed Hammer",
    "Fist of the Heavens",

    "Teeth",
    "Bone Spear",
    "Bone Spirit",
    "Poison Dagger",
    "Poison Nova",
    "Poison Explosion",

    "Fire Trauma",
    "Shock Field",
    "Fists of Fire",
    "Charged Bolt Sentry",
    "Wake of Fire Sentry",
    "Claws of Thunder",
    "Lightning Sentry",
    "Inferno Sentry",
    "Blades of Ice",
    "Death Sentry",
};

struct SkillLocation {
    int skillPage   = 0;
    int skillRow    = 0;
    int skillColumn = 0;
    int listRow     = 0;
    int reqlevel    = 0;
};

struct SkillDamageInfo {
    QString EType;
    int     ELen;
    int     HitShift;
};

struct SkillInfo {
    QString         uppercaseKey;
    QString         lowercaseKey;
    QString         charClass; // ass|bar|...
    SkillLocation   loc;
    SkillDamageInfo dmg;
    QStringList     reqSkills;

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
        dmg.ELen     = row["ELen"].toInt();
        dmg.HitShift = row["HitShift"].toInt();
        dmg.EType    = row["EType"];
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
        row["ELen"]     = dmg.ELen ? QString("%1").arg(dmg.ELen) : "";
        row["HitShift"] = QString("%1").arg(dmg.HitShift);
        row["EType"]    = dmg.EType;
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

    void randomizeDmg(SkillParsedData& data, QRandomGenerator& rng, const bool ensureDifferent)
    {
        const QStringList elements{ "ltng", "fire", "cold", "mag", "pois" };

        for (SkillInfo& skillInfo : data) {
            if (!s_skillsOkToRandomizeDamage.contains(skillInfo.uppercaseKey))
                continue;
            const bool wasPoison = skillInfo.dmg.EType == "pois";
            {
                QStringList elementsTmp = elements;
                if (ensureDifferent)
                    elementsTmp.removeAll(skillInfo.dmg.EType);
                skillInfo.dmg.EType = elementsTmp[rng.bounded(elementsTmp.size())];
            }
            const bool isPoison = skillInfo.dmg.EType == "pois";
            if (isPoison) {
                skillInfo.dmg.ELen = 50;
                skillInfo.dmg.HitShift -= 4;
                skillInfo.dmg.HitShift = std::max(0, skillInfo.dmg.HitShift);
            }
            if (wasPoison) {
                skillInfo.dmg.HitShift += 4;
                skillInfo.dmg.HitShift = std::min(8, skillInfo.dmg.HitShift);
            }
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
               << addHelp(new CheckboxWidget(tr("Randomize skill tabs within each character"), "skillTree", true, this),
                          tr("That DOES not move skills between characters! What it can do:\n"
                             "-Move a skill to a different skill tab;\n"
                             "-Move a skill to a higher/lower level;\n"
                             "-Change skill requirements. \n"
                             "So, after level 50 there is not much of a difference. Synergies are also untouched."))
               << addHelp(new CheckboxWidget(tr("Randomize element types for some damage skills"), "skillDamage", true, this),
                          tr("For some skills, where it is easy to change damage type, it will randomize between 5 different elements:\n"
                             "Cold, Fire, Lightning, Magic and Poison\n"
                             "WARNING! Skill descriptions won't be updated and will be completely wrong!\n"
                             "Also, character info screen does not show difference between cold and magic correctly.\n"
                             "You need to find by yourself what damage (magic or cold) was rolled.\n"
                             "For poison, all damage is dealt over 2 seconds and slightly more than original."))
               << addHelp(new CheckboxWidget(tr("Ensure that element types are different from original"), "ensureDifferent", true, this),
                          tr("If the source element is Cold, then new randomized element never should be Cold.\n"
                             "If this turned off, ~20% of skills will have same behaviour."))

    );
    closeLayout();
}

QString ConfigPageSkillRandomizer::pageHelp() const
{
    return tr("This allow to bring some randomization into character skills.");
}

void ConfigPageSkillRandomizer::generate(DataContext& output, QRandomGenerator& rng, const GenerationEnvironment& env) const
{
    const bool skillTreeRandomize   = getWidgetValue("skillTree");
    const bool skillDamageRandomize = getWidgetValue("skillDamage");
    const bool ensureDifferent      = getWidgetValue("ensureDifferent");

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
    if (skillDamageRandomize)
        charTree.randomizeDmg(skillParsedData, rng, ensureDifferent);
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
