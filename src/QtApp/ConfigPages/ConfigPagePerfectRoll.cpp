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
               << new CheckboxWidget(tr("Perfect rolls for crafting in the Cube"), "craft", true, this)
               << new CheckboxWidget(tr("Perfect Uniques"), "uniques", true, this)
               << new CheckboxWidget(tr("Perfect Rune Word rolls"), "runeWords", true, this)
               << new CheckboxWidget(tr("Perfect Set items"), "setItems", true, this)
               << new CheckboxWidget(tr("Perfect Magic/Rare affixes"), "affixes", true, this)
               << new CheckboxWidget(tr("Always max defense Armor"), "armor", true, this));
    closeLayout();
}

QString ConfigPagePerfectRoll::pageHelp() const
{
    return tr("Remove random effect when rolling between min and max value for something.");
}

}
