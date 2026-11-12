/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

RowLayout {
    id: root
    property QtObject value_key: null

    spacing: 10
    Layout.fillWidth: true

    Label {
        text: qsTr("Random seed:")
    }
    WidgetHelpIcon {
        tooltipText: qsTr("Seed determine how reproduceable generation results are.<br>If seed stays the same, output of generation is also always exactly the same.<br>If you want each generation be stable and reproduceable, remove <b>Create new seed</b>")
    }
    DynamicLineEdit {
        value_key: root.value_key

        Layout.minimumWidth: 100
    }
    Button {
        text: qsTr("Generate new")
        onClicked: root.value_key.value = appui.makeSeed()
    }
}
