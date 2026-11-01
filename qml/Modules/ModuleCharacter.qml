import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.moduleCharacter
    enabled_key: null

    property bool isD2R: config.moduleMain.versionMajor.value === 1

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

    WidgetSlider {
        caption: qsTr("Additional stat points gain per level")
        tooltip: ""
        value_key: root.value_context.extraStatPerLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Additional skill points gain per level")
        tooltip: ""
        value_key: root.value_context.extraSkillPerLevel
        isCompact: false
        visible: isD2R
    }

    WidgetSlider {
        caption: qsTr("Additional Health gain per level")
        value_key: root.value_context.extraHpPerLevel
        isCompact: false
    }
    WidgetSlider {
        caption: qsTr("Additional Mana gain per level")
        value_key: root.value_context.extraManaPerLevel
        isCompact: false
    }
    WidgetSlider {
        caption: qsTr("Health potion speed increase")
        value_key: root.value_context.hpPotEfficacy
        isCompact: false
    }
    WidgetSlider {
        caption: qsTr("Mana potion speed increase")
        value_key: root.value_context.mpPotEfficacy
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Maximum hard points for skill")
        tooltip: ""
        value_key: root.value_context.skillPointLimit
        isCompact: false
        visible: isD2R
    }
    WidgetSlider {
        caption: qsTr("Change Minimal level required to spent points into skill")
        tooltip: qsTr("For example, if you select 50%, you can get level 30 skill on level 15 instead.")
        value_key: root.value_context.skillsReqLevel
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change mercenaries Health, multiply by")
        tooltip: ""
        value_key: root.value_context.mercHP
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Change mercenaries Damage, multiply by")
        tooltip: ""
        value_key: root.value_context.mercDam
        isCompact: false
    }
}
