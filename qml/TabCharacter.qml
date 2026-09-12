import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "character"
    caption: qsTr("Character")
    tooltip: qsTr("First three options allow you to change starting items for characters: <br>add Cube, replace id scroll with Tome (useful for No-town runs), and replace health with mana potions. <br>Next two sliders allow you to change how many skill and stat poits you get on levelup. <br>Last slider allow you to lower Strength and Dexterity requirements on all items. ")

    WidgetCheckbox {
        caption: qsTr("Add Horadric Cube as starting item")
        tooltip: ""
        value_context: root.value_context
        value_key: "addCube"
    }

    WidgetCheckbox {
        caption: qsTr("Replace ID scroll with ID tome")
        tooltip: ""
        value_context: root.value_context
        value_key: "idTome"
    }

    WidgetCheckbox {
        caption: qsTr("Replace starting Health pots with Mana")
        tooltip: ""
        value_context: root.value_context
        value_key: "manaPots"
    }

    WidgetCheckbox {
        caption: qsTr("Add Teleport as starting skill for all classes")
        tooltip: ""
        value_context: root.value_context
        value_key: "addTeleport"
    }

    WidgetSlider {
        caption: qsTr("Stat points gain per level")
        tooltip: ""
        from: 1
        to: 25
        value_context: root.value_context
        value_key: "statPerLevel"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Skill points gain per level (D2R only!)")
        tooltip: ""
        from: 1
        to: 20
        value_context: root.value_context
        value_key: "skillPerLevel"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Maximum hard points for skill (D2R only!)")
        tooltip: ""
        from: 1
        to: 60
        value_context: root.value_context
        value_key: "skillPointLimit"
        isCompact: false
    }

    WidgetExponentialSlider {
        caption: qsTr("Change mercenaries Health, multiply by")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "mercHP"
        isCompact: false
        suffix: "%"
    }

    WidgetExponentialSlider {
        caption: qsTr("Change mercenaries Damage, multiply by")
        tooltip: ""
        denom: 5
        mult: 10
        value_context: root.value_context
        value_key: "mercDam"
        isCompact: false
        suffix: "%"
    }
}
