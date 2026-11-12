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
    value_context: config.maps
    enabled_key: null
    caption: qsTr("Area levels")

    RowLayout {
        spacing: 10
        Layout.fillWidth: true
        Label {
            text: qsTr("Change size of maze levels:")
        }
        RadioGroup {
            model: [qsTr("Disable"), qsTr("Compact"), qsTr("All to Normal"), qsTr("All to Hell")]
            value_key: root.value_context.mazeMode
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
