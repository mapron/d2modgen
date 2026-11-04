/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

TextField {
    id: textInput

    property QtObject value_key: null

    function setTextValue(txt) {
        text = txt;
        value_key.value = txt;
    }

    color: textInput.palette.text

    text: value_key.value

    selectByMouse: true

    onEditingFinished: value_key.value = text

    background: Rectangle {
        color: textInput.palette.base
        border.color: textInput.acceptableInput ? palette.accent : "red"
        border.width: textInput.acceptableInput ? 1 : 2
        radius: 3
    }
}
