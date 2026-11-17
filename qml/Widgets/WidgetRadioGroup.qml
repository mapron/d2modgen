/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

WidgetCommonControl {
    spacing: 10
    property alias value_key: internal.value_key
    property alias model: internal.model

    control: RadioGroup {
        id: internal
    }
}
