/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ColumnLayout {
    id: root
    Layout.topMargin: 20
    property string caption: ""
    property string tooltip: ""

    property alias value_key1: internal.value_key1
    property alias value_key2: internal.value_key2

    RowLayout {
        spacing: 6
        visible: !root.isCompact
        Layout.fillWidth: true

        Label {
            text: root.caption
        }
        WidgetHelpIcon {
            tooltipText: root.tooltip
        }

        Button {
            text: qsTr("Reset")
            onClicked: {
                root.value_key1.value = -100;
                root.value_key2.value = 175;
            }
        }
        Button {
            text: qsTr("Make immune")
            onClicked: {
                root.value_key1.value = 100;
                root.value_key2.value = 135;
            }
        }
        Button {
            text: qsTr("Sunder immunity")
            onClicked: {
                root.value_key1.value = -100;
                root.value_key2.value = 95;
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }

    DynamicRangeSlider {
        id: internal
    }
}
