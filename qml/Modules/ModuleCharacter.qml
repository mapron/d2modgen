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
    value_context: config.character
    enabled_key: null

    property bool isD2R: config.main.versionMajor.value === 1

    caption: qsTr("Character")
    tooltip: qsTr("First three options allow you to change starting items for characters: <br>add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. <br>Next two sliders allow you to change how many skill and stat poits you get on levelup. <br>Last slider allow you to lower Strength and Dexterity requirements on all items. ")

    WidgetCheckbox {
        caption: qsTr("Add Horadric Cube as starting item")
        tooltip: ""
        value_key: root.value_context.addCube
    }

    WidgetCheckbox {
        caption: qsTr("Replace ID scroll with ID tome")
        tooltip: ""
        value_key: root.value_context.idTome
    }

    WidgetCheckbox {
        caption: qsTr("Replace TP scroll with TP tome")
        tooltip: ""
        value_key: root.value_context.tpTome
    }

    WidgetCheckbox {
        caption: qsTr("Replace starting Health pots with Mana")
        tooltip: ""
        value_key: root.value_context.manaPots
    }

    WidgetCheckbox {
        caption: qsTr("Add Teleport as starting skill for all classes")
        tooltip: ""
        value_key: root.value_context.addTeleport
    }

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
        caption: qsTr("Minimal level to spent points into skill")
        tooltip: qsTr("For example, if you select 50%, you can get level 30 skill on level 15 instead.")
        value_key: root.value_context.skillsReqLevel
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
}
