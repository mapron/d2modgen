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

    WidgetPreset {
        model: [qsTr("Select preset..."), qsTr("More, MOAR mobs - a lot of them"), qsTr("A little bit of juice and flesh"), qsTr("I like pain"),]
        presetIds: ["", "crazy", "strong", "pain"]
        value_key: config.monsterRandomizer
    }
    WidgetSeed {
        value_key: root.value_context.seed
    }

    RowLayout {
        spacing: 6
        Layout.fillWidth: true

        LabelLarge {
            text: qsTr("Area population randomizer")
        }

        WidgetHelpIcon {
            tooltipText: qsTr("Game area is what you see in red text on screen and in corner of the map" //
            + "<br>(Blood Moor, River of Flame, etc). " //
            + "<br>Each area have so-called 'spawns' - what monster type can possibly genrated in the game" //
            + "<br>Example: Tal Rasha tombs have 6 possible monster types" //
            + "<br>and each time you create a game, area will have 3 out of this 6 spawns (with rare exceptions)" //
            + "<br>This sections allow you to bring new monsters to the area," //
            + "<br>remove existing spawsn and modify properies of what is left.")
        }

        Item {
            Layout.fillWidth: true
        }
    }

    WidgetSlider {
        caption: qsTr("How many original monsters to keep")
        value_key: root.value_context.spawnKeepOriginal
    }
    WidgetSlider {
        caption: qsTr("Amount of random super-uniques")
        value_key: root.value_context.superuniqueRando
    }

    WidgetRangeSlider {
        caption: qsTr("Amount of guest spawns in area")
        tooltip: qsTr("This option 'invites' guests from other areas and acts in each area.")
        value_key1: root.value_context.guestsMin
        value_key2: root.value_context.guestsMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount in area to change monster AI <br><b>Has few bugs, read the tooltip</b>")
        tooltip: qsTr("This option makes 'chimeras' - monsters with visuals of one type and AI for another.<br>This can cause flickering or on some rare occasion invisible monster sprite.<br>Sometimes combination renders monster harmless (it stays in idle animation or have 0 damage).<br><b>Do not set this too high for better experience!</b>")
        tooltipWarning: true

        value_key1: root.value_context.chimeraAIMin
        value_key2: root.value_context.chimeraAIMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount to change resistances")
        tooltip: qsTr("This will swap moster resistances and immunities:<br>for example, monster Fire immunity (value 130) become Poison instead,<br>and monster high-value physical (75) becoming Magic")
        value_key1: root.value_context.rollResistancesMin
        value_key2: root.value_context.rollResistancesMax
    }

    WidgetRangeSlider {
        caption: qsTr("Amount to change stats (except res)")
        tooltip: qsTr("This affects walk and run speed, monster HP, Damage,<br>experience reward, defense and attack rating,<br>and important: <b>elemental damage</b>")
        value_key1: root.value_context.rollStatMin
        value_key2: root.value_context.rollStatMax
    }

    Separator {}

    WidgetSlider {
        caption: qsTr("Minimal area id to enable randomizer for Normal")
        tooltip: qsTr("Value = 0 : start from Blood Moor<br>Value = 12 : Den of Evil, Und. Passage Lvl 1 and all Wilderness areas are safe<br>Value = 40 : Act 2 <br>Value = 75 : Act 3<br>Value = 103 : Act 4<br>Value = 109 : Act 5<br>Value = 133 : Skip entire Normal difficulty (Baal's chamber is 132)")
        value_key: root.value_context.minNormalZoneId
    }
}
