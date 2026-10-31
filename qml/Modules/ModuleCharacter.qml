import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleCharacter
    enabled_key: config.enabledModules.moduleCharacter
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
        caption: qsTr("Replace starting Health pots with Mana")
        tooltip: ""
        value_key: root.value_context.manaPots
    }

    WidgetCheckbox {
        caption: qsTr("Add Teleport as starting skill for all classes")
        tooltip: ""
        value_key: root.value_context.addTeleport
    }

    WidgetRangeSlider {
        caption: qsTr("Stat points gain per level")
        tooltip: ""
        value_key: root.value_context.statPerLevel
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Skill points gain per level (D2R only!)")
        tooltip: ""
        value_key: root.value_context.skillPerLevel
        isCompact: false
    }

    WidgetRangeSlider {
        caption: qsTr("Maximum hard points for skill (D2R only!)")
        tooltip: ""
        value_key: root.value_context.skillPointLimit
        isCompact: false
    }

    WidgetExponentialSlider {
        caption: qsTr("Change mercenaries Health, multiply by")
        tooltip: ""
        value_key: root.value_context.mercHP
        isCompact: false
    }

    WidgetExponentialSlider {
        caption: qsTr("Change mercenaries Damage, multiply by")
        tooltip: ""
        value_key: root.value_context.mercDam
        isCompact: false
    }
}
