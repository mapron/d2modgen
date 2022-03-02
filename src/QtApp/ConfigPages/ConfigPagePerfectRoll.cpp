/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPagePerfectRoll.hpp"

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPagePerfectRoll>();
}

ConfigPagePerfectRoll::ConfigPagePerfectRoll(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(makeEditors({
        "craft",
        "uniques",
        "runeWords",
        "setItems",
        "affixes",
        "armor",
    }));
    closeLayout();
}

QString ConfigPagePerfectRoll::pageHelp() const
{
    return tr("Remove random effect when rolling between min and max value for something.");
}

QMap<std::string, QString> ConfigPagePerfectRoll::widgetTitles() const
{
    return {
        { "craft", tr("Perfect rolls for crafting in the Cube") },
        { "uniques", tr("Perfect Uniques") },
        { "runeWords", tr("Perfect Rune Word rolls") },
        { "setItems", tr("Perfect Set items") },
        { "affixes", tr("Perfect Magic/Rare affixes") },
        { "armor", tr("Always max defense Armor") },
    };
}

}
