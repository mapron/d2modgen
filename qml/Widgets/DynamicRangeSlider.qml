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
    property QtObject value_key1: null
    property QtObject value_key2: null

    // private
    property string suffix: value_key1.isPercent ? "%" : ""
    property bool isLinear: !value_key1.hasLogScale

    property int from: value_key1.min
    property int to: value_key1.max

    RangeSlider {
        id: internalSlider
        Layout.fillWidth: true
        from: isLinear ? root.from : 0.0
        to: isLinear ? root.to : 2.0
        first.value: SliderFunctions.valueToSlider(isLinear, root.from, root.to, value_key1.value)
        second.value: SliderFunctions.valueToSlider(isLinear, root.from, root.to, value_key2.value)
        first.handle.implicitWidth: 16
        first.handle.implicitHeight: 16
        second.handle.implicitWidth: 16
        second.handle.implicitHeight: 16

        first.onMoved: value_key1.value = SliderFunctions.sliderToValue(isLinear, root.from, root.to, first.value)
        second.onMoved: value_key2.value = SliderFunctions.sliderToValue(isLinear, root.from, root.to, second.value)

        readonly property real midPoint: from + (to - from) / 2

        first.handle.z: internalSlider.first.pressed ? 2 : (internalSlider.second.pressed ? 0 : (internalSlider.first.value >= midPoint ? 1 : 0))

        second.handle.z: internalSlider.second.pressed ? 2 : (internalSlider.first.pressed ? 0 : (internalSlider.first.value < midPoint ? 1 : 0))

        live: true
    }

    Rectangle {
        Layout.preferredWidth: internalRow.implicitWidth + 4
        Layout.preferredHeight: internalRow.implicitHeight + 4

        color: textInput1.palette.base
        property bool isValidInput: textInput1.acceptableInput && textInput2.acceptableInput

        border.color: isValidInput ? palette.accent : "red"
        border.width: isValidInput ? 1 : 2
        radius: 3

        RowLayout {
            id: internalRow
            anchors.centerIn: parent
            spacing: 0

            SliderTextInput {
                id: textInput1
                value_key: root.value_key1
            }

            Label {
                id: spacerLabel
                text: ".."
            }

            SliderTextInput {
                id: textInput2
                value_key: root.value_key2
            }

            SuffixText {
                id: percentSign
                text: root.suffix
            }
        }
    }
}
