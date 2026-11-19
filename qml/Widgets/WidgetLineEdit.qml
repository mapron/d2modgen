/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    property alias value_key: internal.value_key
    property alias placeholderText: internal.placeholderText

    control: DynamicLineEdit {
        id: internal
        Layout.fillWidth: true
    }
}
