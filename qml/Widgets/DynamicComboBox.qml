/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ComboBox {
    id: internalCombo
    Layout.fillWidth: false
    implicitContentWidthPolicy: ComboBox.WidestTextWhenCompleted

    property QtObject value_key: null

    currentIndex: value_key.value

    topPadding: 2
    bottomPadding: 2
    leftPadding: 4

    onActivated: value_key.value = currentIndex
}
