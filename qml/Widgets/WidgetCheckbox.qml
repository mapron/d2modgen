/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    id: root
    property alias value_key: internal.value_key
    property alias checked: internal.checked

    isCompact: true
    controlAfterCaption: false
    hideCaption: true

    control: DynamicCheckbox {
        id: internal
        text: root.caption
    }
}
