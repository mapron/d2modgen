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

    WidgetSlider {
        caption: qsTr("Add max possible simultaneous spawns")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        value_key: root.value_context.numMonExtra
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("How many original monsters to keep in pool")
        value_key: root.value_context.spawnKeepOriginal
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Add new monster spawn type in area, Max")
        tooltip: qsTr("This option defines how many possibilites for monster types are exist in one zone;<br>Make this value lower if you want less variety between game restarts.<br>Note: D2 legacy support only 12 max, so value will be lowered implicitly.")
        value_key: root.value_context.spawnGenerateMin
        isCompact: true
    }
    WidgetSlider {
        caption: qsTr("Add new monster spawn type in area, Max")
        value_key: root.value_context.spawnGenerateMax
        isCompact: true
    }

    WidgetSlider {
        caption: qsTr("Chance to change monster resistances")
        tooltip: ""
        value_key: root.value_context.chanceToSwapResistances
        isCompact: true
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell resistances on NM/Normal")
        tooltip: ""
        value_key: root.value_context.hellResistances
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
        caption: qsTr("Always maxiumum amount of packs")
        tooltip: ""
        value_key: root.value_context.maxPacks
    }
}
