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
    value_context: config.balanceMaps
    enabled_key: null
    caption: qsTr("Area levels")

    LabelLarge {
        text: qsTr("Normal and elite spawn density")
    }
    WidgetSlider {
        caption: qsTr("Add max possible simultaneous spawns")
        tooltip: qsTr("This option choose how many different types will spawn at once (default is 3, which is D2 original behavior).<br>Note that some map tiles have predefined spawns (shaman camps etc) that can not be randomized.")
        value_key: root.value_context.numMonExtra
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

    WidgetCheckbox {
        caption: qsTr("Use Hell elite pack count on Nightmare and Normal")
        tooltip: qsTr("Use as alternative to 'Minimum elite packs' option")
        value_key: root.value_context.hellPacks
    }

    WidgetCheckbox {
        caption: qsTr("Always maxiumum amount of packs")
        tooltip: qsTr("If area have range to spawn, e.g. 10 to 15 packs, it will now always be maximum")
        value_key: root.value_context.maxPacks
    }

    Separator {}

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Change size of maze levels:")
        }

        WidgetHelpIcon {
            tooltipText: qsTr("Compact setting will make all maps either L-shaped with just 3 tiles, or<br>square with just 2 extra tiles (like flayer dungeon maps)")
        }
        RadioGroup {
            model: [qsTr("Disable"), qsTr("Compact"), qsTr("All to Normal"), qsTr("All to Hell")]
            value_key: root.value_context.mazeMode
        }
        Item {
            Layout.fillWidth: true
        }
    }
    Separator {}
    WidgetSlider {
        caption: qsTr("Increase Nightmare area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseNightmare
    }

    WidgetSlider {
        caption: qsTr("Increase Hell area levels, +levels")
        tooltip: ""
        value_key: root.value_context.levelIncreaseHell
    }
    WidgetCheckbox {
        caption: qsTr("Go beyond 85 level for areas")
        tooltip: ""
        value_key: root.value_context.levelIncreaseOverBound
    }
}
