import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick

WidgetCommonTab {
    id: root
    value_context: "monRandomizer"
    caption: qsTr("Monster Randomizer")
    tooltip: qsTr("When this feature enabled, now any area in the game can spawn any basic monsters (Super Uniques, Act bosses etc not affected).<br>For normal difficulty, each monster will have a 'level-adjusted' copy which corresponds<br>to area's level (so you will have adequate monster stats in Act 1 for guest from Act 5).<br>For Nightmare/Hell, adjustment is done by area level.<br>Monster will have adjusted their minion spawns and skill levels, too.")

    WidgetCheckbox {
        caption: qsTr("Random monster spawn in areas")
        tooltip: ""
        value_context: root.value_context
        value_key: "randomizeSpawns"
    }

    WidgetSlider {
        caption: qsTr("Spawned different types in one area")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        from: 2
        to: 10
        value_context: root.value_context
        value_key: "spawnedCount"
        isCompact: false
    }

    WidgetSlider {
        caption: qsTr("Maximum possible monster types in one area (population variety)")
        tooltip: qsTr("This option defines how many possibilites for monster types are exist in one zone;<br>Make this value lower if you want less variety between game restarts.<br>Note: D2 legacy support only 12 max, so value will be lowered implicitly.")
        from: 3
        to: 25
        value_context: root.value_context
        value_key: "maxTypes"
        isCompact: false
    }

    WidgetCheckbox {
        caption: qsTr("Random resistances")
        tooltip: ""
        value_context: root.value_context
        value_key: "randomizeResistances"
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell resistances on NM/Normal")
        tooltip: ""
        value_context: root.value_context
        value_key: "hellResistances"
    }
}
