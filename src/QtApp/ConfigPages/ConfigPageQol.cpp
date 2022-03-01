/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ConfigPageQol.hpp"

namespace D2ModGen {

namespace {
const bool s_init = pageRegisterHelper<ConfigPageQol>();
}

ConfigPageQol::ConfigPageQol(QWidget* parent)
    : ConfigPageAbstract(parent)
{
    initModule();
    addEditors(QList<IValueWidget*>()
               << new CheckboxWidget(tr("Increase tome sizes 20 -> 60"), "tomeSize", false, this)
               << new CheckboxWidget(tr("Increase key chain 12 -> 50"), "keySize", false, this)
               << new CheckboxWidget(tr("Increase quivers size (250,350) -> 511"), "quiverSize", false, this)
               << new CheckboxWidget(tr("Remove limit on Unique charms"), "uniqueCharmLimit", false, this)
               << new CheckboxWidget(tr("Allow using Teleport, BC, BO in town"), "weakenTownSkills", false, this)
               << new SliderWidgetMinMax(tr("Reduce costs of skills and stats on items (affects repair cost mostly)"),
                                         "reduceCost",
                                         5,
                                         100,
                                         100,
                                         this));
    closeLayout();
}

QString ConfigPageQol::pageHelp() const
{
    return tr("Tiny quality things to make you life easier: \n"
              "1. Insreasing stacks for tomes/quivers/keys;\n"
              "2. Remove Unique charm limit (useful with randomizer);\n"
              "3. Allow usage of some skills in the town;\n"
              "4. Reduce item costs. ");
}

}
