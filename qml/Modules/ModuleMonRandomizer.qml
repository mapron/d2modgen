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

    WidgetSlider {
        caption: qsTr("Add max possible simultaneous spawns")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        value_key: root.value_context.numMonExtra
    }

    Separator {}

    LabelLarge {
        text: qsTr("Normal and elite spawn density")
    }
    WidgetSlider {
        caption: qsTr("Minimum density")
        tooltip: qsTr("This is applied before 'Increase density'<br><b>Beware! Most of feeling on density comes from monster pack size and elite spawns!</b>" //
        + "<br>Original game reference values (they same for Normal and Hell):" //
        + "<br>Blood moor = 520" //
        + "<br>Flayer jungle = 650" //
        + "<br>Chaos Sanctuary = 800" //
        + "<br>Tal Tomb = 2200")
        value_key: root.value_context.minAbsDensity
    }
    WidgetSlider {
        caption: qsTr("Minimum elite packs")
        tooltip: qsTr("This is applied before 'Iincrease Boss packs count'<br><b>Beware! Most of feeling on density comes from monster pack size and elite spawns!</b>" //
        + "<br>Original game reference values for Hell:" //
        + "<br>Blood moor = 7-9" //
        + "<br>Flayer jungle = 10-15" //
        + "<br>Chaos Sanctuary = 6-7" //
        + "<br>Tal Tomb = 7-9")
        value_key: root.value_context.minAbsPacks
    }

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

    WidgetSlider {
        caption: qsTr("Increase monster groups population, +count<br><b>Has performance issues, read the tooltip<b>")
        tooltip: qsTr("Beware! This setting have the most impact on getting screen laggy!<br>Having value above +3 will work well only if previous options are not maxed!<br>For D2R loading hundreds of monsters on screen can take several seconds.<br>Game will work, but you will fight invisible mobs.")
        tooltipWarning: true
        value_key: root.value_context.monGroupIncreaseSize
    }

    Separator {}

    LabelLarge {
        text: qsTr("Misc tuning: Normal-specifics, etc.")
    }

    WidgetSlider {
        caption: qsTr("Minimal area id to enable randomizer for Normal")
        tooltip: qsTr("Value = 0 : start from Blood Moor<br>Value = 12 : Den of Evil, Und. Passage Lvl 1 and all Wilderness areas are safe<br>Value = 40 : Act 2 <br>Value = 75 : Act 3<br>Value = 103 : Act 4<br>Value = 109 : Act 5<br>Value = 133 : Skip entire Normal difficulty (Baal's chamber is 132)")
        value_key: root.value_context.minNormalZoneId
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell elite pack count on Nightmare and Normal")
        tooltip: qsTr("Use as alternative to 'Minimum elite packs' option")
        value_key: root.value_context.hellPacks
    }

    WidgetCheckbox {
        caption: qsTr("Use Hell resistances on NM/Normal")
        tooltip: qsTr("Want to deal with immunities in Normal? Here you go.")
        value_key: root.value_context.hellResistances
    }
    WidgetCheckbox {
        caption: qsTr("Always maxiumum amount of packs")
        tooltip: qsTr("If area have range to spawn, e.g. 10 to 15 packs, it will now always be maximum")
        value_key: root.value_context.maxPacks
    }
}
