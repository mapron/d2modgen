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
    // Base properties for data and strings
    property string caption: ""
    property string tooltip: ""
    property bool isCompact: true // do not place caption on separate baseline
    property bool controlAfterCaption: true // used only for isCompact==true
    property bool hideCaption: false // used only for isCompact==true
    property bool stretchCaption: true // used only for isCompact==true

    spacing: 4
    Layout.fillWidth: true

    required property Item control

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

        Item {
            Layout.fillWidth: true
        }
    }

    RowLayout {
        id: mainRowLayout
        spacing: 6

        RowLayout {
            id: captionContainer
            spacing: 6
            visible: root.isCompact && root.controlAfterCaption

            Layout.fillWidth: true
            Layout.preferredWidth: root.stretchCaption ? 0 : -1

            Label {
                visible: !hideCaption
                text: root.caption
            }

            WidgetHelpIcon {
                tooltipText: root.tooltip
            }
            Item {
                visible: root.stretchCaption
                Layout.fillWidth: root.stretchCaption
            }
        }

        RowLayout {
            id: contentContainer
            Layout.fillWidth: true
            Layout.preferredWidth: root.stretchCaption ? 0 : -1
        }

        RowLayout {
            spacing: 6
            visible: root.isCompact && !root.controlAfterCaption

            Label {
                visible: !hideCaption
                text: root.caption
            }

            WidgetHelpIcon {
                tooltipText: root.tooltip
            }
        }

        Component.onCompleted: {
            root.control.parent = contentContainer;
        }
    }
}
