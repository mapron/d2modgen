import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick
import "../Widgets"

WidgetCommonTab {
    id: root
    value_context: config.monsterRandomizer
    enabled_key: config.monsterRandomizer.enabled
    caption: qsTr("Monster Randomizer")
    tooltip: qsTr("When this feature enabled, now any area in the game can spawn any basic monsters (Super Uniques, Act bosses etc not affected).<br>For normal difficulty, each monster will have a 'level-adjusted' copy which corresponds<br>to area's level (so you will have adequate monster stats in Act 1 for guest from Act 5).<br>For Nightmare/Hell, adjustment is done by area level.<br>Monster will have adjusted their minion spawns and skill levels, too.")

    Label {
        text: qsTr("All following options are relative percent to all possible monsters,<br>that can spawn in area (usually 3..7). <br>Example: Tal Rasha tombs have 7 possible monster types (yes there are 2 mummies)")
    }

    WidgetSlider {
        caption: qsTr("How many original monsters to keep")
        value_key: root.value_context.spawnKeepOriginal
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Amount of guest spawns in area, Max")
        tooltip: qsTr("This option 'invites' guests from other zones and acts in each area.")
        value_key: root.value_context.guestsMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount of guest spawns in area, Max")
        value_key: root.value_context.guestsMax
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount in zone to change AI, Min")
        tooltip: ""
        value_key: root.value_context.chimeraAIMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount in zone to change AI, Max")
        tooltip: ""
        value_key: root.value_context.chimeraAIMax
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Amount to change resistances, Min")
        tooltip: ""
        value_key: root.value_context.rollResistancesMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount to change resistances, Max")
        tooltip: ""
        value_key: root.value_context.rollResistancesMax
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount to change stats (except res), Min")
        tooltip: ""
        value_key: root.value_context.rollStatMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Amount to change stats (except res), Max")
        tooltip: ""
        value_key: root.value_context.rollStatMax
        isCompact: true
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }

    WidgetSlider {
        caption: qsTr("Increase density, Min")
        tooltip: ""
        value_key: root.value_context.densityMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Increase density, Max")
        tooltip: qsTr("For each zone, value between Min and Max generated.<br>Next original density of zone is scaled to that percent.")
        value_key: root.value_context.densityMax
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Increase Boss packs count, Min")
        tooltip: ""
        value_key: root.value_context.elitePacksMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Increase Boss packs count, Max")
        tooltip: ""
        value_key: root.value_context.elitePacksMax
        isCompact: true
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 1
        color: palette.accent
    }

    WidgetSlider {
        caption: qsTr("Add max possible simultaneous spawns")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        value_key: root.value_context.numMonExtra
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Increase monster groups population, +count<br><b>Beware! This setting have the most impact on getting screen laggy!<b><br><b>Having value above +3 will work well only if previous options are not maxed!</b>")
        tooltip: ""
        value_key: root.value_context.monGroupIncreaseSize
        isCompact: true
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell elite pack count on Nightmare and Normal")
        tooltip: ""
        value_key: root.value_context.hellPacks
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell resistances on NM/Normal")
        tooltip: ""
        value_key: root.value_context.hellResistances
    }
    WidgetCheckbox {
        caption: qsTr("Always maxiumum amount of packs")
        tooltip: ""
        value_key: root.value_context.maxPacks
    }
}
