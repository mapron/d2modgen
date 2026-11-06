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
    value_context: config.skillRandomizer
    enabled_key: config.skillRandomizer.enabled
    caption: qsTr("Skill Randomizer")

    WidgetSlider {
        caption: qsTr("Chance to change element of skill")
        tooltip: qsTr("Supported 5 different element, physical damage can not be rolled.")
        value_key: root.value_context.chanceElemChange
    }

    WidgetSlider {
        caption: qsTr("Maximum elements rolled on single character class")
        value_key: root.value_context.maximumElements
    }

    Separator {}

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Skill randomizer:")
        }
        RadioGroup {
            model: [qsTr("Disable"), qsTr("Same class shuffle"), qsTr("Select class"), qsTr("Full class rando")]
            value_key: root.value_context.randoMode
        }
        Item {
            Layout.fillWidth: true
        }
    }

    WidgetComboBox {
        caption: qsTr("Pull all skills from single class:")
        model: [qsTr("Amazon"), qsTr("Sorceress"), qsTr("Necromancer"), qsTr("Paladin"), qsTr("Barbarian"), qsTr("Druid"), qsTr("Assassin"), qsTr("Warlock"),]
        value_key: root.value_context.specificClass
        visible: root.value_context.randoMode.value === 2
        stretchCaption: false
    }
    WidgetSlider {
        caption: qsTr("How many skills to keep")
        value_key: root.value_context.keepOriginal
        visible: root.value_context.randoMode.value > 1
    }
    WidgetSlider {
        caption: qsTr("How many skills generate")
        value_key: root.value_context.generate
        visible: root.value_context.randoMode.value > 0
    }

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Adjust all skill damage")
        }
        WidgetHelpIcon {
            tooltipText: qsTr("Weird values because technicalities:<br>easy adjustement only goes with power of 2.<br>Use as last resort to compensate for bad seed.")
        }
        RadioGroup {
            model: [qsTr("1/4"), qsTr("1/2"), qsTr("Normal"), qsTr("2x"), qsTr("4x")]
            value_key: root.value_context.globalHitShift
        }
        Item {
            Layout.fillWidth: true
        }
    }
    Separator {
        visible: root.value_context.randoMode.value === 3
    }
    LabelLarge {
        text: qsTr("Relative chance to select skill from specific class:")
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Amazon")
        value_key: root.value_context.weightAma
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Sorceress")
        value_key: root.value_context.weightSor
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Necromancer")
        value_key: root.value_context.weightNec
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Paladin")
        value_key: root.value_context.weightPal
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Barbarian")
        value_key: root.value_context.weightBar
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Druid")
        value_key: root.value_context.weightDru
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Assassin")
        value_key: root.value_context.weightAss
        visible: root.value_context.randoMode.value === 3
    }
    WidgetSlider {
        caption: qsTr("Warlock")
        value_key: root.value_context.weightWar
        visible: root.value_context.randoMode.value === 3
    }
    Separator {}
    LabelLarge {
        text: qsTr("Change skill requirement for each skill row:")
    }
    WidgetSlider {
        caption: qsTr("Row 1 (from level 0)")
        value_key: root.value_context.lvlreqRow1
    }
    WidgetSlider {
        caption: qsTr("Row 2 (from level 6)")
        value_key: root.value_context.lvlreqRow2
    }
    WidgetSlider {
        caption: qsTr("Row 3 (from level 12)")
        value_key: root.value_context.lvlreqRow3
    }
    WidgetSlider {
        caption: qsTr("Row 4 (from level 18)")
        value_key: root.value_context.lvlreqRow4
    }
    WidgetSlider {
        caption: qsTr("Row 5 (from level 24)")
        value_key: root.value_context.lvlreqRow5
    }
    WidgetSlider {
        caption: qsTr("Row 6 (from level 30)")
        value_key: root.value_context.lvlreqRow6
    }
}
