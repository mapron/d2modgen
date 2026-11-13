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
    value_context: config.balanceSkills
    caption: qsTr("Skill balance")

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
