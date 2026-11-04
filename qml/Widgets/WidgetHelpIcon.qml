/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Controls.Fusion

Label {
    id: root
    property string tooltipText: ""
    property bool isWarning: false

    leftPadding: 0
    rightPadding: 10

    horizontalAlignment: Text.AlignLeft

    visible: tooltipText !== ""
    text: root.isWarning === true ? "⚠" : "🛈"
    font.pointSize: 14
    color: root.isWarning === true ? "#FFB256" : window.palette.highlight

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        ToolTip {
            id: instantTip
            text: root.tooltipText
            visible: parent.containsMouse
            delay: 0
            timeout: 10000

            // Automatically flips text color matching light/dark states
            contentItem: Text {
                text: instantTip.text
                color: window.palette.windowText
                font.pointSize: 10
                textFormat: Text.RichText
            }

            // Smooth, adaptive background frame
            background: Rectangle {
                color: window.palette.window

                border.color: window.palette.highlight
                border.width: 2
                radius: 4
            }
        }
    }
}
