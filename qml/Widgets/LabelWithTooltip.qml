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

    property string caption: ""
    property string tooltip: ""

    spacing: 6
    Layout.fillWidth: true

    LabelLarge {
        text: root.caption
    }

    WidgetHelpIcon {
        tooltipText: root.tooltip
    }

    Item {
        Layout.fillWidth: true
    }
}
