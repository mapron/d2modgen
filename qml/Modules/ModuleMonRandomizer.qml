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
    }

    WidgetRangeSlider {
        caption: qsTr("Amount of guest spawns in area")
        tooltip: qsTr("This option 'invites' guests from other zones and acts in each area.")
        value_key1: root.value_context.guestsMin
        value_key2: root.value_context.guestsMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount in zone to change AI <br><b>WARNING: experimental, read tooltip!</b>")
        tooltip: qsTr("This can cause crashes, lags, flickering, and so on!<br>Also some monsters can become harmless due to skill set change.<br>Do not set this too high.")
        value_key1: root.value_context.chimeraAIMin
        value_key2: root.value_context.chimeraAIMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount to change resistances")
        tooltip: ""
        value_key1: root.value_context.rollResistancesMin
        value_key2: root.value_context.rollResistancesMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount to change stats (except res)")
        tooltip: ""
        value_key1: root.value_context.rollStatMin
        value_key2: root.value_context.rollStatMax
    }

    Separator {}

    WidgetRangeSlider {
        caption: qsTr("Increase density")
        tooltip: qsTr("For each zone, value between Min and Max generated.<br>Next original density of zone is scaled to that percent.")
        value_key1: root.value_context.densityMin
        value_key2: root.value_context.densityMax
    }
    WidgetRangeSlider {
        caption: qsTr("Increase Boss packs count")
        tooltip: ""
        value_key1: root.value_context.elitePacksMin
        value_key2: root.value_context.elitePacksMax
    }

    Separator {}

    WidgetSlider {
        caption: qsTr("Add max possible simultaneous spawns")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        value_key: root.value_context.numMonExtra
    }

    WidgetSlider {
        caption: qsTr("Increase monster groups population, +count<br><b>Can drop performance for D2R, see tooltip<b>")
        tooltip: qsTr("Beware! This setting have the most impact on getting screen laggy!<br>Having value above +3 will work well only if previous options are not maxed!<br>For D2R loading hundreds of monsters on screen can take several seconds.<br>Game will work, but you will fight invisible mobs.")
        value_key: root.value_context.monGroupIncreaseSize
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
