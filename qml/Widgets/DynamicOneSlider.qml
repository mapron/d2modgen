/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

import "SliderFunctions.js" as SliderFunctions

RowLayout {
    id: root
    spacing: 10
    Layout.fillWidth: true

    // --- PUBLIC API ---
    property QtObject value_key: null

    // private
    property string suffix: value_key.isPercent ? "%" : ""
    property bool isLinear: !value_key.hasLogScale

    property int from: value_key.min
    property int to: value_key.max

    Slider {
        id: internalSlider
        Layout.fillWidth: true
        from: isLinear ? root.from : 0.0
        to: isLinear ? root.to : 2.0
        value: SliderFunctions.valueToSlider(isLinear, root.from, root.to, value_key.value)
        handle.implicitWidth: 16
        handle.implicitHeight: 16

        onMoved: value_key.value = SliderFunctions.sliderToValue(isLinear, root.from, root.to, value)
        live: true
    }

    Rectangle {
        Layout.preferredWidth: internalRow.implicitWidth + 4
        Layout.preferredHeight: internalRow.implicitHeight + 4

        color: textInput.palette.base
        property bool isValidInput: textInput.acceptableInput

        border.color: isValidInput ? palette.accent : "red"
        border.width: isValidInput ? 1 : 2
        radius: 3

        RowLayout {
            id: internalRow
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0

            SliderTextInput {
                id: textInput
                value_key: root.value_key
            }

            SuffixText {
                id: percentSign
                text: root.suffix
            }
        }
    }
}
