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
    addEditors(QList<IValueWidget*>()
               << makeEditor("craft", tr("Perfect rolls for crafting in the Cube"))
               << makeEditor("uniques", tr("Perfect Uniques"))
               << makeEditor("runeWords", tr("Perfect Rune Word rolls"))
               << makeEditor("setItems", tr("Perfect Set items"))
               << makeEditor("affixes", tr("Perfect Magic/Rare affixes"))
               << makeEditor("armor", tr("Always max defense Armor")));
    closeLayout();
}

QString ConfigPagePerfectRoll::pageHelp() const
{
    return tr("Remove random effect when rolling between min and max value for something.");
}

}
