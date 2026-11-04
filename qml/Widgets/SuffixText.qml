/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Text {
    id: root
    visible: text !== ""

    color: palette.text
    opacity: 0.8

    Layout.fillWidth: false
    Layout.rightMargin: 4
}
