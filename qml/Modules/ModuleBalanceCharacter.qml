/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.balanceCharacter
    enabled_key: null

    property bool isD2R: appConfig.versionMajor.value === 1

    caption: qsTr("Character")
    tooltip: qsTr("First three options allow you to change starting items for characters: <br>add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. <br>Next two sliders allow you to change how many skill and stat poits you get on levelup. <br>Last slider allow you to lower Strength and Dexterity requirements on all items. ")

    WidgetSlider {
        caption: qsTr("Additional stat points gain per level")
        tooltip: ""
        value_key: root.value_context.extraStatPerLevel
    }

    WidgetSlider {
        caption: qsTr("Additional skill points gain per level")
        tooltip: qsTr("D2R only")
        value_key: root.value_context.extraSkillPerLevel
    }

    WidgetSlider {
        caption: qsTr("Additional Health gain per level")
        tooltip: qsTr("D2R only")
        value_key: root.value_context.extraHpPerLevel
    }
    WidgetSlider {
        caption: qsTr("Additional Mana gain per level")
        tooltip: qsTr("D2R only")
        value_key: root.value_context.extraManaPerLevel
    }
    WidgetSlider {
        caption: qsTr("Health potion speed increase")
        value_key: root.value_context.hpPotEfficacy
    }
    WidgetSlider {
        caption: qsTr("Mana potion speed increase")
        value_key: root.value_context.mpPotEfficacy
    }

    WidgetSlider {
        caption: qsTr("Maximum hard points for skill")
        tooltip: qsTr("D2R only")
        value_key: root.value_context.skillPointLimit
    }

    WidgetSlider {
        caption: qsTr("Change mercenaries Health")
        tooltip: ""
        value_key: root.value_context.mercHP
    }

    WidgetSlider {
        caption: qsTr("Change mercenaries Damage")
        tooltip: ""
        value_key: root.value_context.mercDam
    }

    Separator {}

    WidgetSlider {
        caption: qsTr("Normal difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.normalResPenalty
    }

    WidgetSlider {
        caption: qsTr("Nightmare difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.nightmareResPenalty
    }

    WidgetSlider {
        caption: qsTr("Hell difficulty resistance penalty, -all%")
        tooltip: ""
        value_key: root.value_context.hellResPenalty
    }
    WidgetCheckbox {
        caption: qsTr("Disable experience penalty after level 70")
        tooltip: qsTr("That means you won't have around 1% EXP at higher levels, so high level grind will be much faster.")
        value_key: root.value_context.disableExpPenalty
    }
}
